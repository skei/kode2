#ifndef kode_text_widget_included
#define kode_text_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_panel_widget.h"

//----------------------------------------------------------------------

class KODE_TextWidget
: public KODE_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawText       = true;
  const char* MText           = "";
  KODE_Color  MTextColor      = KODE_Color(0.1f);
  uint32_t    MTextAlignment  = KODE_TEXT_ALIGN_LEFT;
  KODE_FRect  MTextOffset     = KODE_FRect(2);

//------------------------------
public:
//------------------------------

  KODE_TextWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("KODE_SliderWidget");
    MDrawBorder = false;
  }

  virtual ~KODE_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw) {
    MDrawText = ADraw;
  }

  virtual void setText(const char* AText) {
    MText = AText;
  }

  virtual void setTextAlignment(uint32_t AAlignment) {
    MTextAlignment = AAlignment;
  }

  virtual void setTextColor(KODE_Color AColor) {
    MTextColor = AColor;
  }

  virtual void drawText(KODE_BasePainter* APainter) {
    KODE_FRect rect = getRect();
    rect.shrink(MTextOffset);
    if (MDrawText) {
      APainter->drawText(rect,MText,MTextAlignment,MTextColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter);
    paintChildren(APainter,ARect,AMode);
    drawText(APainter);
    drawBorder(APainter);

  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
