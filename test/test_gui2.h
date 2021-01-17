
#define KODE_CAIRO
#define KODE_CTX
#define KODE_XCB

#define KODE_GUI_XCB
//#define KODE_DEBUG_XCB

//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_image.h"
#include "gui/kode_surface.h"
#include "gui/kode_window.h"
#include "gui/kode_widget.h"
#include "gui/kode_painter.h"

#include "gui/cairo/kode_cairo_painter.h"
#include "gui/ctx/kode_ctx_painter.h"
//#include "gui/xcb/kode_xcb_painter.h"

//----------------------------------------------------------------------

class myWindow
: public KODE_Window {

private:

  KODE_Image*   MImage    = KODE_NULL;
  KODE_Surface* MSurface  = KODE_NULL;
  KODE_Painter* MPainter  = KODE_NULL;

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle, void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {

    MImage = KODE_New KODE_Image(this,256,256,getDepth());

    KODE_Bitmap* bitmap = MImage->getBitmap();
    bitmap->fill(0xff808080);
    bitmap->fillArea(10,10,200,60,0xffffffff);

    KODE_CtxPainter* ctx_painter = KODE_New KODE_CtxPainter(MImage);
      ctx_painter->setColor(0x80ff0000);
      ctx_painter->setLineWidth(20);
      ctx_painter->moveTo(0,0);
      ctx_painter->curveTo(255,0,255,255,0,255);
      ctx_painter->stroke();
      ctx_painter->flush();
    KODE_Delete ctx_painter;

    MSurface = KODE_New KODE_Surface(this,256,256);
    KODE_Painter* MPainter = KODE_New KODE_Painter(MSurface);
      MPainter->blit(0,0,MImage,128,32,256,256);
      MPainter->setLineWidth(15);
      MPainter->drawLine(10,20,50,200,0x8000ff00);
      MPainter->drawText(50,90,"hello world!",0x8000ff00);
      MPainter->setLineWidth(1);
      MPainter->drawRect(60,100,100,110,0x8000ff00);
    KODE_Delete MPainter;

  }

  virtual ~myWindow() {
    KODE_Delete MImage;
    KODE_Delete MSurface;
  }

public:

  void on_window_paint(uint32_t AXpos, uint32_t AYpos, uint32_t AWidth, uint32_t AHeight) final {

    fill(AXpos,AYpos,AWidth,AHeight,0x666666);

    blit(0,0,MImage);
    blit(276,10,MSurface,0,0,192,192);

    KODE_CairoPainter* painter = KODE_New KODE_CairoPainter(this);
      painter->setColor(0x800000ff);
      painter->setLineWidth(50);
      painter->arc(  300,200,75,0.785375,3.1415,0);
      painter->stroke();
    KODE_Delete painter;

  }

};

//----------------------------------------------------------------------

KODE_Window*  window;

int main(void) {
  window = KODE_New myWindow(640,480,"KODE_Window");
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
  return 0;
}

