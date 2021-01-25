
#define KODE_NO_GUI

#include "base/kode.h"
#include "base/utils/kode_math.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

// cat /proc/sys/kernel/random/uuid
// 244276fa-50ca-4e0f-8344-ec9a9effed4c
uint8_t myPluginId[16]  = {0x24,0x42,0x76,0xfa,0x50,0xca,0x4e,0x0f,0x83,0x44,0xec,0x9a,0x9e,0xff,0xed,0x4c};

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    //KODE_TRACE;

    MName       = "sa.ports.dynamic";
    MAuthor     = "skei.audio";
    MLongId = myPluginId;

    appendParameter( KODE_New KODE_FloatParameter( "pre gain",   0,   -24, 24   )); // db
    appendParameter( KODE_New KODE_FloatParameter( "post gain",  0,   -24, 24   )); // db
    appendParameter( KODE_New KODE_FloatParameter( "threshold",  0,   -24, 0    )); // db
    appendParameter( KODE_New KODE_FloatParameter( "ratio",      3,    1,  10   ));
    appendParameter( KODE_New KODE_FloatParameter( "attack",     2000, 1,  3000 )); // db/sec
    appendParameter( KODE_New KODE_FloatParameter( "release",    300,  1,  3000 )); // db/sec

  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

    float FPreGain;
    float FPostGain;
    float FThreshold;
    float FRatio;
    float FAttack;
    float FRelease;
    float FGain = 1.0f;

    bool need_recalc = true;
    float par_attack;
    float par_release;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
  }

private:

  void recalc(float srate) {
    FAttack  = 1.0f / powf(2.0f,(par_attack/srate)/6.0f);
    FRelease = powf(2.0f,(par_release/srate)/6.0f);
  }

public:

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    switch(AIndex) {
      case 0:  FPreGain   = powf(2.0f,AValue/6.0f); break;
      case 1:  FPostGain  = powf(2.0f,AValue/6.0f); break;
      case 2:  FThreshold = powf(2.0f,AValue/6.0f); break;
      case 3:  FRatio     = 1.0f / AValue; break;
      case 4:  FAttack    = par_attack = AValue; break; //1.0f / powf(2.0f,(AValue/MSampleRate)/6.0f); break;
      case 5:  FRelease   = par_release = AValue; break; //powf(2.0f,(AValue/MSampleRate)/6.0f); break;
    }
    need_recalc = true;
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {
    if (need_recalc) {
      need_recalc = false;
      recalc(AContext->samplerate);
    }
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;
      float spl0,spl1;
      float input;
      float target;
      spl0 = *in0++;
      spl1 = *in1++;
      input = FPreGain * KODE_Max(abs(spl0),abs(spl1));
      if (input > FThreshold) {
        target = FThreshold + ((input-FThreshold)*FRatio);
      }
      else target = 1;
      if (FGain > target) FGain *= FAttack;
      else FGain *= FRelease;
      spl0 *= (FGain*FPostGain);
      spl1 *= (FGain*FPostGain);
      *out0++ = spl0;
      *out1++ = spl1;
    }
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
