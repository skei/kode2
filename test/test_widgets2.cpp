
#define KODE_GUI_XCB

//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class myWindow
: public KODE_Window {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {
    setFillBackground();

    KODE_PanelWidget* panel1 = KODE_New KODE_PanelWidget(KODE_FRect(  10, 10,  30, 30 ));
    KODE_PanelWidget* panel2 = KODE_New KODE_PanelWidget(KODE_FRect(  50, 10,  30, 30 ));
    KODE_PanelWidget* panel3 = KODE_New KODE_PanelWidget(KODE_FRect(  10, 50,  30, 30 ));
    KODE_PanelWidget* panel4 = KODE_New KODE_PanelWidget(KODE_FRect(  50, 50,  30, 30 ));

    KODE_ImageWidget* image1 = KODE_New KODE_ImageWidget(KODE_FRect(  90, 10, 100,100 ));

    panel1->setCursor(KODE_CURSOR_FINGER);
    panel2->setCursor(KODE_CURSOR_MOVE);
    panel3->setCursor(KODE_CURSOR_PENCIL);
    panel4->setCursor(KODE_CURSOR_WAIT);

    appendWidget(panel1);
    appendWidget(panel2);
    appendWidget(panel3);
    appendWidget(panel4);

    appendWidget(image1);


  }

  virtual ~myWindow() {
  }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

int main(void) {
  myWindow* window = new myWindow(800,600,"KODE_Window");
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;

}
