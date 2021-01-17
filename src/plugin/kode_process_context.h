#ifndef kode_process_context_included
#define kode_process_context_included
//----------------------------------------------------------------------

#include "base/kode.h"

#define KODE_PROCESS_MAX_INPUTS   8
#define KODE_PROCESS_MAX_OUTPUTS  8

struct KODE_ProcessContext {
  uint32_t      numinputs     = 0;
  uint32_t      numoutputs    = 0;
  uint32_t      numsamples    = 0;
  float*        inputs[KODE_PROCESS_MAX_INPUTS]   = {0};
  float*        outputs[KODE_PROCESS_MAX_OUTPUTS] = {0};
  float         samplerate    = 0.0f;
  uint32_t      blocksize                         = 0;
  float         tempo                             = 0.0f;
  uint32_t      samplepos                         = 0;
  float         beatpos                           = 0.0;
  uint32_t      timesig_num                       = 0;
  uint32_t      timesig_denom                     = 0;
  uint32_t      playstate                         = 0;
  //float         time          = 0.0f;
  //float         input_time    = 0.0f;
  //float         output_time   = 0.0f;
};

//----------------------------------------------------------------------
#endif
