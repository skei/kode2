#ifndef kode_image_widget_included
#define kode_image_widget_included
//----------------------------------------------------------------------

#define KODE_IMAGE_WIDGET_DRAW_NONE     0
#define KODE_IMAGE_WIDGET_DRAW_SURFACE  1
#define KODE_IMAGE_WIDGET_DRAW_BITMAP   2

//#include "gui//kode_image.h"
#include "gui/widgets/kode_panel_widget.h"

class KODE_ImageWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------


//------------------------------
protected:
//------------------------------

  uint32_t      MDrawMode         = KODE_IMAGE_WIDGET_DRAW_SURFACE;
  KODE_Bitmap*  MBitmap           = KODE_NULL;
  KODE_Surface* MSurface          = KODE_NULL;
  bool          MBitmapAllocated  = false;
  bool          MSurfaceAllocated = false;

//------------------------------
public:
//------------------------------

  KODE_ImageWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    setHint("image");
    MFillBackground = false;
    MDrawBorder = false;
  }

  //----------

  KODE_ImageWidget(KODE_FRect ARect, KODE_Bitmap* ABitmap)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    setHint("image");
    MFillBackground = false;
    MDrawBorder = false;
    setBitmap(ABitmap);
  }

  //----------

  KODE_ImageWidget(KODE_FRect ARect,KODE_Drawable* ATarget, KODE_Surface* ASurface)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    setHint("image");
    MFillBackground = false;
    MDrawBorder = false;
    setImage(ATarget,ASurface);
  }

  //----------

  KODE_ImageWidget(KODE_FRect ARect,KODE_Drawable* ATarget, KODE_Bitmap* ABitmap)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    setHint("image");
    MFillBackground = false;
    MDrawBorder = false;
    setImage(ATarget,ABitmap);
  }

  //----------

  KODE_ImageWidget(KODE_FRect ARect,KODE_Drawable* ATarget, uint8_t* ABuffer, uint32_t ASize, KODE_Color ABackground)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    setHint("image");
    MFillBackground = false;
    MDrawBorder = false;
    setImage(ATarget,ABuffer,ASize,ABackground);
  }

  //----------

  KODE_ImageWidget(KODE_FRect ARect,KODE_Drawable* ATarget, const char* AFilename)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    setHint("image");
    MFillBackground = false;
    MDrawBorder = false;
    setImage(ATarget,AFilename,KODE_COLOR_GRAY);
  }

  //----------

  virtual ~KODE_ImageWidget() {
    if (MBitmapAllocated && MBitmap) KODE_Delete MBitmap;
    if (MSurfaceAllocated && MSurface) KODE_Delete MSurface;
  }

//------------------------------
public:
//------------------------------

  /*
    0 = none
    1 = surface
    2 = bitmap
  */

  virtual void setDrawMode(uint32_t AMode) {
    MDrawMode = AMode;
  }

  //----------

  virtual void setBitmap(KODE_Bitmap* ABitmap) {
    MDrawMode = KODE_IMAGE_WIDGET_DRAW_BITMAP;
    MBitmap = ABitmap;
    MBitmapAllocated = false;
  }


  //----------

  virtual void setImage(KODE_Drawable* ATarget, KODE_Surface* ASurface) {
    MDrawMode = KODE_IMAGE_WIDGET_DRAW_SURFACE;
    MSurface = ASurface;
    MSurfaceAllocated = false;
  }


  //virtual void setImage(KODE_Drawable* ATarget, KODE_Image* AImage) {
  //  MDrawMode = KODE_IMAGE_WIDGET_DRAW_SURFACE;
  //  MSurface = KODE_New KODE_Surface(ATarget,AImage->getWidth(),AImage->getHeight());
  //  MSurfaceAllocated = true;
  //  KODE_Painter* painter = KODE_New KODE_Painter(MSurface);
  //  painter->drawBitmap(0,0,AImage);
  //  //painter->flush();
  //  KODE_Delete painter;
  //}

  //----------

  virtual void setImage(KODE_Drawable* ATarget, KODE_Bitmap* ABitmap) {
    MDrawMode = KODE_IMAGE_WIDGET_DRAW_SURFACE;
    MSurface = KODE_New KODE_Surface(ATarget,ABitmap->getWidth(),ABitmap->getHeight());
    MSurfaceAllocated = true;
    KODE_Painter* painter = KODE_New KODE_Painter(MSurface);
    painter->uploadBitmap(0,0,ABitmap);
    //painter->flush();
    KODE_Delete painter;
  }

  //----------

  virtual void setImage(KODE_Drawable* ATarget, uint8_t* ABuffer, uint32_t ASize, KODE_Color ABackground) {
    MDrawMode = KODE_IMAGE_WIDGET_DRAW_SURFACE;
    KODE_Bitmap* bitmap = KODE_New KODE_Bitmap(ABuffer,ASize);
    bitmap->premultAlpha( (uint32_t)ABackground );
    setImage(ATarget,bitmap);
    KODE_Delete bitmap;
  }

  //----------

  virtual void setImage(KODE_Drawable* ATarget, const char* AFilename, KODE_Color ABackground) {
    MDrawMode = KODE_IMAGE_WIDGET_DRAW_SURFACE;
    KODE_Bitmap* bitmap = KODE_New KODE_Bitmap(AFilename);
    bitmap->premultAlpha( (uint32_t)ABackground );
    setImage(ATarget,bitmap);
    KODE_Delete bitmap;
  }

//------------------------------
public:
//------------------------------

  virtual void drawImage(KODE_BasePainter* APainter) {
    switch (MDrawMode) {
      case KODE_IMAGE_WIDGET_DRAW_NONE:
        break;
      case KODE_IMAGE_WIDGET_DRAW_SURFACE:
        if (MSurface) {
          KODE_FRect rect = KODE_FRect(0,0,MSurface->getWidth(),MSurface->getHeight());
          APainter->drawBitmap(getRect().x,getRect().y,MSurface,rect);
        }
        break;
      case KODE_IMAGE_WIDGET_DRAW_BITMAP:
        if (MBitmap) {
          APainter->uploadBitmap(getRect().x,getRect().y,MBitmap);
        }
        break;
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
