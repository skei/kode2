
#define KODE_NO_GUI
#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET
//#define KODE_DEBUG_VST3

//----------

#include "kode.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_parameters.h"

//

// 20 hz = 44100/20 = 2205
#define KODE_MAX_SAMPLES 4096

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {

    #ifdef KODE_DEBUG
      setName("sa_frac_delay_debug");
    #else
      setName("sa_frac_delay");
    #endif
    setAuthor("skei.audio");
    setVersion(0x00000001);
    setCanReceiveMidi();

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    KODE_Parameter* par;
    par = appendParameter( new KODE_FloatParameter( "delay",     100, 0, KODE_MAX_SAMPLES ));
    par->setLabel("spl");
    par = appendParameter( new KODE_FloatParameter( "feedback",  0.9, 0, 0.99 ));
    par = appendParameter( new KODE_FloatParameter( "wet",      -6, -60, 0 ));
    par->setLabel("db");
    par = appendParameter( new KODE_FloatParameter( "dry",      -6, -60, 0 ));
    par->setLabel("db");

  }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  float   buf[KODE_MAX_SAMPLES*2*2];
  int32_t pos   = 0;
  float   size  = 0.0f;
  float   frac  = 0.0f;
  float   z0    = 0.0f;
  float   z00   = 0.0f;
  float   z01   = 0.0f;

  float   dly   = 0.0f;
  float   fbk   = 0.0f;
  float   wet   = 0.0f;
  float   dry   = 0.0f;

  float sample_rate = 0.0f;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

//------------------------------
public:
//------------------------------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    sample_rate = ASamplerate;
  }

  //----------

  void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
    uint32_t m = AMsg1 & 0xF0/*240*/;
    if (m == 0x90/*(9*16)*/) {
      uint32_t note = AMsg2 & 127;
      float freq = 440.0 * powf(2,(note-69.0)/12.0);
      //uint32 vel = AMsg3 & 127;
      //wet = vel / 127;
      dly = sample_rate / freq;
      //slider1 := dly;
      //slider3 := wet;
      //sliderchange(slider1+slider3);
      size = floorf(dly);
      frac = dly - size;
      pos  = 0;
      z0   = 0;
    }
  }

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch (AIndex) {
      case 0: dly = AValue; size = floorf(dly); frac = dly-size; break;
      case 1: fbk = AValue;
      case 2: wet = powf(2.0f,AValue / 6.0f);
      case 3: dry = powf(2.0f,AValue / 6.0f);
    }
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    uint32_t numsamples = AContext->numsamples;
    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];

    for (uint32_t i=0; i<numsamples; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;

      //processSample(AContext,spl0,spl1);

      int32_t pos2 = pos*2;
      float   in0 = spl0;
      float   in1 = spl1;
      float   d00 = buf[pos2];
      float   d01 = buf[pos2+1];
      float   i00,i01;

      if (dly < 1) {
        i00 = (in0 * (1 - frac)) + (d00 * frac);
        i01 = (in1 * (1 - frac)) + (d01 * frac);
      } else {
        i00 = (d00 * (1 - frac)) + (z00 * frac);
        i01 = (d01 * (1 - frac)) + (z01 * frac);
      }

      buf[pos2]   = in0 + (fbk * i00);
      buf[pos2+1] = in1 + (fbk * i01);

      z00 = d00;
      z01 = d01;

      pos += 1;
      if (pos>=size) pos = 0; //-=size

      spl0 = (in0 * dry) + (i00 * wet);
      spl1 = (in1 * dry) + (i01 * wet);

      *output0++ = spl0;
      *output1++ = spl1;
    }
  }

  //----------

};

#undef KODE_MAX_SAMPLES

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
