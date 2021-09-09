
#define KODE_GUI_XCB

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"
#include "gui/widgets/kode_saheader_widget.h"

//----------------------------------------------------------------------

class myWindow : public KODE_Window {

private:


public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
    setFillBackground();

    // sa header

    //KODE_SAHeaderWidget* header1 = new KODE_SAHeaderWidget( KODE_FRect(10,10,128,64), "test_gui3", "v0.0.0", this );
    //appendWidget(header1);

    // panel

    KODE_PanelWidget* panel1 = new KODE_PanelWidget( KODE_FRect(10,64,128,36) );
    appendWidget(panel1);

    // text

    KODE_TextWidget* text1 = new KODE_TextWidget( KODE_FRect(10,104,128,16) );
    text1->setText("Text");
    appendWidget(text1);

    // value

    KODE_ValueWidget* value1 = new KODE_ValueWidget( KODE_FRect(10,124,128,16) );
    value1->setText("Value");
    appendWidget(value1);

    // drag value

    KODE_DragValueWidget* dragvalue1 = new KODE_DragValueWidget( KODE_FRect(10,144,128,16) );
    dragvalue1->setText("DragValue");
    appendWidget(dragvalue1);

    // slider

    KODE_SliderWidget* slider1 = new KODE_SliderWidget( KODE_FRect(10,164,128,16) );
    slider1->setValue(0.5f);
    slider1->setDefaultValue(0.5f);
    slider1->setText("Slider");
    appendWidget(slider1);

    // button

    KODE_ButtonWidget* button1 = new KODE_ButtonWidget( KODE_FRect(10,184,128,16) );
    button1->setIsToggle(false);
    button1->setText("Button: On", "Button: Off");
    appendWidget(button1);

    // switch

    KODE_ButtonWidget* switch1 = new KODE_ButtonWidget( KODE_FRect(10,204,128,16) );
    switch1->setIsToggle(true);
    switch1->setText("Switch: On", "Switch: Off");
    appendWidget(switch1);

    //------------------------------

    // panel

    KODE_KnobWidget* knob1 = new KODE_KnobWidget( KODE_FRect(148,10,40,40) );
    appendWidget(knob1);

  }

  virtual ~myWindow() {
  }

  //void on_widget_paint(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) final {
  //}

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

