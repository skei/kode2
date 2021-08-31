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
//    MContainer->setChildOffset(0,-18);
    KODE_Widget::appendWidget(MContainer);
  }

  //----------

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
    float rect_h = MContainer->getRect().h;
    if (rect_h > 0) {
      float thumb_ratio = rect_h / content.h;         // szize of thumb (0..1)
      float thumb = KODE_Clamp(thumb_ratio,0,1);
      MVerticalScrollBar->setThumbSize(thumb,false);
//      float scrolly = 1.0f - thumb_ratio;             // scrollable 0..1
//      scrolly *= content.h;                           // number of pixels we can scroll
//      scrolly *= MVerticalScrollBar->getValue();
//      scrollChildren(0,scrolly);
    }
    else {
      MVerticalScrollBar->setThumbSize(1,false);
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_setSize(float AWidth, float AHeight) override {
    KODE_PRINT;
    KODE_Widget::on_widget_setSize(AWidth,AHeight);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(KODE_Widget* ASender) override {
    if (ASender == MVerticalScrollBar) {
      //KODE_Print("vertical\n");
    }
    if (ASender == MHorizontalScrollBar) {
      //KODE_Print("horizontal\n");
    }
    KODE_Widget::do_widget_update(ASender);
  }

  //----------

  void do_widget_resized(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) override {
    KODE_PRINT;
    KODE_Widget::do_widget_resized(ASender);
  }



};

//----------------------------------------------------------------------
#endif



#if 0

#endif // 0
