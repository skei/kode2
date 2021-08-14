#ifndef kode_image_widget_included
#define kode_image_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_panel_widget.h"

class KODE_ImageWidget
: public KODE_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool            MDrawImage  = true;
  KODE_FRect      MImageRect  = KODE_FRect(0);
  KODE_Drawable*  MDrawable   = KODE_NULL;
  KODE_Bitmap*    MBitmap     = KODE_NULL;
  //KODE_Surface*   MSurface    = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_ImageWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    MFillBackground = false;
    MDrawBorder = false;
  }

  //----------

  virtual ~KODE_ImageWidget() {
    //if (MSurface) KODE_Delete MSurface;
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawImage(bool ADraw) {
    MDrawImage = ADraw;;
  }

  //----------

  virtual void setImage(KODE_Drawable* ADrawable) {
    MDrawable = ADrawable;
    MImageRect = KODE_FRect(0,0,ADrawable->getWidth(),ADrawable->getHeight());
  }

  //----------

  virtual void setImage(KODE_Bitmap* ABitmap) {
    MBitmap = ABitmap;
    MImageRect = KODE_FRect(0,0,ABitmap->getWidth(),ABitmap->getHeight());
    //MSurface = KODE_New KODE_Surface(ATarget,MBitmap->getWidth(),MBitmap->getHeight());
    //KODE_Painter* painter = KODE_New KODE_Painter(MSurface);
    //painter->uploadBitmap(0,0,MBitmap);
  }

  virtual void setImage(uint8_t* ABuffer, uint32_t ASize) {
    MBitmap = KODE_New KODE_Bitmap(ABuffer,ASize);
    MBitmap->premultAlpha( MBackgroundColor );
  }

  //----------

  virtual void drawImage(KODE_BasePainter* APainter) {
    if (MDrawImage) {
      if (MDrawable) {
        APainter->drawBitmap(MRect.x,MRect.y,MDrawable,MImageRect);
      }
      else if (MBitmap) {
        APainter->uploadBitmap(MRect.x,MRect.y,MBitmap);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter);
    drawImage(APainter);
    paintChildren(APainter,ARect,AMode);
    drawBorder(APainter);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
