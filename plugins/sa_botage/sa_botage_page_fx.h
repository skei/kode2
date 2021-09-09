#ifndef sa_botage_page_fx_included
#define sa_botage_page_fx_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"

class sa_botage_page_fx
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  sa_botage_page_fx(KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {
    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    setDrawBorder(true);
    setFillBackground(false);
  }

  //----------

  virtual ~sa_botage_page_fx() {
  }

};

//----------------------------------------------------------------------
#endif
