#ifndef kode_plugin_base_included
#define kode_plugin_base_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_parameter.h"
#include "plugin/kode_program.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

class KODE_BaseInstance;
class KODE_BaseEditor;

//----------------------------------------------------------------------

class KODE_BaseInstance {
public:
  KODE_BaseInstance(KODE_Descriptor* ADescriptor) {}
  virtual ~KODE_BaseInstance() {}
public:
  virtual void on_open() {}
  virtual void on_close() {}
  virtual void on_initialize() {}
  virtual void on_terminate() {}
  virtual void on_activate() {}
  virtual void on_deactivate() {}
  virtual void on_prepare(float ASamplerate, uint32_t ABlocksize) {}
  virtual void on_parameter(uint32_t AIndex, float AValue, uint32_t AOffset=0, uint32_t AMode=0) {}
  virtual void on_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AOffset=0, uint32_t AMode=0) {}
  virtual void on_processBlock(KODE_ProcessContext* AContext) {}
  virtual KODE_BaseEditor* on_openEditor(void* AParent) { return KODE_NULL; }
  virtual void on_closeEditor(KODE_BaseEditor* AEditor) {}
  virtual void on_updateEditor(KODE_BaseEditor* AEditor) {}
public:
  //virtual void  setParameterValue(uint32_t AIndex, float AValue) {}
  //virtual float getParameterValue(uint32_t AIndex) { return 0.0f; }
  virtual void  setDefaultParameterValues() {}
  virtual void  updateAllParameters() {}
  virtual void  updateParameterFromEditor(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------

class KODE_BaseEditor {
public:
  KODE_BaseEditor(KODE_BaseInstance* AInstance) {}
  virtual ~KODE_BaseEditor() {}
public:
  virtual void updateParameterFromHost(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------
#endif

