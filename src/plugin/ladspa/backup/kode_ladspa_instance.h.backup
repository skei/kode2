#ifndef kode_ladspa_instance_included
#define kode_ladspa_instance_included
//----------------------------------------------------------------------

// in progress

#include "base/kode.h"
//#include "base/kode_queue.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_instance_listener.h"
#include "plugin/kode_process_context.h"
#include "plugin/ladspa/kode_ladspa.h"

//typedef KODE_Queue<uint32_t,KODE_MAX_PROCESS_EVENTS>  KODE_ParameterQueue;

//----------------------------------------------------------------------

class KODE_LadspaInstance
: public KODE_InstanceListener {
  
  //friend class KODE_LadspaPlugin;

//------------------------------
private:
//------------------------------

  KODE_Descriptor*  MDescriptor     = KODE_NULL;
  KODE_Instance*    MInstance       = KODE_NULL;
  //KODE_Editor*      MEditor         = KODE_NULL;
  float**           MInputPtrs      = KODE_NULL;
  float**           MOutputPtrs     = KODE_NULL;
  float**           MParameterPtrs  = KODE_NULL;
  uint32_t          MNumInputs      = 0;
  uint32_t          MNumOutputs     = 0;
  uint32_t          MNumParameters  = 0;
  //float*            MHostValues     = KODE_NULL;
  //float*            MProcessValues  = KODE_NULL;
  float             MSampleRate     = 0.0f;
  

//------------------------------
public:
//------------------------------

  KODE_LadspaInstance(KODE_Instance* AInstance, uint32_t ASampleRate) {
    MDescriptor     = AInstance->getDescriptor();
    MInstance       = AInstance;
    MSampleRate     = ASampleRate;
    MNumInputs      = MDescriptor->getNumInputs();
    MNumOutputs     = MDescriptor->getNumOutputs();
    MNumParameters  = MDescriptor->getNumParameters();
    MInputPtrs      = (float**)KODE_Malloc(MNumInputs     * sizeof(float*));
    MOutputPtrs     = (float**)KODE_Malloc(MNumOutputs    * sizeof(float*));
    MParameterPtrs  = (float**)KODE_Malloc(MNumParameters * sizeof(float*));
    //MHostValues     = (float*) KODE_Malloc(MNumParameters * sizeof(float ));
    //MProcessValues  = (float*) KODE_Malloc(MNumParameters * sizeof(float ));

    //instance->on_open();
    //MInstance->on_initialize(); // open?
    
  }
  
  //----------

  virtual ~KODE_LadspaInstance() {
    if (MInstance) delete MInstance;
    if (MInputPtrs)     KODE_Free(MInputPtrs);
    if (MOutputPtrs)    KODE_Free(MOutputPtrs);
    if (MParameterPtrs) KODE_Free(MParameterPtrs);
    //if (MHostValues)    KODE_Free(MHostValues);
    //if (MProcessValues) KODE_Free(MProcessValues);
  }
  
  //----------

  //KODE_Instance* getInstance() {
  //  return MInstance;
  //}
//------------------------------
public:
//------------------------------

  void ladspa_connect_port(unsigned long Port, LADSPA_Data * DataLocation) {
    //LADSPA_Trace("ladspa: connect_port Port:%i DataLocation:%p\n",Port,DataLocation);
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
    //LADSPA_Trace("ladspa: activate\n");
    if (MInstance) {
      //MSampleRate = MPlugin->getSampleRate();
      //MInstance->on_stateChange(kps_initialize);
      //MInstance->on_initialize();
      MInstance->on_activate();
    }
  }

  //----------

  void ladspa_run(unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: run SampleCount:%i\n",SampleCount);
    
    if (MInstance) {
      for (uint32_t i=0; i<MNumParameters; i++) {
        float v = *MParameterPtrs[i];
        //MHostValues[i] = v;
        if (v != MInstance->getParamValue(i)) {
          MInstance->setParamValue(i,v); // almoste3qual
          
          KODE_Parameter* param = MDescriptor->getParameter(i);
          /*if (param)*/ v = param->from01(v);
          
          MInstance->on_parameterChange(i,v);
        }
      }
      
      KODE_ProcessContext context;
      uint32_t num_in  = MDescriptor->getNumInputs();
      uint32_t num_out = MDescriptor->getNumOutputs();
      for (uint32_t i=0; i<num_in; i++)  { context.inputs[i]  = MInputPtrs[i]; }
      for (uint32_t i=0; i<num_out; i++) { context.outputs[i] = MOutputPtrs[i]; }
      context.playstate     = KODE_PLUGIN_PLAYSTATE_NONE;
      context.samplepos     = 0;
      context.beatpos       = 0.0f;
      context.tempo         = 0.0f;
      context.timesig_num   = 0;
      context.timesig_denom = 0;
      context.numInputs     = num_in;
      context.numOutputs    = num_out;
      context.numSamples    = SampleCount;
      context.samplerate    = MSampleRate;
      MInstance->on_process(&context);
    }
  }

  //----------

  void ladspa_run_adding(unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: run_adding SampleCount:%i\n",SampleCount);
  }

  //----------

  void ladspa_set_run_adding_gain(LADSPA_Data Gain) {
    //LADSPA_Trace("ladspa: set_run_adding_gain Gain:%.3f\n",Gain);
  }

  //----------

  void ladspa_deactivate() {
    //LADSPA_Trace("ladspa: deactivate\n");
    if (MInstance) {
      MInstance->on_deactivate();
      //MInstance->on_stop();
    }
  }

  //----------

  void ladspa_cleanup() {
    //LADSPA_Trace("ladspa: cleanup\n");
    if (MInstance) {
      MInstance->on_terminate();
      //MInstance->on_close();
    }
  }

  //----------

  
};

//----------------------------------------------------------------------
#endif
