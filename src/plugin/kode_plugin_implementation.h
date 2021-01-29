#ifndef kode_plugin_implementation_included
#define kode_plugin_implementation_included
//----------------------------------------------------------------------

#include "kode.h"

//----------

#ifdef KODE_PLUGIN_EXE
  //#include "plugin/exe/kode_exe.h"
  //#include "plugin/exe/kode_exe_utils.h"
  #include "plugin/exe/kode_exe_instance.h"
  #include "plugin/exe/kode_exe_plugin.h"
  typedef KODE_ExeInstance KODE_ImplementedInstance;
#endif

//----------

#ifdef KODE_PLUGIN_VST3
  //#include "plugin/vst3/kode_vst3.h"
  //#include "plugin/vst3/kode_vst3_utils.h"
  #include "plugin/vst3/kode_vst3_instance.h"
  #include "plugin/vst3/kode_vst3_plugin.h"
  typedef KODE_Vst3Instance KODE_ImplementedInstance;
#endif

//----------------------------------------------------------------------
#endif
