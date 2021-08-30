
class KODE_main_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_main_panel( KODE_FRect ARect, KODE_Drawable* ATarget)
  : KODE_PanelWidget(ARect) {

    //---------- right top ----------

    //layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    layout.innerBorder = 10;
    layout.spacing = 10;

    // image

    KODE_ImageWidget* image = KODE_New KODE_ImageWidget(KODE_FRect(256,256));
    image->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    //image->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    image->setDrawBorder();
    image->setCursor(KODE_CURSOR_QUESTION);
    KODE_Bitmap* bitmap = KODE_New KODE_Bitmap(256,256);
    bitmap->genFractalCloud();
    //image->setBitmap(bitmap);
    image->setImage(ATarget,bitmap);
    //image->setImage(this,"/home/skei/Pictures/skei_256.png");
    KODE_Delete bitmap;
    appendWidget(image);

    // vertical  scrollbar

    KODE_ScrollBarWidget* scrollbar2 = KODE_New KODE_ScrollBarWidget(KODE_FRect(10));
    scrollbar2->layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
    scrollbar2->setDirection(KODE_VERTICAL);
    appendWidget(scrollbar2);

    // keyboard

    KODE_KeyboardWidget* keyboard1 = KODE_New KODE_KeyboardWidget(KODE_FRect(60));
    keyboard1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(keyboard1);

    // knob

    KODE_KnobWidget* knob1 = KODE_New KODE_KnobWidget(KODE_FRect(50,50));
    knob1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    knob1->setValue(0.3);
    appendWidget(knob1);

  }

  //----------

  virtual ~KODE_main_panel() {
  }

};
