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

  sa_botage_page_fx(KODE_Editor* AEditor, KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {

    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    layout.spacing = KODE_FPoint(5);
    setDrawBorder(true);
    setFillBackground(false);

    //KODE_KeyboardWidget* keyboard = new KODE_KeyboardWidget(KODE_FRect(50,200));
    //keyboard->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT_TOP;
    //keyboard->setVertical(true);
    //appendWidget(keyboard);

    KODE_GridWidget* grid = new KODE_GridWidget(KODE_FRect(100,193),16,16);
    grid->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    grid->setNumCells(16,16);
    grid->setDrawLines(true,true);
    //grid->setSelectMultipleCells(false);
    appendWidget(grid);

  }

  //----------

  virtual ~sa_botage_page_fx() {
  }

};

//----------------------------------------------------------------------
#endif
