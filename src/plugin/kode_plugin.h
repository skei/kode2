#ifndef kode_plugin_included
#define kode_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

#ifdef KODE_PLUGIN_EXE
  #include "plugin/exe/kode_exe_plugin.h"
  #define KODE_PLUGIN_ENTRYPOINT KODE_EXE_PLUGIN_ENTRYPOINT
#endif

#ifdef KODE_PLUGIN_VST2
  #include "plugin/vst2/kode_vst2_plugin.h"
  #define KODE_PLUGIN_ENTRYPOINT KODE_VST2_PLUGIN_ENTRYPOINT
#endif

#ifdef KODE_PLUGIN_VST3
  #include "plugin/vst3/kode_vst3_plugin.h"
  #define KODE_PLUGIN_ENTRYPOINT KODE_VST3_PLUGIN_ENTRYPOINT
#endif


//----------------------------------------------------------------------
#endif
