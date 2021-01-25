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
private:
//------------------------------


//------------------------------
protected:
//------------------------------

  KODE_FRect  MBorderOffset         = KODE_FRect(5,0,5,0);

  bool        MFillBackground       = true;
  KODE_Color  MBackgroundColor      = KODE_Color(0.5f);
  KODE_Color  MHoverBackgroundColor = KODE_Color(0.6f);
  KODE_Color  MInterBackgroundColor = KODE_Color(0.7f);

  bool        MDrawBorder           = true;
  KODE_Color  MBorderColor          = KODE_Color(0.7f);
  KODE_Color  MHoverBorderColor     = KODE_Color(0.8f);
  KODE_Color  MInterBorderColor     = KODE_Color(0.9f);

//------------------------------
public:
//------------------------------

  KODE_PanelWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    MName = "KODE_PanelWidget";
  }

  virtual ~KODE_PanelWidget() {
  };

//------------------------------
protected:
//------------------------------

  virtual void fillBackground(KODE_Painter* APainter) {
    if (MFillBackground) {
      //KODE_FRect rect = MRect;
      //rect.shrink(MBorderOffset);
      KODE_Color color = MBackgroundColor;
      if (MIsClicked) color = MInterBackgroundColor;
      else if (MIsHovering)  color = MHoverBackgroundColor;
      APainter->fillRect(MRect,color);
    }
  }

  virtual void drawBorder(KODE_Painter* APainter) {
    if (MDrawBorder) {
      //KODE_FRect rect = MRect;
      //rect.shrink(MBorderOffset);
      APainter->drawRect(MRect,MBorderColor);
    }
  }

//------------------------------
public:
//------------------------------

  KODE_FRect getBorderOffset()                    { return MBorderOffset; };
  void setBorderOffset(KODE_FRect AOffset)        { MBorderOffset = AOffset; };

  void setDrawBorder(bool ADraw=true)             { MDrawBorder = ADraw; }
  void setBorderColor(KODE_Color AColor)          { MBorderColor = AColor; }
  void setHoverBorderColor(KODE_Color AColor)     { MHoverBorderColor = AColor; }
  void setInterBorderColor(KODE_Color AColor)     { MInterBorderColor = AColor; }

  void setFillBackground(bool AFill=true)         { MFillBackground = AFill; }
  void setBackgroundColor(KODE_Color AColor)      { MBackgroundColor = AColor; }
  void setHoverBackgroundColor(KODE_Color AColor) { MHoverBackgroundColor = AColor; }
  void setInterBackgroundColor(KODE_Color AColor) { MInterBackgroundColor = AColor; }

//------------------------------
public: // widget
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) override {
    APainter = APainter;
    fillBackground(APainter);
    paintChildWidgets(APainter,ARect,AMode);
    drawBorder(APainter);
  }

};

//----------------------------------------------------------------------
#endif

