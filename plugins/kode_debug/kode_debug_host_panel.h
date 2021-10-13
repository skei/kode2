#ifndef kode_debug_host_panel_included
#define kode_debug_host_panel_included
//----------------------------------------------------------------------

#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class kode_debug_host_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_ValueWidget*   wdgPid          = KODE_NULL;
  KODE_ValueWidget*   wdgTid          = KODE_NULL;
  KODE_TextWidget*    wdgHostName = KODE_NULL;

//------------------------------
public:
//------------------------------

  kode_debug_host_panel(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("kode_debug_host_panel");
    setHint("host");

    wdgPid            = new KODE_ValueWidget( KODE_FRect( 10, 10, 180,16),  "process id" );
    wdgTid            = new KODE_ValueWidget( KODE_FRect( 10, 30, 180,16),  "thread id" );
    wdgHostName       = new KODE_TextWidget(  KODE_FRect( 10, 60, 60,16)   );

    appendWidget(wdgPid);
    appendWidget(wdgTid);
    appendWidget(wdgHostName);

    wdgPid->setDigits(0);
    wdgTid->setDigits(0);

  }

};

//----------------------------------------------------------------------
#endif
