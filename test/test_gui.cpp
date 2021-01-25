
#define KODE_GUI_XCB
#define KODE_PAINTER_CAIRO
//#define KODE_PAINTER_XCB

//#define KODE_NO_WINDOW_BUFFERING

#include "base/kode.h"
#include "base/types/kode_color.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

  KODE_PanelWidget* addPanel(KODE_Window* window, float width, float height, uint32_t align) {
    KODE_PanelWidget* panel = KODE_New KODE_PanelWidget(KODE_FRect(width,height));
    panel->setAlignment(align);
    panel->setInnerBorder(KODE_FRect(10));
    panel->setWidgetSpacing(KODE_FPoint(5));
    window->appendChildWidget(panel);
    return panel;
  }

//----------------------------------------------------------------------

int main() {

  KODE_Parameter* parameter = KODE_New KODE_Parameter("param1",0.75f);
  KODE_Window* window = KODE_New KODE_Window(640,480,"hello world!");
  window->setFillBackground();
  //window->setBackgroundColor( KODE_Color(0) );

    KODE_Surface* sa_surface = KODE_New KODE_Surface(window,40,40);
    KODE_SAHeaderWidget* sa_header = KODE_New KODE_SAHeaderWidget(sa_surface,"test_plugin","v0.0.1","VST3");
    window->appendChildWidget(sa_header);

    KODE_PanelWidget* panel1 = addPanel(window,170,100,KODE_WIDGET_ALIGN_FILL_RIGHT);
    KODE_PanelWidget* panel2 = addPanel(window,100,100,KODE_WIDGET_ALIGN_FILL_BOTTOM);
    KODE_PanelWidget* panel3 = addPanel(window,100,100,KODE_WIDGET_ALIGN_FILL_LEFT);
    KODE_PanelWidget* panel4 = addPanel(window,100,100,KODE_WIDGET_ALIGN_FILL_TOP);

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

    widget = KODE_New KODE_SliderWidget(KODE_FRect(150,22));
    widget->setName("X");
    widget->setAlignment(KODE_WIDGET_ALIGN_FILL_TOP);
    panel3->appendChildWidget(widget);

    widget = KODE_New KODE_SliderWidget(KODE_FRect(150,22));
    widget->setAlignment(KODE_WIDGET_ALIGN_FILL_TOP);
    panel4->appendChildWidget(widget);

  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
  KODE_Delete parameter;

  KODE_Delete sa_surface;

  return 0;
}
