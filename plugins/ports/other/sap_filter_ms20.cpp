
#define KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

const char* txt_type[2] = { "lowpass", "highpass" };

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    //KODE_TRACE;

    MName       = "sap_filter_ms20";
    MAuthor     = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    //appendParameter( new KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( new KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

    appendParameter( new KODE_TextParameter(  "type", 0,    2,    txt_type  ));
    appendParameter( new KODE_FloatParameter( "freq", 1.0                   ));
    appendParameter( new KODE_FloatParameter( "reso", 0.0,  0.0,  1.1       ));
    appendParameter( new KODE_FloatParameter( "clip", 0.5,  0.01, 4         ));
    appendParameter( new KODE_FloatParameter( "pre",  1.0,  0,    2         ));
    appendParameter( new KODE_FloatParameter( "post", 1.0,  0,    2         ));

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

  float     z1 = 0.0f;
  float     z2 = 0.0f;
  uint32_t  p_type;
  float     p_freq, p_reso, p_clip;
  float     p_pre, p_post;
  //float     dt; // 1/srate

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
  }

  virtual ~myInstance() {
    //KODE_TRACE;
  }

private:

    // arbitrarily made up, asymmetric to introduce even harmonics

    float ms20_filter_shape_gain(float x) {
      return 1 / sqrt(1.0 + 0.66 * x + x * x);
    }

    //----------

    float ms20_filter_shape(float clip_level, float x) {
      return x * ms20_filter_shape_gain(x / clip_level);
    }

    //----------

    float ms20_filter_tick(float freq, float reso, float clip_level, float lp_input, float hp_input) {
      float k = fmin(1.0, freq*KODE_PI2);
      float z2_prime = z2 + hp_input;
      float z1_prime = z1 + ms20_filter_shape(clip_level, 2 * z2_prime * reso);
      z1 += (lp_input - z1_prime) * k;
      z1_prime += (lp_input - z1_prime) * k;
      z2 += (z1_prime - z2_prime) * k;
      z2_prime += (z1_prime - z2_prime) * k;
      return z2_prime;
    }

    //----------

    float ms20_filter_tick_lp(float freq, float reso, float clip_level, float input) {
      return ms20_filter_tick(freq, reso, clip_level, input, 0.0);
    }

    //----------

    float ms20_filter_tick_hp(float freq, float reso, float clip_level, float input) {
      return ms20_filter_tick(freq, reso, clip_level, 0.0, input);
    }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
      float v3 = AValue*AValue*AValue;
      float v5 = AValue*AValue*AValue*AValue*AValue;
      switch(AIndex) {
        case 0: p_type  = AValue; break;
        case 1: p_freq  = v5;     break;
        case 2: p_reso  = AValue; break;
        case 3: p_clip  = AValue; break;
        case 4: p_pre   = v3;     break;
        case 5: p_post  = v3;     break;
      }
  }

  void on_plugin_process(KODE_ProcessContext* AContext) final {
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

      // todo: move switch outside of loop?
      switch (p_type) {
        case 0: // lowpass
          spl0 = ms20_filter_tick_lp(p_freq*0.5,p_reso,p_clip,spl0*p_pre) * p_post;
          break;
        case 1: // highpass
          spl0 = ms20_filter_tick_hp(p_freq*0.5,p_reso,p_clip,spl0*p_pre) * p_post;
          break;
      }

      spl1 = spl0;
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
