#ifndef kode_plugin_base_included
#define kode_plugin_base_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_parameter.h"
#include "plugin/kode_program.h"
#include "plugin/kode_process_context.h"

#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_BaseInstance;
class KODE_BaseEditor;

//----------------------------------------------------------------------

class KODE_BaseInstance {
public:
  KODE_BaseInstance(KODE_Descriptor* ADescriptor) {}
  virtual ~KODE_BaseInstance() {}
public:
  virtual KODE_Descriptor*  getDescriptor() { return KODE_NULL; }
public:
  virtual void              setDefaultParameterValues() {}
  virtual void              updateAllParameters() {}
  virtual void              updateAllEditorParameters(KODE_BaseEditor* AEditor, bool ARedraw=true) {}
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
  virtual void              on_plugin_process(KODE_ProcessContext* AContext) {}
  virtual uint32_t          on_plugin_saveState(void** ABuffer, uint32_t AMode) { *ABuffer = KODE_NULL; return 0; }
  virtual void              on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) {}
  virtual KODE_BaseEditor*  on_plugin_openEditor(void* AParent) { return KODE_NULL; }
  virtual void              on_plugin_closeEditor(KODE_BaseEditor* AEditor) {}
  virtual void              on_plugin_updateEditor(KODE_BaseEditor* AEditor) {}
};

//----------------------------------------------------------------------

class KODE_BaseEditor {
public:
  KODE_BaseEditor(KODE_BaseInstance* AInstance) {}
  virtual ~KODE_BaseEditor() {}
public:
  virtual void              connectParameter(KODE_Widget* AWidget, uint32_t AParameter, uint32_t AParamIndex=0) {}
  virtual void              updateParameterFromHost(uint32_t AIndex, float AValue, bool ARedraw=true) {}
};

//----------------------------------------------------------------------
#endif

