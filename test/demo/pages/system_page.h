

class system_page
: public KODE_ScrollBoxWidget {

//------------------------------
public:
//------------------------------

  uint32_t  MTimerCounter     = 0;
  char      MValueBuffer[256] = {0};
  char      MTimerText[256]   = {0};


//------------------------------
public:
//------------------------------

  system_page()
  : KODE_ScrollBoxWidget(true,false) {
    setName("system_page");
    setHint("system page");
    MContent->layout.innerBorder = 10;
    MContent->layout.spacing = 10;

    // textbox

    KODE_TextBoxWidget* textbox = new KODE_TextBoxWidget( KODE_FRect(50) );
    textbox->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    //textbox->layout.innerBorder = 5;
    textbox->flags.sizePercent = true;
    textbox->getContentWidget()->layout.innerBorder = 5;
    appendWidget(textbox);

    // add text to textbox
    addTextBoxText(textbox);

    // timer text

    //KODE_TextWidget* timer_text = new KODE_TextWidget(KODE_FRect(200,20));
    //timer_text->setText("Timer: ");
    //timer_text->layout.alignment = KODE_WIDGET_ALIGN_TOP_LEFT;
    //appendWidget(timer_text);

  }

//------------------------------
public:
//------------------------------

  void addTextBoxText(KODE_TextBoxWidget* textbox) {
    #if defined(KODE_LINUX)
      textbox->appendText("os: linux");
    #elif defined(KODE_WINDOW)
      textbox->appendText("os: window");
    #else
      textbox->appendText("os: ?");
    #endif

    #if defined(KODE_32BIT)
      #if defined(KODE_LITTLE_ENDIAN)
        textbox->appendText("architecture: 32bit, little endian");
      #else
        textbox->appendText("architecture: 32bit, big endian");
      #endif
    #else // #elif defined(KODE_64BIT)
      #if defined(KODE_LITTLE_ENDIAN)
        textbox->appendText("architecture: 64bit, little endian");
      #else
        textbox->appendText("architecture: 64bit, big endian");
      #endif
    #endif

    #if defined(KODE_PLUGIN_EXE)
      textbox->appendText("plugin format: exe");
    #elif defined(KODE_PLUGIN_VST3)
      textbox->appendText("plugin format: vst3");
    #elif defined(KODE_PLUGIN_LV2)
      textbox->appendText("plugin format: lv2");
    #endif

    #if defined(KODE_GUI_XCB)
      #if defined(KODE_USE_CAIRO)
        textbox->appendText("gui: xcb (+ cairo)");
      #else
        textbox->appendText("gui: xcb");
      #endif
    #elif defined(KODE_GUI_CAIRO)
      textbox->appendText("gui: cairo (+ xcb)");
    #endif
  }

  //----------

  void timer() {
    MTimerCounter += 1;
    KODE_IntToString(MValueBuffer,MTimerCounter);
    strcat(MTimerText,"Timer: ");
    strcat(MTimerText,MValueBuffer);
    do_widget_redraw(this,getRect(),0);
    //KODE_Print("%s\n",MTimerText);
  }

};
