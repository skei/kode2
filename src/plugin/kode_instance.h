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

class KODE_Instance
: public KODE_ImplementedInstance {

//------------------------------
private:
//------------------------------

  uint32_t  MNumParameters    = 0;
  float*    MParameterValues  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Descriptor*  MDescriptor = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Instance() {
    KODE_Assert(0==1); // use the other constructor!
  }


  KODE_Instance(KODE_Descriptor* ADescriptor)
  : KODE_ImplementedInstance(/*ADescriptor*/) {
    setDescriptor(ADescriptor);
    MDescriptor       = ADescriptor;
    MNumParameters    = MDescriptor->getNumParameters();
    MParameterValues  = (float*)KODE_Malloc(MNumParameters * sizeof(float));
  }

  //----------

  virtual ~KODE_Instance() {
    if (MParameterValues) KODE_Free(MParameterValues);
  }

//------------------------------
public:
//------------------------------

  void setDefaultParameterValues() override {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float value = parameter->getDefValue();
      MParameterValues[i] = value;
    }
  }

  //----------

  void updateAllParameters() override {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float value = MParameterValues[i];
      float v = parameter->from01(value);
      on_plugin_parameter(i,v);
      // if editor is open ...
    }
  }

  //----------

  //void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
  //  // set parameter value
  //  // notify host
  //}

};


//----------------------------------------------------------------------
#endif
