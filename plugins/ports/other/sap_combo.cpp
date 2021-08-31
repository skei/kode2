
// BUGGY

#define KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

#define BUFFER_SIZE 10000

const char* model_txt[7] = {
  "DI",         // DI
  "Spkr Sim",   // Speaker sim
  "Radio",      // Radio
  "MB 1\"",     // Mesa Boogie 1"
  "MB 8\"",     // Mesa Boogie 8"
  "4x12 ^",     // Marshall 4x12" celestion
  "4x12 >"      // scooped-out metal
};

const char* process_txt[2] = {
  "MONO",
  "STEREO"
};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : KODE_Descriptor() {
    setName("sap_combo");
    setAuthor("skei.audio");
    setVersion(0x00000001);
    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );
    appendParameter( new KODE_TextParameter(  "Model",    0,   7,   model_txt ));
    appendParameter( new KODE_FloatParameter( "Drive",    0,  -100, 100, 1 ));
    appendParameter( new KODE_FloatParameter( "Bias",     0,  -100, 100, 1 ));
    appendParameter( new KODE_FloatParameter( "Output",   0,  -20,  20,  1 ));
    appendParameter( new KODE_TextParameter(  "Process",  0,   2,   process_txt ));
    appendParameter( new KODE_FloatParameter( "HPF Freq", 0,   0,   100, 1 ));
    appendParameter( new KODE_FloatParameter( "HPF Reso", 50,  0,   100, 1 ));
  }

  //----------

  virtual ~myDescriptor() {
    //KODE_TRACE;
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

//------------------------------
private:
//------------------------------

  bool need_recalc = true;

  float   fParam1 = 0.0f;
  float   fParam2 = 0.0f;
  float   fParam3 = 0.0f;
  float   fParam4 = 0.0f;
  float   fParam5 = 0.0f;
  float   fParam6 = 0.0f;
  float   fParam7 = 0.0f;
  float   clip = 0.0f;
  float   drive = 0.0f;
  float   trim = 0.0f;
  float   lpf = 0.0f;
  float   hpf = 0.0f;
  float   mix1 = 0.0f;
  float   mix2 = 0.0f;
  float   ff1 = 0.0f;
  float   ff2 = 0.0f;
  float   ff3 = 0.0f;
  float   ff4 = 0.0f;
  float   ff5 = 0.0f;
  float   bias = 0.0f;
  float   ff6 = 0.0f;
  float   ff7 = 0.0f;
  float   ff8 = 0.0f;
  float   ff9 = 0.0f;
  float   ff10 = 0.0f;
  float   hhf = 0.0f;
  float   hhq = 0.0f;
  float   hh0 = 0.0f;
  float   hh1 = 0.0f;
  float   buffer[BUFFER_SIZE] = {0};
  float   buffer2[BUFFER_SIZE] = {0};
  int32_t size = 0;
  int32_t bufpos = 0;
  int32_t del1 = 0;
  int32_t del2 = 0;
  int     mode = 0;
  int     ster = 0;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //fParam1 = 1.00f; // select
    //fParam2 = 0.50f; // drive
    //fParam3 = 0.50f; // bias
    //fParam4 = 0.50f; // output
    //fParam5 = 0.40f; // stereo
    //fParam6 = 0.00f; // hpf freq
    //fParam7 = 0.50f; // hpf reso
    bufpos  = 0;
    init();
  }

  virtual ~myInstance() {
  }

