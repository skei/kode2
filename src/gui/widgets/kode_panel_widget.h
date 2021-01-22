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
  uint32_t    MBackgroundColor      = 0xff505050;
  uint32_t    MHoverBackgroundColor = 0xff484848;
  uint32_t    MInterBackgroundColor = 0xff483838;

  bool        MDrawBorder           = true;
  uint32_t    MBorderColor          = 0xff909090;
  uint32_t    MHoverBorderColor     = 0xffb0b0b0;
  uint32_t    MInterBorderColor     = 0xffb0a0a0;

//------------------------------
public:
//------------------------------

  KODE_PanelWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
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
      uint32_t color = MBackgroundColor;
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

  KODE_FRect getBorderOffset()                  { return MBorderOffset; };
  void setBorderOffset(KODE_FRect AOffset)      { MBorderOffset = AOffset; };

  void setDrawBorder(bool ADraw=true)           { MDrawBorder = ADraw; }
  void setBorderColor(uint32_t AColor)          { MBorderColor = AColor; }
  void setHoverBorderColor(uint32_t AColor)     { MHoverBorderColor = AColor; }
  void setInterBorderColor(uint32_t AColor)     { MInterBorderColor = AColor; }

  void setFillBackground(bool AFill=true)       { MFillBackground = AFill; }
  void setBackgroundColor(uint32_t AColor)      { MBackgroundColor = AColor; }
  void setHoverBackgroundColor(uint32_t AColor) { MHoverBackgroundColor = AColor; }
  void setInterBackgroundColor(uint32_t AColor) { MInterBackgroundColor = AColor; }

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

