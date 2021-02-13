#ifndef kode_vst2_included
#define kode_vst2_included
//----------------------------------------------------------------------

#include "base/kode.h"

#ifdef KODE_PLUGIN_VST2_VESTIGE
  typedef int32_t   VstInt32;
  typedef intptr_t  VstIntPtr;
  struct  ERect { int16_t top, left, bottom, right; };
  #include "extern/vst2/vestige.h"
#else
  #define __cdecl
  #include "extern/vst2/aeffect.h"
  #include "extern/vst2/aeffectx.h"
#endif

//----------------------------------------------------------------------

#ifdef KODE_DEBUG_VST2
  #define KODE_VST2TRACE KODE_TRACE
  #define KODE_Vst2Trace KODE_Trace
  #define KODE_Vst2DTrace KODE_DTrace
#else
  #define KODE_VST2TRACE
  #define KODE_Vst2Trace KODE_NoTrace
  #define KODE_Vst2DTrace KODE_NoTrace

#endif

//----------------------------------------------------------------------
#endif





