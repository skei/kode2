
#define KODE_GUI_XCB

//#define KODE_NO_WINDOW_BUFFERING

#include "kode.h"

//----------------------------------------------------------------------

class myWindow : public KODE_Window {

private:

  KODE_Image*   MImage    = KODE_NULL;
  KODE_Surface* MSurface  = KODE_NULL;

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
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

  void on_window_paint(uint32_t AXpos, uint32_t AYpos, uint32_t AWidth, uint32_t AHeight) final {
    fill(AXpos,AYpos,AWidth,AHeight,KODE_Color(0.3,0,0));
    blit( 10,10,MImage);
    blit(120,10,MSurface);
    //KODE_Painter* painter = MBufferPainter;// MWindowPainter;
    KODE_Painter* painter = KODE_New KODE_Painter(this);
    //painter->fillRect(KODE_FRect(AXpos,AYpos,AWidth,AHeight),KODE_Color(0.3f));
    painter->drawBitmap(10,120,MImage,KODE_FRect(0,0,100,100)); // only full image (width)
    painter->drawBitmap(120,120,MSurface,KODE_FRect(0,0,100,100));
    KODE_Delete painter;
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
