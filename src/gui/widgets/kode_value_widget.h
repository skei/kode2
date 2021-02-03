#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

//----------------------------------------------------------------------

class KODE_ValueWidget
: public KODE_TextWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawValueText      = true;
  char        MValueText[32]      = {0};
  KODE_Color  MValueTextColor     = KODE_Color(1.0f);
  uint32_t    MValueTextAlignment = KODE_TEXT_ALIGN_RIGHT;
  KODE_FRect  MValueTextOffset    = KODE_FRect(2);


//------------------------------
public:
//------------------------------

  KODE_ValueWidget(KODE_FRect ARect)
  : KODE_TextWidget(ARect) {
  }

  virtual ~KODE_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValueText(bool ADraw) {
    MDrawValueText = ADraw;
  }

  virtual void setValueTextAlignment(uint32_t AAlignment) {
    MValueTextAlignment = AAlignment;
  }

  virtual void setValueTextColor(KODE_Color AColor) {
    MValueTextColor = AColor;
  }

  virtual void drawValueText(KODE_BasePainter* APainter) {
    KODE_FRect rect = MRect;
    rect.shrink(MValueTextOffset);
    float value = MValue;
    if (MParameter) {
      value = MParameter->from01(value);
    }
    KODE_FloatToString(MValueText,value);
    if (MDrawValueText) {
      APainter->drawText(rect,MValueText,MValueTextAlignment,MValueTextColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    //KODE_ColorWidget::on_widget_paint(APainter,ARect,AMode);
    //drawBorder(APainter);
    fillBackground(APainter);
    paintChildren(APainter,ARect,AMode);
    drawText(APainter);
    drawValueText(APainter);
    drawBorder(APainter);

  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
