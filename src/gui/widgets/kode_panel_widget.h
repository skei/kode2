#ifndef kode_panel_widget_included
#define kode_panel_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_PanelWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  bool MFillBackground        = true;
  KODE_Color MBackgroundColor = KODE_Color(0.6f);

  bool MDrawBorder            = true;
  KODE_Color MBorderColor     = KODE_Color(0.3f);

//------------------------------
public:
//------------------------------

  KODE_PanelWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_PanelWidget");
  }

  virtual ~KODE_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawBorder(bool ADraw) {
    MDrawBorder = ADraw;
  }

  virtual void setBorderColor(KODE_Color AColor) {
    MBorderColor = AColor;
  }

  virtual void setFillBackground(bool AFill) {
    MFillBackground = AFill;
  }

  virtual void setBackgroundColor(KODE_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual void drawBorder(KODE_BasePainter* APainter) {
    if (MDrawBorder) {
      APainter->drawRect(getRect(),MBorderColor);
    }
  }

  virtual void fillBackground(KODE_BasePainter* APainter) {
    if (MFillBackground) {
      APainter->fillRect(getRect(),MBackgroundColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter);
    paintChildren(APainter,ARect,AMode);
    drawBorder(APainter);
  }

  //----------

};

//----------------------------------------------------------------------
#endif
