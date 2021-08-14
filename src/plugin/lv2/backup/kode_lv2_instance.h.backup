#ifndef kode_dssi_instance_included
#define kode_dssi_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_instance_listener.h"

class KODE_DssiInstance
: public KODE_InstanceListener {
  
public:

  KODE_DssiInstance(KODE_Instance* AInstance) {
  }
  
  virtual ~KODE_DssiInstance() {
  }
  
public:

  
};

//----------------------------------------------------------------------
#endif












































#if 0

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_process_context.h"
#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_host.h"

//----------


class KODE_Lv2Instance {

  friend class KODE_Lv2Plugin;

//------------------------------
private:
//------------------------------

  KODE_Lv2Host*             MHost             = nullptr;
  KODE_Descriptor*          MDescriptor       = nullptr;
  KODE_Instance*            MInstance         = nullptr;
  KODE_Editor*              MEditor           = nullptr;

  LV2_URID                  MMidiInputUrid    = 0;
  const LV2_Atom_Sequence*  MAtomSequence     = nullptr;
  float                     MSampleRate       = 0.0f;

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

  KODE_Lv2Instance(KODE_Descriptor* ADescriptor, float ASampleRate) {
    KODE_Trace("KODE_Lv2Instance\n");
    MHost           = new KODE_Lv2Host(ADescriptor);
    MDescriptor     = ADescriptor;
    MInstance       = KODE_CreateInstance(MDescriptor,MHost);
    MSampleRate     = ASampleRate;
    //
    MNumInputs      = MDescriptor->getNumInputs();
    MNumOutputs     = MDescriptor->getNumOutputs();
    MNumParameters  = MDescriptor->getNumParameters();
    MInputPtrs      = (float**)malloc(MNumInputs     * sizeof(float*));
    MOutputPtrs     = (float**)malloc(MNumOutputs    * sizeof(float*));
    MParameterPtrs  = (float**)malloc(MNumParameters * sizeof(float*));
    MHostValues     = (float*) malloc(MNumParameters * sizeof(float ));
    MProcessValues  = (float*) malloc(MNumParameters * sizeof(float ));

    //instance->on_open();
    MInstance->on_initialize(); // open?

  }

  //----------

  ~KODE_Lv2Instance() {
    KODE_Trace("~KODE_Lv2Instance\n");
    if (MHost) delete MHost;
    if (MInputPtrs)     free(MInputPtrs);
    if (MOutputPtrs)    free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);
    if (MHostValues)    free(MHostValues);
    if (MProcessValues) free(MProcessValues);
    if (MInstance)      delete MInstance;

  }

//------------------------------
private:
//------------------------------

  void setMidiInputUrid(LV2_URID lv2_midi_input_urid) {
    MMidiInputUrid = lv2_midi_input_urid;
  }

//------------------------------
private:
//------------------------------

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
    if (MDescriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
      MAtomSequence = (const LV2_Atom_Sequence*)data_location;
      port -= 1;
    }
  }

  //----------

  void lv2_activate() {
    //LV2_Trace("lv2_activate\n");
    if (MInstance) {
      MInstance->on_start();
      MInstance->on_activate();
    }
  }

  //----------

  void lv2_run(uint32_t sample_count) {
    //LV2_Trace("lv2_run\n");
    if (MInstance) {
      // parameters
      for (uint32_t i=0; i<MNumParameters; i++) {
        float v = *MParameterPtrs[i];
        MHostValues[i] = v;
        if (v != MProcessValues[i]) {
          MProcessValues[i] = v;
          // to/from01 ??
          //KODE_Parameter* param = MPlugin->getParameter(i);
          //if (param) v = param->from01(v);
          MInstance->on_parameterChange(i,v);
        }
      }
      // midi
      if (MDescriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
        uint32_t offset = 0;
        LV2_ATOM_SEQUENCE_FOREACH(MAtomSequence, ev) {
          if (ev->body.type == MMidiInputUrid) {
            const uint8_t* const msg = (const uint8_t*)(ev + 1);
            offset = (uint32_t)ev->time.frames;
            MInstance->on_midiEvent(offset,msg[0],msg[1],msg[2]);
          }
        }
      }
      KODE_ProcessContext context;

      //context.inputs      = MInputPtrs;
      //context.outputs     = MOutputPtrs;
      uint32_t i;
      for (i=0; i<MDescriptor->getNumInputs(); i++)  { context.inputs[i]  = MInputPtrs[i]; }
      for (i=0; i<MDescriptor->getNumOutputs(); i++) { context.outputs[i] = MOutputPtrs[i]; }

      context.length      = sample_count;
      context.offset      = 0;
      context.samplerate  = MSampleRate;
      context.oversample  = 1;
      MInstance->on_processBlock(&context);
      //todo: flush midi
    }
  }

  //----------

  void lv2_deactivate() {
    //LV2_Trace("lv2_deactivate\n");
    if (MInstance) {
      MInstance->on_deactivate();
      MInstance->on_stop();
    }
  }

  //----------

  void lv2_cleanup() {
    //LV2_Trace("lv2_cleanup\n");
    if (MInstance) {
      MInstance->on_terminate();
      //MInstance->on_stop();
    }
  }

  //----------

  const void* lv2_extension_data(const char* uri) {
    //LV2_Trace("lv2_extension_data: uri %s\n",uri);
    return nullptr;
  }

};


#endif // 0
