


//----------

/*
  left_panel doesn't clip others when resizing
    if we encapsulate all other panels into a right_panel, and then add sixer,
    etc to this, it should work?
    because clipping rectangle when drawing right side isn't taking left_panel
    into account..
    hmm...

*/

//----------

#define WAVEFORM_SIZE 1024

class gui_page
//: public KODE_ScrollBoxWidget {
: public KODE_PanelWidget {

private:

  const char* button_text[5] = {
    "1","2","3","44","five"
  };

  float waveform_buffer[WAVEFORM_SIZE*2] = {0};


public:

  gui_page(KODE_Drawable* ATarget)
  //: KODE_ScrollBoxWidget() {
  : KODE_PanelWidget() {

    for (uint32_t i=0; i<WAVEFORM_SIZE*2; i++) waveform_buffer[i] = KODE_RandomRange(-1,1);

    setName("system_page");
    setHint("system page");
    //MContent->layout.innerBorder = 10;
    //MContent->layout.spacing = 10;
    //layout.innerBorder = 10;
    //layout.spacing = 10;
    setFillBackground(false);
    setDrawBorder(false);

    //--------------------
    // left
    //--------------------

    //KODE_PanelWidget* left_panel = new KODE_PanelWidget(200);
    KODE_ScrollBoxWidget* left_panel = new KODE_ScrollBoxWidget(KODE_FRect(200,200),true,false);
    left_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
      KODE_ScrollBoxWidget* left_content = (KODE_ScrollBoxWidget*)left_panel->getContentWidget();
      left_content->layout.innerBorder = KODE_FRect(10,10,10,10);
      left_content->layout.spacing = 10;
      left_content->setFillBackground(false);
      left_content->setDrawBorder(false);
    left_panel->setFillBackground(false);
    left_panel->setDrawBorder(false);

    appendWidget(left_panel);

    //----------

    // panels

    KODE_PanelWidget* panel1 = new KODE_PanelWidget(KODE_FRect(20,20));
    panel1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    panel1->setFillBackground(false);
    panel1->setDrawBorder(true);
    panel1->setBorderColor(KODE_Color(0.35));
    left_panel->appendWidget(panel1);

    KODE_PanelWidget* panel2 = new KODE_PanelWidget(KODE_FRect(20,20));
    panel2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    panel2->setFillBackground(true);
    panel2->setDrawBorder(false);
    left_panel->appendWidget(panel2);

    // text

    KODE_TextWidget* text1 = new KODE_TextWidget(KODE_FRect(20,20),"Text");
    text1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    text1->setFillBackground(true);
    //text1->setDrawBorder(true);
    left_panel->appendWidget(text1);

    // button

    KODE_ButtonWidget* button1 = new KODE_ButtonWidget(KODE_FRect(20,20));
    button1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    button1->setDrawBorder(false);
    //button1->setFillBackground(true);
    button1->setText("Button On","Button Off");
    button1->setTextColor(KODE_COLOR_DARK_GRAY,KODE_COLOR_LIGHT_GRAY);
    button1->setBackgroundColor(KODE_Color(0.6),KODE_Color(0.4));
    left_panel->appendWidget(button1);

    KODE_ButtonWidget* button2 = new KODE_ButtonWidget(KODE_FRect(20,20));
    button2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    button2->setIsToggle();
    button2->setDrawBorder(false);
    //button2->setFillBackground(true);
    button2->setText("Toggle On","Toggle Off");
    button2->setTextColor(KODE_COLOR_DARK_GRAY,KODE_COLOR_LIGHT_GRAY);
    button2->setBackgroundColor(KODE_Color(0.6),KODE_Color(0.4));
    left_panel->appendWidget(button2);

    // value

    KODE_ValueWidget* value1 = new KODE_ValueWidget(KODE_FRect(20,20));
    value1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    value1->setFillBackground(true);
    //value1->setDrawBorder(true);
    value1->setText("Value");
    value1->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
    //value1->setTextOffset(KODE_FRect(4,0,0,0));
    left_panel->appendWidget(value1);

    // drag value

    KODE_DragValueWidget* dragvalue1 = new KODE_DragValueWidget(KODE_FRect(20,20));
    dragvalue1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    dragvalue1->setFillBackground(true);
    //dragvalue1->setDrawBorder(true);
    dragvalue1->setText("DragValue");
    dragvalue1->setLabel("%");
    dragvalue1->setDrawLabel(true);
    dragvalue1->setValue(0.2);
    dragvalue1->setDefaultValue(0.2);
    //dragvalue1->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
    //dragvalue1->setTextOffset(KODE_FRect(4,0,0,0));
    //dragvalue1->setLabel("...");
    //dragvalue1->setDrawLabel(true);
    left_panel->appendWidget(dragvalue1);

    // slider

    KODE_SliderWidget* slider1 = new KODE_SliderWidget(KODE_FRect(20,20));
    slider1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    slider1->setFillBackground(true);
    slider1->setDrawBorder(false);
    slider1->setText("Slider");
    //slider1->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
    //slider1->setTextOffset(KODE_FRect(4,0,0,0));
    //slider1->setLabel("...");
    //slider1->setDrawLabel(true);
    slider1->setValue(0.4);
    slider1->setDefaultValue(0.4);
    left_panel->appendWidget(slider1);

    // scroll bar

    KODE_ScrollBarWidget* scrollbar1 = new KODE_ScrollBarWidget(KODE_FRect(20,20));
    scrollbar1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    scrollbar1->setValue(0.4);
    scrollbar1->setBackgroundColor(KODE_Color(0.4));
    left_panel->appendWidget(scrollbar1);

    // range slider

    KODE_RangeSliderWidget* rangeslider1 = new KODE_RangeSliderWidget(KODE_FRect(20,20));
    rangeslider1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    rangeslider1->setValue(0.3);
    rangeslider1->setValue2(0.9);
    rangeslider1->setBackgroundColor(KODE_Color(0.4));
    left_panel->appendWidget(rangeslider1);

    // menu item

    KODE_MenuItemWidget* menuitem1 = new KODE_MenuItemWidget(KODE_FRect(20,20));
    menuitem1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    menuitem1->setText("MenuItem");
    left_panel->appendWidget(menuitem1);

    // selector

      KODE_MenuWidget* menu1 = new KODE_MenuWidget(KODE_FRect());
      menu1->setItemSize(100,20);
      menu1->setItemLayout(1,5);
      menu1->appendMenuItem("Item1");
      menu1->appendMenuItem("Item2");
      menu1->appendMenuItem("Item3");
      menu1->appendMenuItem("Item4");
      menu1->appendMenuItem("Item5");

    KODE_SelectorWidget* selector1 = new KODE_SelectorWidget(KODE_FRect(20,20));
    selector1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    selector1->setMenu(menu1);
    left_panel->appendWidget(selector1);

    // button row2

    KODE_ButtonRowWidget* buttonrow1 = new KODE_ButtonRowWidget(KODE_FRect(20,20),5,button_text,KODE_BUTTON_ROW_SINGLE);
    buttonrow1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    buttonrow1->setDrawBorder(false);
    buttonrow1->setButtonState(0,true);
    left_panel->appendWidget(buttonrow1);

    KODE_ButtonRowWidget* buttonrow2 = new KODE_ButtonRowWidget(KODE_FRect(20,20),5,button_text,KODE_BUTTON_ROW_MULTI);
    buttonrow2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    buttonrow2->setDrawBorder(false);
    buttonrow2->setButtonState(1,true);
    buttonrow2->setButtonState(2,true);
    left_panel->appendWidget(buttonrow2);

    // text edit

    KODE_TextEditWidget* textedit1 = new KODE_TextEditWidget(KODE_FRect(20,20));
    textedit1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    textedit1->setText("TextEdit");
    left_panel->appendWidget(textedit1);

    // knobs

    KODE_KnobWidget* knob1 = new KODE_KnobWidget(KODE_FRect(32,32));
    knob1->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    left_panel->appendWidget(knob1);

    KODE_KnobWidget* knob2 = new KODE_KnobWidget(KODE_FRect(32,32));
    knob2->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    knob2->setBipolar();
    knob2->setValue(0.5);
    left_panel->appendWidget(knob2);

    // image strip

    KODE_ImageStripWidget* imagestrip1 = new KODE_ImageStripWidget(KODE_FRect(32,32));
    //imagestrip1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    imagestrip1->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    imagestrip1->setImage(ATarget,(uint8_t*)knob2_32x32_65,knob2_32x32_65_size,KODE_Color(0.5f));
    imagestrip1->setupTiles(1,31);
    imagestrip1->setValue(0.6);
    left_panel->appendWidget(imagestrip1);

    // curves

    KODE_CurveWidget* curve1 = new KODE_CurveWidget(KODE_FRect(KODE_FRect(32,32)),false);
    //curve1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    curve1->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    curve1->setValue(0.7);
    left_panel->appendWidget(curve1);

    KODE_CurveWidget* curve2 = new KODE_CurveWidget(KODE_FRect(32,32),true);
    //curve2->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    curve2->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
    curve2->setValue(0.3);
    left_panel->appendWidget(curve2);

    // group / waveform

    KODE_GroupBoxWidget* groupbox1 = new KODE_GroupBoxWidget(KODE_FRect(100,100));
    groupbox1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    groupbox1->getTitleBar()->setText("Waveform","waveform");
    groupbox1->close();
    left_panel->appendWidget(groupbox1);

        KODE_WaveformWidget* waveform1 = new KODE_WaveformWidget();
        waveform1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
        waveform1->setBufferSize(WAVEFORM_SIZE);
        waveform1->setBuffer(waveform_buffer);
        groupbox1->appendWidget(waveform1);

    // group / value graph

    KODE_GroupBoxWidget* groupbox2 = new KODE_GroupBoxWidget(KODE_FRect(100,100));
    groupbox2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    groupbox2->getTitleBar()->setText("ValueGraph","value_graph");
    groupbox2->close();
    left_panel->appendWidget(groupbox2);

      KODE_ValueGraphWidget* valuegraph1 = new KODE_ValueGraphWidget(KODE_FRect(100,100),16);
      valuegraph1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
      //valuegraph1->setBufferSize(WAVEFORM_SIZE);
      //valuegraph1->setBuffer(waveform_buffer);
      for (uint32_t i=0; i<16; i++) valuegraph1->appendNode(waveform_buffer[i],0);
      groupbox2->appendWidget(valuegraph1);

    // group / slider bank

    KODE_GroupBoxWidget* groupbox3 = new KODE_GroupBoxWidget(KODE_FRect(100,100),100);
    groupbox3->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    groupbox3->getTitleBar()->setText("SliderBank","slider_bank");
    groupbox3->close();
    left_panel->appendWidget(groupbox3);

      KODE_SliderBankWidget* sliderbank1 = new KODE_SliderBankWidget(KODE_FRect(),16);
      sliderbank1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
      //valuegraph1->setBufferSize(WAVEFORM_SIZE);
      //valuegraph1->setBuffer(waveform_buffer);
      //for (uint32_t i=0; i<16; i++) valuegraph1->appendNode(waveform_buffer[i],0);
      groupbox3->appendWidget(sliderbank1);

    // group / keyboard

    KODE_GroupBoxWidget* groupbox4 = new KODE_GroupBoxWidget(KODE_FRect(100,100));
    groupbox4->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    groupbox4->getTitleBar()->setText("Keyboard","keyboard");
    groupbox4->close();
    left_panel->appendWidget(groupbox4);

      KODE_KeyboardWidget* keyboard1 = new KODE_KeyboardWidget();
      keyboard1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
      //keyboard1->setBufferSize(WAVEFORM_SIZE);
      //keyboard1->setBuffer(waveform_buffer);
      groupbox4->appendWidget(keyboard1);

    //--------------------
    // sizer |
    //--------------------

    KODE_SizerWidget* left_sizer = new KODE_SizerWidget(KODE_FRect(5,5),KODE_SIZER_LEFT,left_panel);
    appendWidget(left_sizer);

    //--------------------
    // top
    //--------------------

    KODE_PanelWidget* top_panel = new KODE_PanelWidget(KODE_FRect(100,100));
    top_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    top_panel->layout.innerBorder = KODE_FRect(10,10,10,10);
    top_panel->layout.spacing = 10;
    top_panel->setFillBackground(false);
    top_panel->setDrawBorder(false);
    appendWidget(top_panel);

    //--------------------
    // sizer -
    //--------------------

    KODE_SizerWidget* top_sizer = new KODE_SizerWidget(KODE_FRect(5,5,5,5),KODE_SIZER_TOP,top_panel);
    appendWidget(top_sizer);

    //--------------------
    // right
    //--------------------

    KODE_ScrollBoxWidget* right_panel = new KODE_ScrollBoxWidget(KODE_FRect(200,200),true,false);
    right_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
    right_panel->setFillBackground(false);
    right_panel->setDrawBorder(false);
    appendWidget(right_panel);

      KODE_ScrollBoxWidget* right_content = (KODE_ScrollBoxWidget*)right_panel->getContentWidget();
      right_content->layout.innerBorder = KODE_FRect(10,10,10,10);
      right_content->layout.spacing = 10;
      right_content->setFillBackground(false);
      right_content->setDrawBorder(false);

//    // group / waveform
//
//    KODE_GroupBoxWidget* groupbox1 = new KODE_GroupBoxWidget(100);
//    groupbox1->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
//    groupbox1->getTitleBar()->setText("Waveform","waveform");
//    groupbox1->close();
//    right_panel->appendWidget(groupbox1);
//
//        KODE_WaveformWidget* waveform1 = new KODE_WaveformWidget();
//        waveform1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
//        waveform1->setBufferSize(WAVEFORM_SIZE);
//        waveform1->setBuffer(waveform_buffer);
//        groupbox1->appendWidget(waveform1);
//
//    // group / value graph
//
//    KODE_GroupBoxWidget* groupbox2 = new KODE_GroupBoxWidget(100);
//    groupbox2->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
//    groupbox2->getTitleBar()->setText("ValueGraph","value_graph");
//    groupbox2->close();
//    right_panel->appendWidget(groupbox2);
//
//      KODE_ValueGraphWidget* valuegraph1 = new KODE_ValueGraphWidget(0,16);
//      valuegraph1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
//      //valuegraph1->setBufferSize(WAVEFORM_SIZE);
//      //valuegraph1->setBuffer(waveform_buffer);
//      for (uint32_t i=0; i<16; i++) valuegraph1->appendNode(waveform_buffer[i],0);
//      groupbox2->appendWidget(valuegraph1);
//
//    // group / slider bank
//
//    KODE_GroupBoxWidget* groupbox3 = new KODE_GroupBoxWidget(100);
//    groupbox3->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
//    groupbox3->getTitleBar()->setText("SliderBank","slider_bank");
//    groupbox3->close();
//    right_panel->appendWidget(groupbox3);
//
//      KODE_SliderBankWidget* sliderbank1 = new KODE_SliderBankWidget(0,16);
//      sliderbank1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
//      //valuegraph1->setBufferSize(WAVEFORM_SIZE);
//      //valuegraph1->setBuffer(waveform_buffer);
//      //for (uint32_t i=0; i<16; i++) valuegraph1->appendNode(waveform_buffer[i],0);
//      groupbox3->appendWidget(sliderbank1);
//
//    // group / keyboard
//
//    KODE_GroupBoxWidget* groupbox4 = new KODE_GroupBoxWidget(100);
//    groupbox4->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
//    groupbox4->getTitleBar()->setText("Keyboard","keyboard");
//    groupbox4->close();
//    right_panel->appendWidget(groupbox4);
//
//      KODE_KeyboardWidget* keyboard1 = new KODE_KeyboardWidget();
//      keyboard1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
//      //keyboard1->setBufferSize(WAVEFORM_SIZE);
//      //keyboard1->setBuffer(waveform_buffer);
//      groupbox4->appendWidget(keyboard1);

    //--------------------
    // sizer |
    //--------------------

    KODE_SizerWidget* right_sizer = new KODE_SizerWidget(KODE_FRect(5,5,5,5),KODE_SIZER_RIGHT,right_panel);
    appendWidget(right_sizer);

    //--------------------
    // bottom
    //--------------------

    KODE_PanelWidget* bottom_panel = new KODE_PanelWidget(KODE_FRect(100,100));
    bottom_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    bottom_panel->layout.innerBorder = KODE_FRect(10,10,10,10);
    bottom_panel->layout.spacing = 10;
    bottom_panel->setFillBackground(false);
    bottom_panel->setDrawBorder(false);
    appendWidget(bottom_panel);

//    // knobs
//
//    KODE_KnobWidget* knob1 = new KODE_KnobWidget(32);
//    knob1->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
//    bottom_panel->appendWidget(knob1);
//
//    KODE_KnobWidget* knob2 = new KODE_KnobWidget(32);
//    knob2->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
//    knob2->setBipolar();
//    knob2->setValue(0.5);
//    bottom_panel->appendWidget(knob2);
//
//    // image strip
//
//    KODE_ImageStripWidget* imagestrip1 = new KODE_ImageStripWidget(KODE_FRect(32,32));
//    //imagestrip1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
//    imagestrip1->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
//    imagestrip1->setImage(ATarget,(uint8_t*)knob2_32x32_65,knob2_32x32_65_size,KODE_Color(0.5f));
//    imagestrip1->setupTiles(1,31);
//    imagestrip1->setValue(0.6);
//    bottom_panel->appendWidget(imagestrip1);
//
//    // curves
//
//    KODE_CurveWidget* curve1 = new KODE_CurveWidget(KODE_FRect(32),false);
//    //curve1->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
//    curve1->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
//    curve1->setValue(0.7);
//    bottom_panel->appendWidget(curve1);
//
//    KODE_CurveWidget* curve2 = new KODE_CurveWidget(KODE_FRect(32),true);
//    //curve2->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
//    curve2->layout.alignment = KODE_WIDGET_ALIGN_STACK_HORIZ;
//    curve2->setValue(0.3);
//    bottom_panel->appendWidget(curve2);


    //--------------------
    // sizer -
    //--------------------

    KODE_SizerWidget* bottom_sizer = new KODE_SizerWidget(KODE_FRect(5,5,5,5),KODE_SIZER_BOTTOM,bottom_panel);
    appendWidget(bottom_sizer);

    //--------------------
    // main
    //--------------------

    KODE_ScrollBoxWidget* main_panel = new KODE_ScrollBoxWidget();
    main_panel->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    main_panel->setFillBackground(false);
    main_panel->setDrawBorder(false);
    appendWidget(main_panel);

    KODE_GraphWidget* graph1 = new KODE_GraphWidget();
    graph1->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    main_panel->appendWidget(graph1);

    KODE_GraphModule* module;

    for (uint32_t i=0; i<6; i++) {
      module = new KODE_GraphModule();
      uint32_t inum = KODE_RandomRangeInt(1,5);
      uint32_t onum = KODE_RandomRangeInt(0,5);
      module->numInputs = inum;
      module->numOutputs = onum;
      for (uint32_t j=0; j<inum; j++) module->inputs[j] =  KODE_RandomRangeInt(0,1);
      for (uint32_t j=0; j<onum; j++) module->outputs[j] = KODE_RandomRangeInt(0,1);
      graph1->addModule(module, 10 + i*50, 10 + i*30, "module" );
    }

    //--------------------
    //
    //--------------------

    appendWidget(menu1);

  }

};

#undef WAVEFORM_SIZE
