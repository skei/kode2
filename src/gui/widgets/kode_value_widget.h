#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

#define KODE_VALUE_WIDGET_LABEL_SPACE 4.0f

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

  bool        MDrawLabel          = false;
  const char* MLabel              = "";
  KODE_Color  MLabelColor         = KODE_Color(0.8f);

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

  virtual void setDrawValueText(bool ADraw=true) {
    MDrawValueText = ADraw;
  }

  virtual void setValueTextAlignment(uint32_t AAlignment) {
    MValueTextAlignment = AAlignment;
  }

  virtual void setValueTextColor(KODE_Color AColor) {
    MValueTextColor = AColor;
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

  //----------

  virtual void drawValueText(KODE_IPainter* APainter) {
    if (MDrawValueText) {
      KODE_FRect  value_rect  = MRect;
      KODE_FRect  label_rect  = MRect;
      float       value       = MValue;
      const char* label       = MLabel;
      if (MParameter) {
        value = MParameter->from01(value);
        label = MParameter->getLabel();
      }
      value_rect.shrink(MValueTextOffset);
      label_rect.shrink(MValueTextOffset);
      if (MDrawLabel) {
        float width = APainter->getTextWidth(label);
        label_rect.x = value_rect.x2() - width;
        label_rect.w = width;
        value_rect.w -= (width + KODE_VALUE_WIDGET_LABEL_SPACE);
      }
      KODE_FloatToString(MValueText,value);
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

  void on_widget_paint(KODE_IPainter* APainter, KODE_FRect ARect, uint32_t AMode) override {
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
