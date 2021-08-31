#ifndef kode_scroll_box_widget_includedbutton
#define kode_scroll_box_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"
#include "gui/widgets/kode_panel_widget.h"
#include "gui/widgets/kode_scroll_bar_widget.h"

class KODE_ScrollBoxWidget
: public KODE_Widget {

//------------------------------
public:
//------------------------------

  bool showHorizontalScrollBar = false;
  bool showVerticalScrollBar = true;

  KODE_ScrollBarWidget* MVerticalScrollBar    = KODE_NULL;
  KODE_ScrollBarWidget* MHorizontalScrollBar  = KODE_NULL;
  KODE_PanelWidget*     MContainer            = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_ScrollBoxWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {

    setName("KODE_ScrollBoxWidget");
    setHint("scrollbox");

    if (showVerticalScrollBar) {
      MVerticalScrollBar = new KODE_ScrollBarWidget( KODE_FRect(10) );
      MVerticalScrollBar->layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
      MVerticalScrollBar->setDirection(KODE_VERTICAL);
      KODE_Widget::appendWidget(MVerticalScrollBar);
    }

    if (showHorizontalScrollBar) {
      MHorizontalScrollBar = new KODE_ScrollBarWidget( KODE_FRect(10) );
      MHorizontalScrollBar->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
      MHorizontalScrollBar->setDirection(KODE_HORIZONTAL);
      KODE_Widget::appendWidget(MHorizontalScrollBar);
    }

    MContainer = new KODE_PanelWidget( KODE_FRect() );
    MContainer->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    MContainer->layout.spacing = 5;
    MContainer->setChildOffset(0,-18);

    KODE_Widget::appendWidget(MContainer);

  }

  virtual ~KODE_ScrollBoxWidget() {
  }

//------------------------------
public:
//------------------------------

//  void resize(void) {
//    MContainer->flags.active = true;
//    MContainer->flags.visible = true;
//    //setResized( KODE_Point(0,0) );
//    setHeight(MOpenSize);
//    setInitialHeight(MOpenSize);
//    do_widget_resized(this/*,MRect.w,MOpenSize*/);
//  }

//------------------------------
public:
//------------------------------

  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContainer->appendWidget(AWidget);
  }

  //----------

  void alignChildren() override {
    KODE_Widget::alignChildren();
    KODE_FRect content = MContainer->getContentRect();
    //KODE_Print("content %.0f,%.0f,%.0f,%.0f\n",content.x,content.y,content.w,content.h);

    float rect_h = MContainer->getRect().h; //getRect().h;
    if (rect_h > 0) {
      float h = rect_h / content.h;
      float thumb = KODE_Clamp(h,0,1);
      //KODE_Print("rect_h %.2f h %.2f thumb %.2f\n",rect_h,h,thumb);
      MVerticalScrollBar->setThumbSize(thumb,false);
    }
    else {
      MVerticalScrollBar->setThumbSize(1,false);
    }
  }


//------------------------------
public:
//------------------------------

//  void on_widget_setPos(float AXpos, float AYpos) override {
//  }

//  void on_widget_setSize(float AWidth, float AHeight) override {
//  }

//  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
//  }

//  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
//    //if (flags.autoHint) do_widget_setHint(this,MHint);
//  }

//  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
//    //if (flags.autoHint) do_widget_setHint(this,"");
//  }

//  void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) override {
//  }

//------------------------------
public:
//------------------------------

  //void do_widget_update(KODE_Widget* ASender) final {
  //  if (ASender == MVerticalScrollBar) {
  //    KODE_Print("vertical\n");
  //  }
  //  if (ASender == MHorizontalScrollBar) {
  //    KODE_Print("horizontal\n");
  //  }
  //  KODE_Widget::do_widget_update(ASender);
  //}


//  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) final {
//    KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
//  }

//  void do_widget_moved(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) final {
//    KODE_Widget::do_widget_moved(ASender,ADeltaX,ADeltaY);
//  }

//  void do_widget_resized(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_resized(ASender);
//  }

//  void do_widget_grabMouseCursor(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabMouseCursor(ASender);
//  }

//  void do_widget_grabKeyboard(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabKeyboard(ASender);
//  }

//  void do_widget_grabModal(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabModal(ASender);
//  }

//  void do_widget_setMouseCursor(KODE_Widget* ASender, int32_t ACursor) final {
//    KODE_Widget::do_widget_setMouseCursor(ASender,ACursor);
//  }

//  void do_widget_setMouseCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) final {
//    KODE_Widget::do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
//  }

//  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) final {
//    KODE_Widget::do_widget_setHint(ASender,AHint);
//  }

//  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) final {
//    KODE_Widget::do_widget_notify(AWidget,AValue);
//  }

};

//----------------------------------------------------------------------
#endif



#if 0

#endif // 0
