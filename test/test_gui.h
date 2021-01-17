
#define KODE_GUI_XCB
//#define KODE_DEBUG_XCB
#define KODE_CAIRO

//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_image.h"
#include "gui/kode_painter.h"
#include "gui/kode_surface.h"
#include "gui/kode_window.h"
#include "gui/kode_widget.h"

#include "gfx/kode_ctx.h"

//----------------------------------------------------------------------

//void calc_bitmap(KODE_Bitmap* ABitmap) {
//  uint32_t* ptr = ABitmap->getBuffer();
//  uint32_t w = ABitmap->getWidth();
//  uint32_t h = ABitmap->getHeight();
//  for (uint32_t y=0; y<h; y++) {
//    for (uint32_t x=0; x<w; x++) {
//      uint8_t r = x;
//      uint8_t g = y;
//      uint8_t b = 0;
//      uint8_t a = 255;
//      uint32_t c = (a << 24) + (r << 16) + (g << 8) + b;
//      *ptr++ = c;
//    }
//  }
//}
//
////----------
//
////#include "../data/lemuria.h"
////#include "../data/font1.h"
//
//void test_painter(KODE_Bitmap* bitmap) {
//  KODE_Bitmap* png = KODE_New KODE_Bitmap(lemuria,lemuria_size);
//  KODE_Bitmap* subwindow = bitmap->createSubBitmap(128,0,128,256);
//  //bitmap->loadFont("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
//  bitmap->initFont(font1);
//  calc_bitmap(bitmap);
//  bitmap->blendBitmap(0,0,png,160);
//  subwindow->rotozoom(png, KODE_PI05/2, 0.3f, 0,0, 0,0, true);
//  bitmap->drawHLine(     10, 20,   100,          0xffffff);
//  bitmap->drawVLine(     20, 10,   100,          0xffffff);
//  bitmap->drawRectangle( 30, 30,   100,  100,    0xffffff);
//  bitmap->fillRectangle( 40, 40,    90,   90,    0xffffff);
//  bitmap->drawLine(      50, 50,    70,   80,    0x000000);
//  //bitmap->drawWideLine(  60, 50,    80,   80, 5, 0x000000);
//  bitmap->drawQuadBezier(  110, 10,   150,   10,  150,  50,             0x00ff00);
//  bitmap->drawCubicBezier( 110, 60,   130,   60,  130, 100,   150, 100, 0xffffff);
//  bitmap->setFontSize(20);
//  bitmap->drawText(10,180,"line2",0x00ffff);
//  bitmap->setFontSize(40);
//  bitmap->drawText(10,150,"Hello world",0xffff00);
//  bitmap->freeFont();
//  //bitmap->drawArc(128,128, 120,60, 0, 0,2);
//  KODE_Delete subwindow;
//  KODE_Delete png;
//}

//----------------------------------------------------------------------

void test_cairo(KODE_Bitmap* bitmap) {
  #define ARC_WIDTH     16
  #define NEEDLE_WIDTH  8
  float value = 0.3f;
  float x = 128.0;
  float y = 128.0;
  float radius = 100.0;
  float angle1 = -0.65f;
  float angle2 = angle1 + (value * 0.8f);
  float angle3 = angle1 + 0.8f;
  float needle_length = radius - (ARC_WIDTH);
  cairo_surface_t* cs = bitmap->createCairoSurface();
  cairo_t* cr = cairo_create(cs);
  angle1 *= KODE_PI2;
  angle2 *= KODE_PI2;
  angle3 *= KODE_PI2;
  cairo_set_line_width(cr,ARC_WIDTH);
  cairo_set_source_rgba(cr,0.5f,0.5f,0.5f,1);
  cairo_arc(cr,x,y,radius,angle1,angle3);
  cairo_stroke(cr);
  cairo_set_source_rgba(cr,1,1,1,1);
  cairo_arc(cr,x,y,radius,angle1,angle2);
  cairo_stroke(cr);
  cairo_set_source_rgba(cr,1,0,0,1);
  //cairo_set_line_width(cr,6);
  //cairo_arc(cr,x,y,10.0,0,KODE_PI2);
  //cairo_fill(cr);
  //cairo_arc (cr, xc, yc, radius, angle1, angle1);
  //cairo_line_to(cr, xc, yc);
  //cairo_arc (cr, xc, yc, radius, angle2, angle2);
  //cairo_line_to (cr, xc, yc);
  float v = ((1.0f - value) * 0.8f) + 0.1f; // 0.1 = (1.0 - 0.8) / 2.0
  float a = sinf( v * KODE_PI2  );
  float b = cosf( v * KODE_PI2 );
  float x1  = x;
  float y1  = y;
  float x2  = x + (a * needle_length);
  float y2  = y + (b * needle_length);
  cairo_set_source_rgba(cr,1,1,0,1);
  cairo_set_line_width(cr,NEEDLE_WIDTH);
  cairo_move_to(cr,x1,y1);
  cairo_line_to(cr,x2,y2);
  cairo_stroke (cr);
  cairo_surface_flush(cs);
  //MPainter->blit(0,0,MImage,0,0,128,256); // srcW/srcH doesn't work with xcbPainter (image -> surface)
  cairo_surface_destroy(cs);
  cairo_destroy(cr);
}

//----------------------------------------------------------------------

void test_ctx(KODE_Bitmap* bitmap) {
  KODE_Ctx* ctx = KODE_New KODE_Ctx(bitmap);
  //ctx->setColor(0xff888888);
  //ctx->rectangle(0,0,256,256);
  //ctx->fill();
  ctx->loadFont("ubuntu-r","/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf");
  ctx->setFont("ubuntu-r");
  ctx->setFontSize(50);
  ctx->setColor(0xffffffff);
  ctx->moveTo(32,128);
  ctx->setLineWidth(1);
  ctx->strokeText("hello world!");
  ctx->flush();
  KODE_Delete ctx;
}

//----------------------------------------------------------------------

class myWindow
: public KODE_Window {

private:

  KODE_Bitmap*  MPng      = KODE_NULL;
  KODE_Bitmap*  MBitmap   = KODE_NULL;
  KODE_Image*   MImage    = KODE_NULL;
  KODE_Surface* MSurface  = KODE_NULL;
  KODE_Painter* MPainter  = KODE_NULL;

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle, void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {

    MBitmap = KODE_New KODE_Bitmap(256,256);
    //MBitmap->fill(0x880000);

    //----------

    test_ctx(MBitmap);

    MImage = KODE_New KODE_Image(this,MBitmap);
    MSurface  = KODE_New KODE_Surface(this,256,256);

    // copy image -> surface
    MPainter  = KODE_New KODE_Painter(MSurface);
    MPainter->blit(0,0,MImage,0,0,256,256);
    KODE_Delete MPainter;

  }

  virtual ~myWindow() {
    if (MBitmap) KODE_Delete MBitmap;
    if (MImage) KODE_Delete MImage;
    if (MSurface) KODE_Delete MSurface;
  }

public:

public:

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {
    APainter->setFillColor(0x666666);
    APainter->fillRectangle(ARect.x,ARect.y,ARect.w,ARect.h);
    APainter->blit( 10,10,MImage,  0,0,256,256); // does xcb_put_image work with stride <> width*4 ??
    //APainter->blit(276,10,MSurface,0,0,256,256);
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

