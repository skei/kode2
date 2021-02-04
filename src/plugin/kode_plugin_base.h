#ifndef kode_plugin_base_included
#define kode_plugin_base_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_parameter.h"
#include "plugin/kode_program.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

class KODE_IInstance;
class KODE_IEditor;

//----------------------------------------------------------------------

class KODE_IInstance {
//public:
//  KODE_IInstance(KODE_Descriptor* ADescriptor) {}
//  virtual ~KODE_IInstance() {}
public:
  virtual void              setDescriptor(KODE_Descriptor* ADescriptor) {}
  virtual KODE_Descriptor*  getDescriptor() { return KODE_NULL; }
protected:
  virtual void              setDefaultParameterValues() {}
  virtual void              updateAllParameters() {}
  virtual void              updateAllEditorParameters(KODE_IEditor* AEditor) {}
  virtual void              updateParameterFromEditor(uint32_t AIndex, float AValue) {}
public:
  virtual void              on_plugin_open() {}
  virtual void              on_plugin_close() {}
  virtual void              on_plugin_initialize() {}
  virtual void              on_plugin_terminate() {}
  virtual void              on_plugin_activate() {}
  virtual void              on_plugin_deactivate() {}
  virtual void              on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) {}
  virtual void              on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) {}
  virtual void              on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) {}
  virtual void              on_plugin_processBlock(KODE_ProcessContext* AContext) {}
  virtual uint32_t          on_plugin_saveState(void** ABuffer, uint32_t AMode) { *ABuffer = KODE_NULL; return 0; }
  virtual void              on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) {}
  virtual KODE_IEditor*     on_plugin_openEditor(void* AParent) { return KODE_NULL; }
  virtual void              on_plugin_closeEditor(KODE_IEditor* AEditor) {}
  virtual void              on_plugin_updateEditor(KODE_IEditor* AEditor) {}
};

//----------------------------------------------------------------------

class KODE_IEditor {
//public:
//  KODE_IEditor(KODE_IInstance* AInstance) {}
//  virtual ~KODE_IEditor() {}
//public:
//  virtual void              setInstance(KODE_IInstance* AInstance) {}
public:
  virtual void              connectParameter(KODE_Widget* AWidget, uint32_t AParameter) {}
  virtual void              updateParameterFromHost(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------
#endif

