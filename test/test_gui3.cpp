
#define KODE_GUI_XCB

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class myWindow : public KODE_Window {

private:


public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
    setFillBackground();

    KODE_SAHeaderWidget* header1 = KODE_New KODE_SAHeaderWidget( KODE_FRect(10,10,128,64), "test_gui3", "v0.0.0", this );
    appendWidget(header1);

    KODE_ButtonWidget* button1 = KODE_New KODE_ButtonWidget( KODE_FRect(10,64,128,16) );
    button1->setIsToggle(true);
    appendWidget(button1);

    KODE_ButtonWidget* button2 = KODE_New KODE_ButtonWidget( KODE_FRect(10,84,128,16) );
    button2->setIsToggle(false);
    appendWidget(button2);

    KODE_SliderWidget* slider1 = KODE_New KODE_SliderWidget( KODE_FRect(10,104,128,16) );
    appendWidget(slider1);

  }

  virtual ~myWindow() {
  }

  //void on_widget_paint(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) final {
  //}

};

//----------------------------------------------------------------------

int main() {
  myWindow* window = KODE_New myWindow(400,300,"KODE2");
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
  return 0;
}

