#ifndef kode_slider_widget_included
#define kode_slider_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_drag_value_widget.h"

//----------------------------------------------------------------------

class KODE_SliderWidget
: public KODE_DragValueWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawValueBar             = true;
  KODE_Color  MValueBarColor            = KODE_Color(0.7);
  uint32_t    MValueBarDirection        = KODE_RIGHT;
  KODE_FRect  MValueBarOffset           = KODE_FRect(1,1,1,1);
  KODE_Color  MInteractiveValueBarColor = KODE_Color(0.9);

//------------------------------
public:
//------------------------------

  KODE_SliderWidget(KODE_FRect ARect)
  : KODE_DragValueWidget(ARect) {
    setName("KODE_SliderWidget");
    setHint("slider");
    MDrawBorder       = true;
    MBackgroundColor  = KODE_Color(0.7);
    MTextColor        = KODE_Color(0);
    MValueTextColor   = KODE_Color(0);
    MTextOffset       = KODE_FRect(6,0,0,0);
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

  virtual void drawValueBar(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    if (MDrawValueBar) {
      KODE_FRect  rect  = getRect();
      float       value = getValue();
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
      if (isDragging()) {
        APainter->fillRectangle(rect,MInteractiveValueBarColor);
      }
      else {
        APainter->fillRectangle(rect,MValueBarColor);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    drawValueBar(APainter,ARect,AMode);
    drawText(APainter,ARect,AMode);
    drawValueText(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
