#ifndef kode_ladspa_included
#define kode_ladspa_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include <ladspa.h>

//----------

#ifdef KODE_DEBUG_LADSPA
  #define LADSPA_Trace    KODE_Trace
  #define LADSPA_DTrace   KODE_DTrace
  #define LADSPA_TRACE    KODE_TRACE
#else
  #define LADSPA_Trace    KODE_NoTrace
  #define LADSPA_DTrace   KODE_NoTrace
  #define LADSPA_TRACE    {}
#endif

#ifdef KODE_DEBUG_LADSPA_RT
  #define LADSPA_Trace_RT  KODE_Trace
  #define LADSPA_DTrace_RT KODE_DTrace
  #define LADSPA_TRACE_RT  KODE_TRACE
#else
  #define LADSPA_Trace_RT  KODE_NoTrace
  #define LADSPA_DTrace_RT KODE_NoTrace
  #define LADSPA_TRACE_RT  {}
#endif

//----------------------------------------------------------------------
#endif
