#ifndef kode_main_menu_widget_included
#define kode_main_menu_widget_included
//----------------------------------------------------------------------

//#include "gui/kode_widgets/kode_menu_widget.h"
//#include "gui/widgets/kode_text_widget.h"

#include "gui/widgets/kode_menu_widget.h"
#include "gui/widgets/kode_menu_item_widget.h"
#include "gui/widgets/kode_panel_widget.h"
#include "gui/widgets/kode_button_widget.h"

typedef KODE_Array<KODE_MenuWidget*> KODE_MenuWidgetArray ;
//----------

class KODE_MainMenuWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  KODE_MainMenuWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("KODE_MainMenuWidget");
    setHint("mainmenu");
    setFillBackground(false);
    setDrawBorder(false);
    layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = 10;
  }

  //----------

  virtual ~KODE_MainMenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t appendMenu(const char* ALabel, KODE_MenuWidget* AMenu) {
    uint32_t index = getNumChildren();
//    MMenus.append(AMenu);
    KODE_MenuItemWidget* item;
    item = new KODE_MenuItemWidget( KODE_FRect(50,20) );
    item->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    item->setText(ALabel);
    appendWidget(item);
    return index;
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

//  void do_widget_update(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_update(ASender);
//  }

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

