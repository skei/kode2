#ifndef kode_vst2_plugin_included
#define kode_vst2_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/vst2/kode_vst2.h"
#include "plugin/vst2/kode_vst2_host.h"
#include "plugin/vst2/kode_vst2_instance.h"

//----------------------------------------------------------------------

class KODE_Vst2Plugin {

  friend AEffect* kode_vst2_entryPoint(audioMasterCallback audioMaster);

//------------------------------
private:
//------------------------------

  KODE_Descriptor*    MDescriptor   = nullptr;
  KODE_Instance*      MInstance     = nullptr;
  KODE_Editor*        MEditor       = nullptr;
  KODE_Vst2Instance*  MVst2Instance = nullptr;

//------------------------------
public:
//------------------------------

  KODE_Vst2Plugin() {
    //MDescriptor = KODE_CreateDescriptor();
  }

  //----------

  ~KODE_Vst2Plugin() {
    //if (MDescriptor) delete MDescriptor;
  }

//------------------------------
private: // vst2 callbacks
//------------------------------

  static
  VstIntPtr vst2_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
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
  void vst2_setParameter_callback(AEffect* effect, VstInt32 index, float parameter) {
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    vst2_instance->vst2_setParameter(index,parameter);
  }

  //----------

  static
  float vst2_getParameter_callback(AEffect* effect, VstInt32 index) {
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    return vst2_instance->vst2_getParameter(index);
  }

  //----------

  static
  void vst2_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames) {
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    vst2_instance->vst2_process(inputs,outputs,sampleFrames);
  }

  //----------

  static
  void vst2_processDouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames) {
    KODE_Vst2Instance* vst2_instance = (KODE_Vst2Instance*)effect->object;
    vst2_instance->vst2_processDouble(inputs,outputs,sampleFrames);
  }


//------------------------------
private:
//------------------------------

  AEffect* vst2_entryPoint(audioMasterCallback audioMaster) {
    //KODE_Trace("vst2_entryPoint\n");

    MDescriptor   = KODE_GetDescriptor();
    MVst2Instance = new KODE_Vst2Instance(MDescriptor,audioMaster);
    MInstance     = MVst2Instance->getInstance();

    MInstance->on_initialize(); // open?
    //instance->updateAllParameters();

    int32_t flags = effFlagsCanReplacing;
    if (MDescriptor->hasFlag(KODE_PLUGIN_IS_SYNTH)) flags |= effFlagsIsSynth;
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) flags |= effFlagsHasEditor;
    //if (MPlugin->hasFlag(kpf_chunks))      flags |= effFlagsProgramChunks;
    //if (MPlugin->hasFlag(kpf_silentStop))  flags |= effFlagsNoSoundInStop;

    #ifndef KODE_PLUGIN_VST2_VESTIGE
    //if (MDescriptor->hasFlag(KODE_PLUGIN_PROCESS_DOUBLE)) flags |= effFlagsCanDoubleReplacing;
    #endif

    AEffect* effect = MVst2Instance->getAEffect();
    memset(effect,0,sizeof(AEffect));

    effect->magic                     = kEffectMagic;
    effect->uniqueID                  = MDescriptor->getShortId();
    effect->flags                     = flags;
    effect->numInputs                 = MDescriptor->getNumInputs();
    effect->numOutputs                = MDescriptor->getNumOutputs();
    effect->numParams                 = MDescriptor->getNumParameters();
    effect->numPrograms               = 0;//MDescriptor->getNumPresets();
    effect->version                   = MDescriptor->getVersion();
    effect->initialDelay              = 0;//getLatency();
    effect->object                    = MVst2Instance;
    effect->user                      = nullptr;//instance;
    effect->dispatcher                = vst2_dispatcher_callback;
    effect->setParameter              = vst2_setParameter_callback;
    effect->getParameter              = vst2_getParameter_callback;
    effect->processReplacing          = vst2_process_callback;

    #ifndef KODE_PLUGIN_VST2_VESTIGE
    effect->processDoubleReplacing    = vst2_processDouble_callback;
    #endif

    return effect;
  }

};

//----------------------------------------------------------------------
// .so entry point
//----------------------------------------------------------------------

#define KODE_VST2_MAIN_SYMBOL asm ("VSTPluginMain");
AEffect* kode_vst2_entryPoint(audioMasterCallback audioMaster) KODE_VST2_MAIN_SYMBOL

//----------

KODE_Vst2Plugin VST2_PLUGIN;

//----------

__KODE_DLLEXPORT
AEffect* kode_vst2_entryPoint(audioMasterCallback audioMaster) {
  if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0;
  AEffect* effect = VST2_PLUGIN.vst2_entryPoint(audioMaster);
  return effect;
}

//----------------------------------------------------------------------
#endif
