#ifndef kdoe_voice_included
#define kdoe_voice_included
//----------------------------------------------------------------------

#define KODE_MASTER_BEND_RANGE  2.0f
#define KODE_VOICE_BEND_RANGE   48.0f

/*

  // assumes 16 voices

  * mpe:

    note/of = strike/lift
    poly aftertouch = press
    pitchbend = bend, chan 0 = master, range +-2, all voices, chan 1..15 = range +-48
    cc74 = slide

  * note on, velocity 0 = note off
*/

//----------------------------------------------------------------------

#include "kode.h"
#include "audio/kode_audio_math.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

enum KODE_EVoiceStates {
  KODE_VOICE_OFF      = 0,
  KODE_VOICE_PLAYING  = 1,
  KODE_VOICE_RELEASED = 2,
  KODE_VOICE_FINISHED = 3
};

//----------------------------------------------------------------------

struct KODE_VoiceContext {
  KODE_ProcessContext*  processContext  = KODE_NULL;
  float                 samplerate      = 0.0f;
  uint32_t              blocksize       = 0;
  float                 masterBend      = 0.0f;
  float                 masterPress     = 0.0f;
  // modmatrix
  // buffers
};

//----------------------------------------------------------------------

/*
class KODE_Voice {
public:
  void strike(float note, float vel) {}
  void lift(float vel) {}
  void bend(float v) {}
  void press(float v) {}
  void slide(float v) {}
  void ctrl(float v) {}
  void process(KODE_VoiceContext* AContext) {}
};
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class VOICE, int NUM=16>
class KODE_VoiceManager {

//------------------------------
private:
//------------------------------

  KODE_VoiceContext MContext;
  VOICE             MVoices[NUM];
  uint32_t          MVoiceState[NUM];
  int32_t           MVoiceNote[NUM];
  int32_t           MVoiceChannel[NUM];
  int32_t           MNoteToVoice[128*16];

  float             MMasterBend   = 0.0f;
  float             MMasterPress  = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_VoiceManager() {
    for (uint32_t i=0; i<(128*16); i++) MNoteToVoice[i] = -1;
    for (uint32_t i=0; i<NUM; i++) {
      MVoiceState[i]    = KODE_VOICE_OFF;
      MVoiceNote[i]     = -1;
      MVoiceChannel[i]  = -1;
    }
  }

  //----------

  ~KODE_VoiceManager() {
  }

//------------------------------
public:
//------------------------------

  void prepare(float ASamplerate, uint32_t ABlocksize) {
    MContext.samplerate = ASamplerate;
    MContext.blocksize = ABlocksize;
  }

  //----------

  void parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) {
    // parameter
  }

  //----------

  void midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) {

    uint32_t  msg   = AMsg1 & 0xf0;
    uint32_t  ch    = AMsg1 & 0x0f;
    uint32_t  v1    = AMsg2 & 0x7f;
    uint32_t  v2    = AMsg3 & 0x7f;
    uint32_t  bend  = v1 + (v2 * 128);

    switch (msg) {

      case KODE_MIDI_NOTE_OFF:
        voice_lift(ch,v1,v2);
        break;

      case KODE_MIDI_NOTE_ON:
        if (v2 == 0) {
          voice_lift(ch,v1,0);
        }
        else {
          voice_strike(ch,v1,v2);
        }
        break;

      case KODE_MIDI_CONTROL_CHANGE:
        //KODE_DPrint("cc.. ch %i v1 %i v2 %i\n",ch,v1,v2);
        if (ch == 0) {
          master_ctrl(v1,v2);
        }
        else {
          if (v1 == 74) {
            voice_slide(ch,v2);
          }
          else {
            voice_ctrl(ch,v2);
          }
        }
        break;

      case KODE_MIDI_PITCHBEND:
        if (ch == 0) {
          master_bend(bend);
        }
        else {
          voice_bend(ch,bend);
        }
        break;

      case KODE_MIDI_CHANNEL_AFTERTOUCH:
        if (ch == 0) {
          master_press(v1);
        }
        else {
          voice_press(ch,v1);
        }
        break;

      case KODE_MIDI_POLY_AFTERTOUCH:
        voice_press(ch,v1);
        break;

    } // switch
  }

  //----------

  void processBlock(KODE_ProcessContext* AContext) {
  }

//------------------------------
private:
//------------------------------

  int32_t find_voice(bool ATryReleased=true) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == KODE_VOICE_OFF) return i;
      if (MVoiceState[i] == KODE_VOICE_FINISHED) return i;
    }
    if (ATryReleased) {
      for (uint32_t i=0; i<NUM; i++) {
        if (MVoiceState[i] == KODE_VOICE_RELEASED) {
          // voice.kill
          return i;
        }
      }
    }
    return -1;
  }

  //----------

  void kill_voice(uint32_t voice) {
    //MVoices[voice].killVoice
    MVoiceState[voice]    = KODE_VOICE_OFF;
    MVoiceNote[voice]     = -1;
    MVoiceChannel[voice]  = -1;
  }



//------------------------------
private:
//------------------------------

  void voice_strike(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
    uint32_t note = AChannel * 128 + ANote;
    int32_t voice = find_voice(true);
    if (voice >= 0) {
      MNoteToVoice[note] = voice;
      MVoiceNote[voice] = ANote;
      MVoiceChannel[voice] = AChannel;
      MVoiceState[voice] = KODE_VOICE_PLAYING;
      //float fnote = (float)ANote;
      //float hz = KODE_NoteToHz(fnote);
      float v = (float)AVelocity * KODE_INV127F;
      float n = (float)ANote;// + (MMasterBend * KODE_MASTER_BEND_RANGE);
      MVoices[voice].strike(n,v);
    }
  }

  //----------

  void voice_lift(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
    uint32_t note = AChannel * 128 + ANote;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      MNoteToVoice[note] = -1;
      //MVoiceNote[voice] = -1;
      //MVoiceChannel[voice] = -1;
      MVoiceState[voice] = KODE_VOICE_RELEASED;
      float v = (float)AVelocity * KODE_INV127F;
      MVoices[voice].lift(v);
    }
  }

  //----------

  void voice_bend(int32_t AChannel, uint32_t ABend) {
    float v = (ABend * KODE_INV8192F) - 1.0f;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].bend(v,MMasterBend);
      }
    }
  }

  //----------

  void voice_press(int32_t AChannel, uint32_t APress) {
    float v = (float)APress * KODE_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].press(v,MMasterPress);
      }
    }
  }

  //----------

  void voice_slide(int32_t AChannel, uint32_t ASlide) {
    float s = (float)ASlide * KODE_INV127F;
    //KODE_DPrint("slide ch %i s %.3f\n",AChannel,s);
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].slide(s);
      }
    }
  }

  //----------

  void voice_ctrl(int32_t AChannel, uint32_t AValue) {
    float v = (float)AValue * KODE_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].ctrl(v);
      }
    }
  }

  //----------

  void master_ctrl(uint32_t AIndex, uint32_t AValue) {
    //float v = (AValue * KODE_INV8192F) - 1.0f;
    //KODE_DPrint("MMasterBend: %.3f\n",MMasterBend);
  }

  //----------

  void master_bend(uint32_t ABend) {
    MMasterBend = (ABend * KODE_INV8192F) - 1.0f;
    //KODE_DPrint("MMasterBend: %.3f\n",MMasterBend);
  }

  //----------

  void master_press(uint32_t AVelocity) {
    MMasterPress = (float)AVelocity * KODE_INV127F;
    //KODE_DPrint("MMasterPress: %.3f\n",MMasterPress);
  }

  //----------

};

//----------------------------------------------------------------------
#endif

