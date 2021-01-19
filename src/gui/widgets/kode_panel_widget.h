#ifndef kode_panel_widget_included
#define kode_panel_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_PanelWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  bool      MFillBackground   = true;
  uint32_t  MBackgroundColor  = 0xff666666;

//------------------------------
public:
//------------------------------

  KODE_PanelWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
  }

  virtual ~KODE_PanelWidget() {
  };

//------------------------------
public:
//------------------------------

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(uint32_t AColor)  { MBackgroundColor = AColor; }

//------------------------------
public: // widget
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {
    if (MFillBackground) {
      KODE_FRect rect = MRect;
      rect.intersect(ARect);
      APainter->fillRect(rect,MBackgroundColor);
    }
    KODE_Widget::on_widget_paint(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif

