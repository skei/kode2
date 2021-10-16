
#define KODE_GUI_CAIRO

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_parameters.h"

const char* b8_txt[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };

int main() {

  KODE_IntParameter* param = new KODE_IntParameter("param", "%", 100, 50, 200 );

  KODE_Window* window = new KODE_Window(640,480,"test_widget");
  window->setFillBackground(true);
  KODE_SliderWidget* slider = new KODE_SliderWidget(KODE_FRect(10,10,200,20));
  window->appendWidget(slider);
  slider->setParameter(param,0);
  slider->setValue(0.2);
  //slider->setValue2(0.8);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
