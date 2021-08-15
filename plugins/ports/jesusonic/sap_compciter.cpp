
// based on the 'compciter' jesusonic effect by 'loser'

/*
  (C) 2007, Michael Gruhn.

  NO WARRANTY IS GRANTED.  THIS PLUG-IN  IS PROVIDED FOR FREE  ON  AN  "AS  IS"
  BASIS, WITHOUT WARRANTY OF ANY KIND.  NO LIABILITY IS GRANTED, INCLUDING, BUT
  NOT LIMITED TO, ANY DIRECT OR INDIRECT,  SPECIAL, INCIDENTAL OR CONSEQUENTIAL
  DAMAGE ARISING OUT OF THE USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER
  FAILTURE  OF  MALFUNCTION  INCLUDED.  THE  USE  OF THE  SOURCE  CODE,  EITHER
  PARTIALLY OR IN TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S
  INTENTION,  AND USED WITH ACKNOWLEDGEMENT OF THE AUTHOR.  LAST BUT NOT LEAST,
  BY USING THIS  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR,  AS WELL
  AS YOUR CLAIM TO ENTRUST SOMEBODY ELSE WITH DOING SO.
*/

//----------------------------------------------------------------------

//#define KODE_DEBUG
//#define KODE_DEBUG_VST2

//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

#define KODE_NO_GUI

//----------------------------------------------------------------------

#include "kode.h"
//#include "plugin/kode_descriptor.h"
//#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName = "sap_compciter";
    MAuthor = "skei.audio";
    MVersion = 0x00000001;

    appendInput( KODE_New KODE_PluginPort("input1"));
    appendInput( KODE_New KODE_PluginPort("input2"));
    appendOutput( KODE_New KODE_PluginPort("output1"));
    appendOutput( KODE_New KODE_PluginPort("output2"));

    /*
    slider1:0<0,60,.1>Drive (dB)
    slider2:25<0,100,1>Distortion (%)
    slider3:5000<800,12000,1>Highpass (Hz)
    slider4:-6<-60,24,1>Wet (dB)
    slider5:0<-120,0,1>Dry (dB)
    */

    appendParameter( KODE_New KODE_FloatParameter("drive",  0.0f,     0.0f,   60.0f) );
    appendParameter( KODE_New KODE_FloatParameter("dist",   25.0f,    0.0f,   100.0f) );
    appendParameter( KODE_New KODE_FloatParameter("hp",     5000.0f,  800.0f, 12000.0f) );
    appendParameter( KODE_New KODE_FloatParameter("wet",   -6.0f,    -60.0f,  24.0f) );
    appendParameter( KODE_New KODE_FloatParameter("dry",    0.0f,    -120.0f, 0.0f) );

  }

  //----------

  virtual ~myDescriptor() {
  }

};

//----------------------------------------------------------------------

/*
  @init
  gain = seekgain = 1;
  c = 8.65617025;
  dc = 10^-30;
  gain = seekGain = 1;
*/

class myInstance : public KODE_Instance {

private:

  bool need_recalc = false;

  float slider1   = 0;
  float slider2   = 0;
  float slider3   = 0;
  float slider4   = 0;
  float slider5   = 0;

  float threshDB  = 0;
  float thresh    = 0;
  float ratio     = 0;
  float release   = 0;
  float t0        = 0;
  float t1        = 0;
  float blp       = 0;
  float alp       = 0;
  float wet       = 0;
  float dry       = 0;

  float t00       = 0;
  float t01       = 0;
  float t10       = 0;
  float t11       = 0;
  float t20       = 0;
  float t21       = 0;

  // @init

  float gain      = 1.0f;
  float seekgain  = 1.0f;
  float c         = 8.65617025;
  float dc        = 10^-30;



public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  virtual ~myInstance() {
  }

private:

  /*
  @slider
  threshDB = -slider1;
  thresh = exp(threshDB/c);
  ratio = 1/20;
  release = exp( -60 / (    pow(1-slider2/100,3)   *500*srate/1000) / c );
  t0 = t1 = 0;
  blp = -exp(-2*$pi*slider3*3 / srate );
  alp = 1.0 + blp;
  wet = exp(slider4/c) / exp(( threshDB -threshDB*ratio )/c);
  dry = exp(slider5/c);
  seekgain = 1;
  */

  void recalc(float srate) {
    need_recalc = false;
    threshDB  = -slider1;
    thresh    = exp(threshDB/c);
    ratio     = 1/20;
    release   = exp( -60 / (    pow(1-slider2/100,3)   *500*srate/1000) / c );
    t0        = 0;
    t1        = 0;
    blp       = -exp(-2*KODE_PI*slider3*3 / srate );
    alp       = 1.0 + blp;
    wet       = exp(slider4/c) / exp(( threshDB -threshDB*ratio )/c);
    dry       = exp(slider5/c);
    seekgain  = 1;
  }

public:

  //void on_open()        { KODE_TRACE; }
  //void on_close()       { KODE_TRACE; }
  //void on_initialize()  { KODE_TRACE; }
  //void on_terminate()   { KODE_TRACE; }
  //void on_activate()    { KODE_TRACE; }
  //void on_deactivate()  { KODE_TRACE; }
  //void on_startPreset() { KODE_TRACE; }
  //void on_endPreset()   { KODE_TRACE; }

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    KODE_Print("%i %.3f\n",AIndex,AValue);
    switch(AIndex) {
      case 0: slider1 = AValue; break;
      case 1: slider2 = AValue; break;
      case 2: slider3 = AValue; break;
      case 3: slider4 = AValue; break;
      case 4: slider5 = AValue; break;
    }
    need_recalc = true;
  }

  /*
  @sample
  s0 =      (t00 = alp*spl0-blp*t00);
  s1 =      (t01 = alp*spl1-blp*t01);
  s0 =      (t10 = alp*s0  -blp*t10);
  s1 =      (t11 = alp*s1  -blp*t11);
  s0 = spl0-(t20 = alp*s0-blp*t20);
  s1 = spl1-(t21 = alp*s1-blp*t21);
  rms = max( abs(spl0) , abs(spl1) );
  seekGain = ((rms > thresh) ? exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms : 1);
  gain = ((gain > seekGain) ? seekGain : min( gain/release , seekGain ));
  spl0 = spl0*dry + s0 *gain * wet;
  spl1 = spl1*dry + s1 *gain * wet;
  */

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    if (need_recalc) recalc(AContext->samplerate);
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;
      float spl0 = *in0++;
      float spl1 = *in1++;
      float s0,s1;
      t00 = alp * spl0 - blp * t00;
      t01 = alp * spl1 - blp * t01;
      s0 = t00;
      s1 = t01;
      t10 = alp * s0 - blp * t10;
      t11 = alp * s1 - blp * t11;
      s0 = t10;
      s1 = t11;
      t20 = alp * s0 - blp * t20;
      t21 = alp * s1 - blp * t21;
      s0 = spl0 - t20;
      s1 = spl1 - t21;
      float rms = fmax( abs(spl0) , abs(spl1) );
      //seekgain = ((rms > thresh) ? exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms : 1);
      if (rms > thresh) seekgain = exp((threshDB + (log(rms)*c-threshDB)*ratio) /c) / rms;
      else seekgain = 1;
      //gain = ((gain > seekGain) ? seekGain : min( gain/release , seekGain ));
      if (gain > seekgain) {
        gain = seekgain;
      }
      else {
        gain = fmin( gain/release , seekgain );
      }
      spl0 = spl0*dry + s0 *gain * wet;
      spl1 = spl1*dry + s1 *gain * wet;
      *out0++ = spl0;
      *out1++ = spl1;
    }
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
