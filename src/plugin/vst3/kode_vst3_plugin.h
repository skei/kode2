#ifndef kode_vst3_plugin_included
#define kode_vst3_plugin_included
//----------------------------------------------------------------------
//
// needs kode_vst3.h, which is gpl3
// so this file also needs to be gpl3
//
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
    KODE_Vst3Print("1\n");
    MRefCount = 1;
  }

  //----------

  virtual ~KODE_VST3Plugin() {
    KODE_VST3PRINT;
  }

//------------------------------
public: // FUnknown
//------------------------------

  uint32_t KODE_VST3_PLUGIN_API addRef() final {
    MRefCount++;
    KODE_Vst3Print("-> %i\n",MRefCount);
    return MRefCount;
  }

  //----------

  uint32_t KODE_VST3_PLUGIN_API release() final {
    const uint32_t r = --MRefCount;
    KODE_Vst3Print("-> %i %s",r, (r==0) ? "(delete)\n" : "\n" );
    if (r == 0) KODE_Delete this;
    return r;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API queryInterface(const KODE_Vst3Id _iid, void** obj) final {
    KODE_Vst3Print("iid: ");
    KODE_Vst3PrintIID(_iid);
    if (KODE_iidEqual(KODE_Vst3IPluginFactory2_iid,_iid)) {
      *obj = (KODE_Vst3IPluginFactory2*)this;
      KODE_Vst3DPrint(" (IPluginFactory2) -> Ok\n");
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IPluginFactory3_iid,_iid)) {
      *obj = (KODE_Vst3IPluginFactory3*)this;
      KODE_Vst3DPrint(" (IPluginFactory3) -> Ok\n");
      addRef();
      return kode_vst3_ResultOk;
    }
    *obj = nullptr;
    KODE_Vst3DPrint(" (unknown) -> NoInterface\n");
    return kode_vst3_NoInterface;
  }