//------------------------------
private:
//------------------------------

  void init(void) {
    memset(buffer,  0, BUFFER_SIZE * sizeof(float));
    memset(buffer2, 0, BUFFER_SIZE * sizeof(float));
    ff1  = 0.0f;
    ff2  = 0.0f;
    ff3  = 0.0f;
    ff4  = 0.0f;
    ff5  = 0.0f;
    ff6  = 0.0f;
    ff7  = 0.0f;
    ff8  = 0.0f;
    ff9  = 0.0f;
    ff10 = 0.0f;
    hh0  = 0.0f;
    hh1  = 0.0f;
  }

  //----------

  float filterFreq(float hz, float srate) {
    float j;
    float k;
    float r = 0.999f;
    j = r * r - 1;
    k = (float)(2.f - 2.f * r * r * cosf(0.647f * hz / srate ));
    return (float)((sqrtf(k*k - 4.f*j*j) - k) / (2.f*j));
  }

  //----------

  void recalc(float srate) {
    ster = 0;
    if (fParam5 > 0.5) ster = 1;
    hpf = filterFreq(25.0f,srate);
    switch (int(fParam1 * 6.9)) {
      case 0: // DI
        trim = 0.5f;
        lpf  = 0.0f;
        mix1 = (float)0.0;
        mix2 = (float)0.0;
        del1 = 0;
        del2 = 0;
        break;
      case 1: // speaker sim
        trim = 0.53f;
        lpf  = filterFreq(2700.f,srate);
        mix1 = (float)0.0;
        mix2 = (float)0.0;
        del1 = 0;
        del2 = 0;
        hpf = filterFreq(382.f,srate);
        break;
      case 2: // radio
        trim = 1.10f;
        lpf  = filterFreq(1685.f,srate);
        mix1 = -1.70f;
        mix2 = 0.82f;
        del1 = int(srate / 6546.f);
        del2 = int(srate / 4315.f);
        break;
      case 3: // mesa boogie 1"
        trim = 0.98f;
        lpf  = filterFreq(1385.f,srate);
        mix1 = -0.53f;
        mix2 = 0.21f;
        del1 = int(srate / 7345.f);
        del2 = int(srate / 1193.f);
        break;
      case 4: // mesa boogie 8"
        trim = 0.96f;
        lpf  = filterFreq(1685.f,srate);
        mix1 = -0.85f;
        mix2 = 0.41f;
        del1 = int(srate / 6546.f);
        del2 = int(srate / 3315.f);
        break;
      case 5: // Marshall 4x12" celestion
        trim = 0.59f;
        lpf  = filterFreq(2795.f,srate);
        mix1 = -0.29f;
        mix2 = 0.38f;
        del1 = int(srate / 982.f);
        del2 = int(srate / 2402.f);
        hpf  = filterFreq(459.f,srate);
        break;
      case 6: // scooped-out metal
        trim = 0.30f;
        lpf  = filterFreq(1744.f,srate);
        mix1 = -0.96f;
        mix2 = 1.6f;
        del1 = int(srate / 356.f);
        del2 = int(srate / 1263.f);
        hpf  = filterFreq(382.f,srate);
        break;
    }
    mode = (fParam2<0.5)? 1 : 0;
    if (mode) {
      // soft clipping
      drive = (float)pow(10.f, 2.f - 6.f * fParam2);
      trim *= 0.55f + 150.f * (float)pow(fParam2,4.0f);
    }
    else {
      // hard clipping
      drive = 1.f;
      clip = 11.7f - 16.f*fParam2;
      if(fParam2>0.7) {
        drive = (float)pow(10.0f, 7.f*fParam2 - 4.9f);
        clip = 0.5f;
      }
    }
    bias = 1.2f * fParam3 - 0.6f;
    if(fParam2>0.5) bias /= (1.f + 3.f * (fParam2-0.5f));
               else bias /= (1.f + 3.f * (0.5f-fParam2));
    trim *= (float)pow(10.f, 2.f*fParam4 - 1.f);
    if (ster) trim *=2.f;
    hhf = fParam6;
    hhq = 1.1f - fParam7;
    if (fParam6>0.05f) drive = drive * (1 + 0.1f * drive);
  }

