#ifndef kode_container_widget_included
#define kode_container_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_ContainerWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_Widget*  MContent = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_ContainerWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_ContainerWidget");
    setHint("container");
    flags.visible = false;
    flags.active = false;
    MContent = new KODE_Widget(ARect);

  }

  virtual ~KODE_ContainerWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void showContainer(void) {
    if (!MContent->flags.visible) {
      MContent->flags.active = true;
      MContent->flags.visible = true;
//      setHeight(MOpenSize);
//      setInitialHeight(MOpenSize);
//      do_widget_resized(this);
    }
  }

  //----------

  virtual void hideContainer(void) {
    if (MContent->flags.visible) {
      MContent->flags.active = false;
      MContent->flags.visible = false;
//      setHeight(MClosedSize);
//      setInitialHeight(MClosedSize);
//      do_widget_resized(this);
    }
  }

//------------------------------
public:
//------------------------------

  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContent->appendWidget(AWidget);
  }

  //----------

//  void alignChildren(bool ARecursive=true) override {
//    KODE_Widget::alignChildren();
//    KODE_FRect content = MContent->getContentRect();
//    float rect_w = MContent->getRect().w;
//    float rect_h = MContent->getRect().h;
//    if (rect_h > 0) {
//      float thumb_ratio = rect_h / content.h;         // szize of thumb (0..1)
//      float thumb_size = KODE_Clamp(thumb_ratio,0,1);
//      MVerticalScrollBar->setThumbSize(thumb_size,false);
//      float visible = MVerticalScrollBar->getThumbSize();
//      float pos     = MVerticalScrollBar->getThumbPos();
//      float prev    = 0.0f; // MVerticalScrollBar->getPrevThumbPos();
//      updateScroll(MVerticalScrollBar,visible,pos,prev);
//    }
//    else {
//      MVerticalScrollBar->setThumbSize(1,false);
//    }
//    if (rect_w > 0) {
//      float thumb_ratio = rect_w / content.w;         // szize of thumb (0..1)
//      float thumb_size = KODE_Clamp(thumb_ratio,0,1);
//      MHorizontalScrollBar->setThumbSize(thumb_size,false);
//      float visible = MHorizontalScrollBar->getThumbSize();
//      float pos     = MHorizontalScrollBar->getThumbPos();
//      float prev    = 0.0f; // MHorizontalScrollBar->getPrevThumbPos();
//      updateScroll(MHorizontalScrollBar,visible,pos,prev);
//    }
//    else {
//      MHorizontalScrollBar->setThumbSize(1,false);
//    }
//  }

//------------------------------
public:
//------------------------------

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

  void do_widget_update(KODE_Widget* ASender) override {
    KODE_Widget::do_widget_update(ASender);
    //if (ASender == MTitleBar) {
    //  if (MClosable) {
    //    if (MTitleBar->getValue() >= 0.5f) open();
    //    else close();
    //  }
    //}
  }

  //----------

//  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) override {
//    KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
//  }

//  void do_widget_moved(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) override {
//    KODE_Widget::do_widget_moved(ASender,ADeltaX,ADeltaY);
//  }

//  void do_widget_resized(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_resized(ASender);
//  }

//  void do_widget_grabMouseCursor(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_grabMouseCursor(ASender);
//  }

//  void do_widget_grabKeyboard(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_grabKeyboard(ASender);
//  }

//  void do_widget_grabModal(KODE_Widget* ASender) override {
//    KODE_Widget::do_widget_grabModal(ASender);
//  }

//  void do_widget_setMouseCursor(KODE_Widget* ASender, int32_t ACursor) override {
//    KODE_Widget::do_widget_setMouseCursor(ASender,ACursor);
//  }

//  void do_widget_setMouseCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) override {
//    KODE_Widget::do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
//  }

//  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) override {
//    KODE_Widget::do_widget_setHint(ASender,AHint);
//  }

//  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) override {
//    KODE_Widget::do_widget_notify(AWidget,AValue);
//  }

};

//----------------------------------------------------------------------
#endif

