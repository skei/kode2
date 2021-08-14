#ifndef kode_button_widget_included
#define kode_button_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

//----------------------------------------------------------------------

class KODE_ButtonWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  bool        MIsToggle           = false;

  const char* MOnText             = "On";
  KODE_Color  MOnBackgroundColor  = KODE_Color(0.7f);
  KODE_Color  MOnBorderColor      = KODE_Color(0.3f);
  KODE_Color  MOnTextColor        = KODE_Color(0.3f);
  uint32_t    MOnTextAlignment    = KODE_TEXT_ALIGN_CENTER;
  KODE_FRect  MOnTextOffset       = KODE_FRect(2);

  const char* MOffText            = "Off";
  KODE_Color  MOffBackgroundColor = KODE_Color(0.3f);
  KODE_Color  MOffBorderColor     = KODE_Color(0.7f);
  KODE_Color  MOffTextColor       = KODE_Color(0.7f);
  uint32_t    MOffTextAlignment   = KODE_TEXT_ALIGN_CENTER;
  KODE_FRect  MOffTextOffset      = KODE_FRect(2);

//------------------------------
public:
//------------------------------

  KODE_ButtonWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    //MDrawBorder = false;
  }

  virtual ~KODE_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setIsToggle(bool AIsToggle=true) {
    MIsToggle = AIsToggle;
  }

  virtual void setText(const char* AOn, const char* AOff) {
    MOnText = AOn;
    MOffText = AOff;
  }

  virtual void setTextAlignment(uint32_t AOn, uint32_t AOff) {
    MOnTextAlignment = AOn;
    MOffTextAlignment = AOff;
  }

  virtual void setTextColor(KODE_Color AOn, KODE_Color AOff) {
    MOnTextColor = AOn;
    MOffTextColor = AOff;
  }

  virtual void setBackgroundColor(KODE_Color AOn, KODE_Color AOff) {
    MOnBackgroundColor = AOn;
    MOffBackgroundColor = AOff;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    //KODE_ColorWidget::on_widget_paint(APainter,ARect,AMode);
    if (MValue >= 0.5f) { // on
      APainter->fillRect(MRect,MOnBackgroundColor);
      KODE_FRect rect = MRect;
      rect.shrink(MOnTextOffset);
      APainter->drawText(rect,MOnText,MOnTextAlignment,MOnTextColor);
      APainter->drawRect(MRect,MOnBorderColor);
    }
    else { // off
      APainter->fillRect(MRect,MOffBackgroundColor);
      KODE_FRect rect = MRect;
      rect.shrink(MOffTextOffset);
      APainter->drawText(rect,MOffText,MOffTextAlignment,MOffTextColor);
      APainter->drawRect(MRect,MOffBorderColor);
    }
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
      if (MIsToggle) {
        MValue = 1.0f - MValue;
      }
      else {
        MValue = 1.0f;
      }
      update();
      redraw();
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
      if (!MIsToggle) {
        MValue = 0.0f;
        update();
        redraw();
      }
    }
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif

