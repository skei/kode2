#ifndef kode_vst2_plugin_included
#define kode_vst2_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance_listener.h"
#include "plugin/vst2/kode_vst2.h"
#include "plugin/vst2/kode_vst2_instance.h"

//----------------------------------------------------------------------

template<class DESC, class INST>
class KODE_Vst2Plugin {
  
//------------------------------
private:
//------------------------------
  
  KODE_Descriptor*  MDescriptor = KODE_NULL;
  
//------------------------------
public:
//------------------------------

  /*
    KODE_Vst2Plugin is static/global, so don't create anytihng
    in the constructor!
  */

  KODE_Vst2Plugin() {
    KODE_VST2TRACE;
    //KODE_TRACE;
    //MDescriptor = KODE_New DESC();
  }
  
  //----------

  ~KODE_Vst2Plugin() {
    KODE_VST2TRACE;
    //KODE_TRACE;
    if (MDescriptor) KODE_Delete MDescriptor;
  }
  
//------------------------------
private: // vst2 callbacks
//------------------------------

  static
  VstIntPtr dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
    VstIntPtr result = 0;
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    result = vst2_instance->vst2_dispatcher(opcode,index,value,ptr,opt);
    if (opcode==effClose) {
      delete vst2_instance; // (KODE_Vst2Instance*)effect->object;
    }
    return result;
  }

  //----------

  static
  void setParameter_callback(AEffect* effect, VstInt32 index, float parameter) {
    //KODE_TRACE;
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    vst2_instance->vst2_setParameter(index,parameter);
  }

  //----------

  static
  float getParameter_callback(AEffect* effect, VstInt32 index) {
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    return vst2_instance->vst2_getParameter(index);
  }

  //----------

  static
  void process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames) {
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    vst2_instance->vst2_process(inputs,outputs,sampleFrames);
  }

  //----------

  static
  void processDouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames) {
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    vst2_instance->vst2_processDouble(inputs,outputs,sampleFrames);
  }

//------------------------------
public:
//------------------------------

  AEffect* entrypoint(audioMasterCallback audioMaster) {
    KODE_VST2TRACE;
    
    if (!MDescriptor) MDescriptor = KODE_New DESC();
    
    INST* instance = KODE_New INST(MDescriptor);
    KODE_Vst2Instance* vst2_instance = KODE_New KODE_Vst2Instance(instance,audioMaster);
    instance->setListener(vst2_instance);
    
    instance->on_open();
    //instance->on_initialize();
    instance->setDefaultParameterValues();
    instance->updateAllParameters();
    
    int32_t flags = effFlagsCanReplacing;
    if (MDescriptor->hasFlag(KODE_PLUGIN_IS_SYNTH)) flags |= effFlagsIsSynth;
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) flags |= effFlagsHasEditor;
    //if (MPlugin->hasFlag(kpf_chunks))      flags |= effFlagsProgramChunks;
    //if (MPlugin->hasFlag(kpf_silentStop))  flags |= effFlagsNoSoundInStop;
    //#ifndef KODE_PLUGIN_VST2_VESTIGE
    //if (MDescriptor->hasFlag(KODE_PLUGIN_PROCESS_DOUBLE)) flags |= effFlagsCanDoubleReplacing;
    //#endif
    
    AEffect* effect = vst2_instance->getAEffect();
    memset(effect,0,sizeof(AEffect));
    
    effect->magic                     = kEffectMagic;
    effect->uniqueID                  = MDescriptor->getShortId();
    effect->flags                     = flags;
    effect->numInputs                 = MDescriptor->getNumInputs();
    effect->numOutputs                = MDescriptor->getNumOutputs();
    effect->numParams                 = MDescriptor->getNumParameters();
    effect->numPrograms               = MDescriptor->getNumPresets();
    effect->version                   = MDescriptor->getVersion();
    effect->initialDelay              = 0;
    effect->object                    = vst2_instance;
    effect->user                      = nullptr;//instance;
    effect->dispatcher                = dispatcher_callback;
    effect->setParameter              = setParameter_callback;
    effect->getParameter              = getParameter_callback;
    effect->processReplacing          = process_callback;
    
    #ifndef KODE_PLUGIN_VST2_VESTIGE
    effect->processDoubleReplacing    = processDouble_callback;
    #endif
    
    return effect;
  }
  
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_VST2_MAIN_SYMBOL asm ("VSTPluginMain");
AEffect* kode_vst2_entrypoint(audioMasterCallback audioMaster) KODE_VST2_MAIN_SYMBOL

//----------

#define KODE_VST2_MAIN(DESC,INST)                                   \
                                                                    \
  KODE_Vst2Plugin<DESC,INST> _VST2_PLUGIN;                          \
                                                                    \
  __KODE_DLLEXPORT                                                  \
  AEffect* kode_vst2_entrypoint(audioMasterCallback audioMaster) {  \
    KODE_VST2TRACE;                                                 \
    /*KODE_TRACE;*/                                                 \
    if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;       \
    AEffect* effect = _VST2_PLUGIN.entrypoint(audioMaster);         \
    return effect;                                                  \
  }

//----------------------------------------------------------------------
#endif
