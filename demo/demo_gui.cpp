
//#define KODE_GUI_XCB
#define KODE_GUI_CAIRO

//#define KODE_USE_CAIRO
//#define KODE_USE_CAIRO

#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
//#define KODE_DEBUG_PRINT_SOCKET
//nc -U -l -k /tmp/kode.socket

//----------

#include "kode.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"
#include "gui/kode_widgets.h"

#include "../data/img/sa_logo_40_white_trans.h"

#include "demo_gui/left_panel.h"
#include "demo_gui/right_panel.h"
#include "demo_gui/top_panel.h"
#include "demo_gui/bottom_panel.h"
#include "demo_gui/main_panel.h"


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
private:
//------------------------------

const char* param_txt[5] = {
  "one",
  "two",
  "three",
  "4",
  "..and so on"
};

//------------------------------
public:
//------------------------------

  myDescriptor() {

    #ifdef KODE_DEBUG
      setName("demo_gui_debug");
    #else
      setName("demo_gui");
    #endif

    setAuthor("author");
    setVersion(0x0101001);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(1024,768);
    #endif

    KODE_Parameter* param1 = new KODE_FloatParameter("float",  0.2f,  -10.0f, 2.0f, 0.2f );
    KODE_Parameter* param2 = new KODE_PowParameter(  "pow 2",  440.0f, 2.0f, true, 20, 20000, 1);
    KODE_Parameter* param3 = new KODE_TextParameter( "text",   0,      5, param_txt );

    //param1->setLabel("db");
    param2->setLabel("hz");
    //param3->setLabel("%");

    appendParameter(param1);
    appendParameter(param2);
    appendParameter(param3);

  }
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

class myEditor
: public KODE_Editor
,  public KODE_MenuListener {

//------------------------------
private:
//------------------------------

  KODE_TextWidget* footer = KODE_NULL;

//------------------------------
public:
//------------------------------

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setTitle("demo_gui");
    setFillBackground();
    setBackgroundColor(KODE_COLOR_GRAY);
    layout.innerBorder = 10;
    layout.spacing = 10;

    // header

    KODE_SAHeaderWidget* header = new KODE_SAHeaderWidget(KODE_FRect(60));
    header->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    header->setImage(this,(uint8_t*)sa_logo_40_white_trans,sa_logo_40_white_trans_size,KODE_COLOR_GRAY);
    header->setPluginName("demo_gui");
    header->setPluginVersion("v0.0.0");
    appendWidget(header);

    // footer

    footer = new KODE_TextWidget( KODE_FRect(20) );
    footer->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    footer->setText("text..");
    footer->setTextColor(KODE_COLOR_BLACK);
    footer->setBackgroundColor( KODE_COLOR_GRAY );
    appendWidget(footer);

    //

    KODE_left_panel*    left_panel    = new KODE_left_panel(   KODE_FRect(200) );
    KODE_right_panel*   right_panel   = new KODE_right_panel(  KODE_FRect(200) );
    KODE_top_panel*     top_panel     = new KODE_top_panel(    KODE_FRect(100),this );
    KODE_bottom_panel*  bottom_panel  = new KODE_bottom_panel( KODE_FRect(100) );
    KODE_main_panel*    main_panel    = new KODE_main_panel(   KODE_FRect(),   this );

    KODE_SizerWidget*   left_sizer    = new KODE_SizerWidget(KODE_FRect(5),KODE_SIZER_LEFT,left_panel);
    KODE_SizerWidget*   right_sizer   = new KODE_SizerWidget(KODE_FRect(5),KODE_SIZER_RIGHT,right_panel);
    KODE_SizerWidget*   top_sizer     = new KODE_SizerWidget(KODE_FRect(5),KODE_SIZER_TOP,top_panel);
    KODE_SizerWidget*   bottom_sizer  = new KODE_SizerWidget(KODE_FRect(5),KODE_SIZER_BOTTOM,bottom_panel);

    appendWidget(left_panel);
    appendWidget(left_sizer);
    appendWidget(top_panel);
    appendWidget(top_sizer);
    appendWidget(right_panel);
    appendWidget(right_sizer);
    appendWidget(bottom_panel);
    appendWidget(bottom_sizer);
    appendWidget(main_panel);

    appendWidget(left_panel->menu1);

    // parameters.. be careful !!!

    connectParameter( left_panel->getChild(0), 0 );
    connectParameter( left_panel->getChild(1), 1 );
    connectParameter( left_panel->getChild(2), 2 );

  }

  //----------

  //virtual ~myEditor() {
  //}

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

//  virtual void do_widget_update(KODE_Widget* ASender) {
//    KODE_Editor::do_widget_update(ASender);
//  }
//  virtual void on_window_move(uint32_t AXpos, uint32_t AYpos) override {
//    //KODE_Print(" %i,%i\n",AXpos,AYpos);
//    KODE_Editor::on_window_move(AXpos,AYpos);
//  }
//
//  virtual void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
//    //KODE_Print(" %i,%i\n",AWidth,AHeight);
//    KODE_Editor::on_window_resize(AWidth,AHeight);
//  }
//
//  virtual void on_window_paint(uint32_t AXpos, uint32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
//    //KODE_Print(" %i,%i : %i,%i\n",AXpos,AYpos,AWidth,AHeight);
//    KODE_Editor::on_window_paint(AXpos,AYpos,AWidth,AHeight);
//  }

//------------------------------
public:
//------------------------------

  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) override {
    //if (AHint[0] != 0) {
      footer->setText(AHint);
//      footer->redraw();
    //}
    //KODE_Editor::do_widget_setHint(ASender,AHint);
  }

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
    //KODE_Print("\n");
  }

  //----------

  void on_plugin_close() final {
    //KODE_Print("\n");
  }

  //----------

  void on_plugin_initialize() final {
    //KODE_Print("\n");
  }

  //----------

  void on_plugin_terminate() final {
    //KODE_Print("\n");
  }

  //----------

  void on_plugin_activate() final {
    //KODE_Print("\n");
    MCounter = 0;
  }

  //----------

  void on_plugin_deactivate() final {
    //KODE_Print("\n");
  }

  //----------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    //KODE_Print("sr %.3f bs %i\n",ASamplerate,ABlocksize);
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
    //KODE_Print("parent %p\n",AParent);
    //myEditor* editor = (myEditor*)new myEditor(this,AParent);
    //return editor;
    MEditor = new myEditor(this,AParent);
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Print("\n");
    //KODE_Assert(AEditor == MEditor);
    if (MEditor) {
      //delete (myEditor*)AEditor;
      delete MEditor;
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




