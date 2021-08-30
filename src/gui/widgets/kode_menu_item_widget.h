#ifndef kode_menu_item_widget_included
#define kode_menu_item_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

#define KODE_MENU_NOTIFY_CLOSE 1

class KODE_MenuItemWidget
: public KODE_TextWidget {

//------------------------------
public:
//------------------------------

  KODE_MenuItemWidget(KODE_FRect ARect)
  : KODE_TextWidget(ARect) {
    setName("KODE_MenuItemWidget");
    setHint("menuitem");
    setDrawText(true);
    setFillBackground(true);
    setDrawBorder(false);
    setBackgroundColor(KODE_COLOR_LIGHT_GRAY);
    setTextColor(KODE_COLOR_DARK_GRAY);
    setCursor(KODE_CURSOR_FINGER);
  }

  virtual ~KODE_MenuItemWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
      do_widget_update(this);
    }
    if (AButton == KODE_BUTTON_RIGHT) {
      do_widget_notify(this,KODE_MENU_NOTIFY_CLOSE);
    }
  }

  //----------

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) final {
    //MIsHovering = true;
    setBackgroundColor(KODE_COLOR_DARK_GRAY);
    setTextColor(KODE_COLOR_LIGHT_GRAY);
    do_widget_redraw(this,getRect(),0);
    do_widget_setMouseCursor(this,KODE_CURSOR_FINGER);
    if (flags.autoHint) do_widget_setHint(this,getHint());
  }

  //----------

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
    //MIsHovering = false;
    setBackgroundColor(KODE_COLOR_LIGHT_GRAY);
    setTextColor(KODE_COLOR_DARK_GRAY);
    do_widget_redraw(this,getRect(),0);
    do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
    if (flags.autoHint) do_widget_setHint(this,"");
  }

  //----------

  //void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) final {
  //  //MIsHovering = false;
  //  setBackgroundColor(KODE_COLOR_DARK_GRAY);
  //  setTextColor(KODE_COLOR_LIGHT_GRAY);
  //  do_widget_redraw(this,getRect(),0);
  //}

};

//----------------------------------------------------------------------
#endif
