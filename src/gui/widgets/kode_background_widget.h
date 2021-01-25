#ifndef kode_color_widget_included
#define kode_color_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
//#include "base/kode_color.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

class KODE_BackgroundWidget
: public KODE_Widget {

//private:
protected:

  bool        MFillBackground   = true;
  KODE_Color  MBackgroundColor  = KODE_Color(0.5f);

//------------------------------
public:
//------------------------------

  KODE_BackgroundWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
  }

  //----------

  virtual ~KODE_BackgroundWidget() {
  }

//------------------------------
public:
//------------------------------


  virtual void setFillBackground(bool ADraw=true)    { MFillBackground = ADraw; }
  virtual void setBackgroundColor(KODE_Color AColor) { MBackgroundColor = AColor; }

  virtual KODE_Color getBackgroundColor() { return MBackgroundColor; }

//------------------------------
public:
//------------------------------

  virtual void fillBackground(KODE_Painter* APainter, KODE_FRect ARect) {
    if (MFillBackground) {
      APainter->fillRect(MRect,MBackgroundColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect);
    paintChildWidgets(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
