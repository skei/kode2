
#define KODE_GUI_XCB

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_surface.h"
//#include "gui/kode_image.h"
#include "gui/kode_painter.h"

//----------------------------------------------------------------------

class myWindow : public KODE_Window {

private:

  KODE_Bitmap*  MBitmap   = KODE_NULL;
  KODE_Bitmap*  MBitmap2  = KODE_NULL;
  //KODE_Image*   MImage    = KODE_NULL;
  KODE_Surface* MSurface  = KODE_NULL;

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
    setFillBackground();

    MBitmap = new KODE_Bitmap(256,256);
    MBitmap2 = new KODE_Bitmap(256,256);
    //MBitmap->genFractalCloud(3,20);
    MBitmap->genFractalCloudLayer(0);
    MBitmap->genFractalCloudLayer(1);
    MBitmap->genFractalCloudLayer(2);
    //MBitmap->genFractalCloudLayer(3);
    for (uint32_t y=0; y<AWidth; y++) {
      for (uint32_t x=0; x<AHeight; x++) {
        MBitmap->setLayerValue(3,x,y,255-y);
      }
    }
    MBitmap->premultAlpha(0x000000);
    MBitmap->fillArea(0,0,32,16,0xffffff);

    //MBitmap2->rotozoom( MBitmap, -KODE_PI05,  1.0f, -247,0,  0,0, false);
    MBitmap2->rotate( MBitmap, 128,128, 128,128, 0.1f/*-KODE_PI05*/, 0.9f, 0xff0000);
    //MBitmap2->copyBitmap(0,0,MBitmap);

    MSurface = new KODE_Surface(this,256,256);

    KODE_Painter* painter = new KODE_Painter(MSurface);
    //painter->fillRectangle(KODE_FRect(256,256),KODE_Color(1,1,0));
    painter->uploadBitmap(0,0,MBitmap2);
    delete painter;

  }

  virtual ~myWindow() {
    if (MBitmap)  delete MBitmap;
    if (MBitmap2) delete MBitmap2;
    //if (MImage)   delete MImage;
    if (MSurface) delete MSurface;
  }

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    //APainter->drawBitmap(10,120,MImage,KODE_FRect(0,0,100,100)); // only full image (width)
    APainter->drawBitmap(10,10,MSurface,KODE_FRect(256,256));
  }

};

//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(400,300,"KODE2");
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}

