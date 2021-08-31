
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

    MName       = "sap_small_room_reverb";
    MAuthor     = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    //appendParameter( new KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( new KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

    appendParameter( new KODE_FloatParameter( "dry (db)",     0,  -120,0,1 ));
    appendParameter( new KODE_FloatParameter( "wet (db)",    -6,  -120,0,1 ));
    appendParameter( new KODE_FloatParameter( "damping (%)",  50,  0,100,1 ));

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

  bool need_recalc = true;

    float BUFFER[1024*1024];

    float slider1,slider2,slider3;

    uint32_t a0_pos, a1_pos, a2_pos, a3_pos, a4_pos;
    uint32_t a0r_pos, a1r_pos, a2r_pos, a3r_pos, a4r_pos;
    float tmp, tmpr, t, tr;

    float in, out, g, c;
    float dry, wet;

    float   a0_g,    a1_g,    a2_g,    a3_g,    a4_g;

    /*float*/ uint32_t   a0,    a1,    a2,    a3,    a4;
    /*float*/ uint32_t   a0r,   a1r,   a2r,   a3r,   a4r;

    uint32_t  a0_len,  a1_len,  a2_len,  a3_len,  a4_len;
    float   a0_in,   a1_in,   a2_in,   a3_in,   a4_in;
    float   a0_out,  a1_out,  a2_out,  a3_out,  a4_out;

    uint32_t  a0r_len, a1r_len, a2r_len, a3r_len, a4r_len;
    float   a0r_in,  a1r_in,  a2r_in,  a3r_in,  a4r_in;
    float   a0r_out, a1r_out, a2r_out, a3r_out, a4r_out;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
      a0_pos = 0;
      a1_pos = 0;
      a2_pos = 0;
      a3_pos = 0;
      a4_pos = 0;
      a0r_pos = 0;
      a1r_pos = 0;
      a2r_pos = 0;
      a3r_pos = 0;
      a4r_pos = 0;
      tmp = 0;
      tmpr = 0;
      t = 0;
      tr = 0;
  }

  virtual ~myInstance() {
    //KODE_TRACE;
  }

