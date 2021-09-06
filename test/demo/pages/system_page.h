

class system_page
: public KODE_ScrollBoxWidget {

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  system_page()
  : KODE_ScrollBoxWidget() {
    setName("system_page");
    setHint("system_page");
    MContent->layout.innerBorder = 10;

    // textbox

    KODE_TextBoxWidget* textbox = new KODE_TextBoxWidget( KODE_FRect(200) );
    textbox->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    //textbox->layout.innerBorder = 5;
    //textbox->flags.sizePercent = true;
    textbox->getContentWidget()->layout.innerBorder = 5;
    appendWidget(textbox);

    //

    #if defined(KODE_CPP11)
      textbox->appendText("c++11: yes");
    #else
      textbox->appendText("c++11: no");
    #endif

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

    //

  }

//------------------------------
public:
//------------------------------

};
