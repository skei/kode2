#ifndef kode_base_instance_included
#define kode_base_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor.h"
#include "plugin/kode_process_context.h"

#ifndef KODE_NO_GUI
  #include "plugin/kode_editor.h"
#endif

//----------------------------------------------------------------------

class KODE_BaseInstance {

//------------------------------
protected:
//------------------------------

  KODE_Descriptor*  MDescriptor       = KODE_NULL;
  float*            MParameterValues  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_BaseInstance(KODE_Descriptor* ADescriptor) {
    MDescriptor   = ADescriptor;
    createParameterBuffers();
  }

  //----------

  virtual ~KODE_BaseInstance() {
    destroyParameterBuffers();
  }

//------------------------------
public:
//------------------------------

  virtual void          on_create() {}
  virtual void          on_destroy() {}
  virtual void          on_initialize() {}
  virtual void          on_terminate() {}
  virtual void          on_activate() {}
  virtual void          on_deactivate() {}
  virtual void          on_prepare(float ASampleRate) {}
  virtual void          on_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
  virtual void          on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) {}
  virtual void          on_processBlock(KODE_ProcessContext* AContext) {}
  virtual KODE_Editor*  on_openEditor(void* AParent=KODE_NULL) { return KODE_NULL; }
  virtual void          on_closeEditor(KODE_Editor* AEditor) {}
  virtual void          on_updateEditor(KODE_Editor* AEditor) {}
  virtual uint32_t      on_saveState(void** ABuffer, uint32_t AMode) { *ABuffer = KODE_NULL; return 0; }
  virtual void          on_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) {}

//------------------------------
private:
//------------------------------

  void createParameterBuffers() {
    uint32_t num = MDescriptor->getNumParameters();
    MParameterValues = (float*)KODE_Malloc(num * sizeof(float));
  }

  //----------

  void destroyParameterBuffers() {
    KODE_Free(MParameterValues);
  }

//------------------------------
public:
//------------------------------

  void setDefaultParameterValues() {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float value = parameter->getDefValue();
      MParameterValues[i] = value;
    }
  }

  //----------

  void notifyAllParameters() {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float v = MParameterValues[i];
      v = parameter->from01(v);
      on_parameter(i,v,KODE_PARAMETER_UPDATE_ALL);
    }
  }

};


//----------------------------------------------------------------------
#endif
