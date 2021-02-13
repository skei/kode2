#ifndef kode_ladspa_instance_included
#define kode_ladspa_instance_included
//----------------------------------------------------------------------

// who deletes this?

//#define KODE_MAX_PATH_LENGTH      512
//#define KODE_MAX_PROCESS_EVENTS   1024

//----------

#include "base/kode.h"
//#include "base/kode_queue.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_process_context.h"
#include "plugin/ladspa/kode_ladspa.h"
#include "plugin/ladspa/kode_ladspa_host.h"

//----------

//typedef KODE_Queue<uint32_t,KODE_MAX_PROCESS_EVENTS>  KODE_ParameterQueue;

//----------------------------------------------------------------------

class KODE_LadspaInstance {

  friend class KODE_LadspaPlugin;

//------------------------------
private:
//------------------------------

  KODE_LadspaHost*  MHost           = nullptr;
  KODE_Descriptor*  MDescriptor     = nullptr;
  KODE_Instance*    MInstance       = nullptr;
  KODE_Editor*      MEditor         = nullptr;
  float**           MInputPtrs      = nullptr;
  float**           MOutputPtrs     = nullptr;
  float**           MParameterPtrs  = nullptr;
  uint32_t          MNumInputs      = 0;
  uint32_t          MNumOutputs     = 0;
  uint32_t          MNumParameters  = 0;
  float*            MHostValues     = nullptr;
  float*            MProcessValues  = nullptr;
  float             MSampleRate     = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_LadspaInstance(KODE_Descriptor* ADescriptor, float ASampleRate) {
    KODE_Trace("KODE_LadspaInstance\n");

    MHost           = new KODE_LadspaHost(ADescriptor);
    MDescriptor     = ADescriptor;
    MInstance       = KODE_CreateInstance(MDescriptor,MHost);

    MSampleRate     = ASampleRate;

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

  ~KODE_LadspaInstance() {
    KODE_Trace("~KODE_LadspaInstance\n");
    if (MHost) delete MHost;
    if (MInstance) delete MInstance;

    if (MInputPtrs)     free(MInputPtrs);
    if (MOutputPtrs)    free(MOutputPtrs);
    if (MParameterPtrs) free(MParameterPtrs);
    if (MHostValues)    free(MHostValues);
    if (MProcessValues) free(MProcessValues);

  }

//------------------------------
public:
//------------------------------

  KODE_Instance* getInstance() {
    return MInstance;
  }

//------------------------------
private:
//------------------------------

  // TODO: do better

  void ladspa_connect_port(unsigned long Port, LADSPA_Data * DataLocation) {
    LADSPA_Trace("ladspa: connect_port Port:%i DataLocation:%p\n",Port,DataLocation);
    //if (Port < 0) return;
    if (Port < MNumInputs) {
      MInputPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumInputs;
    if (Port < MNumOutputs) {
      MOutputPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumOutputs;
    if (Port < MNumParameters) {
      MParameterPtrs[Port] = (float*)DataLocation;
      return;
    }
    Port -= MNumParameters;
  }

  //----------

  void ladspa_activate() {
    LADSPA_Trace("ladspa: activate\n");
    if (MInstance) {
      //MSampleRate = MPlugin->getSampleRate();
      //MInstance->on_stateChange(kps_initialize);
      MInstance->on_start();
      MInstance->on_activate();
    }
  }

  //----------

  void ladspa_run(unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: run SampleCount:%i\n",SampleCount);
    if (MInstance) {
      for (uint32_t i=0; i<MNumParameters; i++) {
        float v = *MParameterPtrs[i];
        MHostValues[i] = v;
        if (v != MProcessValues[i]) {
          MProcessValues[i] = v;
          //KODE_Parameter* param = MPlugin->getParameter(i);
          //if (param) v = param->from01(v);
          MInstance->on_parameterChange(i,v);
        }
      }
      //MInstance->on_processBlock(MInputPtrs,MOutputPtrs,SampleCount);
      KODE_ProcessContext context;
      //context.inputs      = MInputPtrs;
      //context.outputs     = MOutputPtrs;
      uint32_t i;
      for (i=0; i<MDescriptor->getNumInputs(); i++)  { context.inputs[i]  = MInputPtrs[i]; }
      for (i=0; i<MDescriptor->getNumOutputs(); i++) { context.outputs[i] = MOutputPtrs[i]; }

      context.length      = SampleCount;
      context.offset      = 0;
      context.samplerate  = MSampleRate;
      context.oversample  = 1;
      MInstance->on_processBlock(&context);
    }
  }

  //----------

  void ladspa_run_adding(unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: run_adding SampleCount:%i\n",SampleCount);
  }

  //----------

  void ladspa_set_run_adding_gain(LADSPA_Data Gain) {
    LADSPA_Trace("ladspa: set_run_adding_gain Gain:%.3f\n",Gain);
  }

  //----------

  void ladspa_deactivate() {
    LADSPA_Trace("ladspa: deactivate\n");
    if (MInstance) {
      MInstance->on_deactivate();
      MInstance->on_stop();
    }
  }

  //----------

  void ladspa_cleanup() {
    LADSPA_Trace("ladspa: cleanup\n");
    if (MInstance) {
      MInstance->on_terminate();
      //MInstance->on_close();
    }
  }

  //----------

};

//----------------------------------------------------------------------
#endif
