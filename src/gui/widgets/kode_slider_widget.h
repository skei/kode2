#ifndef kode_slider_widget_included
#define kode_slider_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_value_widget.h"

//----------------------------------------------------------------------

class KODE_SliderWidget
: public KODE_ValueWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawValueBar       = true;
  KODE_Color  MValueBarColor      = KODE_Color(0.5f, 0.5f, 0.0f);
  uint32_t    MValueBarDirection  = KODE_RIGHT;
  KODE_FRect  MValueBarOffset     = KODE_FRect(1);

//------------------------------
public:
//------------------------------

  KODE_SliderWidget(KODE_FRect ARect)
  : KODE_ValueWidget(ARect) {
  }

  virtual ~KODE_SliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValueBar(bool ADraw=true) {
    MDrawValueBar = ADraw;
  }

  virtual void setValueBarColor(KODE_Color AColor) {
    MValueBarColor = AColor;
  }

  virtual void setValueBarDirection(uint32_t ADirection) {
    MValueBarDirection = ADirection;
  }

  //----------

  virtual void drawValueBar(KODE_IPainter* APainter) {
    if (MDrawValueBar) {
      KODE_FRect  rect  = MRect;
      float       value = MValue;
      rect.shrink(MValueBarOffset);
      float w = rect.w * value;
      float h = rect.h * value;
      switch (MValueBarDirection) {
        case KODE_LEFT:
          rect.x = rect.x2() - w;
          rect.w = w;
          break;
        case KODE_RIGHT:
          rect.w = w;
          break;
        case KODE_UP:
          rect.y = rect.y2() - h;
          rect.h = h;
          break;
        case KODE_DOWN:
          rect.h = h;
          break;
      }
      APainter->fillRect(rect,MValueBarColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_IPainter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter);
    paintChildren(APainter,ARect,AMode);
    drawValueBar(APainter);
    drawText(APainter);
    drawValueText(APainter);
    drawBorder(APainter);
  }

};

//----------------------------------------------------------------------
#endif
