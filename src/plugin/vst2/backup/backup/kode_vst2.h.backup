#ifndef kode_vst2_included
#define kode_vst2_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------

#ifdef KODE_PLUGIN_VST2_VESTIGE
  #include "extern/vst2/vestige.h"
#else
  #define __cdecl
  #include "extern/vst2/aeffect.h"
  #include "extern/vst2/aeffectx.h"
#endif

//----------------------------------------------------------------------

#ifdef KODE_DEBUG_VST2
  #define VST2_Trace    KODE_Trace
  #define VST2_DTrace   KODE_DTrace
  #define VST2_TRACE    KODE_TRACE
#else
  #define VST2_Trace    KODE_NoTrace
  #define VST2_DTrace   KODE_NoTrace
  #define VST2_TRACE    {}
#endif

//

#ifdef KODE_DEBUG_VST2_RT
  #define VST2_Trace_RT  KODE_Trace
  #define VST2_DTrace_RT KODE_DTrace
  #define VST2_TRACE_RT  KODE_TRACE
#else
  #define VST2_Trace_RT  KODE_NoTrace
  #define VST2_DTrace_RT KODE_NoTrace
  #define VST2_TRACE_RT  {}
#endif

//----------------------------------------------------------------------
#endif
