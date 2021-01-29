#ifndef kode_process_context_included
#define kode_process_context_included
//----------------------------------------------------------------------

struct KODE_ProcessContext {
  uint32_t  mode;
  uint32_t  numSamples;
  float* inputs[8];
  float* outputs[8];
  float samplerate;
  float tempo;
  uint32_t timesig;
  uint32_t samplepos;
  float beatpos;
  uint32_t playState;
};


//----------------------------------------------------------------------
#endif
