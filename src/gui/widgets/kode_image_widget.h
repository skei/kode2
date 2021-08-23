#ifndef kode_image_widget_included
#define kode_image_widget_included
//----------------------------------------------------------------------

#include "gui//kode_image.h"
#include "gui/widgets/kode_panel_widget.h"

class KODE_ImageWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

  bool MSurfaceAllocated = false;

//------------------------------
protected:
//------------------------------

  bool          MDrawImage        = true;
  KODE_Surface* MSurface          = KODE_NULL;


//------------------------------
public:
//------------------------------

  KODE_ImageWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("KODE_ImageWidget");
    MFillBackground = false;
    MDrawBorder = false;
  }

  //----------

  virtual ~KODE_ImageWidget() {
    if (MSurfaceAllocated && MSurface) KODE_Delete MSurface;
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawImage(bool ADraw) {
    MDrawImage = ADraw;
  }

  //----------

  virtual void setImage(KODE_Drawable* ATarget, KODE_Surface* ASurface) {
    MSurface = ASurface;
    MSurfaceAllocated = false;
  }


  virtual void setImage(KODE_Drawable* ATarget, KODE_Image* AImage) {
    MSurface = KODE_New KODE_Surface(ATarget,AImage->getWidth(),AImage->getHeight());
    MSurfaceAllocated = true;
    KODE_Painter* painter = KODE_New KODE_Painter(MSurface);
    painter->drawBitmap(0,0,AImage);
  }

  //----------

  virtual void setImage(KODE_Drawable* ATarget, KODE_Bitmap* ABitmap) {
    MSurface = KODE_New KODE_Surface(ATarget,ABitmap->getWidth(),ABitmap->getHeight());
    MSurfaceAllocated = true;
    KODE_Painter* painter = KODE_New KODE_Painter(MSurface);
    painter->uploadBitmap(0,0,ABitmap);
  }

  //----------

  virtual void setImage(KODE_Drawable* ATarget, uint8_t* ABuffer, uint32_t ASize) {
    KODE_Bitmap* bitmap = KODE_New KODE_Bitmap(ABuffer,ASize);
    bitmap->premultAlpha();
    setImage(ATarget,bitmap);
    KODE_Delete bitmap;
  }

  //----------

  virtual void setImage(KODE_Drawable* ATarget, const char* AFilename) {
    KODE_Bitmap* bitmap = KODE_New KODE_Bitmap(AFilename);
    bitmap->premultAlpha();
    setImage(ATarget,bitmap);
    KODE_Delete bitmap;
  }

  //----------

  virtual void drawImage(KODE_BasePainter* APainter) {
    if (MDrawImage) {
      if (MSurface) {
        KODE_FRect rect = KODE_FRect(0,0,MSurface->getWidth(),MSurface->getHeight());
        APainter->drawBitmap(getRect().x,getRect().y,MSurface,rect);
      }
      //else if (MBitmap) {
      //  APainter->uploadBitmap(getRect().x,getRect().y,MBitmap);
      //}
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
