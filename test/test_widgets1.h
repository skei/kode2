
#define KODE_CAIRO
#define KODE_CTX

#define KODE_XCB
#define KODE_GUI_XCB
//#define KODE_DEBUG_XCB

//#define KODE_NO_WINDOW_BUFFERING

//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widget.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

int main(void) {
  KODE_Window* window = KODE_New KODE_Window(640,480,"KODE_Window");
  window->setFillBackground();
  KODE_Widget* widget;
  widget = KODE_New KODE_testWidget( KODE_FRect(10,10,100,100) );
  window->appendChildWidget(widget);
  widget = KODE_New KODE_testWidget( KODE_FRect(120,10,100,100) );
  window->appendChildWidget(widget);
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
  return 0;
}


