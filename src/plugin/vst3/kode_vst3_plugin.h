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
class VST3_Plugin
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t  MRefCount     = 1;
  FUnknown* MHostContext  = nullptr;
  DESC      MDescriptor;

//------------------------------
public:
//------------------------------

  VST3_Plugin() {
    MRefCount = 1;
    KODE_Print("VST3_Plugin\n");
  }

  virtual ~VST3_Plugin() {
    KODE_Print("~VST3_Plugin\n");
  }

//------------------------------
public:
//------------------------------

  //--------------------
  // FUnknown
  //--------------------

  uint32 PLUGIN_API addRef() final {
    KODE_Print("VST3_Plugin.addRef\n");
    MRefCount++;
    return MRefCount;
  }

  uint32 PLUGIN_API release() final {
    KODE_Print("VST3_Plugin.release\n");
    const uint32_t r = --MRefCount;
    if (r == 0) delete this;
    return r;
  }

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) final {
    KODE_Print("VST3_Plugin.queryInterface\n");
    if (/*FUnknownPrivate::*/iidEqual(IPluginFactory2_iid,_iid)) {
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (/*FUnknownPrivate::*/iidEqual(IPluginFactory3_iid,_iid)) {
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    *obj = nullptr;
    return kNoInterface;
  }

  //--------------------
  // IPluginFactory
  //--------------------

  tresult PLUGIN_API getFactoryInfo(PFactoryInfo* info) final {
    KODE_Print("VST3_Plugin.getFactoryInfo\n");
    strcpy(info->vendor,MDescriptor.author);
    strcpy(info->url,MDescriptor.url);
    strcpy(info->email,MDescriptor.email);
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  int32 PLUGIN_API countClasses() final {
    KODE_Print("VST3_Plugin.countClasses\n");
    return 1;
  }

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) final {
    KODE_Print("VST3_Plugin.getClassInfo\n");
    switch (index) {
      case 0:
        memcpy(info->cid,MDescriptor.plugin_id,16);
        info->cardinality = PClassInfo::kManyInstances;
        strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
        strncpy(info->name,MDescriptor.name,PClassInfo::kNameSize);
        return kResultOk;
    }
    return kResultFalse;
  }

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) final {
    KODE_Print("VST3_Plugin.createInstance\n");
    if (/*FUnknownPrivate::*/iidEqual(MDescriptor.plugin_id,cid)) {
      INST* instance = new INST(/*&MDescriptor*/);
      instance->_setDescriptor(&MDescriptor);
      instance->on_create();
      instance->_setDefaultParameterValues();
      instance->_updateAllParameters();
      *obj = (/*Vst::*/IComponent*)instance;
      return kResultOk;
    }
    *obj = nullptr;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) final {
    KODE_Print("VST3_Plugin.getClassInfo2\n");
    switch (index) {
      case 0:
        memcpy(info->cid,MDescriptor.plugin_id,16);
        info->cardinality = PClassInfo::kManyInstances;
        strcpy(info->category,kVstAudioEffectClass);
        strcpy(info->name,MDescriptor.name);
        info->classFlags = 0;
        if (MDescriptor.is_synth) {
          strcpy(info->subCategories,/*Vst::PlugType::*/kInstrument);
        }
        else {
          strcpy(info->subCategories,/*Vst::PlugType::*/kFx);
        }
        strcpy(info->vendor,MDescriptor.author);
        strcpy(info->version,MDescriptor.version_text);
        strcpy(info->sdkVersion,kVstVersionString);
        return kResultOk;
    }
    return kResultFalse;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) final {
    KODE_Print("VST3_Plugin.getClassInfoUnicode\n");
    return kResultFalse;
  }

  tresult PLUGIN_API setHostContext(FUnknown* context) final {
    KODE_Print("VST3_Plugin.setHostContext\n");
    MHostContext = context;
    return kResultOk;
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

//#ifdef __gnu_linux__
//  #define __VST3_DLLEXPORT __attribute__ ((visibility ("default")))
//#endif
//
//#ifdef _WIN32
//  #define __VST3_DLLEXPORT __attribute__ ((dllexport))
//#endif

//----------

#define VST3_MAIN_SYMBOL  asm ("GetPluginFactory");
IPluginFactory* PLUGIN_API vst3_entrypoint() VST3_MAIN_SYMBOL

//----------

#define KODE_VST3_ENTRYPOINT(DESC,INST)                 \
                                                        \
  /*__VST3_DLLEXPORT*/                                  \
  __KODE_DLLEXPORT                                      \
  IPluginFactory* PLUGIN_API vst3_entrypoint() {        \
    KODE_Print("GetPluginFactory\n");                   \
    return KODE_NULL; /*new VST3_Plugin<DESC,INST>();*/ \
  }


//----------------------------------------------------------------------
#endif