//------------------------------
public:
//------------------------------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
      switch(AIndex) {
        case 0: fParam1 = AValue; break;
        case 1: fParam2 = AValue; break;
        case 2: fParam3 = AValue; break;
        case 3: fParam4 = AValue; break;
        case 4: fParam5 = AValue; break;
        case 5: fParam6 = AValue; break;
        case 6: fParam7 = AValue; break;
      }
      //recalc();
      need_recalc = true;
  }

  //----------

  // in1/in2 -> in0/in1

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    if (need_recalc) {
      recalc(AContext->samplerate);
      need_recalc = false;
    }

    //uint32_t len = AContext->numSamples;

    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];

    uint32_t num_samples = AContext->numsamples;

    float   a   = 0.0f;
    float   b   = 0.0f;
    float   trm = 0.0f;
    float   m1  = mix1;
    float   m2  = mix2;
    float   clp = clip;
    float   o   = lpf;
    float   i   = 1.0f - lpf;
    float   o2  = hpf;
    float   bi  = bias;
    float   drv = drive;
    float   f1  = ff1;
    float   f2  = ff2;
    float   f3  = ff3;
    float   f4  = ff4;
    float   f5  = ff5;
    float   a2  = 0.0f;
    float   b2  = 0.0f;
    float   f6  = ff6;
    float   f7  = ff7;
    float   f8  = ff8;
    float   f9  = ff9;
    float   f10 = ff10;
    float   hf  = hhf;
    float   hq  = hhq;
    float   h0  = hh0;
    float   h1  = hh1;
    int32_t d1  = del1;
    int32_t d2  = del2;
    int32_t bp  = bufpos;

    //VstInt32 d1=del1, d2=del2, bp = bufpos;

    trm = trim * i * i * i * i;

    //--in1;
    //--in2;
    //--out1;
    //--out2;

    b = 0;
    if (ster) { //stereo
      //while (--ANumSamples >= 0) {
      while (num_samples > 0) {
        num_samples -= 1;
        a  = drv * (*in0++ + bi);
        a2 = drv * (*in1++ + bi);
        if (mode) {
          b = (a > 0.0f) ? a : -a;
          b = a / (1.0f + b);
          b2 = (a2 > 0.f) ? a2 : -a2;
          b2 = a2 / (1.0f + b2);
        }
        else {
          b = (a >  clp) ?  clp : a;
          b = (a < -clp) ? -clp : b;
          b2 = (a2 >  clp) ?  clp : a2; // distort
          b2 = (a2 < -clp) ? -clp : b2;
        }
        *(buffer  + bp) = b;
        *(buffer2 + bp) = b2;
        b += (m1* *(buffer  + ((bp + d1) % 1000))) + (m2* *(buffer  + ((bp + d2) % 1000)));
        b2+= (m1* *(buffer2 + ((bp + d1) % 1000))) + (m2* *(buffer2 + ((bp + d2) % 1000)));
        f1 = o * f1 + trm * b;
        f2 = o * f2 + f1;
        f3 = o * f3 + f2;
        f4 = o * f4 + f3;
        f5 = o2 * (f5 - f4) + f4;
        f6 = o * f6 + trm * b2;
        f7 = o * f7 + f6;
        f8 = o * f8 + f7;
        f9 = o * f9 + f8; // -24dB/oct filter
        f10 = o2 * (f10 - f9) + f9; // high pass
        b  = f4 - f5;
        b2 = f9 - f10;
        if (bp == 0) bufpos = 999;
        else bufpos = bp - 1;
        *out0++ = b;
        *out1++ = b2;
      }
    } // stereo
    else { // mono
      if (mode) { // soft clip
        //while (--ANumSamples >= 0) {
        while (num_samples > 0) {
          num_samples -= 1;
          a = drv * (*in0++ + *in1++ + bi);
          h0 += hf * (h1 + a); // resonant highpass (Chamberlin SVF)
          h1 -= hf * (h0 + hq * h1);
          a += h1;
          b = (a > 0.f) ? a : -a;
          b = a / (1.0f + b);
          *(buffer + bp) = b;
          b += (m1* *(buffer + ((bp + d1) % 1000)))
             + (m2* *(buffer + ((bp + d2) % 1000)));
          f1 = o * f1 + trm * b;
          f2 = o * f2 + f1;
          f3 = o * f3 + f2;
          f4 = o * f4 + f3; // -24dB/oct filter
          f5 = o2 * (f5 - f4) + f4; //high pass
          b = f4 - f5;
          bp = (bp == 0) ? 999 : bp - 1; // buffer position
          *out0++ = b;
          *out1++ = b;
        }
      } // soft-clip
      else { //hard clip
        //while(--ANumSamples >= 0) {
        while (num_samples > 0) {
          num_samples -= 1;
          a = drv * (*in0++ + *in1++ + bi);
          h0 += hf * (h1 + a); //resonant highpass (Chamberlin SVF)
          h1 -= hf * (h0 + hq * h1);
          a += h1;
          b = (a >  clp) ?  clp : a; //distort
          b = (a < -clp) ? -clp : b;
          *(buffer + bp) = b;
          b += (m1* *(buffer + ((bp + d1) % 1000)))
             + (m2* *(buffer + ((bp + d2) % 1000)));
          f1 = o * f1 + trm * b;
          f2 = o * f2 + f1;
          f3 = o * f3 + f2;
          f4 = o * f4 + f3; //-24dB/oct filter
          f5 = o2 * (f5 - f4) + f4; //high pass //also want smile curve here...
          b = f4 - f5;
          bp = (bp == 0) ? 999 : bp - 1; //buffer position
          *out0++ = b;
          *out1++ = b;
        }
      } // hard-clip
    } // mono
    bufpos = bp;
    if (fabs(f1) < 1.0e-10) { ff1 = 0.0f; ff2 = 0.0f; ff3 = 0.0f; ff4 = 0.0f; ff5  = 0.0f;  }
                       else { ff1 = f1;   ff2 = f2;   ff3 = f3;   ff4 = f4;   ff5  = f5;    }
    if (fabs(f6) < 1.0e-10) { ff6 = 0.0f; ff7 = 0.0f; ff8 = 0.0f; ff9 = 0.0f; ff10 = 0.0f;  }
                       else { ff6 = f6;   ff7 = f7;   ff8 = f8;   ff9 = f9;   ff10 = f10;   }
    if (fabs(h0) < 1.0e-10) { hh0 = hh1 = 0.0f; }
                       else { hh0 = h0; hh1 = h1; }

  }

  //----------

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
