#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_editor.h"
#include "gui/kode_widgets.h"

#include "sa_botage_header.h"
#include "sa_botage_page_prob.h"
#include "sa_botage_page_arr.h"
#include "sa_botage_page_fx.h"


class sa_botage_editor
: public KODE_Editor {

//------------------------------
public:
//------------------------------

  sa_botage_editor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setFillBackground(true);
    setBackgroundColor(0.5f);

    layout.innerBorder = KODE_FRect(10,10,10,10);
    layout.spacing = 10;

    // header

    KODE_ImageWidget* header_image = new KODE_ImageWidget(KODE_FRect(160,60));
    header_image->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    header_image->setDrawBorder(true);
    header_image->setFillBackground(false);
    header_image->setBackgroundColor(0.5);
    header_image->setImage(this,(uint8_t*)sa_botage_header,sa_botage_header_size,KODE_Color(0.5));
    appendWidget(header_image);

    // -----

    //KODE_SymbolWidget* hline1 = new KODE_SymbolWidget(KODE_FRect(10),KODE_SYMBOL_HLINE);
    //hline1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    //hline1->setColor(KODE_COLOR_LIGHT_GRAY);
    //appendWidget(hline1);

    // footer

    KODE_TextWidget* footer = new KODE_TextWidget(KODE_FRect(0,0,20,20),"footer...");
    footer->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    footer->setTextColor(KODE_Color(0.9));
    //footer->setDrawBorder(true);
    appendWidget(footer);

    // buffer controls

    KODE_SliderWidget* slider_num_beats = new KODE_SliderWidget(KODE_FRect(150,20));
    slider_num_beats->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    slider_num_beats->setText("Num Beats");
    appendWidget(slider_num_beats);

    KODE_SliderWidget* slider_beat_div = new KODE_SliderWidget(KODE_FRect(150,20));
    slider_beat_div->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    slider_beat_div->setText("Beat Div");
    appendWidget(slider_beat_div);

    // waveform

    KODE_WaveformWidget* waveform = new KODE_WaveformWidget(KODE_FRect(40,40));
    waveform->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    waveform->setBackColor(KODE_Color(0.4));
    //waveform->setNumAreas(0);
    //waveform->setNumGrid(0);
    //waveform->setNumMarkers(0);
    appendWidget(waveform);

    // tabs

    KODE_TabsWidget* tabs = new KODE_TabsWidget (KODE_FRect(100,100),3);
    tabs->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    //tabs->layout.innerBorder = 10;
    tabs->layout.spacing = 10;
    appendWidget(tabs);

      // pages

      sa_botage_page_prob* page_prob = new sa_botage_page_prob();
      sa_botage_page_arr* page_arr = new sa_botage_page_arr();
      sa_botage_page_fx* page_fx = new sa_botage_page_fx();
      tabs->appendPage("Prob",page_prob);
      tabs->appendPage("Arr",page_arr);
      tabs->appendPage("FX",page_fx);
      tabs->selectPage(0);

  }

  //----------

  //virtual ~sa_botage_editor() {
  //}

};

//----------------------------------------------------------------------
#endif
