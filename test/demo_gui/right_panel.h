
class KODE_right_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_right_panel( KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {

    layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
    layout.innerBorder = KODE_FRect(10,10,10,10);
    layout.spacing = 10;

    // tabs

    KODE_TabsWidget* tabs1 = new KODE_TabsWidget( KODE_FRect(180,150), 5 );
    tabs1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;

      KODE_PanelWidget* page1 = new KODE_PanelWidget(KODE_FRect(100,100));
      page1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
      page1->setFillBackground();
      page1->setDrawBorder();
      page1->setBackgroundColor(KODE_COLOR_DARK_RED);
      page1->setBorderColor(KODE_COLOR_BRIGHT_RED);
      tabs1->appendPage("page1",page1);

      KODE_PanelWidget* page2 = new KODE_PanelWidget(KODE_FRect(100,100));
      page2->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
      page2->setFillBackground();
      page2->setDrawBorder();
      page2->setBackgroundColor(KODE_COLOR_DARK_GREEN);
      page2->setBorderColor(KODE_COLOR_BRIGHT_GREEN);
      tabs1->appendPage("page2",page2);

      KODE_PanelWidget* page3 = new KODE_PanelWidget(KODE_FRect(100,100));
      page3->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
      page3->setFillBackground();
      page3->setDrawBorder();
      page3->setBackgroundColor(KODE_COLOR_DARK_BLUE);
      page3->setBorderColor(KODE_COLOR_BRIGHT_BLUE);
      tabs1->appendPage("page3",page3);

    tabs1->selectPage(0);
    appendWidget(tabs1);

    // scroll box

    KODE_ScrollBoxWidget* scrollbox1 = new KODE_ScrollBoxWidget( KODE_FRect(100,100) );
    scrollbox1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    scrollbox1->getContentWidget()->layout.innerBorder = KODE_FRect(10,10,10,10);
    appendWidget(scrollbox1);

    // knobs

    KODE_KnobWidget* knob;
    for (uint32_t i=0; i<50; i++) {
      //if ((i+1) & 15)
      //  knob = new KODE_KnobWidget(KODE_FRect(32,32));
      //else
      knob = new KODE_KnobWidget(KODE_FRect(40,40));
      //knob->layout.alignment = KODE_WIDGET_ALIGN_TOP_LEFT;
      knob->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
      knob->setValue((float)i * 0.01);
      scrollbox1->appendWidget(knob);
    }

  }

  //----------

  virtual ~KODE_right_panel() {
  }

};
