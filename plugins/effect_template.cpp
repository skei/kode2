
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
      setName("plugin_debug");
    #else
      setName("plugin");
    #endif
    setAuthor("author");
    setVersion(0x00000001);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    //KODE_Parameter* parameter;
    //parameter = appendParameter( new KODE_Parameter("param1",0.2f) );
    //parameter = appendParameter( new KODE_Parameter("param2",0.7f) );
    //parameter->setLabel("db");
    //parameter = appendParameter( new KODE_Parameter("param3",0.4f) );
    //parameter->setLabel("%");

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
      //processSample(AContext,spl0,spl1);
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
