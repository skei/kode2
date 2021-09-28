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

  bool        MFillBackground   = true;
  KODE_Color  MBackgroundColor  = KODE_Color(0.6f);

  bool        MDrawBorder       = true;
  KODE_Color  MBorderColor      = KODE_Color(0.3f);
  uint32_t    MBorderEdges      = KODE_EDGES_ALL;

//------------------------------
public:
//------------------------------

  KODE_PanelWidget(KODE_FRect ARect=KODE_FRect())
  : KODE_Widget(ARect) {
    setName("KODE_PanelWidget");
    setHint("panel");
  }

  virtual ~KODE_PanelWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawBorder(bool ADraw=true) {
    MDrawBorder = ADraw;
  }

  virtual void setBorderColor(KODE_Color AColor) {
    MBorderColor = AColor;
  }

  virtual void setBorderEdges(uint32_t AEdges) {
    MBorderEdges = AEdges;
  }

  virtual void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
  }

  virtual void setBackgroundColor(KODE_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  virtual void drawBorder(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    APainter->setLineWidth(1);
    if (MDrawBorder) {

      switch (MBorderEdges) {
      }
      if (MBorderEdges == KODE_EDGES_ALL) {
        APainter->drawRectangle(getRect(),MBorderColor);
      }
      else {
        KODE_FRect rect = getRect();
        float x1 = rect.x;
        float y1 = rect.y;
        float x2 = rect.x2();
        float y2 = rect.y2();
        if (MBorderEdges & KODE_EDGE_TOP)     APainter->drawLine(x1,y1,x2,y1,MBorderColor);
        if (MBorderEdges & KODE_EDGE_BOTTOM)  APainter->drawLine(x1,y2,x2,y2,MBorderColor);
        if (MBorderEdges & KODE_EDGE_LEFT)    APainter->drawLine(x1,y1,x1,y2,MBorderColor);
        if (MBorderEdges & KODE_EDGE_RIGHT)   APainter->drawLine(x2,y1,x2,y2,MBorderColor);
      }
    }
  }

  virtual void fillBackground(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    if (MFillBackground) {
      //APainter->fillRectangle(getRect(),MBackgroundColor);
      APainter->fillRectangle(ARect,MBackgroundColor);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {

//    KODE_FRect mrect = getRect();
//    //APainter->resetClip();
//    APainter->setClip(mrect);

    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    paintChildren(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);

//    APainter->resetClip();

  }

  //----------

};

//----------------------------------------------------------------------
#endif
