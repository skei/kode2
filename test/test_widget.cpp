
#define KODE_GUI_CAIRO

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_parameters.h"

const char* b8_txt[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };

int main() {
  KODE_Window* window = new KODE_Window(640,480,"test_widget");

    window->setFillBackground(true);

    KODE_IntParameter* param = new KODE_IntParameter("param", "%", 100, 50, 200 );
    KODE_RangeSliderWidget* rangeslider = new KODE_RangeSliderWidget(KODE_FRect(10,10,200,20));
    rangeslider->setValue(0.2);
    rangeslider->setValue2(0.8);
    rangeslider->setParameter(param,0);
    window->appendWidget(rangeslider);

    KODE_ButtonRowWidget* buttonrow = new KODE_ButtonRowWidget(KODE_FRect(10,40,200,20),8,b8_txt,KODE_BUTTON_ROW_MULTI);
    window->appendWidget(buttonrow);

  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
