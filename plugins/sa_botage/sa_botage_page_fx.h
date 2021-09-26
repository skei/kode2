#ifndef sa_botage_page_fx_included
#define sa_botage_page_fx_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class sa_botage_page_fx
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  sa_botage_page_fx(KODE_Editor* AEditor, KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {

    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    layout.spacing = KODE_FPoint(5);
    setDrawBorder(false);
    setFillBackground(false);

//    __KODE_UNUSED KODE_ButtonWidget* button = KODE_NULL;
    __KODE_UNUSED KODE_SliderWidget* slider = KODE_NULL;

//    //KODE_KeyboardWidget* keyboard = new KODE_KeyboardWidget(KODE_FRect(50,200));
//    //keyboard->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT_TOP;
//    //keyboard->setVertical(true);
//    //appendWidget(keyboard);
//
//    KODE_GridWidget* grid = new KODE_GridWidget(KODE_FRect(100,193),16,16);
//    grid->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
//    grid->setNumCells(16,16);
//    grid->setDrawLines(true,true);
//    //grid->setSelectMultipleCells(false);
//    appendWidget(grid);



    // FILTER

    slider = new KODE_SliderWidget(KODE_FRect(0,0,200,20));
    slider->setText("Filter");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_PROB);

    slider = new KODE_SliderWidget(KODE_FRect(0,25,200,20));
    slider->setText("Type");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_TYPE);

    slider = new KODE_SliderWidget(KODE_FRect(0,50,200,20));
    slider->setText("Freq");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_FREQ);

    slider = new KODE_SliderWidget(KODE_FRect(0,75,95,20));
    slider->setText("Res");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_RES);

    slider = new KODE_SliderWidget(KODE_FRect(105,75,95,20));
    slider->setText("Clip");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_CLIP);

    slider = new KODE_SliderWidget(KODE_FRect(0,100,95,20));
    slider->setText("Pre");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_PRE);


    slider = new KODE_SliderWidget(KODE_FRect(105,100,95,20));
    slider->setText("Post");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_POST);

    slider = new KODE_SliderWidget(KODE_FRect(0,125,95,20));
    slider->setText("Min");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_FREQ_MIN);

    slider = new KODE_SliderWidget(KODE_FRect(105,125,95,20));
    slider->setText("Max");
    slider->setDrawLabel(true);
    appendWidget(slider);
    AEditor->connectParameter(slider,PAR_FX_FILTER_FREQ_MAX);

    // RINGMOD

//    slider = new KODE_SliderWidget(KODE_FRect(0,85,200,20));
//    slider->setText("Ringmod");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(0,110,95,20));
//    slider->setText("Freq");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(105,110,95,20));
//    slider->setText("Amt");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);



    // DELAY

//    slider = new KODE_SliderWidget(KODE_FRect(210,0,200,20));
//    slider->setText("Delay");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(210,25,95,20));
//    slider->setText("Time");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(315,25,95,20));
//    slider->setText("FB");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    // REVERB
//
//    slider = new KODE_SliderWidget(KODE_FRect(210,60,200,20));
//    slider->setText("Reverb");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(210,85,95,20));
//    slider->setText("Time");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(315,85,95,20));
//    slider->setText("Damp");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);



    // LO-FO

//    slider = new KODE_SliderWidget(KODE_FRect(420,0,200,20));
//    slider->setText("LoFi");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(420,25,95,20));
//    slider->setText("Rate");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);
//
//    slider = new KODE_SliderWidget(KODE_FRect(525,25,95,20));
//    slider->setText("Bits");
//    slider->setDrawLabel(true);
//    appendWidget(slider);
//    //AEditor->connectParameter(slider,PAR_FX_DELAY_PROB);


  }

  //----------

  virtual ~sa_botage_page_fx() {
  }

};

//----------------------------------------------------------------------
#endif
