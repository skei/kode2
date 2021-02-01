#ifndef kode_exe_instance_included
#define kode_exe_instance_included
//----------------------------------------------------------------------

#include "plugin/kode_plugin_base.h"

//----------------------------------------------------------------------

class KODE_ExeInstance
: public KODE_BaseInstance {

//------------------------------
public:
//------------------------------

  KODE_ExeInstance(KODE_Descriptor* ADescriptor)
  : KODE_BaseInstance(ADescriptor) {
  }

  //----------

  virtual ~KODE_ExeInstance() {
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
