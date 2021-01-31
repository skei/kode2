#ifndef kode_instance_included
#define kode_instance_included
//----------------------------------------------------------------------

#include "kode.h"

//----------

#ifdef KODE_PLUGIN_EXE
  #include "plugin/exe/kode_exe_instance.h"
  typedef KODE_ExeInstance KODE_ImplementedInstance;
#endif

//----------

#ifdef KODE_PLUGIN_VST3
  #include "plugin/vst3/kode_vst3_instance.h"
  typedef KODE_Vst3Instance KODE_ImplementedInstance;
#endif

//----------------------------------------------------------------------
#endif
