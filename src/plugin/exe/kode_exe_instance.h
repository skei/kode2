#ifndef kode_exe_instance_included
#define kode_exe_instance_included
//----------------------------------------------------------------------

#include "plugin/kode_plugin_base.h"

//----------------------------------------------------------------------

class KODE_ExeInstance
: public KODE_IInstance {



//------------------------------
private:
//------------------------------

  KODE_Descriptor* MDescriptor = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_ExeInstance(/*KODE_Descriptor* ADescriptor*/) {
  }

  //----------

  virtual ~KODE_ExeInstance() {
  }

//------------------------------
public:
//------------------------------

  void  setDescriptor(KODE_Descriptor* ADescriptor) override {
    MDescriptor = ADescriptor;
  }

  KODE_Descriptor* getDescriptor() override {
    return MDescriptor;
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
