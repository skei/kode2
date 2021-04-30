#ifndef kode_process_context_included
#define kode_process_context_included
//----------------------------------------------------------------------

struct KODE_ProcessContext {
  uint32_t  mode;
  uint32_t  offset;
  uint32_t  numsamples;
  uint32_t  numinputs;
  uint32_t  numoutputs;
  float*    inputs[8];
  float*    outputs[8];
  float     samplerate;
  float     tempo;
  uint32_t  timesignum;
  uint32_t  timesigdenom;
  uint32_t  samplepos;
  float     beatpos;
  uint32_t  playstate;
};


//----------------------------------------------------------------------
#endif
