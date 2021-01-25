
#define KODE_NO_GUI

#include "base/utils/kode_math.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

/*
  based on the 'saturation' jesusonic effect by tale/loser
  http://forum.cockos.com/showpost.php?p=1537127&postcount=3
  see bottom of this file for original
*/

#define scale ( 1.1 / 3.0 )
const char* txt_func[2] = { "sine", "cubic" };

// cat /proc/sys/kernel/random/uuid
// f00a28ae-715c-40c5-b2de-783c93fbe966
uint8_t myPluginId[16]  = {0xf0,0x0a,0x28,0xae,0x71,0x5c,0x40,0xc5,0xb2,0xde,0x78,0x3c,0x93,0xfb,0xe9,0x66};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName       = "sa.ports.saturation";
    MAuthor     = "skei.audio";
    MLongId = myPluginId;

    appendParameter( KODE_New KODE_Parameter(     "saturation", 0 ));
    appendParameter( KODE_New KODE_IntParameter(  "stages",     1, 1, 10 ));
    appendParameter( KODE_New KODE_TextParameter( "function",   1, 2, txt_func ));
  }

  //----------

  virtual ~myDescriptor() {
    //KODE_TRACE;
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  float     p_saturation;
  uint32_t  p_stages;
  uint32_t  p_function;
  float     a, div, gain;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

private:

  void recalc(void) {
    a = KODE_Max(0, KODE_Min(1, p_saturation));
    if (a > 0) {
      gain = 1 - (1 - 1/(float)p_stages) * a;
      float f = 1.0f;
      switch(p_function) {
        case 0:
          a *= KODE_PI05;
          f = sin(a);
          break;
        case 1:
          f = a - a*a*a * scale;
          break;
      }
      div = 1 / f;
    }
  }

public:

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    switch(AIndex) {
      case 0: p_saturation = AValue; break;
      case 1: p_stages = AValue;     break;
      case 2: p_function = AValue;   break;
    }
    recalc();
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {
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
        //----------
        if (a>0) {
          float x0 = KODE_Max(-1, KODE_Min(1, spl0));
          float x1 = KODE_Max(-1, KODE_Min(1, spl1));
          switch(p_function) {
            case 0:
              for (uint32_t j=0; j<p_stages; j++) {
                x0 = sin(x0 * a) * div;
                x1 = sin(x1 * a) * div;
              }
              break;
            case 1:
              for (uint32_t j=0; j<p_stages; j++) {
                x0 *= a; x0 = (x0 - x0*x0*x0 * scale) * div;
                x1 *= a; x1 = (x1 - x1*x1*x1 * scale) * div;
              }
              break;
          }
          spl0 = x0 * gain;
          spl1 = x1 * gain;
        } // a>0
        //----------
        *output0++ = spl0;
        *output1++ = spl1;


    }
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)





#if 0

desc:Another loser/Saturation rewrite, thanks to SaulT!
slider1:0<0,1,0.01>Saturation
slider2:1<1,10,1>Stages
slider3:1<0,1,1{Sine,Cubic}>Function

// Sine:  f(x) = sin(x * a * 0.5 * pi)/sin(a * 0.5 * pi)
// Cubic: f(x) = ((x*a)-((x*a)^3)/3)/(a-(a^3)/3)

@init

scale = 1.1 / 3;

@slider

a = max(0, min(1, slider1));
a > 0 ? (
  n = max(1, floor(slider2 + 0.5));
  f = slider3 < 0.5;

  gain = 1 - (1 - 1/n) * a;
  div = 1/(f ? sin(a *= 0.5*$pi) : a - a*a*a * scale);
);

@sample

a > 0 ? (
  x0 = max(-1, min(1, spl0));
  x1 = max(-1, min(1, spl1));

  f ? loop(n,
    x0 = sin(x0 * a) * div;
    x1 = sin(x1 * a) * div;
  ) : loop(n,
    x0 *= a; x0 = (x0 - x0*x0*x0 * scale) * div;
    x1 *= a; x1 = (x1 - x1*x1*x1 * scale) * div;
  );

  spl0 = x0 * gain;
  spl1 = x1 * gain;
);

#endif // 0

