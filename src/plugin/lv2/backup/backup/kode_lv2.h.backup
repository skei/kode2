#ifndef kode_lv2_included
#define kode_lv2_included
//----------------------------------------------------------------------

//#define KODE_PLUGIN_LV2_DUMP_TTL
//#define KODE_MAX_PATH_SIZE 512

//----------

#include "base/kode.h"

#include <lv2.h>
//#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/event/event-helpers.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/uri-map/uri-map.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

//----------

#ifdef KODE_DEBUG_LV2
  #define LV2_Trace    KODE_Trace
  #define LV2_DTrace   KODE_DTrace
  #define LV2_TRACE    KODE_TRACE
#else
  #define LV2_Trace    KODE_NoTrace
  #define LV2_DTrace   KODE_NoTrace
  #define LV2_TRACE    {}
#endif

#ifdef KODE_DEBUG_LV2_RT
  #define LV2_Trace_RT  KODE_Trace
  #define LV2_DTrace_RT KODE_DTrace
  #define LV2_TRACE_RT  KODE_TRACE
#else
  #define LV2_Trace_RT  KODE_NoTrace
  #define LV2_DTrace_RT KODE_NoTrace
  #define LV2_TRACE_RT  {}
#endif

//----------------------------------------------------------------------
#endif
