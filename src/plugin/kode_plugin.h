#ifndef kode_plugin_included
#define kode_plugin_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"

#ifndef KODE_NO_GUI
#include "plugin/kode_editor.h"
#endif

//----------------------------------------------------------------------

#ifdef KODE_PLUGIN_EXE
  #include "plugin/exe/kode_exe_plugin.h"
  #define KODE_PLUGIN_ENTRYPOINT KODE_EXE_ENTRYPOINT
#endif

//----------

#ifdef KODE_PLUGIN_LV2
  #include "plugin/lv2/kode_lv2_plugin.h"
  #define KODE_PLUGIN_ENTRYPOINT KODE_LV2_ENTRYPOINT
#endif
//----------

#ifdef KODE_PLUGIN_VST3
  #include "plugin/vst3/kode_vst3_plugin.h"
  #define KODE_PLUGIN_ENTRYPOINT KODE_VST3_ENTRYPOINT
#endif

//----------------------------------------------------------------------
#endif
