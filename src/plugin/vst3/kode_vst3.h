#ifndef kode_vst3_included
#define kode_vst3_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/vst3/kode_vst3_api.h"

//----------------------------------------------------------------------

#ifdef KODE_DEBUG_VST3
  #define VST3_Print   KODE_Print
  #define VST3_DPrint  KODE_DPrint
  #define VST3_PRINT   KODE_PRINT
#else
  #define VST3_Print   KODE_NoPrint
  #define VST3_DPrint  KODE_NoPrint
  #define VST3_PRINT   {}
#endif

//----------------------------------------------------------------------
#endif
