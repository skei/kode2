
#define KODE_NO_GUI

//#define KODE_DEBUG_PRINT_TIME
//#define KODE_DEBUG_PRINT_THREAD
//#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_parameters.h"

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
      setName("sa_curve_shaper_debug");
    #else
      setName("sa_curve_shaper");
    #endif
    setAuthor("skei.audio");
    setVersion(0x00000001);

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    appendParameter( KODE_New KODE_FloatParameter( "curve", 0.5f ) );

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
  float MCurve = 0.5f;

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
    if (AIndex == 0) MCurve = AValue;
    //MNeedRecalc = true;
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    //if (MNeedRecalc) {
    //  MNeedRecalc = false;
    //  //recalc(AContext);
    //}
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      spl0 = KODE_Curve( fabs(spl0), MCurve ) * KODE_Sign(spl0);
      spl1 = KODE_Curve( fabs(spl1), MCurve ) * KODE_Sign(spl1);
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

