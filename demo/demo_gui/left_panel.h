
class KODE_left_panel
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

const char* buttonrow_txt[5] = {
  "A",
  "B",
  "C",
  "D",
  "eee"
};


//------------------------------
public:
//------------------------------

  KODE_MenuWidget* menu1 = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_left_panel( KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {

    layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    layout.innerBorder = 10;
    layout.spacing = 10;

    //setFillBackground(false);
    //setDrawBorder(false);

    //----------

    menu1 = new KODE_MenuWidget( KODE_FRect(250,150) );
    menu1->setMenuSize(2,5);
    menu1->setItemWidth(100);
    menu1->setItemHeight(20);
    menu1->appendMenuItem("Item1");
    menu1->appendMenuItem("Item2");
    menu1->appendMenuItem("Item3");
    menu1->appendMenuItem("Item4");
    menu1->appendMenuItem("Item5");
    menu1->appendMenuItem("Item6");
    menu1->appendMenuItem("Item7");
    menu1->appendMenuItem("Item8");

    //----------

    // sliders

    KODE_SliderWidget* slider1 = new KODE_SliderWidget(KODE_FRect(20));
    slider1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    slider1->setValue(0.2);
    slider1->setText("Slider 1");
    slider1->setDrawLabel();
    //slider1->setLabel("db");
    slider1->setLabelColor(KODE_COLOR_LIGHT_RED);
    slider1->setAutoHideCursor(false);
    slider1->setAutoGrabCursor(false);
    appendWidget(slider1);

    KODE_SliderWidget* slider2 = new KODE_SliderWidget(KODE_FRect(20));
    slider2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    slider2->setValue(0.7);
    slider2->setText("Slider Two");
    slider2->setDrawLabel();
    //slider2->setLabel("db");
    slider2->setBackgroundColor(KODE_COLOR_DARK_RED);
    slider2->setValueBarColor(KODE_COLOR_RED);
    slider2->setTextColor(KODE_COLOR_WHITE);
    slider2->setValueTextColor(KODE_COLOR_BRIGHT_YELLOW);
    slider2->setAutoHideCursor(false);
    slider2->setAutoGrabCursor(true);
    appendWidget(slider2);

    KODE_SliderWidget* slider3 = new KODE_SliderWidget(KODE_FRect(20));
    slider3->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    slider3->setValue(0.4);
    slider3->setText("Slider 3");
    slider3->setDrawLabel();
    //slider3->setLabel("db");
    slider3->setAutoHideCursor(true);
    slider3->setAutoGrabCursor(true);
    appendWidget(slider3);

    // scrollbar

    KODE_ScrollBarWidget* scrollbar1 = new KODE_ScrollBarWidget(KODE_FRect(20));
    scrollbar1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(scrollbar1);

    // range sloder

    KODE_RangeSliderWidget* rangeslider1 = new KODE_RangeSliderWidget(KODE_FRect(20));
    rangeslider1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    rangeslider1->setValue(0.25);
    rangeslider1->setValue2(0.75);
    appendWidget(rangeslider1);

    // buttons

    KODE_ButtonWidget* button1 = new KODE_ButtonWidget(KODE_FRect(20));
    button1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(button1);

    KODE_ButtonWidget* button2 = new KODE_ButtonWidget(KODE_FRect(20));
    button2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    button2->setIsToggle();
    appendWidget(button2);

    // menu item

    KODE_MenuItemWidget* menuitem1 = new KODE_MenuItemWidget(KODE_FRect(20));
    menuitem1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    menuitem1->setText("MenuItem");
    appendWidget(menuitem1);

    // selector

    KODE_SelectorWidget* selector1 = new KODE_SelectorWidget(KODE_FRect(20));
    selector1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    selector1->setMenu(menu1);
    appendWidget(selector1);

    // group box

    KODE_GroupBoxWidget* groupbox = new KODE_GroupBoxWidget(KODE_FRect(150));
    groupbox->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(groupbox);

    // buttonrows

    KODE_ButtonRowWidget* buttonrow1 = new KODE_ButtonRowWidget(KODE_FRect(20),5,buttonrow_txt,KODE_BUTTON_ROW_SINGLE);
    buttonrow1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(buttonrow1);

    KODE_ButtonRowWidget* buttonrow2 = new KODE_ButtonRowWidget(KODE_FRect(20),5,buttonrow_txt,KODE_BUTTON_ROW_MULTI);
    buttonrow2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(buttonrow2);

    // grid

    KODE_GridWidget* grid1 = new KODE_GridWidget(KODE_FRect(100));
    grid1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(grid1);

  }

  //----------

  virtual ~KODE_left_panel() {
  }

};
