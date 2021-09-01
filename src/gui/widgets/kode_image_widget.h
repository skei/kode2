#ifndef kode_image_widget_included
#define kode_image_widget_included
//----------------------------------------------------------------------

#define KODE_IMAGE_WIDGET_DRAW_NONE     0
#define KODE_IMAGE_WIDGET_DRAW_SURFACE  1
#define KODE_IMAGE_WIDGET_DRAW_BITMAP   2

#include "gui/widgets/kode_panel_widget.h"

class KODE_ImageWidget
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_ImageWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    setHint("image");
    setFillBackground(false);
    setDrawBorder(false);
  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(KODE_BasePainter* APainter) {
    if (MWidgetSurface) {
      KODE_FRect rect = KODE_FRect(0,0,MWidgetSurface->getWidth(),MWidgetSurface->getHeight());
      APainter->drawBitmap(getRect().x,getRect().y,MWidgetSurface,rect);
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect);
    drawImage(APainter);
    paintChildren(APainter,ARect,AMode);
    drawBorder(APainter);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
