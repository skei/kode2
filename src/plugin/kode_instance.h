#ifndef kode_instance_included
#define kode_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
//#include "plugin/kode_editor.h"
//#include "plugin/kode_descriptor.h"
//#include "plugin/kode_plugin_event.h"
//#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------
//
// implemented instance
//
//----------------------------------------------------------------------

#ifdef KODE_PLUGIN_EXE
  #include "plugin/exe/kode_exe_instance.h"
  typedef KODE_ExeInstance KODE_ImplementedInstance;
#endif

#ifdef KODE_PLUGIN_VST2
  #include "plugin/vst2/kode_vst2_instance.h"
  typedef KODE_Vst2Instance KODE_ImplementedInstance;
#endif

#ifdef KODE_PLUGIN_VST3
  #include "plugin/vst3/kode_vst3_instance.h"
  typedef KODE_Vst3Instance KODE_ImplementedInstance;
#endif

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

typedef KODE_ImplementedInstance KODE_Instance;

//----------

/*

class KODE_Instance
: public KODE_ImplementedInstance {

//------------------------------
private:
//------------------------------


//------------------------------
public:
//------------------------------

  KODE_Instance(KODE_Descriptor* ADescriptor)
  : KODE_ImplementedInstance(ADescriptor) {
  }

  //----------

  virtual ~KODE_Instance() {
  }

};

*/

//----------------------------------------------------------------------
#endif