//------------------------------
public:
//------------------------------

  //--------------------
  // IPluginFactory
  //--------------------

  int32_t KODE_VST3_PLUGIN_API getFactoryInfo(KODE_Vst3PFactoryInfo* info) final {
    KODE_Vst3Print(" -> Ok\n");
    KODE_Strcpy(info->vendor,MDescriptor.getAuthor());
    KODE_Strcpy(info->url,MDescriptor.getUrl());
    KODE_Strcpy(info->email,MDescriptor.getEmail());
    info->flags = KODE_Vst3PFactoryInfo::kode_vst3_NoFlags;
    KODE_Vst3DPrint(". author: '%s'\n",info->vendor);
    KODE_Vst3DPrint(". url: '%s'\n",info->url);
    KODE_Vst3DPrint(". email: '%s'\n",info->email);
    KODE_Vst3DPrint(". flags: %i\n",info->flags);
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API countClasses() final {
    KODE_Vst3Print(" -> 1\n");
    return 1;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getClassInfo(int32_t index, KODE_Vst3PClassInfo* info) final {
    KODE_Vst3Print("index: %i",index);
    switch (index) {
      case 0:
        KODE_Vst3DPrint(" -> Ok\n");
        KODE_Memcpy(info->cid,MDescriptor.getLongId(),16);
        info->cardinality = KODE_Vst3PClassInfo::kode_vst3_ManyInstances;
        KODE_Strncpy(info->category,kode_vst3_VstAudioEffectClass,KODE_Vst3PClassInfo::kode_vst3_CategorySize);
        KODE_Strncpy(info->name,MDescriptor.getName(),KODE_Vst3PClassInfo::kode_vst3_NameSize);
        KODE_Vst3DPrint(". cid: ");   KODE_Vst3PrintIID(info->cid);   KODE_Vst3DPrint("\n");
        KODE_Vst3DPrint(". cardinality: %i (%s)\n",info->cardinality,info->cardinality?"ManyInstances":"");
        KODE_Vst3DPrint(". category: '%s'\n",info->category);
        KODE_Vst3DPrint(". name: '%s'\n",info->name);
        return kode_vst3_ResultOk;
    }
    KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API createInstance(const char* cid, const char* _iid, void** obj) final {
    KODE_Vst3Print("cid: ");
    //KODE_Vst3DPrint(cid);
    KODE_Vst3PrintIID(cid);
    if (KODE_iidEqual(MDescriptor.getLongId(),cid)) {
      KODE_Vst3DPrint(" (%s) -> Ok\n",MDescriptor.getName());
      INST* instance = KODE_New INST(&MDescriptor);
      instance->on_plugin_open();
      instance->setDefaultParameterValues();
      instance->updateAllParameters();
      *obj = (KODE_Vst3IComponent*)instance;
      return kode_vst3_ResultOk;
    }
    *obj = nullptr;
    KODE_Vst3DPrint(" (unknown) -> False\n");
    return kode_vst3_NotImplemented;
  }

//------------------------------
public: // IPluginFactory2
//------------------------------

  int32_t KODE_VST3_PLUGIN_API getClassInfo2(int32_t index, KODE_Vst3PClassInfo2* info) final {
    KODE_Vst3Print("index: %i",index);
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
        KODE_Vst3DPrint(" -> Ok\n");
          KODE_Vst3DPrint(". cid: ");   KODE_Vst3PrintIID(info->cid);   KODE_Vst3DPrint("\n");
          KODE_Vst3DPrint(". cardinality: %i (%s)\n",info->cardinality,info->cardinality?"ManyInstances":"");
          KODE_Vst3DPrint(". category: '%s'\n",info->category);
          KODE_Vst3DPrint(". name: '%s'\n",info->name);
          KODE_Vst3DPrint(". classFlags: %i\n",info->classFlags);
          KODE_Vst3DPrint(". subCategories: '%s'\n",info->subCategories);
        return kode_vst3_ResultOk;
    }
    KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IPluginFactory3
//------------------------------

  int32_t KODE_VST3_PLUGIN_API getClassInfoUnicode(int32_t index, KODE_Vst3PClassInfoW* info) final {
    KODE_Vst3Print("index: %i -> False\n",index);
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setHostContext(KODE_Vst3FUnknown* context) final {
    KODE_Vst3Print("context: %p -> Ok\n",context);
    MHostContext = context;
    return kode_vst3_ResultOk;
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#define VST3_MAIN_SYMBOL asm ("GetPluginFactory");
KODE_Vst3IPluginFactory* KODE_VST3_PLUGIN_API vst3_entrypoint() VST3_MAIN_SYMBOL;

#define VST3_MODULE_ENTRY_SYMBOL asm ("ModuleEntry");
bool vst3_module_entry(void* sharedLibraryHandle) VST3_MODULE_ENTRY_SYMBOL;

#define VST3_MODULE_EXIT_SYMBOL asm ("ModuleExit");
bool vst3_module_exit(void) VST3_MODULE_EXIT_SYMBOL;

//----------

#define KODE_VST3_ENTRYPOINT(DESC,INST)                               \
                                                                      \
  __KODE_DLLEXPORT                                                    \
  KODE_Vst3IPluginFactory* KODE_VST3_PLUGIN_API vst3_entrypoint() {   \
    KODE_Vst3Print("\n");                                             \
    return KODE_New KODE_VST3Plugin<DESC,INST>();                     \
  }                                                                   \
                                                                      \
  void* moduleHandle = 0;                                             \
  /*int counter = 0;*/                                                \
  static int counter {0};                                             \
                                                                      \
  __KODE_DLLEXPORT                                                    \
  bool vst3_module_entry(void* sharedLibraryHandle) {                 \
    KODE_Vst3Print("\n");                                             \
    if (++counter == 1) {                                             \
      moduleHandle = sharedLibraryHandle;                             \
      /* init plugin here */                                          \
      return true;                                                    \
    }                                                                 \
    return true;                                                      \
  }                                                                   \
                                                                      \
  __KODE_DLLEXPORT                                                    \
  bool vst3_module_exit(void) {                                       \
    KODE_Vst3Print("\n");                                             \
    if (--counter == 0) {                                             \
      moduleHandle = nullptr;                                         \
      /* cleanup plugin here */                                       \
      return true;                                                    \
    }                                                                 \
    return true;                                                      \
  }

//----------------------------------------------------------------------
#endif
