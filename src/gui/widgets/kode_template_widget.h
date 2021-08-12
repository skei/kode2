#ifndef kode_template_widget_included
#define kode_template_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_TemplateWidget
: public KODE_Widget {

//------------------------------
public:
//------------------------------

  KODE_TemplateWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
  }

  virtual ~KODE_TemplateWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_setPos(float AXpos, float AYpos) final {
  }

  void on_widget_setSize(float AWidth, float AHeight) final {
  }

  void on_widget_paint(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) final {
  }

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
  }

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState) final {
  }

  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState) final {
  }

  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState) final {
  }

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom) final {
  }

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) final {
  }

  void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) final {
  }

//------------------------------
public:
//------------------------------

  void do_widget_update(KODE_Widget* ASender) final {
    if (MParent) MParent->do_widget_update(ASender);
  }

  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) final {
    if (MParent) MParent->do_widget_redraw(ASender,ARect,AMode);
  }

  void do_widget_grabMouse(KODE_Widget* ASender) final {
    if (MParent) MParent->do_widget_grabMouse(ASender);
  }

  void do_widget_grabKeyboard(KODE_Widget* ASender) final {
    if (MParent) MParent->do_widget_grabKeyboard(ASender);
  }

  void do_widget_grabModal(KODE_Widget* ASender) final {
    if (MParent) MParent->do_widget_grabModal(ASender);
  }

  void do_widget_setMouseCursor(KODE_Widget* ASender, int32_t ACursor) final {
    if (MParent) MParent->do_widget_setMouseCursor(ASender,ACursor);
  }

  void do_widget_setMousePos(KODE_Widget* ASender, float AXpos, float AYpos) final {
    if (MParent) MParent->do_widget_setMousePos(ASender,AXpos,AYpos);
  }

  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) final {
    if (MParent) MParent->do_widget_setHint(ASender,AHint);
  }

  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) final {
    if (MParent) MParent->do_widget_notify(AWidget,AValue);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
