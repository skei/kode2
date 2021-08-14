#ifndef kode_color_widget_included
#define kode_color_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_ColorWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  bool MFillBackground        = true;
  KODE_Color MBackgroundColor = KODE_Color(0.6f);

//------------------------------
public:
//------------------------------

  KODE_ColorWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
  }

  //----------

  KODE_ColorWidget(KODE_FRect ARect, KODE_Color AColor)
  : KODE_Widget(ARect) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual ~KODE_ColorWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackgrund(bool AFill) {
    MFillBackground = AFill;
  }

  virtual void setBackgrundColor(KODE_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual void fillBackground(KODE_BasePainter* APainter) {
    if (MFillBackground) {
      APainter->fillRect(MRect,MBackgroundColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter);
    //KODE_Widget::on_widget_paint(APainter,ARect,AMode);
    paintChildren(APainter,ARect,AMode);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
