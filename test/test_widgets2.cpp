
#define KODE_GUI_XCB

#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD

//nc -U -l -k /tmp/kode.socket
//#define KODE_DEBUG_PRINT_SOCKET

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
      setName("test_widgets2_debug");
    #else
      setName("test_widgets2");
    #endif

    setAuthor("author");
    setVersion(0x0101001);

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(800,600);
    #endif

    //KODE_Parameter* parameter;
    //parameter = appendParameter( KODE_New KODE_Parameter("param1",0.2f) );
    //parameter = appendParameter( KODE_New KODE_Parameter("param2",0.7f) );
    //parameter->setLabel("db");
    //parameter = appendParameter( KODE_New KODE_Parameter("param3",0.4f) );
    //parameter->setLabel("%");

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

  void add_squares(float x, float y, KODE_Color c1, KODE_Color c2, KODE_Color c3, KODE_Color c4) {
    KODE_PanelWidget* panel1 = KODE_New KODE_PanelWidget(KODE_FRect(  x,     y,  30, 30 ));
    KODE_PanelWidget* panel2 = KODE_New KODE_PanelWidget(KODE_FRect(  x+40,  y,  30, 30 ));
    KODE_PanelWidget* panel3 = KODE_New KODE_PanelWidget(KODE_FRect(  x+80,  y,  30, 30 ));
    KODE_PanelWidget* panel4 = KODE_New KODE_PanelWidget(KODE_FRect(  x+120, y,  30, 30 ));
    panel1->setBackgroundColor(c1);
    panel2->setBackgroundColor(c2);
    panel3->setBackgroundColor(c3);
    panel4->setBackgroundColor(c4);
    panel1->setCursor(KODE_CURSOR_FINGER);
    panel2->setCursor(KODE_CURSOR_MOVE);
    panel3->setCursor(KODE_CURSOR_PENCIL);
    panel4->setCursor(KODE_CURSOR_WAIT);
    appendWidget(panel1);
    appendWidget(panel2);
    appendWidget(panel3);
    appendWidget(panel4);
  }

//------------------------------
public:
//------------------------------

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setFillBackground();

    // colors

    add_squares( 10, 10,  KODE_COLOR_DARK_RED,     KODE_COLOR_RED,     KODE_COLOR_LIGHT_RED,     KODE_COLOR_BRIGHT_RED     );
    add_squares( 10, 50,  KODE_COLOR_DARK_ORANGE,  KODE_COLOR_ORANGE,  KODE_COLOR_LIGHT_ORANGE,  KODE_COLOR_BRIGHT_ORANGE  );
    add_squares( 10, 90,  KODE_COLOR_DARK_YELLOW,  KODE_COLOR_YELLOW,  KODE_COLOR_LIGHT_YELLOW,  KODE_COLOR_BRIGHT_YELLOW  );
    add_squares( 10, 130, KODE_COLOR_DARK_LIME,    KODE_COLOR_LIME,    KODE_COLOR_LIGHT_LIME,    KODE_COLOR_BRIGHT_LIME    );
    add_squares( 10, 170, KODE_COLOR_DARK_GREEN,   KODE_COLOR_GREEN,   KODE_COLOR_LIGHT_GREEN,   KODE_COLOR_BRIGHT_GREEN   );
    add_squares( 10, 210, KODE_COLOR_DARK_GREEN2,  KODE_COLOR_GREEN2,  KODE_COLOR_LIGHT_GREEN2,  KODE_COLOR_BRIGHT_GREEN2  );
    add_squares( 10, 250, KODE_COLOR_DARK_CYAN,    KODE_COLOR_CYAN,    KODE_COLOR_LIGHT_CYAN,    KODE_COLOR_BRIGHT_CYAN    );
    add_squares( 10, 290, KODE_COLOR_DARK_CYAN2,   KODE_COLOR_CYAN2,   KODE_COLOR_LIGHT_CYAN2,   KODE_COLOR_BRIGHT_CYAN2   );
    add_squares( 10, 330, KODE_COLOR_DARK_BLUE1,   KODE_COLOR_BLUE1,   KODE_COLOR_LIGHT_BLUE1,   KODE_COLOR_BRIGHT_BLUE1   );
    add_squares( 10, 370, KODE_COLOR_DARK_BLUE,    KODE_COLOR_BLUE,    KODE_COLOR_LIGHT_BLUE,    KODE_COLOR_BRIGHT_BLUE    );
    add_squares( 10, 410, KODE_COLOR_DARK_BLUE2,   KODE_COLOR_BLUE2,   KODE_COLOR_LIGHT_BLUE2,   KODE_COLOR_BRIGHT_BLUE2   );
    add_squares( 10, 450, KODE_COLOR_DARK_MAGENTA, KODE_COLOR_MAGENTA, KODE_COLOR_LIGHT_MAGENTA, KODE_COLOR_BRIGHT_MAGENTA );
    add_squares( 10, 490, KODE_COLOR_DARK_RED2,    KODE_COLOR_RED2,    KODE_COLOR_LIGHT_RED2,    KODE_COLOR_BRIGHT_RED2    );
    add_squares( 10, 530, KODE_COLOR_DARK_GRAY,    KODE_COLOR_GRAY,    KODE_COLOR_LIGHT_GRAY,    KODE_COLOR_WHITE          );

    // image

    KODE_ImageWidget* image1 = KODE_New KODE_ImageWidget(KODE_FRect(  170, 10, 256,256 ));
    image1->setDrawBorder();
    image1->setCursor(KODE_CURSOR_QUESTION);
    KODE_Bitmap* bitmap = KODE_New KODE_Bitmap(128,128);
    bitmap->genFractalCloud();
    //image1->setBitmap(bitmap);
    image1->setImage(this,bitmap);
    //image1->setImage(this,"/home/skei/Pictures/skei_256.png");
    KODE_Delete bitmap;
    appendWidget(image1);

    //

    KODE_PanelWidget* panel = KODE_New KODE_PanelWidget( KODE_FRect(-20,-30, 50,25 ) );
    panel->setCursor(KODE_CURSOR_PLUS);
    panel->setFillBackground(false);
    panel->setBorderColor(KODE_COLOR_WHITE);
    panel->options.sizePercent = true;
    panel->layout.alignment = KODE_WIDGET_ALIGN_BOTTOM_RIGHT;
    appendWidget(panel);

  }

  //----------

  //virtual ~myEditor() {
  //}

//------------------------------
public:
//------------------------------

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
    //myEditor* editor = (myEditor*)KODE_New myEditor(this,AParent);
    //return editor;
    MEditor = KODE_New myEditor(this,AParent);
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Print("\n");
    //KODE_Assert(AEditor == MEditor);
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




