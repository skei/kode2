
/*

  based on the 'exciter' jesusonic plugin by 'loser'

  (C) 2007, Michael Gruhn.
  NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
  WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
  ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
  OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
  MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
  TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
  USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
  PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
  ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
  ENTRUST SOMEBODY ELSE WITH DOING SO.

*/

//----------------------------------------------------------------------

//#define KODE_DEBUG
//#define KODE_DEBUG_TRACE_SOCKET
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
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName = "sap_exciter";
    MAuthor = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    //MFlags |= KODE_PLUGIN_HAS_EDITOR;

    /*
    slider1:2000<100,18000,1>Frequency (Hz)
    slider2:0<0,6,.1>Clip Boost (dB)
    slider3:0<0,100,1>Harmonics (%)
    slider4:-6<-120,0,.1>Mix Back (dB
    */

    appendParameter( KODE_New KODE_FloatParameter(  "freq",   2000.0f,  100.0f, 18000.0f  ));
    appendParameter( KODE_New KODE_FloatParameter(  "boost",     0.0f,    0.0f,     6.0f  ));
    appendParameter( KODE_New KODE_FloatParameter(  "harm",      0.0f,    0.0f,   100.0f  ));
    appendParameter( KODE_New KODE_FloatParameter(  "mix",      -6.0f, -120.0f,     0.0f  ));

  }


  //----------

  virtual ~myDescriptor() {
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

    float slider1     = 0;
    float slider2     = 0;
    float slider3     = 0;
    float slider4     = 0;
    bool  need_recalc = true;
    float tmplONE     = 0;
    float tmprONE     = 0;
    float tmplTWO     = 0;
    float tmprTWO     = 0;

    float c_ampDB     = 8.65617025;
    float cDenorm     = 10^-30;
    float clipBoost   = 0;
    float mixBack     = 0;
    float hdistr      = 0;
    float foo         = 0;
    float freq        = 0;
    float x           = 0;
    float a0          = 0;
    float b1          = 0;


public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {

    /*
    @init
    c_ampDB = 8.65617025;
    cDenorm=10^-30;
    */

    //c_ampDB = 8.65617025;
    //cDenorm = 10^-30;

  }

  virtual ~myInstance() {
  }

public:

  /*
  @slider
  clipBoost = exp(slider2/c_ampDB);
  mixBack = exp(slider4/c_ampDB);
  hdistr = min(slider3/100,.9);
  foo = 2*hdistr/(1-hdistr);
  freq = min(slider1,srate);
  x = exp(-2.0*$pi*freq/srate);
  a0 = 1.0-x;
  b1 = -x
  */

  void recalc(float srate) {
    need_recalc = false;
    clipBoost = exp(slider2 / c_ampDB);
    mixBack   = exp(slider4 / c_ampDB);
    hdistr    = KODE_Min(slider3 / 100.0f, 0.9f);
    foo       = 2.0f * hdistr / (1.0f - hdistr);
    freq      = KODE_Min(slider1,srate);
    x         = exp(-2.0f * KODE_PI * freq / srate);
    a0        = 1.0f - x;
    b1        = -x;
  }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch(AIndex) {
      case 0: slider1 = AValue; break;
      case 1: slider2 = AValue; break;
      case 2: slider3 = AValue; break;
      case 3: slider4 = AValue; break;
    }
    need_recalc = true;
  }

  /*
  @sample
  s0 = spl0;
  s1 = spl1;
  s0 -= (tmplONE = a0*s0 - b1*tmplONE + cDenorm);
  s1 -= (tmprONE = a0*s1 - b1*tmprONE + cDenorm);
  s0 = min(max(s0*clipBoost,-1),1);
  s1 = min(max(s1*clipBoost,-1),1);
  s0 = (1+foo)*s0/(1+foo*abs(spl0));
  s1 = (1+foo)*s1/(1+foo*abs(spl1));
  s0 -= (tmplTWO = a0*s0 - b1*tmplTWO + cDenorm);
  s1 -= (tmprTWO = a0*s1 - b1*tmprTWO + cDenorm);
  spl0 += s0 * mixBack;
  spl1 += s1 * mixBack;
  */


  void on_plugin_process(KODE_ProcessContext* AContext) final {

    float srate = AContext->samplerate;
    if (need_recalc) recalc(srate);

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

      float s0 = spl0;
      float s1 = spl1;

      tmplONE = a0*s0 - b1*tmplONE;// + cDenorm;
      tmprONE = a0*s1 - b1*tmprONE;// + cDenorm;

      //TODO: denorm

      s0 -= tmplONE;
      s1 -= tmprONE;;
      s0 = KODE_Min(KODE_Max(s0*clipBoost,-1),1);
      s1 = KODE_Min(KODE_Max(s1*clipBoost,-1),1);
      s0 = (1+foo) * s0 / (1 + foo * abs(spl0));
      s1 = (1+foo) * s1 / (1 + foo * abs(spl1));

      tmplTWO = a0*s0 - b1*tmplTWO;// + cDenorm
      tmprTWO = a0*s1 - b1*tmprTWO;// + cDenorm;

      //TODO: denorm

      s0 -= tmplTWO;
      s1 -= tmprTWO;

      spl0 += (s0 * mixBack);
      spl1 += (s1 * mixBack);

      *out0++ = spl0;
      *out1++ = spl1;

    }
  }


  //KODE_Editor* on_openEditor(void* AParent) { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) {}
  //void on_updateEditor(KODE_Editor* AEditor) {}

};



//----------------------------------------------------------------------

//KODE_EXE_MAIN(KODE_Descriptor,KODE_Instance);
//KODE_VST3_MAIN(KODE_Descriptor,KODE_Instance);

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
