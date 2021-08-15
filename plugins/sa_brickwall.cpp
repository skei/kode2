
#define KODE_NO_GUI

//#define KODE_DEBUG_PRINT_TIME
//#define KODE_DEBUG_PRINT_THREAD
//#define KODE_DEBUG_PRINT_SOCKET

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

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    appendParameter( KODE_New KODE_Parameter( "level", 1.0f ) );

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

  //bool MNeedRecalc = false;
  float MLimit  = 1.0f;
  float MGain   = 1.0f;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
public:
//------------------------------

  //void on_plugin_open() final {
  //}

  //----------

  //void on_plugin_close() final {
  //}

  //----------

  //void on_plugin_initialize() final {
  //}

  //----------

  //void on_plugin_terminate() final {
  //}

  //----------

  //void on_plugin_activate() final {
  //}

  //----------

  //void on_plugin_deactivate() final {
  //}

  //----------

  //void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
  //}

  //----------

  //void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
  //}

  //----------

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

  //----------

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);