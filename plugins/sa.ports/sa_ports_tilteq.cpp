
#define KODE_NO_GUI

#include "base/kode.h"
#include "base/utils/kode_math.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

// (C) lubomir i. ivanov
// used with permission

// cat /proc/sys/kernel/random/uuid
// fc334b02-9a3d-467c-930a-5a113b624c65
uint8_t myPluginId[16]  = {0xfc,0x33,0x4b,0x02,0x9a,0x3d,0x46,0x7c,0x93,0x0a,0x5a,0x11,0x3b,0x62,0x4c,0x65};

const char* str_proc[2] = {"stereo","mono" };

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName       = "sa.ports.tilteq";
    MAuthor     = "skei.audio";
    MLongId = myPluginId;

    appendParameter( KODE_New KODE_TextParameter(  "processing",   0,  2,  str_proc  ));
    appendParameter( KODE_New KODE_FloatParameter( "center freq",  50, 0,  100, 0.25 ));
    appendParameter( KODE_New KODE_FloatParameter( "tilt (lo/hi)", 0, -6,  6         ));
    appendParameter( KODE_New KODE_FloatParameter( "gain",         0, -25, 25        ));

  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  float amp, sr3;
  float outgain, lgain, hgain;
  uint32_t mono;
  float lp_out;
  float lp_out_r;
  float a0, b1;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
      amp = 6/log(2); //Ln
      a0 = 0;
      b1 = 0;
      lp_out = 0;
      lp_out_r = 0;
  }

public:

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    //p : ZParameter;
    //v : Single;
    float sx,f0,omega,n,gain,gfactor,g1,g2;
    //p := FParameters[AIndex];
    //v := p.from01(AValue);
    switch(AIndex) {
      case 0: // slider1
        mono = KODE_Trunc(AValue);
        break;
      case 1: // slider2
        sx = 16+AValue*1.20103;
        f0 = floor(exp(sx*log(1.059))*8.17742);
        //f0 := trunc(Exp(sx*ln(1.059))*8.17742); // log(x), log2? orig had Logf
        omega = 2*KODE_PI*f0;
        n = 1/(sr3 + omega); // sr3 = 0 in constructor...
        a0 = 2*omega*n;
        b1 = (sr3 - omega)*n;
        break;
      case 2: // slider3
        gain = AValue;
        gfactor = 4;
        if (gain>0) {
          g1 = -gfactor*gain;
          g2 = gain;
        }
        else {
          g1 = -gain;
          g2 = gfactor*gain;
        }
        lgain = exp(g1/amp)-1;
        hgain = exp(g2/amp)-1;
        break;
      case 3: // slider4
        outgain = exp(AValue/amp);
        break;
    }
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {

    sr3 = 3 * AContext->samplerate;

    uint32_t len = AContext->numsamples;
    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;

      float inp,outp, inp_r,outp_r;
      float spl0 = *input0++;//+ DENORM;
      float spl1 = *input1++;//+ DENORM;
      if (mono==1) {
        //process mono
        inp = (spl0+spl1)*0.5;
        lp_out = a0*inp + b1*lp_out;
        outp = inp + lgain*lp_out + hgain*(inp - lp_out);
        spl0 = outp*outgain;
        spl1 = outp*outgain;
      }
      else {
        //process stereo
        inp = spl0;
        lp_out = a0*inp + b1*lp_out;
        outp = inp + lgain*lp_out + hgain*(inp - lp_out);
        spl0 = outp*outgain;

        inp_r = spl1;
        lp_out_r = a0*inp_r + b1*lp_out_r;
        outp_r = inp_r + lgain*lp_out_r + hgain*(inp_r - lp_out_r);
        spl1 = outp_r*outgain;
      }
      *output0++ = spl0 ;//- DENORM;
      *output1++ = spl1 ;//- DENORM;


    }
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
