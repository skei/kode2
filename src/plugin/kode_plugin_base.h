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
//public:
//  KODE_BaseInstance(KODE_Descriptor* ADescriptor) {}
//  virtual ~KODE_BaseInstance() {}
public:
  virtual void setDescriptor(KODE_Descriptor* ADescriptor) {}
  virtual KODE_Descriptor* getDescriptor() { return KODE_NULL; }
public:
  virtual void      setDefaultParameterValues() {}
  virtual void      updateAllParameters() {}
  virtual void      updateParameterFromEditor(uint32_t AIndex, float AValue) {}
public:
  virtual void      on_plugin_open(/*KODE_Descriptor* ADescriptor*/) {}
  virtual void      on_plugin_close() {}
  virtual void      on_plugin_initialize() {}
  virtual void      on_plugin_terminate() {}
  virtual void      on_plugin_activate() {}
  virtual void      on_plugin_deactivate() {}
  virtual void      on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) {}
  virtual void      on_plugin_parameter(uint32_t AIndex, float AValue, uint32_t AMode=0/*, uint32_t AOffset=0*/) {}
  virtual void      on_plugin_midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AOffset=0, uint32_t AMode=0) {}
  virtual void      on_plugin_processBlock(KODE_ProcessContext* AContext) {}
  virtual void*     on_plugin_openEditor(void* AParent) { return KODE_NULL; }
  virtual void      on_plugin_closeEditor(void* AEditor) {}
  virtual void      on_plugin_updateEditor(void* AEditor) {}
  virtual uint32_t  on_plugin_saveState(void** ABuffer, uint32_t AMode) { *ABuffer = KODE_NULL; return 0; }
  virtual void      on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) {}
};

//----------------------------------------------------------------------

class KODE_BaseEditor {
//public:
//  KODE_BaseEditor(KODE_BaseInstance* AInstance) {}
//  virtual ~KODE_BaseEditor() {}
public:
  virtual void setInstance(KODE_BaseInstance* AInstance) {}
  virtual void updateParameterFromHost(uint32_t AIndex, float AValue) {}
};

//----------------------------------------------------------------------
#endif

