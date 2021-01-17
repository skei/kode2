#ifndef kode_vst3_plugin_included
#define kode_vst3_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_utils.h"

//----------------------------------------------------------------------

template <class DESC, class INST>
class KODE_Vst3Plugin
: public VST3_IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t          MRefCount     = 1;
  VST3_FUnknown*    MHostContext  = KODE_NULL;
  KODE_Descriptor*  MDescriptor   = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Vst3Plugin() {
    VST3_Print("\n");
    MRefCount = 1;
    MDescriptor = KODE_New DESC();
  }

  //----------

  virtual ~KODE_Vst3Plugin() {
    VST3_Print("\n");
    if (MDescriptor) KODE_Delete MDescriptor;
  }

//------------------------------

//------------------------------
public:
//------------------------------

  //--------------------
  // FUnknown
  //--------------------

  uint32_t  VST3_PLUGIN_API addRef() final {
    VST3_Print("-> %i\n",MRefCount+1);
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32_t  VST3_PLUGIN_API release() final {
    VST3_Print("-> %i\n",MRefCount-1);
    const uint32_t r = --MRefCount;
    if (r == 0) KODE_Delete this;
    return r;
  }

  //----------

  int32_t VST3_PLUGIN_API queryInterface(const VST3_IID _iid, void** obj) final {
    VST3_Print("iid: ");
    #ifdef KODE_DEBUG
      #ifdef KODE_DEBUG_VST3
        VST3_PrintIID(_iid);
      #endif
    #endif

    if (vst3_iid_equal(IPluginFactory2_iid,_iid)) {
      VST3_Print("= IPluginFactory2\n");
      *obj = (VST3_IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (vst3_iid_equal(IPluginFactory3_iid,_iid)) {
      VST3_Print("= IPluginFactory2\n");
      *obj = (VST3_IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    *obj = KODE_NULL;
    return kNoInterface;
  }

  //--------------------
  // IPluginFactory
  //--------------------

  /*
    Class factory that any Plug-in defines for creating class instances.
    From the host's point of view a Plug-in module is a factory which can
    create a certain kind of object(s). The interface IPluginFactory provides
    methods to get information about the classes exported by the Plug-in and a
    mechanism to create instances of these classes (that usually define the
    IPluginBase interface)
  */

  //----------

  int32_t VST3_PLUGIN_API getFactoryInfo(VST3_PFactoryInfo* info) final {
    VST3_Print("\n");
    KODE_Strcpy(info->vendor,MDescriptor->getAuthor());
    KODE_Strcpy(info->url,MDescriptor->getUrl());
    KODE_Strcpy(info->email,MDescriptor->getEmail());
    info->flags = VST3_PFactoryInfo::kNoFlags;
    VST3_Print("-> vendor: %s\n",info->vendor);
    VST3_Print("-> url: %s\n",info->url);
    VST3_Print("-> email: %s\n",info->email);
    VST3_Print("-> flags: %08x\n",info->flags);
    return kResultOk;
  }

  //----------

  int32_t   VST3_PLUGIN_API countClasses() final {
    VST3_Print("-> 1\n");
    return 1;
  }

  //----------

  int32_t VST3_PLUGIN_API getClassInfo(int32_t index, VST3_PClassInfo* info) final {
    VST3_Print("index 0\n",index);
    switch (index) {
      case 0:
        KODE_Memcpy(info->cid,(const char*)MDescriptor->getLongId(),16);
        info->cardinality = VST3_PClassInfo::kManyInstances;
        /*
          gcc complaints, in release build:
          warning: specified bound 32 equals destination size [-Wstringop-truncation]
          > info->category = [kCategorySize]
        */
        //KODE_Strncpy(info->category,vst3_VstAudioEffectClass,VST3_PClassInfo::kCategorySize);
        //KODE_Strncpy(info->name,MDescriptor->getName(),VST3_PClassInfo::kNameSize);
        KODE_Strncpy(info->category,vst3_VstAudioEffectClass,VST3_PClassInfo::kCategorySize-1);
        KODE_Strncpy(info->name,MDescriptor->getName(),VST3_PClassInfo::kNameSize-1);
        //KODE_Strncpy(info->category,vst3_VstAudioEffectClass,VST3_PClassInfo::kCategorySize - 1);
        //KODE_Strncpy(info->name,MDescriptor->getName(),VST3_PClassInfo::kNameSize - 1);
        //info->category[VST3_PClassInfo::kCategorySize-1] = 0;
        //info->name[VST3_PClassInfo::kNameSize-1] = 0;
        VST3_Print("-> cid: ");
        VST3_PrintIID(info->cid);
        VST3_Print("-> cardinality: %08x\n",info->cardinality);
        VST3_Print("-> category: %s\n",info->category);
        VST3_Print("-> name: %s\n",info->name);
        return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  int32_t VST3_PLUGIN_API createInstance(const char* cid, const char* _iid, void** obj) final {
    if (vst3_iid_equal(MDescriptor->getLongId(),cid)) {
      VST3_Print("= myPlugin\n");
      INST* instance = KODE_New INST(MDescriptor);
      instance->on_create();
      instance->setDefaultParameterValues();
      instance->updateAllParameters();
      *obj = (VST3_IComponent*)instance;
      return kResultOk;
    }
    VST3_Print("<unknown> ");
    VST3_PrintIID(cid);
    *obj = KODE_NULL;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  int32_t VST3_PLUGIN_API getClassInfo2(int32_t index, VST3_PClassInfo2* info) final {
    VST3_Print("index %i\n",index);
    switch (index) {
      case 0:
        KODE_Memcpy(info->cid,(const char*)MDescriptor->getLongId(),16);
        info->cardinality = VST3_PClassInfo::kManyInstances;
        KODE_Strcpy(info->category,vst3_VstAudioEffectClass);
        KODE_Strcpy(info->name,MDescriptor->getName());
        info->classFlags = 0;
        if (MDescriptor->isSynth()) {
          KODE_Strcpy(info->subCategories,vst3_Instrument);
        }
        else {
          KODE_Strcpy(info->subCategories,vst3_Fx);
        }
        KODE_Strcpy(info->vendor,MDescriptor->getAuthor());
        KODE_Strcpy(info->version,MDescriptor->getVersionText());
        KODE_Strcpy(info->sdkVersion,vst3_VstVersionString);
        VST3_Print("-> cid: ");
        VST3_PrintIID(info->cid);
        VST3_Print("-> cardinality: %08x\n",info->cardinality);
        VST3_Print("-> category: %s\n",info->category);
        VST3_Print("-> name: %s\n",info->name);
        VST3_Print("-> classFlags: 0\n");
        VST3_Print("-> subCategories: %s\n",info->subCategories);
        VST3_Print("-> vendor: %s\n",info->vendor);
        VST3_Print("-> version: %s\n",info->version);
        VST3_Print("-> sdkVersion: %s\n",info->sdkVersion);
        return kResultOk;
    }
    return kResultFalse;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  int32_t VST3_PLUGIN_API getClassInfoUnicode(int32_t index, VST3_PClassInfoW* info) final {
    VST3_Print("index %i -> false\n",index);
    return kResultFalse;
  }

  //----------

  int32_t VST3_PLUGIN_API setHostContext(VST3_FUnknown* context) final {
    VST3_Print("\n");
    MHostContext = context;
    return kResultOk;
  }

//------------------------------

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_VST3_MAIN_SYMBOL asm ("GetPluginFactory");
VST3_IPluginFactory* VST3_PLUGIN_API kode_vst3_entryPoint() KODE_VST3_MAIN_SYMBOL

//----------

#define KODE_VST3_PLUGIN_ENTRYPOINT(DESC,INST)                  \
                                                                \
  __KODE_DLLEXPORT                                              \
  VST3_IPluginFactory* VST3_PLUGIN_API kode_vst3_entryPoint() { \
    VST3_Print("GetPluginFactory\n");                           \
    return KODE_New KODE_Vst3Plugin<DESC,INST>();               \
  }

//----------------------------------------------------------------------
#endif
