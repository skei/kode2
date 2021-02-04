#ifndef kode_panel_widget_included
#define kode_panel_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_color_widget.h"

//----------------------------------------------------------------------

class KODE_PanelWidget
: public KODE_ColorWidget {

//------------------------------
protected:
//------------------------------

  bool MDrawBorder = true;
  KODE_Color MBorderColor = KODE_Color(0.3f);

//------------------------------
public:
//------------------------------

  KODE_PanelWidget(KODE_FRect ARect)
  : KODE_ColorWidget(ARect) {
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

  virtual void drawBorder(KODE_IPainter* APainter) {
    if (MDrawBorder) {
      APainter->drawRect(MRect,MBorderColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_IPainter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    //KODE_ColorWidget::on_widget_paint(APainter,ARect,AMode);
    fillBackground(APainter);
    paintChildren(APainter,ARect,AMode);
    drawBorder(APainter);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
