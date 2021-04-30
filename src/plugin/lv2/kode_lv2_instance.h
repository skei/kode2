#ifndef kode_lv2_instance_included
#define kode_lv2_instance_included
//----------------------------------------------------------------------

#include "plugin/kode_editor.h"
#include "plugin/kode_plugin_base.h"
#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_utils.h"

//----------------------------------------------------------------------

#define KODE_LV2_QUEUE_SIZE 1024
typedef KODE_Queue<uint32_t,KODE_LV2_QUEUE_SIZE> KODE_Lv2UpdateQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class KODE_Lv2Instance
: public KODE_IInstance {

//------------------------------
private:
//------------------------------

  KODE_Descriptor*          MDescriptor             = KODE_NULL;
  KODE_Editor*              MEditor                 = KODE_NULL;

  LV2_URID                  MMidiInputUrid          = 0;
  const LV2_Atom_Sequence*  MAtomSequence           = KODE_NULL;
  float                     MSampleRate             = 0.0f;

  float*                    MParameterValues        = KODE_NULL;
  //float*                    MEditorParameterValues  = KODE_NULL;
  //float*                    MHostParameterValues    = KODE_NULL;
  //KODE_Lv2UpdateQueue       MHostParameterQueue;

  uint32_t                  MNumInputs        = 0;
  uint32_t                  MNumOutputs       = 0;
  uint32_t                  MNumParameters    = 0;
  float**                   MInputPtrs        = nullptr;
  float**                   MOutputPtrs       = nullptr;
  float**                   MParameterPtrs    = nullptr;
  float*                    MHostValues       = nullptr;
  float*                    MProcessValues    = nullptr;

//------------------------------
public:
//------------------------------

  KODE_Lv2Instance(KODE_Descriptor* ADescriptor, float ASampleRate)
  : KODE_IInstance(ADescriptor) {
//    MRefCount = 1;
//    createParameterBuffers();
//    createParameterInfo();
    MDescriptor = ADescriptor;
    MSampleRate = ASampleRate;
    MNumInputs      = MDescriptor->getNumInputs();
    MNumOutputs     = MDescriptor->getNumOutputs();
    MNumParameters  = MDescriptor->getNumParameters();
    MInputPtrs      = (float**)malloc(MNumInputs     * sizeof(float*));
    MOutputPtrs     = (float**)malloc(MNumOutputs    * sizeof(float*));
    MParameterPtrs  = (float**)malloc(MNumParameters * sizeof(float*));
//    MHostValues     = (float*) malloc(MNumParameters * sizeof(float ));
//    MProcessValues  = (float*) malloc(MNumParameters * sizeof(float ));
//    //instance->on_open();
//    MInstance->on_initialize(); // open?


  }

  //----------

  virtual ~KODE_Lv2Instance() {
//    deleteParameterInfo();
//    destroyParameterBuffers();
    if (MInputPtrs)     free(MInputPtrs);
    if (MOutputPtrs)    free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);
    if (MHostValues)    free(MHostValues);
    if (MProcessValues) free(MProcessValues);
  }

//------------------------------
public:
//------------------------------

  KODE_Descriptor* getDescriptor() override {
    return MDescriptor;
  }

  //----------

  // (shouldn't be public)

  void _setMidiInputUrid(LV2_URID lv2_midi_input_urid) {
    MMidiInputUrid = lv2_midi_input_urid;
  }

//------------------------------
public:
//------------------------------

  void setDefaultParameterValues() override {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float value = parameter->getDefValue();
      MParameterValues[i] = value;
    }
  }

  //----------

  void updateAllParameters() override {
    //KODE_PRINT;
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float value = MParameterValues[i];
      float v = parameter->from01(value);
      on_plugin_parameter(0,i,v);
      // if editor is open ...
    }
  }

  //----------

  void updateAllEditorParameters(KODE_IEditor* AEditor, bool ARedraw=true) override {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      float value = MParameterValues[i];
      //KODE_Parameter* parameter = MDescriptor->getParameter(i);
      //float v = parameter->from01(value);
      float v = value;
      //on_plugin_parameter(0,i,v);
      AEditor->updateParameterFromHost(i,v,ARedraw);
    }
  }

  //----------

  void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
