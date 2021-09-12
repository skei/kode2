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

  sa_botage_page_rep(KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {
    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    setDrawBorder(false);
    setFillBackground(false);

      KODE_SliderWidget* slider;

      // range

      slider = new KODE_SliderWidget(KODE_FRect(0,0,190,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Range");
      slider->setDrawLabel(true);
      slider->setLabel("%");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(0,25,90,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Min");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(100,25,90,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Max");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);

      // slice

      slider = new KODE_SliderWidget(KODE_FRect(0,50,190,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Slice");
      slider->setDrawLabel(true);
      slider->setLabel("%");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(0,75,90,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Min");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(100,75,90,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Max");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);

      // loop

      slider = new KODE_SliderWidget(KODE_FRect(200,0,150,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Inc Loop");
      slider->setDrawLabel(true);
      slider->setLabel("%");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(200,25,150,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Dec Loop");
      slider->setDrawLabel(true);
      slider->setLabel("%");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(200,50,150,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Inc Speed");
      slider->setDrawLabel(true);
      slider->setLabel("%");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(200,75,150,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("Dec Speed");
      slider->setDrawLabel(true);
      slider->setLabel("%");
      appendWidget(slider);

      // amt

      slider = new KODE_SliderWidget(KODE_FRect(360,0,80,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("amt");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(360,25,80,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("amt");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(360,50,80,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("amt");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);

      slider = new KODE_SliderWidget(KODE_FRect(360,75,80,20));
      slider->layout.alignment = KODE_WIDGET_ALIGN_CLIENT;
      slider->setText("amt");
      //slider->setDrawLabel(true);
      //slider->setLabel("");
      appendWidget(slider);


  }

  //----------

  virtual ~sa_botage_page_rep() {
  }

};

//----------------------------------------------------------------------
#endif
