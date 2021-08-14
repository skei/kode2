#ifndef kode_exe_instance_included
#define kode_exe_instance_included
//----------------------------------------------------------------------

//#include "plugin/kode_plugin_base.h"
#ifndef KODE_NO_GUI
  #include "plugin/base/kode_base_editor.h"
#endif

#include "plugin/base/kode_base_instance.h"

//----------------------------------------------------------------------

class KODE_ExeInstance
: public KODE_BaseInstance {



//------------------------------
private:
//------------------------------

  KODE_Descriptor* MDescriptor = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_ExeInstance(KODE_Descriptor* ADescriptor)
  : KODE_BaseInstance(ADescriptor) {
    MDescriptor = ADescriptor;
  }

  //----------

  virtual ~KODE_ExeInstance() {
  }

//------------------------------
public:
//------------------------------

  //void  setDescriptor(KODE_Descriptor* ADescriptor) override {
  //  MDescriptor = ADescriptor;
  //}

  KODE_Descriptor* getDescriptor() override {
    return MDescriptor;
  }

//------------------------------
public:
//------------------------------

//  void setDefaultParameterValues() override {
//    uint32_t num = MDescriptor->getNumParameters();
//    for (uint32_t i=0; i<num; i++) {
//      KODE_Parameter* parameter = MDescriptor->getParameter(i);
//      float value = parameter->getDefValue();
//      MParameterValues[i] = value;
//    }
//  }

  //----------

  void updateAllParameters() override {
    //KODE_PRINT;
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      //float v = MParameterValues[i];
      float v = parameter->getDefValue();
      v = parameter->from01(v);
      on_plugin_parameter(0,i,v);
      // if editor is open ...
    }
  }

  //----------

  #ifndef KODE_NO_GUI
  void updateAllEditorParameters(KODE_BaseEditor* AEditor, bool ARedraw=true) override {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float v = parameter->getDefValue();
      //v = parameter->from01(v);
      AEditor->updateParameterFromHost(i,v,ARedraw);
    }
  }
  #endif

  //----------

  #ifndef KODE_NO_GUI
  void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
    //if (MParameterValues[AIndex] != AValue) {
    //  MParameterValues[AIndex] = AValue;
      KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
      float v = parameter->from01(AValue);
      on_plugin_parameter(0,AIndex,v,0);
    //}
  }
  #endif

//------------------------------
private:
//------------------------------

//  void createParameterBuffers() {
//    uint32_t size = MDescriptor->getNumParameters() * sizeof(float);
//    //MNumParameters = MDescriptor->getNumParameters();
//    MParameterValues        = (float*)KODE_Malloc(size);
//    //MEditorParameterValues  = (float*)KODE_Malloc(size);
//    MHostParameterValues    = (float*)KODE_Malloc(size);
//    KODE_Memset(MParameterValues,       0,size);
//    //KODE_Memset(MEditorParameterValues, 0,size);
//    KODE_Memset(MHostParameterValues,   0,size);
//  }
//
//  //----------
//
//  void destroyParameterBuffers() {
//    if (MParameterValues)       KODE_Free(MParameterValues);
//    //if (MEditorParameterValues) KODE_Free(MEditorParameterValues);
//    if (MHostParameterValues)   KODE_Free(MHostParameterValues);
//  }
//
//  //----------

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
