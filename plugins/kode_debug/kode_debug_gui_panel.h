#ifndef kode_debug_gui_panel_included
#define kode_debug_gui_panel_included
//----------------------------------------------------------------------

#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class kode_debug_gui_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_ValueWidget*   wdgPid          = KODE_NULL;
  KODE_ValueWidget*   wdgTid          = KODE_NULL;
  KODE_ValueWidget*   wdgGuiDelta     = KODE_NULL;

  KODE_ValueWidget*   wdgXcbPid       = KODE_NULL;
  KODE_ValueWidget*   wdgXcbTid       = KODE_NULL;

  KODE_ValueWidget*   wdgWidth        = KODE_NULL;
  KODE_ValueWidget*   wdgHeight       = KODE_NULL;
  KODE_ValueWidget*   wdgDepth        = KODE_NULL;

  KODE_TextWidget*    wdgRedText      = KODE_NULL;
  KODE_TextWidget*    wdgGreenText    = KODE_NULL;
  KODE_TextWidget*    wdgBlueText     = KODE_NULL;

  //KODE_TextWidget*    wdgMouseHover   = KODE_NULL;
  //KODE_TextWidget*    wdgMouseModal   = KODE_NULL;
  //KODE_TextWidget*    wdgMouseClicked = KODE_NULL;
  //KODE_TextWidget*    wdgMouseLocked  = KODE_NULL;
  //KODE_TextWidget*    wdgKeyInput     = KODE_NULL;

//------------------------------
public:
//------------------------------

  kode_debug_gui_panel(KODE_FRect ARect, KODE_Editor* AEditor)
  : KODE_PanelWidget(ARect) {
    setName("kode_debug_gui_panel");
    setHint("gui");

    wdgPid            = new KODE_ValueWidget( KODE_FRect( 10, 10, 180,16), "process id" );
    wdgTid            = new KODE_ValueWidget( KODE_FRect( 10, 30, 180,16), "thread id" );
    wdgGuiDelta       = new KODE_ValueWidget( KODE_FRect( 10, 50, 180,16), "time" );

    wdgXcbPid         = new KODE_ValueWidget( KODE_FRect( 10, 80, 180,16), "xcb p.id" );
    wdgXcbTid         = new KODE_ValueWidget( KODE_FRect( 10,100, 180,16), "xcb t.id" );

    wdgWidth          = new KODE_ValueWidget( KODE_FRect( 10,130, 180,16), "width" );
    wdgHeight         = new KODE_ValueWidget( KODE_FRect( 10,150, 180,16), "height" );
    wdgDepth          = new KODE_ValueWidget( KODE_FRect( 10,170, 180,16), "depth" );

    wdgRedText        = new KODE_TextWidget(  KODE_FRect( 10,200, 60,16), "RED" );
    wdgGreenText      = new KODE_TextWidget(  KODE_FRect( 70,200, 60,16), "GREEN" );
    wdgBlueText       = new KODE_TextWidget(  KODE_FRect(130,200, 60,16), "BLUE" );

    //wdgMouseHover     = new KODE_TextWidget(  KODE_FRect(130, 90, 90,16) );
    //wdgMouseModal     = new KODE_TextWidget(  KODE_FRect(130,105, 90,16) );
    //wdgMouseClicked   = new KODE_TextWidget(  KODE_FRect(130,120, 90,16) );
    //wdgMouseLocked    = new KODE_TextWidget(  KODE_FRect(130,135, 90,16) );
    //wdgKeyInput       = new KODE_TextWidget(  KODE_FRect(130,150, 90,16) );

    appendWidget(wdgPid);
    appendWidget(wdgTid);
    appendWidget(wdgGuiDelta);

    appendWidget(wdgXcbPid);
    appendWidget(wdgXcbTid);

    appendWidget(wdgWidth);
    appendWidget(wdgHeight);
    appendWidget(wdgDepth);

    appendWidget(wdgRedText);
    appendWidget(wdgGreenText);
    appendWidget(wdgBlueText);

    //appendWidget(wdgMouseHover);
    //appendWidget(wdgMouseModal);
    //appendWidget(wdgMouseClicked);
    //appendWidget(wdgMouseLocked);
    //appendWidget(wdgKeyInput);

    wdgPid->setDigits(0);
    wdgTid->setDigits(0);
    wdgGuiDelta->setDigits(2);
    wdgGuiDelta->setDrawLabel(true);
    wdgGuiDelta->setLabel("ms");
    wdgGuiDelta->setLabelColor(0.9);

    wdgXcbPid->setDigits(0);
    wdgXcbTid->setDigits(0);

    wdgWidth->setValue( AEditor->getWidth() );
    wdgWidth->setDigits(0);
    wdgHeight->setValue( AEditor->getHeight() );
    wdgHeight->setDigits(0);
    wdgDepth->setValue( AEditor->getDepth() );
    wdgDepth->setDigits(0);

    wdgRedText->setTextColor(KODE_COLOR_BRIGHT_RED);
    wdgRedText->setFillBackground(true);
    wdgRedText->setBackgroundColor(KODE_COLOR_BLACK);
    wdgGreenText->setTextColor(KODE_COLOR_BRIGHT_GREEN);
    wdgGreenText->setFillBackground(true);
    wdgGreenText->setBackgroundColor(KODE_COLOR_BLACK);
    wdgBlueText->setTextColor(KODE_COLOR_BRIGHT_BLUE1);
    wdgBlueText->setFillBackground(true);
    wdgBlueText->setBackgroundColor(KODE_COLOR_BLACK);

  }

};

//----------------------------------------------------------------------
#endif
