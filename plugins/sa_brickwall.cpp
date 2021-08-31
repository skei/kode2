
#define KODE_NO_GUI

#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"
#include "plugin/kode_plugin.h"

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
      setName("sa_brickwall_debug");
    #else
      setName("sa_brickwall");
    #endif
    setAuthor("skei.audio");
    setVersion(0x00000001);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    appendParameter( new KODE_Parameter( "level", 1.0f ) );

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

  float MLimit  = 1.0f;
  float MGain   = 1.0f;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

//------------------------------
public:
//------------------------------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    if (AIndex == 0) {
      float v = AValue;
      MLimit = v * v * v;
      if (v < 0.00001f) v = 0.00001f;
      MGain = 1.0f / v;
    }
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;

      spl0 = KODE_Clamp(spl0, -MLimit, MLimit) * MGain;
      spl1 = KODE_Clamp(spl1, -MLimit, MLimit) * MGain;

      *out0++ = spl0;
      *out1++ = spl1;
    }
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
