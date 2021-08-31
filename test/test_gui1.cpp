
#define KODE_GUI_XCB

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_surface.h"
//#include "gui/kode_image.h"
#include "gui/kode_painter.h"

//----------------------------------------------------------------------

class myWindow : public KODE_Window {

private:



public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
  }

  virtual ~myWindow() {
  }

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
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
