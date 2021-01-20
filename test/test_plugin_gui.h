
#define KODE_GUI_XCB
#define KODE_PAINTER_CAIRO
//#define KODE_NO_WINDOW_BUFFERING

//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

#define EDITOR_WIDTH  256
#define EDITOR_HEIGHT 256

uint8_t myPluginId[16]  = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint8_t myEditorId[16]  = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

public:

  myDescriptor() {
    KODE_PRINT;

    #ifdef KODE_DEBUG
    MName             = "test_plugin1 (debug)";
    #else
    MName             = "test_plugin1";
    #endif

    MAuthor           = "skei.audio";
    MLongId           = myPluginId;
    MLongEditorId     = myEditorId;
    MNumInputs        = 2;
    MNumOutputs       = 2;
    MIsSynth          = false;
    MCanSendMidi      = false;
    MCanReceiveMidi   = true;
    MHasEditor        = true;
    MCanResizeEditor  = false;
    MEditorWidth      = EDITOR_WIDTH;
    MEditorHeight     = EDITOR_HEIGHT;
    appendParameter( KODE_New KODE_Parameter("param1", 0.1f) );
    appendParameter( KODE_New KODE_Parameter("param2", 0.2f) );
    appendParameter( KODE_New KODE_Parameter("param3", 0.3f) );
  }

  virtual ~myDescriptor() {
    KODE_PRINT;
  }

};

//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    KODE_PRINT;
  }

  virtual ~myInstance() {
    KODE_PRINT;
  }

public:

  void on_create() final {
    KODE_PRINT;
  }

  void on_destroy() final {
    KODE_PRINT;
  }

  void on_initialize() final {
    KODE_PRINT;
  }

  void on_terminate() final {
    KODE_PRINT;
  }

  void on_activate() final {
    KODE_PRINT;
  }

  void on_deactivate() final {
    KODE_PRINT;
  }

  void on_prepare(float ASampleRate) final {
    KODE_Print("samplerate %.2f\n");
  }

  void on_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    KODE_Print("offset %i msg1 %i msg2 %i msg3 %i\n",AOffset,AMsg1,AMsg2,AMsg3);
  }

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    KODE_Print("index %i value %.2f (mode %i)\n",AIndex,AValue,AMode);
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {
    //KODE_PRINT;
  }

  KODE_Editor* on_openEditor(void* AParent) final {
    KODE_PRINT;
    KODE_Editor* editor = KODE_New KODE_Editor(MDescriptor,this,AParent);
    KODE_PanelWidget* panel;
    panel = KODE_New KODE_PanelWidget( KODE_FRect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT) );
    panel->setBackgroundColor(0xff606060);
    editor->appendChildWidget(panel);
    KODE_Widget* widget;
    widget = KODE_New KODE_SliderWidget(KODE_FRect(10,10,236,25));
    panel->appendChildWidget(widget);
    editor->connectParameterIndex(widget,0);
    widget = KODE_New KODE_SliderWidget(KODE_FRect(10,40,236,25));
    panel->appendChildWidget(widget);
    editor->connectParameterIndex(widget,1);
    widget = KODE_New KODE_SliderWidget(KODE_FRect(10,70,236,25));
    panel->appendChildWidget(widget);
    editor->connectParameterIndex(widget,2);
    return editor;
  }

  void on_closeEditor(KODE_Editor* AEditor) final {
    KODE_PRINT;
    KODE_Delete AEditor;
  }

  void on_updateEditor(KODE_Editor* AEditor) final {
    //KODE_PRINT;
  }

  uint32_t on_saveState(void** ABuffer, uint32_t AMode) final {
    KODE_PRINT;
    *ABuffer = KODE_NULL;
    return 0;
  }

  void on_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
    KODE_PRINT;
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