//    //MEditorParameterValues[AIndex] = AValue;
//    queueParameterToHost(AIndex,AValue);
  }

//----------------------------------------------------------------------
// lv2
//----------------------------------------------------------------------

  void lv2_connect_port(uint32_t port, void* data_location) {
    //LV2_Trace("lv2_connect_port: port %i data_location 0x%x\n",port,data_location);
    if (port < MNumInputs) {
      MInputPtrs[port] = (float*)data_location;
      return;
    }
    port -= MNumInputs;
    if (port < MNumOutputs) {
      MOutputPtrs[port] = (float*)data_location;
      return;
    }
    port -= MNumOutputs;
    if (port < MNumParameters) {
      MParameterPtrs[port] = (float*)data_location;
      return;
    }
    port -= MNumParameters;
    if (MDescriptor->canReceiveMidi()) {
      MAtomSequence = (const LV2_Atom_Sequence*)data_location;
      port -= 1;
    }
  }

  //----------

  void lv2_activate() {
    //KODE_Trace("lv2_activate\n");
//    on_start();
//    on_activate();
  }

  //----------

  void lv2_run(uint32_t sample_count) {
    //KODE_Trace("lv2_run\n");
    // parameters
    for (uint32_t i=0; i<MNumParameters; i++) {
      float v = *MParameterPtrs[i];
      MHostValues[i] = v;
      if (v != MProcessValues[i]) {
        MProcessValues[i] = v;
        // to/from01 ??
        //KODE_Parameter* param = MPlugin->getParameter(i);
        //if (param) v = param->from01(v);
        on_plugin_parameter(0,i,v);
      }
    }
    // midi
    if (MDescriptor->canReceiveMidi()) {
      uint32_t offset = 0;
      LV2_ATOM_SEQUENCE_FOREACH(MAtomSequence, ev) {
        if (ev->body.type == MMidiInputUrid) {
          const uint8_t* const msg = (const uint8_t*)(ev + 1);
          offset = (uint32_t)ev->time.frames;
          on_plugin_midi(offset,msg[0],msg[1],msg[2]);
        }
      }
    }
    KODE_ProcessContext context;

    //context.inputs      = MInputPtrs;
    //context.outputs     = MOutputPtrs;

    context.numinputs = MDescriptor->getNumInputs();
    context.numoutputs = MDescriptor->getNumOutputs();
    for (uint32_t i=0; i<context.numinputs; i++)  { context.inputs[i]  = MInputPtrs[i]; }
    for (uint32_t i=0; i<context.numoutputs; i++) { context.outputs[i] = MOutputPtrs[i]; }
    context.numsamples  = sample_count;
    context.samplerate  = MSampleRate;
//    context.offset      = 0;
//    context.oversample  = 1;
    on_plugin_process(&context);
    //todo: flush midi
  }

  //----------

  void lv2_deactivate() {
    //KODE_Trace("lv2_deactivate\n");
//    MInstance->on_deactivate();
//    MInstance->on_stop();
  }

  //----------

  void lv2_cleanup() {
    //KODE_Trace("lv2_cleanup\n");
    //on_stop();
//    on_terminate();
  }

  //----------

  static
  const void* lv2_extension_data_callback(const char* uri) {
    //KODE_Trace("lv2: lv2_extension_data_callback\n");
    return KODE_NULL;
  }

};

//----------------------------------------------------------------------
#endif

























#if 0



//------------------------------
private:
//------------------------------

  void lv2_connect_port(uint32_t port, void* data_location) {
  }

  //----------

  void lv2_activate() {
  }

  //----------

  void lv2_run(uint32_t sample_count) {
  }

  //----------

  void lv2_deactivate() {
  }

  //----------

  void lv2_cleanup() {
  }

  //----------

  const void* lv2_extension_data(const char* uri) {
    //LV2_Trace("lv2_extension_data: uri %s\n",uri);
    return nullptr;
  }

};


#endif // 0
