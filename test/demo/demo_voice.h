#ifndef demo_voice_included
#define demo_voice_included
//----------------------------------------------------------------------

//#include "kode.h"

class demo_voice {

//------------------------------
private:
//------------------------------

  KODE_VoiceContext*  MContext      = KODE_NULL;
  float               MNote         = 0.0f;
  float               MBend         = 0.0f;
  float               MMasterBend   = 0.0f;
  float               MMasterPress  = 0.0f;

  float srate = 0.0f; // sample rate
  float ph    = 0.0f; // phase
  float phadd = 0.0f; // phase add

//------------------------------
public:
//------------------------------

  // content of AContext not valid yet..
  // just keep track of the ptr..

  void prepare(KODE_VoiceContext* AContext, float ASampleRate, uint32_t ABlockSize) {
    MContext = AContext;
    srate = ASampleRate;
  }

  //----------

  uint32_t strike(float note, float vel) {
    MNote = note;
    MBend = 0.0f;
    float hz = KODE_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    ph = 0.0f;
    phadd = hz / srate;
    return KODE_VOICE_PLAYING;
  }

  //----------

  uint32_t lift(float vel) {
    return KODE_VOICE_FINISHED;
  }

  //----------

  void bend(float b) {
    MBend = b;
    float hz = KODE_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / srate;
  }

  //----------

  void press(float p) {
  }

  //----------

  void slide(float s) {
  }

  //----------

  void ctrl(uint32_t i, uint32_t v) {
  }

  //----------

  void master_bend(float mb) {
    MMasterBend = mb;
    float hz = KODE_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / srate;
  }

  //----------

  void master_press(float mp) {
    MMasterPress = mp;
  }

  //----------

  void master_ctrl(uint32_t i, float mc) {
  }

  //----------

  void parameter(uint32_t i, float c) {
  }

  //----------

  uint32_t process(uint32_t AMode) {
    uint32_t num = MContext->processContext->numsamples;
    float*out0 = MContext->processContext->outputs[0];
    float*out1 = MContext->processContext->outputs[1];
    for (uint32_t i=0; i<num; i++) {
      float v = (ph * 2.0f) - 1.0f;
      ph += phadd;
      ph = KODE_Fract(ph);
      *out0++ += v * 0.1f;
      *out1++ += v * 0.1f;
    }
    return AMode;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
