
#define KODE_GUI_XCB

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

/*
  based on:
    BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
    http://forum.cockos.com/showthread.php?t=91439
    license: ?
    from the forum:
    skei (me) "i converted both the sonic maximizer (just the first version,
               yet..) and the sonic enhancer to vst, available here [...] i
               haven't studied the licensing agreements, so if you don't want
               me to port your plugins, let me know, and i'll remove it, no
               problem.."
    domino49: "great!!"
    so i think it should be ok?
*/

//----------------------------------------------------------------------

//#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 -KODE_PI2 * freqHP
#define freqLP_p2 -KODE_PI2 * freqLP

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : KODE_Descriptor() {
    #ifdef KODE_DEBUG
      setName("sa_sonic_maximizer_debug");
    #else
      setName("sa_sonic_maximizer");
    #endif
    setAuthor("skei.audio");
    setVersion(0x00000001);
    appendInput( KODE_New KODE_PluginPort("input1"));
    appendInput( KODE_New KODE_PluginPort("input2"));
    appendOutput( KODE_New KODE_PluginPort("output1"));
    appendOutput( KODE_New KODE_PluginPort("output2"));
    appendParameter( KODE_New KODE_FloatParameter("Low Cont",  1,  0,  10, 0.1 ) );
    appendParameter( KODE_New KODE_FloatParameter("Process",   1,  0,  10, 0.1 ) );
    appendParameter( KODE_New KODE_FloatParameter("Outout",    1, -30, 0,  0.1 ) );
  }

  //----------

  //virtual ~myDescriptor() {
  //}

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

//------------------------------
private:
//------------------------------

  float slider1     = 0.0f;
  float slider2     = 0.0f;
  float slider3     = 0.0f;
  float band1       = 0.0f;
//float band2       = 1.0f;
  float band3       = 0.0f;
  float tmplLP      = 0.0f;
  float tmprLP      = 0.0f;
  float tmplHP      = 0.0f;
  float tmprHP      = 0.0f;
  float amp         = 0.0f;
  float xLP         = 0.0f;
  float xHP         = 0.0f;
  float a0LP        = 0.0f;
  float a0HP        = 0.0f;
  float b1LP        = 0.0f;
  float b1HP        = 0.0f;
  bool need_recalc  = true;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    xHP  = expf(freqHP_p2/srate);
    a0HP = 1.0 - xHP;
    b1HP = -xHP;
    xLP  = expf(freqLP_p2/srate);
    a0LP = 1.0 - xLP;
    b1LP = -xLP;
  }

//------------------------------
public:
//------------------------------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch (AIndex) {
      case 0:
        slider1 = AValue;
        band1 = expf( (slider1+3) / c_ampdB );
        break;
      case 1:
        slider2 = AValue;
        band3 = expf( (slider2+3) / c_ampdB );
        break;
      case 2:
        slider3 = AValue;
        amp = expf( slider3 / c_ampdB );
        break;
    }
    //band2 := 1; // exp(0/c_ampdB);
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    if (need_recalc) {
      recalc(AContext->samplerate);
      need_recalc = false;
    }
    uint32_t len    = AContext->numsamples;
    float* input0   = AContext->inputs[0];
    float* input1   = AContext->inputs[1];
    float* output0  = AContext->outputs[0];
    float* output1  = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;
      float s0  = *input0++;
      float s1  = *input1++;
      tmplLP    = a0LP*s0 - b1LP*tmplLP;// + cDenorm;
      tmprLP    = a0LP*s1 - b1LP*tmprLP;// + cDenorm;
      float sp0 = tmplLP;
      float sp1 = tmprLP;
      tmplHP    = a0HP*s0 - b1HP*tmplHP;// + cDenorm;
      tmprHP    = a0HP*s1 - b1HP*tmprHP;// + cDenorm;
      float sp4 = s0 - tmplHP;
      float sp5 = s1 - tmprHP;
      float sp2 = s0 - sp0 - sp4;
      float sp3 = s1 - sp1 - sp5;
      *output0++ = (sp0 * band1 + sp2 /* * band2 */ + sp4 * band3) * amp;
      *output1++ = (sp1 * band1 + sp3 /* * band2 */ + sp5 * band3) * amp;
    }
  }

  //----------

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
