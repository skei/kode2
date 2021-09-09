#ifndef kode_text_box_widget_included
#define kode_text_box_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class KODE_TextBoxWidget
: public KODE_ScrollBoxWidget {

//------------------------------
protected:
//------------------------------

  uint32_t  MTextHeight = 12;

//------------------------------
public:
//------------------------------

  KODE_TextBoxWidget(KODE_FRect ARect)
  : KODE_ScrollBoxWidget(ARect) {
    setName("KODE_TextBoxWidget");
    setHint("textbox");
    //MContent->layout.innerBorder = 0;
  }

  //----------

  //virtual ~KODE_TextBoxWidget() {
  //}

//------------------------------
public:
//------------------------------

  virtual void appendText(const char* AText) {
    KODE_TextWidget* textwidget = new KODE_TextWidget( KODE_FRect(0,0,250,MTextHeight),AText);
    textwidget->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP_LEFT;
    textwidget->flags.autoSize = true;
    textwidget->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
    MContent->appendWidget(textwidget);
  }

};

//----------------------------------------------------------------------
#endif


