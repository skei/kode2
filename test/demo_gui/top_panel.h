
class KODE_top_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  #define BUFSIZE 1024

  float buffer[BUFSIZE*2] = {0};

//------------------------------
public:
//------------------------------

  KODE_top_panel( KODE_FRect ARect, KODE_Drawable* ATarget)
  : KODE_PanelWidget(ARect) {

    for (uint32_t i=0; i<BUFSIZE*2; i++) buffer[i] = KODE_RandomSigned();

    layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = KODE_FRect(10,10,10,10);
    layout.spacing = 10;

    KODE_Widget* left = new KODE_Widget( KODE_FRect(50,50) );
    left->flags.sizePercent = true;
    left->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    //left->layout.innerBorder = KODE_FRect(10,10,10,10);
    left->layout.spacing = 10;
    appendWidget(left);

    KODE_Widget* right = new KODE_Widget( KODE_FRect() );
    //right->flags.sizepercent = true;
    right->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    //right->layout.innerBorder = KODE_FRect(10,10,10,10);
    right->layout.spacing = 10;
    appendWidget(right);

    // --- left

    // value graph

    KODE_ValueGraphWidget* valuegraph = new KODE_ValueGraphWidget( KODE_FRect(50,50), 16 );
    valuegraph->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    for (uint32_t i=0; i<16; i++) valuegraph->appendNode(KODE_Random(),0);
    left->appendWidget(valuegraph);

    // waveform

    KODE_WaveformWidget* waveform = new KODE_WaveformWidget( KODE_FRect(100,100) );
    waveform->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    waveform->setBuffer(buffer);
    waveform->setBufferSize(BUFSIZE);
    left->appendWidget(waveform);

    // --- right

    // circular waveform

    KODE_CircularWaveformWidget* circwave = new KODE_CircularWaveformWidget( KODE_FRect(100,100) );
    circwave->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    circwave->setBuffer(buffer);
    circwave->setBufferSize(BUFSIZE / 4);
    circwave->setInnerRadius(0.4f);
    circwave->setOuterRadius(1);
    circwave->setBipolar(true);
    right->appendWidget(circwave);

    // knobs

    KODE_Knob2Widget* knob2 = new KODE_Knob2Widget( KODE_FRect(40,80) );
    knob2->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    knob2->setValue(0.25);
    right->appendWidget(knob2);

    KODE_Knob3Widget* knob3 = new KODE_Knob3Widget( KODE_FRect(90,32) );
    knob3->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    knob3->setValue(0.45);
    right->appendWidget(knob3);

    KODE_Knob4Widget* knob4 = new KODE_Knob4Widget( KODE_FRect(120,20) );
    knob4->layout.alignment = KODE_WIDGET_ALIGN_LEFT_TOP;
    knob4->setValue(0.85);
    right->appendWidget(knob4);

  }

  //----------

  virtual ~KODE_top_panel() {
  }

//------------------------------
public:
//------------------------------

};

