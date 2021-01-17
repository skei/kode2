
//#define KODE_DEBUG_EXE
//#define KODE_DEBUG_VST3
//#define KODE_DEBUG_XCB

//#define KODE_CAIRO
//#define KODE_CTX
#define KODE_XCB
#define KODE_GUI_XCB

//#define KODE_NO_WINDOW_BUFFERING

//#define KODE_DEBUG_PRINT_TIME
//#define KODE_DEBUG_PRINT_THREAD
//#define KODE_DEBUG_PRINT_SOCKET

//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

uint8_t myPluginId[16]  = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint8_t myEditorId[16]  = {15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

class myDescriptor
: public KODE_Descriptor {

public:

  myDescriptor() {
    //KODE_PRINT;
    MName             = "test_plugin1";
    MAuthor           = "skei.audio";
    MLongId           = myPluginId;
    MLongEditorId     = myEditorId;
    MNumInputs        = 2;
    MNumOutputs       = 2;
    MIsSynth          = false;
    MCanSendMidi      = false;
    MCanReceiveMidi   = true;
    MHasEditor        = true;
    MCanResizeEditor  = true;
    MEditorWidth      = 640;
    MEditorHeight     = 480;
    appendParameter( KODE_New KODE_Parameter("param1", 0.25f) );
    //appendParameter( KODE_New KODE_Parameter("param2", 0.5f) );
    //appendParameter( KODE_New KODE_Parameter("param3", 1.0f) );
  }

  virtual ~myDescriptor() {
  }

};

//----------------------------------------------------------------------

class myEditor
: public KODE_Editor {

public:

  myEditor(KODE_Descriptor* ADescriptor, KODE_EditorListener* AListener, void* AParent)
  : KODE_Editor(ADescriptor,AListener,AParent) {
    //KODE_PRINT;
    setFillBackground();
    KODE_Widget* widget;
    //widget = KODE_New KODE_WidgetTemplate( KODE_FRect(10,10,100,100) );
    widget = KODE_New KODE_ValueWidget( KODE_FRect(10,20,128,16) );
    connectParameterIndex(widget,0);
    appendChildWidget(widget);
    //widget = KODE_New KODE_WidgetTemplate( KODE_FRect(120,10,100,100) );
    //connectParameterIndex(widget,1);
    //appendChildWidget(widget);
  }

  virtual ~myEditor() {
    //KODE_PRINT;
  }

};

//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_PRINT;
  }

  virtual ~myInstance() {
    //KODE_PRINT;
  }

public:

  void on_create() final {
    //KODE_PRINT;
  }

  void on_destroy() final {
    //KODE_PRINT;
  }

  void on_initialize() final {
    //KODE_PRINT;
  }

  void on_terminate() final {
    //KODE_PRINT;
  }

  void on_activate() final {
    //KODE_PRINT;
  }

  void on_deactivate() final {
    //KODE_PRINT;
  }

  void on_prepare(float ASampleRate) final {
    //KODE_Print("samplerate %.2f\n");
  }

  void on_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    //KODE_Print("offset %i msg1 %i msg2 %i msg3 %i\n",AOffset,AMsg1,AMsg2,AMsg3);
  }

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    //KODE_Print("index %i value %.2f (mode %i)\n",AIndex,AValue,AMode);
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {
    //KODE_PRINT;
  }

  KODE_Editor* on_openEditor(void* AParent) final {
    //KODE_PRINT;
    return KODE_New myEditor(MDescriptor,this,AParent);
  }

  void on_closeEditor(KODE_Editor* AEditor) final {
    //KODE_PRINT;
    KODE_Delete AEditor;
  }

  void on_updateEditor(KODE_Editor* AEditor) final {
    //KODE_PRINT;
  }

  uint32_t on_saveState(void** ABuffer, uint32_t AMode) final {
    //KODE_PRINT;
    *ABuffer = KODE_NULL;
    return 0;
  }

  void on_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
    //KODE_PRINT;
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
