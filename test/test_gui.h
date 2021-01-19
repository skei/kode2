
//#define KODE_CAIRO
#define KODE_GUI_XCB
#define KODE_PAINTER_CAIRO

//#define KODE_NO_WINDOW_BUFFERING

#include "base/kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

/*
class myWidget
: public KODE_Widget {
protected:
public:
  myWidget(KODE_FRect ARect) : KODE_Widget(ARect) {}
  virtual ~myWidget() {};
public: // widget
  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {
    //KODE_Skin* skin = getSkin();
    //skin->drawPanel(APainter,10,10,128,32);
    APainter->setFontSize(  20 );
    APainter->fillRect(     KODE_FRect( 10, 10,128, 32),            0xff777777 );
    APainter->drawText(     KODE_FRect( 10, 10,128, 32), "test",    0xffffffff, KODE_TEXT_ALIGN_CENTER );
    APainter->setLineWidth( 2 );
    APainter->drawEllipse(  KODE_FRect(100,100, 40, 40),            0xffaaaaaa );
    APainter->fillPie(      KODE_FRect(103,103, 34, 34), -0.2, 0.6, 0xff777777 );
    APainter->fillPie(      KODE_FRect(103,103, 34, 34), -0.4, 0.2, 0xffaaaaaa );
  }
};
*/

//----------------------------------------------------------------------

class myWindow
: public KODE_Window {

public:

  myWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_Window(AWidth,AHeight,ATitle,AParent) {

    setFillBackground();
    setBackgroundColor(0xff666666);

    //myWidget* widget = KODE_New myWidget(KODE_FRect(10,10,128,32));
    //appendChildWidget(widget);

    KODE_Widget* widget = KODE_New KODE_ValueWidget(KODE_FRect(10,10,128,32));
    appendChildWidget(widget);


  }

};

//----------------------------------------------------------------------

int main() {
  myWindow* window = KODE_New myWindow(640,480,"hello world!");
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
  return 0;
}