public:

  void recalc(float srate) {
    wet = exp(slider2/8.65617);
    dry = exp(slider1/8.65617);
    g = 1-slider3/100;
    a0 = 0;
    a0_len = (srate*35/1000);
    a0_g = 0.3;
    a1 = a0+a0_len+1;
    a1_len = (srate*22/1000);
    a1_g = 0.4;
    a2 = a1+a1_len+1;
    a2_len = (srate*8.3/1000);
    a2_g = 0.6;
    a3 = a2+a2_len+1;
    a3_len = (srate*66/1000);
    a3_g = 0.1;
    a4 = a3+a3_len+1;
    a4_len = (srate*30/1000);
    a4_g = 0.4;
    // scatter second channels delay length's to give stereo spread
    uint32_t rndcoef = 50;
    a0r = a4+a4_len+1;
    a0r_len = ((srate*35/1000))+rndcoef;
    a1r = a0r+a0r_len+1;
    a1r_len = ((srate*22/1000))-rndcoef;
    a2r = a1r+a1r_len+1;
    a2r_len = ((srate*8.3/1000))+rndcoef;
    a3r = a2r+a2r_len+1;
    a3r_len = ((srate*66/1000))-rndcoef;
    a4r = a3r+a3r_len+1;
    a4r_len = ((srate*30/1000))+rndcoef;
    // the lowpass isn't specified any further so we use a simple RC filter
    c = exp(-2*3.14*4200/srate);
  }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch(AIndex) {
      case 0: slider1 = AValue; break;
      case 1: slider2 = AValue; break;
      case 2: slider3 = AValue; break;
    }
    need_recalc = true;
  }

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    if (need_recalc) {
      need_recalc = false;
      recalc(AContext->samplerate);
    }

    uint32_t num_samples = AContext->numsamples;
    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];
    for (uint32_t i=0; i<num_samples; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;

      float spl0 = *input0++;
      float spl1 = *input1++;

      // left channel

      in = spl0 + tmp*g;
      // ---- AP 1
      a0_in = in;
      a0_out = -a0_in*a0_g + a2_out;
      a1_in = a0_in + a0_out*a0_g;
      // -> nested
        a1_out = -a1_in*a1_g + BUFFER[a1+a1_pos];//a1[a1_pos];
        /*a1[a1_pos]*/ BUFFER[a1+a1_pos] = a1_in + a1_out*a1_g;
        a1_pos += 1;
        if (a1_pos >= a1_len) a1_pos = 0;
        a2_in = a1_out;
        a2_out = -a2_in*a2_g + BUFFER[a2+a2_pos]; //a2[a2_pos];
        /*a2[a2_pos]*/ BUFFER[a2+a2_pos] = a2_in + a2_out*a2_g;
        a2_pos+=1;
        if (a2_pos >= a2_len) a2_pos = 0;
        // delay
        /*a0[a0_pos]*/ BUFFER[a0+a0_pos] = a2_out;
        a0_pos+=1;
        if (a0_pos >= a0_len) a0_pos = 0;
        a2_out = BUFFER[a0+a0_pos]; //a0[a0_pos];
      // <-
      // ---- end AP 1
      // ---- AP 2
      a3_in = a0_out;
      a3_out = -a3_in*a3_g + a4_out;
      a4_in = a3_in + a3_out*a3_g;
      // -> nested
        a4_out = -a4_in*a4_g + BUFFER[a4+a4_pos]; //a4[a4_pos];
        /*a4[a4_pos]*/ BUFFER[a4+a4_pos] = a4_in + a4_out*a4_g;
        a4_pos+=1;
        if (a4_pos >= a4_len) a4_pos = 0;
        // delay
        /*a3[a3_pos]*/ BUFFER[a3+a3_pos] = a4_out;
        a3_pos+=1;
        if (a3_pos >= a3_len) a3_pos = 0;
        a4_out = BUFFER[a3+a3_pos]; //a3[a3_pos];
      // <-
      // ---- end AP 2
      tmp = a3_out;
      tmp = (t = tmp + c*(t-tmp));
      out = a0_out*0.5 + a3_out*0.5;
      spl0 = spl0*dry + out*wet;

      // right channel

      in = spl1 + tmpr*g;
      // ---- AP 1
      a0r_in = in;
      a0r_out = -a0r_in*a0_g + a2r_out;
      a1r_in = a0r_in + a0r_out*a0_g;
      // -> nested
        a1r_out = -a1r_in*a1_g + BUFFER[a1r+a1r_pos]; //a1r[a1r_pos];
        /*a1r[a1r_pos]*/ BUFFER[a1r+a1r_pos] = a1r_in + a1r_out*a1_g;
        a1r_pos+=1;
        if (a1r_pos >= a1r_len) a1r_pos = 0;
        a2r_in = a1r_out;
        a2r_out = -a2r_in*a2_g + BUFFER[a2r+a2r_pos]; //a2r[a2r_pos];
        /*a2r[a2r_pos]*/ BUFFER[a2r+a2r_pos] = a2r_in + a2r_out*a2_g;
        a2r_pos+=1;
        if (a2r_pos >= a2r_len) a2r_pos = 0;
        // delay
        /*a0r[a0r_pos]*/ BUFFER[a0r+a0r_pos] = a2r_out;
        a0r_pos+=1;
        if (a0r_pos >= a0r_len) a0r_pos = 0;
        a2r_out = BUFFER[a0r+a0r_pos]; //a0r[a0r_pos];
      // <-
      // AP 2
      a3r_in = a0r_out;
      a3r_out = -a3r_in*a3_g + a4r_out;
      a4r_in = a3r_in + a3r_out*a3_g;
      // -> nested
        a4r_out = -a4r_in*a4_g + BUFFER[a4r+a4r_pos]; //a4r[a4r_pos];
        /*a4r[a4r_pos]*/ BUFFER[a4r+a4r_pos] = a4r_in + a4r_out*a4_g;
        a4r_pos+=1;
        if (a4r_pos >= a4r_len) a4r_pos = 0;
        // delay
        /*a3r[a3r_pos]*/ BUFFER[a3r+a3r_pos] = a4r_out;
        a3r_pos+=1;
        if (a3r_pos >= a3r_len) a3r_pos = 0;
        a4r_out = BUFFER[a3r+a3r_pos]; //a3r[a3r_pos];
      // <-
      tmpr = a3r_out;
      tmpr = (tr = tmpr + c*(tr-tmpr));
      out = a0r_out*0.5 + a3r_out*0.5;
      spl1 = spl1*dry + out*wet;


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
