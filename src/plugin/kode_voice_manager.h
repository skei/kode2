#ifndef kode_voice_manager_included
#define kode_voice_manager_included
//----------------------------------------------------------------------

#define KODE_MASTER_BEND_RANGE  2.0f
#define KODE_VOICE_BEND_RANGE   48.0f

//----------------------------------------------------------------------

#include "kode.h"
#include "audio/kode_audio_math.h"
#include "audio/kode_audio_utils.h"
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
  void ctrl(uint32_t i, uint32_t v) {}
  void master_bend(float mb) {}
  void master_press(float mp) {}
  void master_ctrl(uint32_t i, uint32_t v) {}
  void parameter(uint32_t i, float v) {}
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

  KODE_VoiceContext MVoiceContext;
  VOICE             MVoices[NUM];
  uint32_t          MVoiceState[NUM];
  int32_t           MVoiceNote[NUM];
  int32_t           MVoiceChannel[NUM];
  int32_t           MNoteToVoice[16*128];

//------------------------------
public:
//------------------------------

  KODE_VoiceManager() {
    for (uint32_t i=0; i<(16*128); i++) MNoteToVoice[i] = -1;
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

  void prepare(float ASampleRate, uint32_t ABlockSize) {
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].prepare(&MVoiceContext,ASampleRate,ABlockSize);
    }
  }

  //----------

  void parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) {
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].parameter(AIndex,AValue);
    }
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
        handle_voice_lift(ch,v1,v2);
        break;
      case KODE_MIDI_NOTE_ON:
        if (v2 == 0) handle_voice_lift(ch,v1,0);
        else handle_voice_strike(ch,v1,v2);
        break;
      case KODE_MIDI_CONTROL_CHANGE:
        if (ch == 0) handle_master_ctrl(v1,v2);
        else {
          if (v1 == 74) handle_voice_slide(ch,v2);
          else handle_voice_ctrl(ch,v1,v2);
        }
        break;
      case KODE_MIDI_PITCHBEND:
        if (ch == 0) handle_master_bend(bend);
        else handle_voice_bend(ch,bend);
        break;
      case KODE_MIDI_CHANNEL_AFTERTOUCH:
        if (ch == 0) handle_master_press(v1);
        else handle_voice_press(ch,v1);
        break;
      case KODE_MIDI_POLY_AFTERTOUCH:
        handle_voice_press(ch,v1);
        break;
    } // switch
  }

  //----------

  void processBlock(KODE_ProcessContext* AContext) {
    preProcess();
    MVoiceContext.processContext = AContext;
    KODE_ClearStereoBuffer(AContext->outputs,AContext->numsamples);
    processPlayingVoices();
    processReleasedVoices();
    postProcess();
  }

//------------------------------
private:
//------------------------------

  void preProcess() {
  }

  //----------

  void postProcess() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == KODE_VOICE_FINISHED) {
        clear_voice(i);
      }
    }
  }

  //----------

  void processPlayingVoices() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == KODE_VOICE_PLAYING) {
        MVoiceState[i] = MVoices[i].process(KODE_VOICE_PLAYING);
      }
    }
  }

  //----------

  void processReleasedVoices() {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == KODE_VOICE_RELEASED) {
        MVoiceState[i] = MVoices[i].process(KODE_VOICE_RELEASED);
      }
    }
  }

  //----------

  int32_t find_voice(bool ATryReleased=true) {
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceState[i] == KODE_VOICE_OFF) return i;
      if (MVoiceState[i] == KODE_VOICE_FINISHED) return i;
    }
    if (ATryReleased) {
      for (uint32_t i=0; i<NUM; i++) {
        if (MVoiceState[i] == KODE_VOICE_RELEASED) {
          return i;
        }
      }
    }
    return -1;
  }

  //----------

  void clear_voice(uint32_t voice) {
    int32_t n  = MVoiceNote[voice];
    int32_t ch = MVoiceChannel[voice];
    if ((n>=0) && (ch>=0)) {
      uint32_t note = (MVoiceChannel[voice] * 128) + MVoiceNote[voice];
      MNoteToVoice[note] = -1;
    }
    MVoiceNote[voice]    = -1;
    MVoiceChannel[voice] = -1;
    MVoiceState[voice]   = KODE_VOICE_OFF;
  }



//------------------------------
private:
//------------------------------

  void handle_voice_strike(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
    uint32_t note = (AChannel * 128) + ANote;
    int32_t voice = find_voice(true);
    if (voice >= 0) {
      clear_voice(voice);
      MNoteToVoice[note] = voice;
      float v = (float)AVelocity * KODE_INV127F;
      float n = (float)ANote;
      MVoiceNote[voice]     = ANote;
      MVoiceChannel[voice]  = AChannel;
      MVoiceState[voice]    = MVoices[voice].strike(n,v);
    }
  }

  //----------

  void handle_voice_lift(int32_t AChannel, uint32_t ANote, uint32_t AVelocity) {
    uint32_t note = (AChannel * 128) + ANote;
    int32_t voice = MNoteToVoice[note];
    if (voice >= 0) {
      float v = (float)AVelocity * KODE_INV127F;
      MVoiceState[voice] = MVoices[voice].lift(v);
      MNoteToVoice[note] = -1;
      //clear_voice(voice);
    }
  }

  //----------

  void handle_voice_bend(int32_t AChannel, uint32_t ABend) {
    float v = (ABend * KODE_INV8192F) - 1.0f;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].bend(v);
      }
    }
  }

  //----------

  void handle_voice_press(int32_t AChannel, uint32_t APress) {
    float v = (float)APress * KODE_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].press(v);
      }
    }
  }

  //----------

  void handle_voice_slide(int32_t AChannel, uint32_t ASlide) {
    float s = (float)ASlide * KODE_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].slide(s);
      }
    }
  }

  //----------

  void handle_voice_ctrl(int32_t AChannel, uint32_t AIndex, uint32_t AValue) {
    //float v = (float)AValue * KODE_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      if (MVoiceChannel[i] == AChannel) {
        MVoices[i].ctrl(AIndex,AValue);
      }
    }
  }

  //----------

  void handle_master_ctrl(uint32_t AIndex, uint32_t AValue) {
    //float mc = (float)AValue * KODE_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].master_ctrl(AIndex,AValue);
    }
  }

  //----------

  void handle_master_bend(uint32_t ABend) {
    float mb = (ABend * KODE_INV8192F) - 1.0f;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].master_bend(mb);
    }
  }

  //----------

  void handle_master_press(uint32_t AVelocity) {
    float mp = (float)AVelocity * KODE_INV127F;
    for (uint32_t i=0; i<NUM; i++) {
      MVoices[i].master_press(mp);
    }
  }

  //----------

};

//----------------------------------------------------------------------
#endif

