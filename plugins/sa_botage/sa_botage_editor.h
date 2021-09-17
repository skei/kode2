#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_editor.h"
#include "gui/kode_widgets.h"

#include "sa_botage_header.h"
#include "sa_botage_page_rep.h"
#include "sa_botage_page_arr.h"
#include "sa_botage_page_fx.h"


class sa_botage_editor
: public KODE_Editor {

//------------------------------
public: //private:
//------------------------------

  KODE_ImageWidget*     WHeader   = KODE_NULL;
  //KODE_TextWidget*      WFooter   = KODE_NULL;
  KODE_SliderWidget*    WNumBeats = KODE_NULL;
  KODE_SliderWidget*    WBeatDiv  = KODE_NULL;
  KODE_WaveformWidget*  WWaveform = KODE_NULL;
  KODE_TabsWidget*      WTabs     = KODE_NULL;
  sa_botage_page_rep*   WPageRep  = KODE_NULL;
  sa_botage_page_arr*   WPageArr  = KODE_NULL;
  sa_botage_page_fx*    WPageFX   = KODE_NULL;

  //

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

    WHeader = new KODE_ImageWidget(KODE_FRect(160,60));
    WHeader->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    WHeader->setDrawBorder(true);
    WHeader->setFillBackground(false);
    WHeader->setBackgroundColor(0.5);
    WHeader->setImage(this,(uint8_t*)sa_botage_header,sa_botage_header_size,KODE_Color(0.5));
    appendWidget(WHeader);

//    // footer
//
//    WFooter = new KODE_TextWidget(KODE_FRect(0,0,20,20),"footer...");
//    WFooter->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
//    WFooter->setTextColor(KODE_Color(0.9));
//    WFooter->setDrawBorder(true);
//    WFooter->setBorderEdges(KODE_EDGE_TOP);
//    appendWidget(WFooter);

    // buffer

    WNumBeats = new KODE_SliderWidget(KODE_FRect(150,20));
    WNumBeats->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    WNumBeats->setText("Num Beats");
    appendWidget(WNumBeats);
    connectParameter(WNumBeats,0);

    WBeatDiv = new KODE_SliderWidget(KODE_FRect(150,20));
    WBeatDiv->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    WBeatDiv->setText("Beat Div");
    appendWidget(WBeatDiv);
    connectParameter(WBeatDiv,1);

    // waveform

    WWaveform = new KODE_WaveformWidget(KODE_FRect(80,80));
    WWaveform->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    WWaveform->setBackgroundColor(KODE_Color(0.4));
    WWaveform->setNumMarkers(2);
    WWaveform->setMarkerColor(0,KODE_COLOR_BRIGHT_RED);
    WWaveform->setMarkerColor(1,KODE_COLOR_BRIGHT_GREEN);
    WWaveform->setNumAreas(2);
    WWaveform->setAreaColor(0,KODE_COLOR_GREEN);
    WWaveform->setAreaColor(1,KODE_COLOR_LIGHT_GREEN);
    appendWidget(WWaveform);

    // tabs

    WTabs = new KODE_TabsWidget( KODE_FRect(), 3 );
    WTabs->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    //WTabs->layout.innerBorder = 10;
    WTabs->layout.spacing = 10;
    appendWidget(WTabs);

    // pages

    WPageRep = new sa_botage_page_rep(this);
    WPageArr = new sa_botage_page_arr(this);
    WPageFX  = new sa_botage_page_fx(this);

    WTabs->appendPage("Repeat",WPageRep);
    WTabs->appendPage("Rearrange",WPageArr);
    WTabs->appendPage("Effects",WPageFX);
    WTabs->selectPage(0);

  }

  //----------

  //virtual ~sa_botage_editor() {
  //}

//------------------------------
public:
//------------------------------

//  void do_widget_update(KODE_Widget* AWidget) override {
//    if (AWidget == WNumBeats) {
//      float value = WNumBeats->getValue();
//      KODE_Print("numbeats %.1f\n",value);
//    }
//    if (AWidget == WBeatDiv) {
//      //KODE_Print("beatdiv\n");
//    }
//  }


};

//----------------------------------------------------------------------
#endif
