

#include "../data/img/knob4_60x60_131.h"

class KODE_main_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_main_panel( KODE_FRect ARect, KODE_Drawable* ATarget)
  : KODE_PanelWidget(ARect) {

    //layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    layout.innerBorder = 10;
    layout.spacing = 10;

    KODE_Widget* right = new KODE_Widget( KODE_FRect(150) );
    //right->flags.sizepercent = true;
    right->layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
    //right->layout.innerBorder = 10;
    right->layout.spacing = 10;
    appendWidget(right);

    KODE_Widget* left = new KODE_Widget( KODE_FRect() );
    //left->flags.sizePercent = true;
    left->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    //left->layout.innerBorder = 10;
    left->layout.spacing = 10;
    appendWidget(left);

    // --- right

    // scrollbar

    KODE_ScrollBarWidget* scrollbar2 = new KODE_ScrollBarWidget(KODE_FRect(10));
    scrollbar2->layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
    scrollbar2->layout.extraBorder.h = 30;
    scrollbar2->setDirection(KODE_VERTICAL);
    right->appendWidget(scrollbar2);

    // image

    KODE_ImageWidget* image = new KODE_ImageWidget(KODE_FRect(128,128));
    image->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    //image->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    image->setDrawBorder();
    //image->setCursor(KODE_CURSOR_QUESTION);
    KODE_Bitmap* bitmap = new KODE_Bitmap(128,128);
    bitmap->genFractalCloud();
    //image->setBitmap(bitmap);
    image->setImage(ATarget,bitmap);
    //image->setImage(this,"/home/skei/Pictures/skei_256.png");
    delete bitmap;
    right->appendWidget(image);

    // --- left

    // keyboard

    KODE_KeyboardWidget* keyboard1 = new KODE_KeyboardWidget(KODE_FRect(60));
    keyboard1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    left->appendWidget(keyboard1);

    // sliderbank

    KODE_SliderBankWidget* sliderbank1 = new KODE_SliderBankWidget(KODE_FRect(60),32);
    sliderbank1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    left->appendWidget(sliderbank1);

    // knob

    KODE_KnobWidget* knob1 = new KODE_KnobWidget(KODE_FRect(50,50));
    //knob1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    knob1->layout.alignment = KODE_WIDGET_STACK_VERT;
    knob1->setValue(0.3);
    knob1->setBipolar();
    left->appendWidget(knob1);

    // image strip

    KODE_ImageStripWidget* imagestrip1 = new KODE_ImageStripWidget(KODE_FRect(60,60));
    //imagestrip1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    imagestrip1->layout.alignment = KODE_WIDGET_STACK_VERT;
    imagestrip1->setImage(ATarget,knob4_60x60_131,knob4_60x60_131_size,KODE_Color(0.6f));
    imagestrip1->setupTiles(1,131);
    imagestrip1->setValue(0.6);
    left->appendWidget(imagestrip1);

    // curves

    KODE_CurveWidget* curve1 = new KODE_CurveWidget(KODE_FRect(50,50),false);
    //curve1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    curve1->layout.alignment = KODE_WIDGET_STACK_VERT;
    curve1->setValue(0.7);
    left->appendWidget(curve1);

    KODE_CurveWidget* curve2 = new KODE_CurveWidget(KODE_FRect(50,50),true);
    //curve2->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    curve2->layout.alignment = KODE_WIDGET_STACK_VERT;
    curve2->setValue(0.3);
    left->appendWidget(curve2);


  }

  //----------

  virtual ~KODE_main_panel() {
  }

};
