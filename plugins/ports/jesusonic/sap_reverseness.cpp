
#define KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

// 64mb
#define BUFFER_SIZE (1024*1024*64)

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    //KODE_TRACE;

    MName       = "sap_reverseness";
    MAuthor     = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

    appendParameter( KODE_New KODE_FloatParameter( "length",   500,  0,   4000 ));
    appendParameter( KODE_New KODE_FloatParameter( "wet mix",  0,   -120, 6    ));
    appendParameter( KODE_New KODE_FloatParameter( "dry mix", -120, -120, 6    ));
    appendParameter( KODE_New KODE_FloatParameter( "overlap",  0.1,  0,   0.5  ));

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

  float     buffer[BUFFER_SIZE];
  bool      init = true;
  uint32_t  pos;
  float     len,wet,dry,fadelen,halflen;
  float     slider1,slider2,slider3,slider4;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
  }

  virtual ~myInstance() {
    //KODE_TRACE;
  }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch(AIndex) {
      case 0: slider1 = AValue; break;
      case 1: slider2 = AValue; break;
      case 2: slider3 = AValue; break;
      case 3: slider4 = AValue; break;
    }
    init = true;
  }

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    if (init) {
      //len = srate * (60 / tempo * slider1);
      len = AContext->samplerate * slider1/1000;
      wet = pow( 2, (slider2/6) );
      dry = pow( 2, (slider3/6) );
      fadelen = len * slider4 * 0.5;
      //freembuf(len * 2);
      halflen = len * 0.5;
      init = false;
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

        if (pos>=len) pos = 0;

        uint32_t p2 = ((len - pos) * 2);
        //s0 = ((len - pos) * 2)[0];
        //s1 = ((len - pos) * 2)[1];
        //(pos * 2)[0] = spl0;
        //(pos * 2)[1] = spl1;
        float s0 = buffer[p2];
        float s1 = buffer[p2+1];
        buffer[p2] = spl0;
        buffer[p2+1] = spl1;

        float pp = len - pos;

        //float w2 = pp < fadelen ? (pp/fadelen) : ( pp > len-fadelen ? (len-pp) / fadelen : 1   );
        float w2;
        if (pp < fadelen) w2 = (pp/fadelen);
        else if (pp > len-fadelen) w2 = (len-pp) / fadelen;
        else w2 = 1.0;

        if (w2 >= 1.0) {
          pp = abs(halflen - pos);
          w2 = pp < fadelen ? pp / fadelen : 1;
        }
        spl0 = s0 * wet * w2 + spl0 * (dry + 1 - w2);
        spl1 = s1 * wet * w2 + spl1 * (dry + 1 - w2);
        //(pos += 1) >= len ? pos = 0;
        pos += 1;

//        if (pos>=len) pos = 0;

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




#if 0

// This effect Copyright (C) 2004 and later Cockos Incorporated
// License: GPL - http://www.gnu.org/licenses/gpl.html

desc: reverseness X sync (reverses chunks)
slider1:4<0,128,1>length (beats)
slider2:0<-120,6,1>wet mix (dB)
slider3:-120<-120,6,1>dry mix (dB)
slider4:0.1<0,0.5>edge overlap (0..0.5)

@slider
  init = 0;

@block
!init ?
(
  len = srate * (60 / tempo * slider1);
  wet = 2 ^ (slider2 / 6);
  dry = 2 ^ (slider3 / 6);
  fadelen = len * slider4 * 0.5;
  freembuf(len * 2);
  halflen = len * 0.5;
  init = 1;
);

@sample

s0 = ((len - pos) * 2)[0];
s1 = ((len - pos) * 2)[1];
(pos * 2)[0] = spl0;
(pos * 2)[1] = spl1;

pp = len - pos;
w2 = pp < fadelen ? pp / fadelen : (pp > len - fadelen ? (len - pp) / fadelen : 1);
w2 == 1 ?
(
  pp = abs(halflen - pos);
  w2 = pp < fadelen ? pp / fadelen : 1;
);

spl0 = s0 * wet * w2 + spl0 * (dry + 1 - w2);
spl1 = s1 * wet * w2 + spl1 * (dry + 1 - w2);

(pos += 1) >= len ? pos = 0;

#endif // 0

