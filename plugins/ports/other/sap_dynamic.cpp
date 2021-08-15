
#define KODE_NO_GUI

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

    //KODE_TRACE;

    MName       = "sap_dynamic";
    MAuthor     = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

    appendParameter( KODE_New KODE_FloatParameter( "pre gain",   0,   -24, 24   )); // db
    appendParameter( KODE_New KODE_FloatParameter( "post gain",  0,   -24, 24   )); // db
    appendParameter( KODE_New KODE_FloatParameter( "threshold",  0,   -24, 0    )); // db
    appendParameter( KODE_New KODE_FloatParameter( "ratio",      3,    1,  10   ));
    appendParameter( KODE_New KODE_FloatParameter( "attack",     2000, 1,  3000 )); // db/sec
    appendParameter( KODE_New KODE_FloatParameter( "release",    300,  1,  3000 )); // db/sec

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

  virtual ~myInstance() {
    //KODE_TRACE;
  }

private:

  void recalc(float srate) {
    FAttack  = 1.0f / powf(2.0f,(par_attack/srate)/6.0f);
    FRelease = powf(2.0f,(par_release/srate)/6.0f);
  }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
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

  void on_plugin_process(KODE_ProcessContext* AContext) final {

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

  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
