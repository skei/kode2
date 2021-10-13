#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

#define KODE_VALUE_WIDGET_LABEL_SPACE 4.0f

//----------------------------------------------------------------------

class KODE_ValueWidget
: public KODE_TextWidget {

//------------------------------
private:
//------------------------------

  char        MValueText[32]      = {0};

//------------------------------
protected:
//------------------------------

  uint32_t    MDigits             = 3;

  // value text
  bool        MDrawValueText      = true;
  KODE_Color  MValueTextColor     = KODE_COLOR_WHITE;
  uint32_t    MValueTextAlignment = KODE_TEXT_ALIGN_RIGHT;
  KODE_FRect  MValueTextOffset    = KODE_FRect(2,2,2,2);

  // label
  bool        MDrawLabel          = false;
  const char* MLabel              = "";
  KODE_Color  MLabelColor         = KODE_Color(0.7f);

//------------------------------
public:
//------------------------------

  KODE_ValueWidget(KODE_FRect ARect, const char* AText="")
  : KODE_TextWidget(ARect) {
    setName("KODE_ValueWidget");
    setHint("value");
    MText = AText;
  }

  virtual ~KODE_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDigits(uint32_t ADigits) {
    MDigits = ADigits;
  }

  virtual void setDrawValueText(bool ADraw=true) {
    MDrawValueText = ADraw;
  }

  virtual void setValueTextAlignment(uint32_t AAlignment) {
    MValueTextAlignment = AAlignment;
  }

  virtual void setValueTextColor(KODE_Color AColor) {
    MValueTextColor = AColor;
  }

  virtual void setValueTextOffset(KODE_FRect AOffset) {
    MValueTextOffset = AOffset;
  }

  virtual void setDrawLabel(bool ADraw=true) {
    MDrawLabel = ADraw;
  }

  virtual void setLabel(const char* ALabel) {
    MLabel = ALabel;
  }

  virtual void setLabelColor(KODE_Color AColor) {
    MLabelColor = AColor;
  }

//------------------------------
public:
//------------------------------

  virtual void drawValueText(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {

    if (MDrawValueText) {

      float value = getValue();
      const char*     label = "";

      KODE_Parameter* param = getParameter();

      if (param) {
        label = param->getLabel();
        param->getDisplayString(value,MValueText);
      }
      else {
        label = MLabel;
        KODE_FloatToString(MValueText,value,MDigits);
      }

      KODE_FRect value_rect = getRect();
      KODE_FRect label_rect = getRect();

      value_rect.shrink(MValueTextOffset);
      label_rect.shrink(MValueTextOffset);

      if (MDrawLabel) {
        float width = APainter->getTextWidth(label);
        label_rect.x = value_rect.x2() - width;
        label_rect.w = width;
        value_rect.w -= (width + KODE_VALUE_WIDGET_LABEL_SPACE);
      }

      if (MDrawValueText) {
        APainter->drawText(value_rect,MValueText,MValueTextAlignment,MValueTextColor);
      }
      if (MDrawLabel) {
        APainter->drawText(label_rect,label,MValueTextAlignment,MLabelColor);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    drawText(APainter,ARect,AMode);
    drawValueText(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
