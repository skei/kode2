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
    KODE_PRINT;
    MRefCount = 1;
  }

  //----------

  virtual ~KODE_VST3Plugin() {
    KODE_PRINT;
  }

//------------------------------
public: // FUnknown
//------------------------------

  uint32_t KODE_VST3_PLUGIN_API addRef() final {
    KODE_PRINT;
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32_t KODE_VST3_PLUGIN_API release() final {
    KODE_PRINT;
    const uint32_t r = --MRefCount;
    if (r == 0) delete this;
    return r;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API queryInterface(const KODE_Vst3Id _iid, void** obj) final {
    KODE_PRINT;
    if (KODE_iidEqual(KODE_Vst3IPluginFactory2_iid,_iid)) {
      *obj = (KODE_Vst3IPluginFactory2*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IPluginFactory3_iid,_iid)) {
      *obj = (KODE_Vst3IPluginFactory3*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
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
    KODE_PRINT;
    strcpy(info->vendor,MDescriptor.getAuthor());
    strcpy(info->url,MDescriptor.getUrl());
    strcpy(info->email,MDescriptor.getEmail());
    info->flags = KODE_Vst3PFactoryInfo::kode_vst3_NoFlags;
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API countClasses() final {
    KODE_PRINT;
    return 1;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getClassInfo(int32_t index, KODE_Vst3PClassInfo* info) final {
    KODE_PRINT;
    switch (index) {
      case 0:
        memcpy(info->cid,MDescriptor.getLongId(),16);
        info->cardinality = KODE_Vst3PClassInfo::kode_vst3_ManyInstances;
        strncpy(info->category,kode_vst3_VstAudioEffectClass,KODE_Vst3PClassInfo::kode_vst3_CategorySize);
        strncpy(info->name,MDescriptor.getName(),KODE_Vst3PClassInfo::kode_vst3_NameSize);
        return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API createInstance(const char* cid, const char* _iid, void** obj) final {
    KODE_PRINT;
    if (KODE_iidEqual(MDescriptor.getLongId(),cid)) {
      INST* instance = new INST(&MDescriptor);
      instance->on_open();
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
    KODE_PRINT;
    switch (index) {
      case 0:
        memcpy(info->cid,MDescriptor.getLongId(),16);
        info->cardinality = KODE_Vst3PClassInfo::kode_vst3_ManyInstances;
        strcpy(info->category,kode_vst3_VstAudioEffectClass);
        strcpy(info->name,MDescriptor.getName());
        info->classFlags = 0;
        if (MDescriptor.isSynth()) {
          strcpy(info->subCategories,kode_vst3_Instrument);
        }
        else {
          strcpy(info->subCategories,kode_vst3_Fx);
        }
        strcpy(info->vendor,MDescriptor.getAuthor());
        strcpy(info->version,MDescriptor.getVersionText());
        strcpy(info->sdkVersion,kode_vst3_VstVersionString);
        return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IPluginFactory3
//------------------------------

  int32_t KODE_VST3_PLUGIN_API getClassInfoUnicode(int32_t index, KODE_Vst3PClassInfoW* info) final {
    KODE_PRINT;
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setHostContext(KODE_Vst3FUnknown* context) final {
    KODE_PRINT;
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
    KODE_PRINT;                                                       \
    return KODE_New KODE_VST3Plugin<DESC,INST>();                     \
  }

//----------------------------------------------------------------------
#endif
