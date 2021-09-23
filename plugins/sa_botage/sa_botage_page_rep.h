#ifndef sa_botage_page_rep_included
#define sa_botage_page_rep_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

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
    AEditor->connectParameter(slider,PAR_REPEAT_PROB);

    // PAR_RANGE_MIN_SLICES

    slider = new KODE_SliderWidget(KODE_FRect(0,25,95,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_RANGE_MIN_SLICES);

    // PAR_RANGE_MAX_SLICES

    slider = new KODE_SliderWidget(KODE_FRect(105,25,95,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_RANGE_MAX_SLICES);

    //

//    button = new KODE_ButtonWidget(KODE_FRect(180,25,20,20));
//    button->setIsToggle(true);
//    button->setDrawTriangle(false);
//    button->setText("2^","2^");
//    button->setDrawBorder(true);
//    button->setTextColor(KODE_COLOR_BLACK,KODE_COLOR_BLACK);
//    button->setBackgroundColor(KODE_COLOR_LIGHT_GRAY,KODE_COLOR_GRAY);
//    appendWidget(button);
//    //AEditor->connectParameter(slider,4);

    // PAR_RANGE_MIN_SUBDIV

    slider = new KODE_SliderWidget(KODE_FRect(0,50,95,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_RANGE_MIN_SUBDIV);

    // PAR_RANGE_MAX_SUBDIV

    slider = new KODE_SliderWidget(KODE_FRect(105,50,95,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_RANGE_MAX_SUBDIV);

    //

//    button = new KODE_ButtonWidget(KODE_FRect(180,50,20,20));
//    button->setIsToggle(true);
//    button->setDrawTriangle(false);
//    button->setText("2^","2^");
//    button->setDrawBorder(true);
//    button->setTextColor(KODE_COLOR_BLACK,KODE_COLOR_BLACK);
//    button->setBackgroundColor(KODE_COLOR_LIGHT_GRAY,KODE_COLOR_GRAY);
//    appendWidget(button);
//    //AEditor->connectParameter(slider,4);

    //-----

    // PAR_LOOP_SIZE_PROB

    //slider = new KODE_SliderWidget(KODE_FRect(210,0,150,20));
    slider = new KODE_SliderWidget(KODE_FRect(210,0,200,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_LOOP_SIZE_PROB);

    // PAR_LOOP_SIZE_MIN/MAX

    //slider = new KODE_SliderWidget(KODE_FRect(370,0,80,20));
    slider = new KODE_SliderWidget(KODE_FRect(210,25,95,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_LOOP_SIZE_MIN);

    slider = new KODE_SliderWidget(KODE_FRect(315,25,95,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_LOOP_SIZE_MAX);

    // PAR_LOOP_SPEED_PROB

    //slider = new KODE_SliderWidget(KODE_FRect(210,50,150,20));
    slider = new KODE_SliderWidget(KODE_FRect(210,60,200,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_LOOP_SPEED_PROB);

    // PAR_LOOP_SPEED_MIN/MAX

    //slider = new KODE_SliderWidget(KODE_FRect(370,25,80,20));
    slider = new KODE_SliderWidget(KODE_FRect(210,85,95,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_LOOP_SPEED_MIN);

    //slider = new KODE_SliderWidget(KODE_FRect(370,25,80,20));
    slider = new KODE_SliderWidget(KODE_FRect(315,85,95,20));
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_LOOP_SPEED_MAX);

    //-----

    // PAR_XFADE_MODE

    slider = new KODE_SliderWidget(KODE_FRect(420,0,200,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_XFADE_MODE);

    // PAR_XFADE_AM

    slider = new KODE_SliderWidget(KODE_FRect(420,25,200,20));
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_XFADE_AMT);

  }

  //----------

  virtual ~sa_botage_page_rep() {
  }

};

//----------------------------------------------------------------------
#endif
