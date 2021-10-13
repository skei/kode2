#ifndef kode_debug_process_panel_included
#define kode_debug_process_panel_included
//----------------------------------------------------------------------

#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class kode_debug_process_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_ValueWidget*   wdgPid          = KODE_NULL;
  KODE_ValueWidget*   wdgTid          = KODE_NULL;

  KODE_ValueWidget*   wdgMode         = KODE_NULL;
  KODE_ValueWidget*   wdgOffset       = KODE_NULL;
  KODE_ValueWidget*   wdgNumSamples   = KODE_NULL;
  KODE_ValueWidget*   wdgNumInputs    = KODE_NULL;
  KODE_ValueWidget*   wdgNumOutputs   = KODE_NULL;
  KODE_TextWidget*    wdgIn0          = KODE_NULL;
  KODE_TextWidget*    wdgIn1          = KODE_NULL;
  KODE_TextWidget*    wdgOut0         = KODE_NULL;
  KODE_TextWidget*    wdgOut1         = KODE_NULL;
  KODE_ValueWidget*   wdgSampleRate   = KODE_NULL;
  KODE_ValueWidget*   wdgTempo        = KODE_NULL;
  KODE_ValueWidget*   wdgTimeSigNum   = KODE_NULL;
  KODE_ValueWidget*   wdgTimeSigDenom = KODE_NULL;
  KODE_ValueWidget*   wdgSamplePos    = KODE_NULL;
  KODE_ValueWidget*   wdgBeatPos      = KODE_NULL;
  KODE_ValueWidget*   wdgPlayState    = KODE_NULL;


//------------------------------
public:
//------------------------------

  kode_debug_process_panel(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("kode_debug_process_panel");
    setHint("process");

    wdgPid          = new KODE_ValueWidget( KODE_FRect( 10, 10,180,16), "process id" );
    wdgTid          = new KODE_ValueWidget( KODE_FRect( 10, 30,180,16), "thread id" );

    wdgMode         = new KODE_ValueWidget( KODE_FRect( 10, 60,180,16), "mode" );
    wdgOffset       = new KODE_ValueWidget( KODE_FRect( 10, 80,180,16), "offset" );
    wdgNumSamples   = new KODE_ValueWidget( KODE_FRect( 10,100,180,16), "num samples" );
    wdgNumInputs    = new KODE_ValueWidget( KODE_FRect( 10,120,180,16), "num inputs" );
    wdgNumOutputs   = new KODE_ValueWidget( KODE_FRect( 10,140,180,16), "num outputs" );
    wdgIn0          = new KODE_TextWidget(  KODE_FRect( 10,160,180,16), "in0" );
    wdgIn1          = new KODE_TextWidget(  KODE_FRect( 10,180,180,16), "in1" );
    wdgOut0         = new KODE_TextWidget(  KODE_FRect( 10,200,180,16), "out0" );
    wdgOut1         = new KODE_TextWidget(  KODE_FRect( 10,220,180,16), "out1" );
    wdgSampleRate   = new KODE_ValueWidget( KODE_FRect( 10,240,180,16), "samplerate" );
    wdgTempo        = new KODE_ValueWidget( KODE_FRect( 10,260,180,16), "tempo" );
    wdgTimeSigNum   = new KODE_ValueWidget( KODE_FRect( 10,280,180,16), "timesig num" );
    wdgTimeSigDenom = new KODE_ValueWidget( KODE_FRect( 10,300,180,16), "timesig denom" );
    wdgSamplePos    = new KODE_ValueWidget( KODE_FRect( 10,320,180,16), "sample pos" );
    wdgBeatPos      = new KODE_ValueWidget( KODE_FRect( 10,340,180,16), "beat pos" );
    wdgPlayState    = new KODE_ValueWidget( KODE_FRect( 10,360,180,16), "play state" );

    appendWidget(wdgPid);
    appendWidget(wdgTid);

    appendWidget(wdgMode);
    appendWidget(wdgOffset);
    appendWidget(wdgNumSamples);
    appendWidget(wdgNumInputs);
    appendWidget(wdgNumOutputs);
    appendWidget(wdgIn0);
    appendWidget(wdgIn1);
    appendWidget(wdgOut0);
    appendWidget(wdgOut1);
    appendWidget(wdgSampleRate);
    appendWidget(wdgTempo);
    appendWidget(wdgTimeSigNum);
    appendWidget(wdgTimeSigDenom);
    appendWidget(wdgSamplePos);
    appendWidget(wdgBeatPos);
    appendWidget(wdgPlayState);

    wdgPid->setDigits(0);
    wdgTid->setDigits(0);

    wdgMode->setDigits(0);
    wdgOffset->setDigits(0);
    wdgNumSamples->setDigits(0);
    wdgNumInputs->setDigits(0);
    wdgNumOutputs->setDigits(0);
    wdgTimeSigNum->setDigits(0);
    wdgTimeSigDenom->setDigits(0);
    wdgSamplePos->setDigits(0);
    wdgPlayState->setDigits(0);


  }

};

//----------------------------------------------------------------------
#endif
