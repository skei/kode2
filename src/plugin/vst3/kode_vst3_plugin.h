#ifndef vst3_plugin_included
#define vst3_plugin_included
//----------------------------------------------------------------------

#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_instance.h"
#include "plugin/vst3/kode_vst3_utils.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

template <class DESC, class INST>
class KODE_VST3Plugin
: public KODE_Vst3IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t            MRefCount     = 1;
  KODE_Vst3FUnknown*  MHostContext  = nullptr;
  DESC                MDescriptor;

//------------------------------
public:
//------------------------------

  KODE_VST3Plugin() {
    //VST3_PRINT;
    MRefCount = 1;
  }

  //----------

  virtual ~KODE_VST3Plugin() {
    //VST3_PRINT;
  }

//------------------------------
public: // FUnknown
//------------------------------

  uint32_t KODE_VST3_PLUGIN_API addRef() final {
    //VST3_Print("");
    MRefCount++;
    //VST3_DPrint("-> %i\n",MRefCount);
    return MRefCount;
  }

  //----------

  uint32_t KODE_VST3_PLUGIN_API release() final {
    //VST3_Print("");
    const uint32_t r = --MRefCount;
    //VST3_DPrint("-> %i %s",r, (r==0) ? "(delete)\n" : "\n" );
    if (r == 0) KODE_Delete this;
    return r;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API queryInterface(const KODE_Vst3Id _iid, void** obj) final {
    //VST3_Print("_iid ");
    //VST3_PrintIID(_iid);
    if (KODE_iidEqual(KODE_Vst3IPluginFactory2_iid,_iid)) {
      //VST3_DPrint(" (IPluginFactory2)\n");
      *obj = (KODE_Vst3IPluginFactory2*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IPluginFactory3_iid,_iid)) {
      //VST3_DPrint(" (IPluginFactory3)\n");
      *obj = (KODE_Vst3IPluginFactory3*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    //VST3_DPrint(" (unknown)\n");
    *obj = nullptr;
    return kode_vst3_NoInterface;
  }

//------------------------------
public:
//------------------------------

  //--------------------
  // IPluginFactory
  //--------------------

  int32_t KODE_VST3_PLUGIN_API getFactoryInfo(KODE_Vst3PFactoryInfo* info) final {
    //VST3_PRINT;
    KODE_Strcpy(info->vendor,MDescriptor.getAuthor());
    KODE_Strcpy(info->url,MDescriptor.getUrl());
    KODE_Strcpy(info->email,MDescriptor.getEmail());
    info->flags = KODE_Vst3PFactoryInfo::kode_vst3_NoFlags;
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API countClasses() final {
    //VST3_Print("-> 1\n");
    return 1;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getClassInfo(int32_t index, KODE_Vst3PClassInfo* info) final {
    //VST3_Print("index %i\n",index);
    switch (index) {
      case 0:
        KODE_Memcpy(info->cid,MDescriptor.getLongId(),16);
        info->cardinality = KODE_Vst3PClassInfo::kode_vst3_ManyInstances;
        KODE_Strncpy(info->category,kode_vst3_VstAudioEffectClass,KODE_Vst3PClassInfo::kode_vst3_CategorySize);
        KODE_Strncpy(info->name,MDescriptor.getName(),KODE_Vst3PClassInfo::kode_vst3_NameSize);
        return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API createInstance(const char* cid, const char* _iid, void** obj) final {
    //VST3_Print("cid ");
    //VST3_PrintIID(cid);
    if (KODE_iidEqual(MDescriptor.getLongId(),cid)) {
      //VST3_DPrint(" (%s)\n",MDescriptor.getName());
      INST* instance = KODE_New INST(&MDescriptor);
      instance->on_plugin_open();
      instance->setDefaultParameterValues();
      instance->updateAllParameters();
      *obj = (KODE_Vst3IComponent*)instance;
      return kode_vst3_ResultOk;
    }
    *obj = nullptr;
    return kode_vst3_NotImplemented;
  }

//------------------------------
public: // IPluginFactory2
//------------------------------

  int32_t KODE_VST3_PLUGIN_API getClassInfo2(int32_t index, KODE_Vst3PClassInfo2* info) final {
    //VST3_Print("index %i\n",index);
    switch (index) {
      case 0:
        KODE_Memcpy(info->cid,MDescriptor.getLongId(),16);
        info->cardinality = KODE_Vst3PClassInfo::kode_vst3_ManyInstances;
        KODE_Strcpy(info->category,kode_vst3_VstAudioEffectClass);
        KODE_Strcpy(info->name,MDescriptor.getName());
        info->classFlags = 0;
        if (MDescriptor.isSynth()) {
          KODE_Strcpy(info->subCategories,kode_vst3_Instrument);
        }
        else {
          KODE_Strcpy(info->subCategories,kode_vst3_Fx);
        }
        KODE_Strcpy(info->vendor,MDescriptor.getAuthor());
        KODE_Strcpy(info->version,MDescriptor.getVersionText());
        KODE_Strcpy(info->sdkVersion,kode_vst3_VstVersionString);
        return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IPluginFactory3
//------------------------------

  int32_t KODE_VST3_PLUGIN_API getClassInfoUnicode(int32_t index, KODE_Vst3PClassInfoW* info) final {
    //VST3_Print("index %i\n",index);
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setHostContext(KODE_Vst3FUnknown* context) final {
    //VST3_Print("context %p\n",context);
    MHostContext = context;
    return kode_vst3_ResultOk;
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#define VST3_MAIN_SYMBOL  asm ("GetPluginFactory");
KODE_Vst3IPluginFactory* KODE_VST3_PLUGIN_API vst3_entrypoint() VST3_MAIN_SYMBOL

//----------

#define KODE_VST3_ENTRYPOINT(DESC,INST)                               \
                                                                      \
  __KODE_DLLEXPORT                                                    \
  KODE_Vst3IPluginFactory* KODE_VST3_PLUGIN_API vst3_entrypoint() {   \
    /*VST3_PRINT;*/                                                   \
    return KODE_New KODE_VST3Plugin<DESC,INST>();                     \
  }

//----------------------------------------------------------------------
#endif
