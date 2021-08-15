
#define KODE_GUI_XCB
#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"

#include "plugin/kode_plugin.h"
#include "gui/kode_widgets.h"

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
      setName("plugin_debug");
    #else
      setName("plugin");
    #endif

    setAuthor("author");
    setVersion(0x0101001);

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    KODE_Parameter* parameter;
    parameter = appendParameter( KODE_New KODE_Parameter("param1",0.2f) );
    parameter = appendParameter( KODE_New KODE_Parameter("param2",0.7f) );
    parameter->setLabel("db");
    parameter = appendParameter( KODE_New KODE_Parameter("param3",0.4f) );
    parameter->setLabel("%");

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,480);
    #endif

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
    widget = (KODE_ValueWidget*)appendWidget(KODE_New KODE_ValueWidget( KODE_FRect(10,10,150,20) ));
    connectParameter(widget,0);

    widget = (KODE_ValueWidget*)appendWidget(KODE_New KODE_ValueWidget( KODE_FRect(10,35,150,20) ));
    widget->setDrawLabel(true);
    connectParameter(widget,1);

    KODE_SliderWidget* slider;
    slider = (KODE_SliderWidget*)appendWidget(KODE_New KODE_SliderWidget( KODE_FRect(10,60,150,20) ));
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

  myEditor*         MEditor       = KODE_NULL;
  bool              MNeedRecalc   = false; // true;
  uint32_t          MCounter      = 0;

  KODE_Uint32Queue  MProcessMessageQueue;
  KODE_Uint32Queue  MGuiMessageQueue;

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
public:
//------------------------------

  void on_plugin_open() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_close() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_initialize() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_terminate() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_activate() final {
    KODE_Print("\n");
    MCounter = 0;
  }

  //----------

  void on_plugin_deactivate() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    KODE_Print("sr %.3f bs %i\n",ASamplerate,ABlocksize);
  }

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("ofs %i idx %i val %.3f mode %i\n",AOffset,AIndex,AValue,AMode);
    MNeedRecalc = true;
    queueProcessMessage(AIndex);
  }

  //----------

  void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
    //KODE_Print("ofs %i msg %02x %02x %02x mode %i\n",AOffset,AMsg1,AMsg2,AMsg3,AMode);
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    flushProcessMessages();
    if (MNeedRecalc) {
      recalc(AContext);
      MNeedRecalc = false;
    }
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      processSample(AContext,spl0,spl1);
      *out0++ = spl0;
      *out1++ = spl1;
    }
    #ifndef KODE_NO_GUI
    if (MEditor) {
      queueGuiMessage(MCounter);
    }
    #endif
    MCounter += 1;
  }

  //----------

  uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {
    //KODE_Print("\n");
    *ABuffer = KODE_NULL;
    return 0;
  }

  //----------

  void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
    //KODE_Print("\n");
  }

  //----------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    KODE_Print("parent %p\n",AParent);
    //myEditor* editor = (myEditor*)KODE_New myEditor(this,AParent);
    //return editor;
    MEditor = KODE_New myEditor(this,AParent);
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    KODE_Print("\n");
    KODE_Assert(AEditor == MEditor);
    if (MEditor) {
      //KODE_Delete (myEditor*)AEditor;
      KODE_Delete MEditor;
      MEditor = KODE_NULL;
    }
  }

  //----------

  void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Print("\n");
    KODE_Assert(AEditor == MEditor);
    if (MEditor) {
      flushGuiMessages();
    }
  }

  #endif

//------------------------------
private:
//------------------------------

  void queueProcessMessage(uint32_t AMessage) {
    MProcessMessageQueue.write(AMessage);
  }

  //----------

  void queueGuiMessage(uint32_t AMessage) {
    MGuiMessageQueue.write(AMessage);
  }

  //----------

  void flushProcessMessages() {
    uint32_t message = 0;
    while (MProcessMessageQueue.read(&message)) {
      //KODE_Print("%i\n",message);
    }
  }

  //----------

  void flushGuiMessages() {
    uint32_t message = 0;
    while (MGuiMessageQueue.read(&message)) {
      //KODE_Print("%i\n",message);
    }
  }

  //----------

  void recalc(KODE_ProcessContext* AContext) {
    //KODE_PRINT;
  }

  //----------

  void processSample(KODE_ProcessContext* AContext, float spl0, float spl1) {
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);















//  void process(int ANumSamples, float* AResult) {
//    int remaining = ANumSamples;
//    int offset = 0;
//    preprocess_events();
//    while (remaining > 0) {
//      if (remaining >= TICKSIZE) {
//        process_events(offset);
//        //MProcess.process();
//        //copy_buffer(AResult + offset,MProcess.getBuffer());
//        remaining -= TICKSIZE;
//        offset += TICKSIZE;
//      }
//      else {
//        process_events(offset,remaining);
//        //MProcess.process(remaining);
//        //copy_buffer(AResult + offset,MProcess.getBuffer(),remaining);
//        remaining = 0;
//      }
//    }
//    postprocess_events();
//  }
