
#define KODE_GUI_XCB

//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_plugin.h"
#include "plugin/vst3/kode_vst3_instance.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {
    KODE_PRINT;
    #ifdef KODE_DEBUG
      setName("plugin_debug");
    #else
      setName("plugin");
    #endif
    setAuthor("author");
    setVersion(0x0101001);
    KODE_Parameter* parameter;
    parameter = appendParameter( KODE_New KODE_Parameter("param1",0.3f) );
    parameter->setLabel("db");
    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,480);
    #endif
  }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

class myEditor
: public KODE_Editor {

//------------------------------
public:
//------------------------------

  myEditor(KODE_IInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {
    KODE_PRINT;
    setFillBackground(true);
    setBackgroundColor(0.5f);
    KODE_ValueWidget* widget;
    widget = (KODE_ValueWidget*)appendWidget(KODE_New KODE_ValueWidget( KODE_FRect(10,10,150,20) ));
    widget->setDrawLabel(true);
    connectParameter(widget,0);
  }

  virtual ~myEditor() {
    KODE_PRINT;
  }

};

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    KODE_PRINT;
  }

  virtual ~myInstance() {
    KODE_PRINT;
  }

//------------------------------
public:
//------------------------------

  void on_plugin_open() final {
    KODE_PRINT;
  }

  void on_plugin_close() final {
    KODE_PRINT;
  }

  void on_plugin_initialize() final {
    KODE_PRINT;
  }

  void on_plugin_terminate() final {
    KODE_PRINT;
  }

  void on_plugin_activate() final {
    KODE_PRINT;
  }

  void on_plugin_deactivate() final {
    KODE_PRINT;
  }

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    KODE_Print("samplerate %.3f blocksize %i\n",ASamplerate,ABlocksize);
  }

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    KODE_Print("offset %i index %i value %.3f mode %i\n",AOffset,AIndex,AValue,AMode);
  }

  void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
    KODE_Print("offset %i msg1 %i msg2 %i msg3 %i mode %i\n",AOffset,AMsg1,AMsg2,AMsg3,AMode);
  }

  void on_plugin_processBlock(KODE_ProcessContext* AContext) final {
    //KODE_PRINT;
  }

  uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {
    KODE_PRINT;
    *ABuffer = KODE_NULL;
    return 0;
  }

  void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
    KODE_PRINT;
  }

  #ifndef KODE_NO_GUI

  KODE_IEditor* on_plugin_openEditor(void* AParent) final {
    KODE_PRINT;
    myEditor* editor = (myEditor*)KODE_New myEditor(this,AParent);
    return editor;
  }

  void  on_plugin_closeEditor(KODE_IEditor* AEditor) final {
    KODE_PRINT;
    KODE_Delete (myEditor*)AEditor;
  }

  void on_plugin_updateEditor(KODE_IEditor* AEditor) final {
    //KODE_PRINT;
  }

  #endif

//------------------------------

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
