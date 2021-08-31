#ifndef kode_group_box_widget_included
#define kode_group_box_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widgets.h"


class KODE_GroupBoxWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_ButtonWidget*  MTitleBar   = KODE_NULL;
  KODE_PanelWidget*   MContainer  = KODE_NULL;
  bool                MClosed     = false;
  bool                MClosable   = true;
  float               MOpenSize   = 0.0f;
  float               MClosedSize = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_GroupBoxWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_GroupBoxWidget");
    setHint("groupbox");
    //setCursor(KODE_CURSOR_);

    MTitleBar = new KODE_ButtonWidget( KODE_FRect(20) );
    MTitleBar->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    MTitleBar->setIsToggle();
    MTitleBar->setText("Open","Closed");
    MTitleBar->setValue(1);

    MContainer = new KODE_PanelWidget(KODE_FRect(0));
    MContainer->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    MContainer->setCursor(KODE_CURSOR_CROSS);

    KODE_Widget::appendWidget( MTitleBar );
    KODE_Widget::appendWidget( MContainer );

    MOpenSize   = ARect.h;
    MClosedSize = MTitleBar->getRect().h;

    //MClosed     = true;
    MClosable   = true;
    //close();

  }

  virtual ~KODE_GroupBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  KODE_ButtonWidget* getTitleBar(void) {
    return MTitleBar;
  }

  //----------

  KODE_PanelWidget* getContainer(void) {
    return MContainer;
  }

  //----------

  //void setTitleText(const char* AOnText, const char* AOffText) {
  //  //MTitleBar->setText(AOnText,AOffText);
  //}

  //void setTitleText(const char* AText) {
  //  MTitleBar->setText(AText);
  //}

  virtual void setClosable(bool AClosable=true) { MClosable = AClosable; }
  //virtual void setOpenSize(float ASize)         { MOpenSize = ASize; }
  //virtual void setClosedSize(float ASize)       { MClosedSize = ASize; }

  //----------

  void open(void) {
    if (MClosed) {
      MClosed = false;
      MContainer->flags.active = true;
      MContainer->flags.visible = true;
//      setResized( KODE_Point(0,0) );
      setHeight(MOpenSize);
      setInitialHeight(MOpenSize);
      do_widget_resized(this/*,MRect.w,MOpenSize*/);
    }
  }

  //----------

  void close(void) {
    if (!MClosed) {
      MClosed = true;
      MContainer->flags.active = false;
      MContainer->flags.visible = false;
//      float rs = MClosedSize - getInitialRect().h;
//      setResized( KODE_Point(0,rs) );
      setHeight(MClosedSize);
      setInitialHeight(MClosedSize);
      do_widget_resized(this/*,MRect.w,MClosedSize*/);
    }
  }

  //----------

  void toggle(void) {
    if (MClosed) open();
    else close();
  }

//------------------------------
public:
//------------------------------

  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContainer->appendWidget(AWidget);
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(KODE_Widget* ASender) final {
    if (ASender == MTitleBar) {
      if (MClosable) {
        if (MTitleBar->getValue() >= 0.5f) open();
        else close();
      }
    }
    KODE_Widget::do_widget_update(ASender);
  }

};

//----------------------------------------------------------------------
#endif

