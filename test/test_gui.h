
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

    KODE_PanelWidget* panel1 = KODE_New KODE_PanelWidget(KODE_FRect(170,100));
    panel1->setAlignment(KODE_WIDGET_ALIGN_FILL_RIGHT);
    panel1->setInnerBorder(KODE_FRect(10));
    panel1->setWidgetSpacing(KODE_FPoint(10));
    window->appendChildWidget(panel1);

    KODE_PanelWidget* panel2 = KODE_New KODE_PanelWidget(KODE_FRect(100));
    panel2->setAlignment(KODE_WIDGET_ALIGN_FILL_BOTTOM);
    panel2->setInnerBorder(KODE_FRect(10));
    panel2->setWidgetSpacing(KODE_FPoint(5));
    window->appendChildWidget(panel2);

    KODE_PanelWidget* panel3 = KODE_New KODE_PanelWidget(KODE_FRect(100));
    panel3->setAlignment(KODE_WIDGET_ALIGN_FILL_LEFT);
    panel3->setInnerBorder(KODE_FRect(10));
    panel3->setWidgetSpacing(KODE_FPoint(10));
    window->appendChildWidget(panel3);

    KODE_PanelWidget* panel4 = KODE_New KODE_PanelWidget(KODE_FRect(100));
    panel4->setAlignment(KODE_WIDGET_ALIGN_FILL_TOP);
    panel4->setInnerBorder(KODE_FRect(10));
    panel4->setWidgetSpacing(KODE_FPoint(10));
    window->appendChildWidget(panel4);

    //

    KODE_Widget* widget = KODE_New KODE_SliderWidget(KODE_FRect(150,22));
    widget->setValue(0.0f);
    widget->setAlignment(KODE_WIDGET_ALIGN_FILL_TOP);
    widget->setParameter(parameter);
    //widget->on_widget_connect(parameter);
    panel1->appendChildWidget(widget);

    widget = KODE_New KODE_SliderWidget(KODE_FRect(150,22));
    widget->setAlignment(KODE_WIDGET_ALIGN_FILL_TOP);
    panel2->appendChildWidget(widget);

    widget = KODE_New KODE_SliderWidget(KODE_FRect(150,22));
    widget->setAlignment(KODE_WIDGET_ALIGN_FILL_TOP);
    panel2->appendChildWidget(widget);

  }

  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
  KODE_Delete parameter;
  return 0;
}
