#ifndef kode_border_widget_included
#define kode_border_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
//#include "base/kode_color.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

class KODE_BorderWidget
: public KODE_BackgroundWidget {

//private:
protected:

  bool        MDrawBorder   = true;
  KODE_Color  MBorderColor  = KODE_Color(0.5f);

//------------------------------
public:
//------------------------------

  KODE_BorderWidget(KODE_FRect ARect)
  : KODE_BackgroundWidget(ARect) {
  }

  //----------

  virtual ~KODE_BorderWidget() {
  }

//------------------------------
public:
//------------------------------


  virtual void setDrawBorder(bool ADraw=true)    { MDrawBorder = ADraw; }
  virtual void setBorderColor(KODE_Color AColor) { MBorderColor = AColor; }

  virtual KODE_Color getBackgroundColor() { return MBackgroundColor; }

//------------------------------
public:
//------------------------------

  virtual void drawBorder(KODE_Painter* APainter, KODE_FRect ARect) {
    if (MDrawBorder) {
      APainter->drawRect(MRect,MBorderColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect);
    //KODE_Widget::on_paint(APainter,ARect);
    paintChildWidgets(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
