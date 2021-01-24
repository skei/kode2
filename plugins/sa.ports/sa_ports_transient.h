/*
  based on:
    ?
    (C) 2007, Michael Gruhn
    http:// ?
    license: gpl
*/

#define KODE_NO_GUI

#include "base/kode.h"
#include "base/utils/kode_math.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

// cat /proc/sys/kernel/random/uuid
// 7c2ae4ce-652a-4d05-b7fa-6ba3ef1d2077
uint8_t myPluginId[16]  = {0x7c,0x2a,0xe4,0xce,0x65,0x2a,0x4d,0x05,0xb7,0xfa,0x6b,0xa3,0xef,0x1d,0x20,0x77};

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    //KODE_TRACE;

    MName       = "sa.ports.transient";
    MAuthor     = "skei.audio";
    MLongId = myPluginId;

    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

    appendParameter( KODE_New KODE_FloatParameter( "attack",  0, -100, 100, 1 ) );
    appendParameter( KODE_New KODE_FloatParameter( "sustain", 0, -100, 100, 1 ) );
    appendParameter( KODE_New KODE_FloatParameter( "output",  0, -24,  6     ) );


  }

  //----------

  virtual ~myDescriptor() {
    //KODE_TRACE;
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  //float MLeft   = 0.0f;
  //float MRight  = 0.0f;

  float attack, sustain, vol;
  float b1Env1, a0Env1, b1Env2, a0Env2, b1Env3, a0Env3;

  float tmpEnv1 = 0.0f;
  float tmpEnv2 = 0.0f;
  float tmpEnv3 = 0.0f;

  bool need_recalc = true;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
  }

  virtual ~myInstance() {
    //KODE_TRACE;
  }

private:

  void recalc(float srate) {
    b1Env1 = -exp(-30 / srate );
    a0Env1 = 1.0 + b1Env1;
    b1Env2 = -exp(-1250 / srate );
    a0Env2 = 1.0 + b1Env2;
    b1Env3 = -exp(-3 / srate );
    a0Env3 = 1.0 + b1Env3;
  }

public:

  //void on_open()        final { KODE_TRACE; }
  //void on_close()       final { KODE_TRACE; }
  //void on_initialize()  final { KODE_TRACE; }
  //void on_terminate()   final { KODE_TRACE; }
  //void on_activate()    final { KODE_TRACE; }
  //void on_deactivate()  final { KODE_TRACE; }
  //void on_startPreset() final { KODE_TRACE; }
  //void on_endPreset()   final { KODE_TRACE; }

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
      switch(AIndex) {
        case 0: attack  = AValue/100;       break;
        case 1: sustain = AValue/50;        break;
        case 2: vol     = pow(2,AValue/6);  break;
      }
  }

  //void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}

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

      float spl0 = *input0++;
      float spl1 = *input1++;
      //...
      float maxSpls = KODE_Max(abs(spl0),abs(spl1));
      tmpEnv1 = a0Env1*maxSpls - b1Env1*tmpEnv1;
      tmpEnv2 = a0Env2*maxSpls - b1Env2*tmpEnv2;
      tmpEnv3 = a0Env3*maxSpls - b1Env3*tmpEnv3;
      float env1 = /*S*/sqrt(tmpEnv1);
      float env2 = /*S*/sqrt(tmpEnv2);
      float env3 = /*S*/sqrt(tmpEnv3);
      float gain = /*S*/exp(log(KODE_Max(env2/env1,1))*attack) * exp( log( KODE_Max(env3/env1,1))*sustain);
      gain *= vol;
      spl0 *= gain;
      spl1 *= gain;
      //env1 := KKillDenorm(env1);
      //env2 := KKillDenorm(env2);
      //env3 := KKillDenorm(env3);
      //...
      *output0++ = spl0;
      *output1++ = spl1;


    }
  }

  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)






