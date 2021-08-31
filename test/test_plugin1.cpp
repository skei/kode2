
//#define KODE_NO_GUI
#define KODE_GUI_XCB

#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"
#include "plugin/kode_plugin.h"
//#include "plugin/kode_editor.h"
#include "gui/kode_widgets.h"

//----------

#define KODE_PLUGIN_MESSAGE_QUEUE_SIZE 32
typedef KODE_Queue<uint32_t,KODE_PLUGIN_MESSAGE_QUEUE_SIZE> KODE_Uint32Queue;

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {

    #ifdef KODE_DEBUG
      setName("kode_debug");
    #else
      setName("kode_release");
    #endif

    setAuthor("skei.audio");
    setVersion(0x00000001);

    setHasEditor();
    setEditorSize(400,300);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    KODE_Parameter* parameter;
    parameter = appendParameter( new KODE_Parameter("param1",0.2f) );
    parameter = appendParameter( new KODE_Parameter("param2",0.7f) );
    parameter->setLabel("db");
    parameter = appendParameter( new KODE_Parameter("param3",0.4f) );
    parameter->setLabel("%");

  }
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

class myEditor
: public KODE_Editor {

//------------------------------
public:
//------------------------------

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setFillBackground(true);
    setBackgroundColor(0.5f);

    KODE_ValueWidget* widget;
    widget = (KODE_ValueWidget*)appendWidget(new KODE_ValueWidget( KODE_FRect(10,10,150,20) ));
    connectParameter(widget,0);

    widget = (KODE_ValueWidget*)appendWidget(new KODE_ValueWidget( KODE_FRect(10,35,150,20) ));
    widget->setDrawLabel(true);
    connectParameter(widget,1);

    KODE_SliderWidget* slider;
    slider = (KODE_SliderWidget*)appendWidget(new KODE_SliderWidget( KODE_FRect(10,60,150,20) ));
    slider->setValueBarDirection(KODE_RIGHT);
    slider->setDrawLabel(true);
    connectParameter(slider,2);

  }

  //----------

  //virtual ~myEditor() {
  //}

};

#endif

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  //KODE_Uint32Queue  MProcessMessageQueue;
  //KODE_Uint32Queue  MGuiMessageQueue;

  //bool MNeedRecalc = false;
  //#ifndef KODE_NO_GUI
  //KODE_BaseEditor* MEditor = KODE_NULL;
  //#endif

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
private:
//------------------------------

  //void queueProcessMessage(uint32_t AMessage) {
  //  MProcessMessageQueue.write(AMessage);
  //}

  //----------

  //void queueGuiMessage(uint32_t AMessage) {
  //  MGuiMessageQueue.write(AMessage);
  //}

  //----------

  //void flushProcessMessages() {
  //  uint32_t message = 0;
  //  while (MProcessMessageQueue.read(&message)) {
  //   //KODE_Print("%i\n",message);
  //  }
  //}

  //----------

  //void flushGuiMessages(KODE_BaseEditor* AEditor) {
  //  uint32_t message = 0;
  //  while (MGuiMessageQueue.read(&message)) {
  //    //KODE_Print("%i\n",message);
  //  }
  //}

//------------------------------
public:
//------------------------------

  //void on_plugin_open() final {
  //}

  //----------

  //void on_plugin_close() final {
  //}

  //----------

  //void on_plugin_initialize() final {
  //}

  //----------

  //void on_plugin_terminate() final {
  //}

  //----------

  //void on_plugin_activate() final {
  //}

  //----------

  //void on_plugin_deactivate() final {
  //}

  //----------

  //void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
  //}

  //----------

  //void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
  //}

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //MNeedRecalc = true;
    //queueProcessMessage(AIndex);
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    //flushProcessMessages();
    //if (MNeedRecalc) {
    //  recalc(AContext);
    //  MNeedRecalc = false;
    //}
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      //processSample(AContext,spl0,spl1);
      *out0++ = spl0;
      *out1++ = spl1;
    }
    //#ifndef KODE_NO_GUI
    //if (MEditor) {
    //  queueGuiMessage(0);
    //}
    //#endif
  }

  //----------

  //uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {
  //  *ABuffer = KODE_NULL;
  //  return 0;
  //}

  //----------

  //void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
  //}

  //----------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    KODE_Print("parent %p\n",AParent);
    myEditor* editor = new myEditor(this,AParent);
    return editor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    KODE_Print("\n");
    if (AEditor) {
      delete (myEditor*)AEditor;
    }
  }

  //----------

  //void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
  //  if (AEditor) {
  //    flushGuiMessages(AEditor);
  //  }
  //}

  #endif

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
