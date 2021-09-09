#ifndef sa_botage_page_prob_included
#define sa_botage_page_prob_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"

class sa_botage_page_prob
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  sa_botage_page_prob(KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {
    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    setDrawBorder(true);
    setFillBackground(false);
  }

  //----------

  virtual ~sa_botage_page_prob() {
  }

};

//----------------------------------------------------------------------
#endif
