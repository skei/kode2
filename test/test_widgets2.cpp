
#define KODE_GUI_XCB

//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"
#include "gui/kode_window.h"
//#include "plugin/kode_plugin.h"
//#include "plugin/vst3/kode_vst3_instance.h"

//----------------------------------------------------------------------

class myWindow
: public KODE_Window {

//------------------------------
private:
//------------------------------

  KODE_SAHeaderWidget*  MHeader   = KODE_NULL;
  KODE_SliderWidget*    MSlider1  = KODE_NULL;


//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
    setFillBackground();

    MHeader = KODE_New KODE_SAHeaderWidget( KODE_FRect(10,10,300,64), "test_widgets2", "v0.0.1 [exe]", this);
    appendWidget(MHeader);

    MSlider1 = KODE_New KODE_SliderWidget( KODE_FRect(10,64,300,20) );
    appendWidget(MSlider1);

  }

  virtual ~myWindow() {
  }

};

//----------------------------------------------------------------------

KODE_Window* create_window() {
  myWindow* window = new myWindow(800,600,"KODE_Window");
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
