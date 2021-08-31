
class KODE_top_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  float buffer[1024] = {0};

//------------------------------
public:
//------------------------------

  KODE_top_panel( KODE_FRect ARect, KODE_Drawable* ATarget)
  : KODE_PanelWidget(ARect) {

    for (uint32_t i=0; i<1024; i++) buffer[i] = KODE_RandomSigned();

    layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = 10;
    layout.spacing = 10;

    // circular waveform

    KODE_CircularWaveformWidget* cwave = new KODE_CircularWaveformWidget( KODE_FRect(0,0,100,100) );
    cwave->setBuffer(buffer);
    cwave->setBufferSize(256);
    cwave->setInnerRadius(0.4f);
    cwave->setOuterRadius(1);
    cwave->setBipolar(true);
    appendWidget(cwave);

    // value grraph

    KODE_ValueGraphWidget* valuegraph = new KODE_ValueGraphWidget( KODE_FRect(0,110,100,45), 16 );
    for (uint32_t i=0; i<16; i++) valuegraph->appendNode(KODE_Random(),0);
    appendWidget(valuegraph);

    // waveform

    KODE_WaveformWidget* waveform = new KODE_WaveformWidget( KODE_FRect(110,0,200,100) );
    waveform->setBuffer(buffer);
    waveform->setBufferSize(1024);
    appendWidget(waveform);

  }

  //----------

  virtual ~KODE_top_panel() {
  }

//------------------------------
public:
//------------------------------

};

