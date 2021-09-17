#ifndef sa_botage_page_rep_included
#define sa_botage_page_rep_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"

class sa_botage_page_rep
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

//------------------------------
public:
//------------------------------

  sa_botage_page_rep(KODE_Editor* AEditor, KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {
    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    setDrawBorder(false);
    setFillBackground(false);

    __KODE_UNUSED KODE_ButtonWidget* button = KODE_NULL;
    __KODE_UNUSED KODE_SliderWidget* slider = KODE_NULL;

    // PAR_REPEAT_PROB

    slider = new KODE_SliderWidget(KODE_FRect(0,0,200,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,2);

    // PAR_RANGE_MIN_SLICES

    slider = new KODE_SliderWidget(KODE_FRect(0,25,80,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,3);

    // PAR_RANGE_MAX_SLICES

    slider = new KODE_SliderWidget(KODE_FRect(90,25,80,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,4);

    //

    button = new KODE_ButtonWidget(KODE_FRect(180,25,20,20));
    button->setIsToggle(true);
    button->setDrawTriangle(false);
    button->setText("2^","2^");
    button->setDrawBorder(true);
    button->setTextColor(KODE_COLOR_BLACK,KODE_COLOR_BLACK);
    button->setBackgroundColor(KODE_COLOR_LIGHT_GRAY,KODE_COLOR_GRAY);
    appendWidget(button);
    //AEditor->connectParameter(slider,4);

    // PAR_RANGE_MIN_SUBDIV

    slider = new KODE_SliderWidget(KODE_FRect(0,50,80,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,5);

    // PAR_RANGE_MAX_SUBDIV

    slider = new KODE_SliderWidget(KODE_FRect(90,50,80,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,6);

    //

    button = new KODE_ButtonWidget(KODE_FRect(180,50,20,20));
    button->setIsToggle(true);
    button->setDrawTriangle(false);
    button->setText("2^","2^");
    button->setDrawBorder(true);
    button->setTextColor(KODE_COLOR_BLACK,KODE_COLOR_BLACK);
    button->setBackgroundColor(KODE_COLOR_LIGHT_GRAY,KODE_COLOR_GRAY);
    appendWidget(button);
    //AEditor->connectParameter(slider,4);

    //-----

    // PAR_LOOP_SIZE_PROB

    slider = new KODE_SliderWidget(KODE_FRect(210,0,150,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,7);

    // PAR_LOOP_SIZE_AMT

    slider = new KODE_SliderWidget(KODE_FRect(370,0,80,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,8);

    // PAR_LOOP_SPEED_PROB

    slider = new KODE_SliderWidget(KODE_FRect(210,25,150,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,9);

    // PAR_LOOP_SPEED_AMT

    slider = new KODE_SliderWidget(KODE_FRect(370,25,80,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,10);

    //-----

    // PAR_XFADE_MODE

    slider = new KODE_SliderWidget(KODE_FRect(470,0,100,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,11);

    // PAR_XFADE_AM

    slider = new KODE_SliderWidget(KODE_FRect(470,25,100,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,12);

  }

  //----------

  virtual ~sa_botage_page_rep() {
  }

};

//----------------------------------------------------------------------
#endif
