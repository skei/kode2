
#define KODE_GUI_XCB

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_surface.h"
#include "gui/kode_image.h"
#include "gui/kode_painter.h"

//----------------------------------------------------------------------

class myWindow : public KODE_Window {

private:

  KODE_Image*   MImage    = KODE_NULL;
  KODE_Surface* MSurface  = KODE_NULL;

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
    setFillBackground();
    MImage = KODE_New KODE_Image(this,100,100);
    MSurface = KODE_New KODE_Surface(this,100,100);
    KODE_Bitmap* bitmap = MImage->getBitmap();
    bitmap->fill(0xff00ff00);
    KODE_Painter* painter = KODE_New KODE_Painter(MSurface);
    painter->fillRect(KODE_FRect(0,0,100,100),KODE_Color(0,0,1));
    painter->drawLine(10,10,90,90,KODE_Color(1));
    painter->drawLine(90,10,10,90,KODE_Color(1));
    KODE_Delete painter;
  }

  virtual ~myWindow() {
    KODE_Delete MImage;
    KODE_Delete MSurface;
  }

  void on_widget_paint(KODE_IPainter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    APainter->drawBitmap(10,120,MImage,KODE_FRect(0,0,100,100)); // only full image (width)
    APainter->drawBitmap(120,120,MSurface,KODE_FRect(0,0,100,100));
  }

};

//----------------------------------------------------------------------

int main() {
  myWindow* window = KODE_New myWindow(400,300,"KODE2");
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
  return 0;
}
