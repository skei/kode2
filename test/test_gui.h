
#define KODE_GUI_XCB
#define KODE_PAINTER_CAIRO

//#define KODE_NO_WINDOW_BUFFERING

#include "base/kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

int main() {

  KODE_Parameter* parameter = KODE_New KODE_Parameter("param1",0.75f);

  KODE_Window* window = KODE_New KODE_Window(640,480,"hello world!");
  window->setFillBackground();
  window->setBackgroundColor(0xff666666);
  {
    KODE_Widget* widget = KODE_New KODE_SliderWidget(KODE_FRect(10,10,150,22));
    widget->setValue(0.0f);
    widget->setParameter(parameter);
    window->appendChildWidget(widget);

    widget = KODE_New KODE_SliderWidget(KODE_FRect(10,37,150,22));
    window->appendChildWidget(widget);

    widget = KODE_New KODE_SliderWidget(KODE_FRect(10,64,150,22));
    window->appendChildWidget(widget);


  }
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;


  KODE_Delete parameter;
  return 0;
}
