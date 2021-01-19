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

  KODE_FRect  MInnerBorder    = KODE_FRect(0,0,0,0);

  bool      MFillBackground   = true;
  bool      MDrawBorder       = true;
  uint32_t  MBackgroundColor  = 0xff666666;
  uint32_t  MBorderColor      = 0xff000000;

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

  void setDrawBorder(bool ADraw=true)       { MDrawBorder = ADraw; }
  void setBorderColor(uint32_t AColor)      { MBorderColor = AColor; }

  void setFillBackground(bool AFill=true)   { MFillBackground = AFill; }
  void setBackgroundColor(uint32_t AColor)  { MBackgroundColor = AColor; }

//------------------------------
public: // widget
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {
    KODE_FRect rect = MRect;
    rect.shrink(MInnerBorder);
    //rect.intersect(ARect);
    if (MFillBackground) {
      APainter->fillRect(rect,MBackgroundColor);
    }
    //KODE_Widget::on_widget_paint(APainter,ARect,AMode);
    paintChildWidgets(APainter,ARect,AMode);
    if (MDrawBorder) {
      APainter->drawRect(rect,MBorderColor);
    }
  }

};

//----------------------------------------------------------------------
#endif

