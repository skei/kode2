
#define KODE_GUI_XCB
#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD

//nc -U -l -k /tmp/kode.socket
//#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"
#include "plugin/kode_plugin.h"
#include "gui/kode_widgets.h"

#include "../data/img/sa_logo_40_white_trans.h"

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

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(1024,768);
    #endif

    //KODE_Parameter* parameter;
    //parameter = appendParameter( new KODE_Parameter("param1",0.2f) );
    //parameter = appendParameter( new KODE_Parameter("param2",0.7f) );
    //parameter->setLabel("db");
    //parameter = appendParameter( new KODE_Parameter("param3",0.4f) );
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
: public KODE_Editor
,  public KODE_MenuListener {

//------------------------------
public:
//------------------------------

  void add_squares(KODE_Widget* AParent, float x, float y, KODE_Color c1, KODE_Color c2, KODE_Color c3, KODE_Color c4) {
    KODE_PanelWidget* panel1 = new KODE_PanelWidget(KODE_FRect(  x,     y,  30, 30 ));
    KODE_PanelWidget* panel2 = new KODE_PanelWidget(KODE_FRect(  x+40,  y,  30, 30 ));
    KODE_PanelWidget* panel3 = new KODE_PanelWidget(KODE_FRect(  x+80,  y,  30, 30 ));
    KODE_PanelWidget* panel4 = new KODE_PanelWidget(KODE_FRect(  x+120, y,  30, 30 ));
    panel1->setBackgroundColor(c1);
    panel2->setBackgroundColor(c2);
    panel3->setBackgroundColor(c3);
    panel4->setBackgroundColor(c4);
    panel1->setCursor(KODE_CURSOR_FINGER);
    panel2->setCursor(KODE_CURSOR_MOVE);
    panel3->setCursor(KODE_CURSOR_PENCIL);
    panel4->setCursor(KODE_CURSOR_WAIT);
    AParent->appendWidget(panel1);
    AParent->appendWidget(panel2);
    AParent->appendWidget(panel3);
    AParent->appendWidget(panel4);
  }

//------------------------------
public:
//------------------------------

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setTitle("test_widgets2");
    setFillBackground();
    setBackgroundColor(KODE_COLOR_GRAY);
    layout.innerBorder = 10;
    layout.spacing = 10;

    //

    KODE_MenuWidget* menu1 = new KODE_MenuWidget( KODE_FRect(200,100) );
    menu1->setMenuSize(2,4);
    menu1->appendMenuItem("Item1");
    menu1->appendMenuItem("Item2");
    menu1->appendMenuItem("Item3");
    menu1->appendMenuItem("Item4");
    menu1->appendMenuItem("Item5");
    menu1->appendMenuItem("Item6");
    menu1->appendMenuItem("Item7");
    menu1->appendMenuItem("Item8");

    //


    KODE_PanelWidget* left_panel = new KODE_PanelWidget(KODE_FRect(200));
    left_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    left_panel->layout.innerBorder = 10;
    left_panel->layout.spacing = 10;
    appendWidget(left_panel);

    KODE_SizerWidget* left_right_sizer = new KODE_SizerWidget(KODE_FRect(5));
    left_right_sizer->setMode(KODE_SIZER_LEFT);
    left_right_sizer->setTarget(left_panel);
    left_right_sizer->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    appendWidget(left_right_sizer);

    KODE_PanelWidget* right_panel = new KODE_PanelWidget(KODE_FRect());
    right_panel->setDrawBorder(false);
    right_panel->setFillBackground(false);
    right_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    //right_panel->layout.innerBorder = 10;
    right_panel->layout.spacing = 10;
    appendWidget(right_panel);

    KODE_PanelWidget* right_bottom_panel = new KODE_PanelWidget(KODE_FRect(200));
    right_bottom_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    right_bottom_panel->layout.innerBorder = 10;
    right_bottom_panel->layout.spacing = 10;
    right_panel->appendWidget(right_bottom_panel);

    KODE_SizerWidget* top_bottom_sizer = new KODE_SizerWidget(KODE_FRect(5));
    top_bottom_sizer->setMode(KODE_SIZER_BOTTOM);
    top_bottom_sizer->setTarget(right_bottom_panel);
    top_bottom_sizer->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    right_panel->appendWidget(top_bottom_sizer);

    KODE_PanelWidget* right_top_panel = new KODE_PanelWidget(KODE_FRect());
    right_top_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    right_top_panel->layout.innerBorder = 10;
    right_top_panel->layout.spacing = 10;
    right_panel->appendWidget(right_top_panel);

    //---------- left ----------

    // sliders

    KODE_SliderWidget* slider1 = new KODE_SliderWidget(KODE_FRect(20));
    slider1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    slider1->setValue(0.2);
    slider1->setText("Slider 1");
    slider1->setDrawLabel();
    slider1->setLabel("db");
    slider1->setLabelColor(KODE_COLOR_LIGHT_RED);
    slider1->setAutoHideCursor(false);
    slider1->setAutoGrabCursor(false);
    left_panel->appendWidget(slider1);

    KODE_SliderWidget* slider2 = new KODE_SliderWidget(KODE_FRect(20));
    slider2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    slider2->setValue(0.7);
    slider2->setText("Slider Two");
    slider2->setBackgroundColor(KODE_COLOR_DARK_RED);
    slider2->setValueBarColor(KODE_COLOR_RED);
    slider2->setTextColor(KODE_COLOR_WHITE);
    slider2->setValueTextColor(KODE_COLOR_BRIGHT_YELLOW);
    slider2->setAutoHideCursor(false);
    slider2->setAutoGrabCursor(true);
    left_panel->appendWidget(slider2);

    KODE_SliderWidget* slider3 = new KODE_SliderWidget(KODE_FRect(20));
    slider3->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    slider3->setValue(0.4);
    slider3->setText("Slider 3");
    slider3->setAutoHideCursor(true);
    slider3->setAutoGrabCursor(true);
    left_panel->appendWidget(slider3);

    // scrollbar

    KODE_ScrollBarWidget* scrollbar1 = new KODE_ScrollBarWidget(KODE_FRect(20));
    scrollbar1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(scrollbar1);

    // range sloder

    KODE_RangeSliderWidget* rangeslider1 = new KODE_RangeSliderWidget(KODE_FRect(20));
    rangeslider1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    rangeslider1->setValue(0.25);
    rangeslider1->setValue2(0.75);
    left_panel->appendWidget(rangeslider1);

    // buttons

    KODE_ButtonWidget* button1 = new KODE_ButtonWidget(KODE_FRect(20));
    button1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(button1);

    KODE_ButtonWidget* button2 = new KODE_ButtonWidget(KODE_FRect());
    button2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    button2->setIsToggle();
    left_panel->appendWidget(button2);

    // menu item

    KODE_MenuItemWidget* menuitem1 = new KODE_MenuItemWidget(KODE_FRect(20));
    menuitem1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    menuitem1->setText("Item1");
    left_panel->appendWidget(menuitem1);

    // selector

    KODE_SelectorWidget* selector1 = new KODE_SelectorWidget(KODE_FRect(20));
    selector1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    selector1->setMenu(menu1);
    left_panel->appendWidget(selector1);

    // group box

    KODE_GroupBoxWidget* groupbox = new KODE_GroupBoxWidget(KODE_FRect(150));
    groupbox->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(groupbox);

    // grid

    KODE_GridWidget* grid1 = new KODE_GridWidget(KODE_FRect(150));
    grid1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(grid1);

    //---------- right top ----------

    // header

    KODE_SAHeaderWidget* header = new KODE_SAHeaderWidget(KODE_FRect(60));
    header->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    header->setImage(this,(uint8_t*)sa_logo_40_white_trans,sa_logo_40_white_trans_size,KODE_COLOR_GRAY);
    header->setPluginName("test_widgets2");
    header->setPluginVersion("v0.0.1");

    right_top_panel->appendWidget(header);

    // image

    KODE_ImageWidget* image = new KODE_ImageWidget(KODE_FRect(256,256));
    image->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    //image->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    image->setDrawBorder();
    image->setCursor(KODE_CURSOR_QUESTION);
    KODE_Bitmap* bitmap = new KODE_Bitmap(256,256);
    bitmap->genFractalCloud();
    //image->setBitmap(bitmap);
    image->setImage(this,bitmap);
    //image->setImage(this,"/home/skei/Pictures/skei_256.png");
    delete bitmap;
    right_top_panel->appendWidget(image);

    // vertical  scrollbar

    KODE_ScrollBarWidget* scrollbar2 = new KODE_ScrollBarWidget(KODE_FRect(0,0,20,20));
    scrollbar2->layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
    scrollbar2->setDirection(KODE_VERTICAL);
    right_top_panel->appendWidget(scrollbar2);

    // keyboard

    KODE_KeyboardWidget* keyboard1 = new KODE_KeyboardWidget(KODE_FRect(100,100));
    keyboard1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    right_top_panel->appendWidget(keyboard1);

    // knob

    KODE_KnobWidget* knob1 = new KODE_KnobWidget(KODE_FRect(50,50));
    knob1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    knob1->setValue(0.3);
    right_top_panel->appendWidget(knob1);

    //---------- right bottom ----------

    // colors

    add_squares( right_bottom_panel, 0,   0,   KODE_COLOR_DARK_RED,     KODE_COLOR_RED,     KODE_COLOR_LIGHT_RED,     KODE_COLOR_BRIGHT_RED     );
    add_squares( right_bottom_panel, 160, 0,   KODE_COLOR_DARK_ORANGE,  KODE_COLOR_ORANGE,  KODE_COLOR_LIGHT_ORANGE,  KODE_COLOR_BRIGHT_ORANGE  );
    add_squares( right_bottom_panel, 320, 0,   KODE_COLOR_DARK_YELLOW,  KODE_COLOR_YELLOW,  KODE_COLOR_LIGHT_YELLOW,  KODE_COLOR_BRIGHT_YELLOW  );
    add_squares( right_bottom_panel, 480, 0,   KODE_COLOR_DARK_LIME,    KODE_COLOR_LIME,    KODE_COLOR_LIGHT_LIME,    KODE_COLOR_BRIGHT_LIME    );

    add_squares( right_bottom_panel, 0,   40,  KODE_COLOR_DARK_GREEN,   KODE_COLOR_GREEN,   KODE_COLOR_LIGHT_GREEN,   KODE_COLOR_BRIGHT_GREEN   );
    add_squares( right_bottom_panel, 160, 40,  KODE_COLOR_DARK_GREEN2,  KODE_COLOR_GREEN2,  KODE_COLOR_LIGHT_GREEN2,  KODE_COLOR_BRIGHT_GREEN2  );
    add_squares( right_bottom_panel, 320, 40,  KODE_COLOR_DARK_CYAN,    KODE_COLOR_CYAN,    KODE_COLOR_LIGHT_CYAN,    KODE_COLOR_BRIGHT_CYAN    );
    add_squares( right_bottom_panel, 480, 40,  KODE_COLOR_DARK_CYAN2,   KODE_COLOR_CYAN2,   KODE_COLOR_LIGHT_CYAN2,   KODE_COLOR_BRIGHT_CYAN2   );

    add_squares( right_bottom_panel, 0,   80,  KODE_COLOR_DARK_BLUE1,   KODE_COLOR_BLUE1,   KODE_COLOR_LIGHT_BLUE1,   KODE_COLOR_BRIGHT_BLUE1   );
    add_squares( right_bottom_panel, 160, 80,  KODE_COLOR_DARK_BLUE,    KODE_COLOR_BLUE,    KODE_COLOR_LIGHT_BLUE,    KODE_COLOR_BRIGHT_BLUE    );
    add_squares( right_bottom_panel, 320, 80,  KODE_COLOR_DARK_BLUE2,   KODE_COLOR_BLUE2,   KODE_COLOR_LIGHT_BLUE2,   KODE_COLOR_BRIGHT_BLUE2   );
    add_squares( right_bottom_panel, 480, 80,  KODE_COLOR_DARK_MAGENTA, KODE_COLOR_MAGENTA, KODE_COLOR_LIGHT_MAGENTA, KODE_COLOR_BRIGHT_MAGENTA );

    add_squares( right_bottom_panel, 0,   120, KODE_COLOR_DARK_RED2,    KODE_COLOR_RED2,    KODE_COLOR_LIGHT_RED2,    KODE_COLOR_BRIGHT_RED2    );
    add_squares( right_bottom_panel, 200, 120, KODE_COLOR_DARK_GRAY,    KODE_COLOR_GRAY,    KODE_COLOR_LIGHT_GRAY,    KODE_COLOR_WHITE          );

    //black
    KODE_PanelWidget* black_panel = new KODE_PanelWidget(KODE_FRect(  160, 120, 30,30 ));
    black_panel->setBackgroundColor(KODE_COLOR_BLACK);
    black_panel->setCursor(KODE_CURSOR_FINGER);
    right_bottom_panel->appendWidget(black_panel);

    //

//    KODE_PanelWidget* panel1 = new KODE_PanelWidget( KODE_FRect(0,-10,50,25) );
//    panel1->setCursor(KODE_CURSOR_PLUS);
//    panel1->setFillBackground(false);
//    panel1->setBorderColor(KODE_COLOR_WHITE);
//    panel1->flags.sizePercent = true;
//    panel1->layout.alignment = KODE_WIDGET_ALIGN_BOTTOM_CENTER;
//    right_top_panel->appendWidget(panel1);
//
//    KODE_PanelWidget* panel2 = new KODE_PanelWidget( KODE_FRect(-10,10,50,25) );
//    panel2->setCursor(KODE_CURSOR_ARROWDIAGLEFT);
//    panel2->setFillBackground(true);
//    panel2->setBackgroundColor(KODE_COLOR_DARK_GREEN);
//    panel2->flags.sizePercent = true;
//    panel2->layout.alignment = KODE_WIDGET_ALIGN_TOP_RIGHT;
//    panel1->appendWidget(panel2);

    appendWidget(menu1);
    //menu1->open(this,0,0);

  }

  //----------

  //virtual ~myEditor() {
  //}

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




