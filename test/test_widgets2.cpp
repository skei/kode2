
#define KODE_GUI_XCB

//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"
#include "gui/kode_window.h"
//#include "plugin/kode_plugin.h"
//#include "plugin/vst3/kode_vst3_instance.h"

//----------------------------------------------------------------------

KODE_Window* create_window() {
  KODE_Window* window = new KODE_Window(800,600,"KODE_Window");
  window->setFillBackground();
  window->appendWidget( KODE_New KODE_SliderWidget( KODE_FRect(64,64,300,20) ));
  return window;
}

//----------------------------------------------------------------------

int main(void) {
  KODE_Window* window = create_window();
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;

}
