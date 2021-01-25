
#define KODE_NO_GUI
#define KODE_NO_PAINTER

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

/*
  based on:
    BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
    http://forum.cockos.com/showthread.php?t=91439

    from the forum:
    skei (me) "i converted both the sonic maximizer (just the first version,
               yet..) and the sonic enhancer to vst, available here [...] i
               haven't studied the licensing agreements, so if you don't want
               me to port your plugins, let me know, and i'll remove it, no
               problem.."
    domino49: "great!!"
*/

//----------------------------------------------------------------------

//#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 -KODE_PI2 * freqHP
#define freqLP_p2 -KODE_PI2 * freqLP

// cat /proc/sys/kernel/random/uuid
// 9c86bef7-b83b-471d-9a4b-4f46346bf53d
uint8_t myPluginId[16]  = {0x9c,0x86,0xbe,0xf7,0xb8,0x3b,0x47,0x1d,0x9a,0x4b,0x4f,0x46,0x34,0x6b,0xf5,0x3d};
//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : KODE_Descriptor() {
    MName       = "sa.ports.sonic_maximizer";
    MAuthor     = "skei.audio";
    MLongId     = myPluginId;
    appendParameter( KODE_New KODE_FloatParameter("Low Cont",  1,  0,  10, 0.1 ) );
    appendParameter( KODE_New KODE_FloatParameter("Process",   1,  0,  10, 0.1 ) );
    appendParameter( KODE_New KODE_FloatParameter("Outout",    1, -30, 0,  0.1 ) );
  }

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
  float band2       = 0.0f;
  float band3       = 0.0f;
  float amp         = 0.0f;
  float tmplLP      = 0.0f;
  float tmprLP      = 0.0f;
  float tmplHP      = 0.0f;
  float tmprHP      = 0.0f;
  float xLP         = 0.0f;
  float xHP         = 0.0f;
  float a0LP        = 0.0f;
  float a0HP        = 0.0f;
  float b1LP        = 0.0f;
  float b1HP        = 0.0f;
  bool  need_recalc = true;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

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

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
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

  void on_processBlock(KODE_ProcessContext* AContext) final {
    if (need_recalc) {
      need_recalc = false;
      recalc(AContext->samplerate);
    }
    uint32_t len = AContext->numsamples;
    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;
      float s0 = *input0++;
      float s1 = *input1++;
      tmplLP = a0LP*s0 - b1LP*tmplLP;// + cDenorm;
      tmprLP = a0LP*s1 - b1LP*tmprLP;// + cDenorm;
      float sp0 = tmplLP;
      float sp1 = tmprLP;
      tmplHP = a0HP*s0 - b1HP*tmplHP;// + cDenorm;
      tmprHP = a0HP*s1 - b1HP*tmprHP;// + cDenorm;
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

