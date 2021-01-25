#ifndef kode_text_widget_included
#define kode_text_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_color.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_border_widget.h"

class KODE_TextWidget
//: public KODE_Widget {
: public KODE_BorderWidget {

//------------------------------
//private:
protected:
//------------------------------

  bool        MDrawText       = true;
  const char* MText           = "";
  KODE_Color  MTextColor      = KODE_Color(0.3f);
  uint32_t    MTextAlignment  = KODE_TEXT_ALIGN_LEFT;
  float       MTextOffset     = 3.0f;
  float       MTextSize       = 14.0f;

//------------------------------
public:
//------------------------------

  KODE_TextWidget(KODE_FRect ARect, const char* AText)
  : KODE_BorderWidget(ARect) {
    MDrawText = true;
    MText = AText;
    //MTextColor = AColor;
    //MTextAlignment = AAlignment;
  }

  //----------

  virtual ~KODE_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw=true)           { MDrawText = ADraw; }
  virtual void setText(const char* AText)             { MText = AText; }
  virtual void setTextColor(KODE_Color AColor)        { MTextColor = AColor; }
  virtual void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }
  virtual void setTextOffset(float AOffset)           { MTextOffset = AOffset; }

//------------------------------
public:
//------------------------------

  virtual void drawText(KODE_Painter* APainter, KODE_FRect ARect) {
    if (MDrawText) {
      KODE_FRect r = MRect;
      switch (MTextAlignment) {
        case KODE_TEXT_ALIGN_LEFT:    r.x += MTextOffset; break;
        case KODE_TEXT_ALIGN_RIGHT:   r.w -= MTextOffset; break;
        case KODE_TEXT_ALIGN_TOP:     r.y += MTextOffset; break;
        case KODE_TEXT_ALIGN_BOTTOM:  r.h -= MTextOffset; break;
      }
      APainter->drawText(r,MText,MTextColor,MTextAlignment);
    }
  }

    //------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,MRect);
    drawText(APainter,MRect);
    paintChildWidgets(APainter,MRect,AMode);
    drawBorder(APainter,MRect);
    //KODE_BorderWidget::on_paint(APainter,ARect);
  }

};

//----------------------------------------------------------------------
#endif
