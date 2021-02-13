#ifndef kode_dssi_included
#define kode_dssi_included
//----------------------------------------------------------------------

#include "base/kode.h"

#include <dssi.h>

//----------

#ifdef KODE_DEBUG_DSSI
  #define DSSI_Trace    KODE_Trace
  #define DSSI_DTrace   KODE_DTrace
  #define DSSI_TRACE    KODE_TRACE
#else
  #define DSSI_Trace    KODE_NoTrace
  #define DSSI_DTrace   KODE_NoTrace
  #define DSSI_TRACE    {}
#endif

#ifdef KODE_DEBUG_DSSI_RT
  #define DSSI_Trace_RT  KODE_Trace
  #define DSSI_DTrace_RT KODE_DTrace
  #define DSSI_TRACE_RT  KODE_TRACE
#else
  #define DSSI_Trace_RT  KODE_NoTrace
  #define DSSI_DTrace_RT KODE_NoTrace
  #define DSSI_TRACE_RT  {}
#endif

//----------------------------------------------------------------------
#endif






