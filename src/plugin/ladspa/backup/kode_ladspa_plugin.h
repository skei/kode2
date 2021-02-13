#ifndef kode_ladspa_plugin_included
#define kode_ladspa_plugin_included
//----------------------------------------------------------------------

// in progress

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance_listener.h"
#include "plugin/ladspa/kode_ladspa.h"
#include "plugin/ladspa/kode_ladspa_instance.h"
#include "plugin/ladspa/kode_ladspa_utils.h"

/*
#include "base/kode.h"
#include "plugin/ladspa/kode_ladspa.h"
#include "plugin/ladspa/kode_ladspa_host.h"
#include "plugin/ladspa/kode_ladspa_instance.h"
*/

//----------------------------------------------------------------------


template<class DESC, class INST>
class KODE_LadspaPlugin
: public KODE_InstanceListener {
  
//------------------------------
private:
//------------------------------
  
  KODE_Descriptor*      MDescriptor       = KODE_NULL;
  LADSPA_Descriptor*    MLadspaDescriptor = {0};
//LADSPA_Descriptor*    MLadspaDescriptor = KODE_NULL;

  //int*                  MPortDescriptors  = KODE_NULL;
  //char**                MPortNames        = KODE_NULL;
  //char*                 MPortNamesBuffer  = KODE_NULL;
  //LADSPA_PortRangeHint* MPortRangeHints   = KODE_NULL;
  
  KODE_LadspaPorts   MPorts;
  
  char*                 MNameBuffer       = KODE_NULL;
  char*                 MLabelBuffer      = KODE_NULL;
  char*                 MMakerBuffer      = KODE_NULL;
  char*                 MCopyrightBuffer  = KODE_NULL;
  
//------------------------------
public:
//------------------------------

  KODE_LadspaPlugin() {
    //KODE_TRACE;
  }
  
  //----------
  
  ~KODE_LadspaPlugin() {
    //KODE_TRACE;
    if (MDescriptor)        KODE_Delete MDescriptor;
    if (MNameBuffer)        KODE_Free(MNameBuffer);
    if (MLabelBuffer)       KODE_Free(MLabelBuffer);
    if (MMakerBuffer)       KODE_Free(MMakerBuffer);
    if (MCopyrightBuffer)   KODE_Free(MCopyrightBuffer);
    KODE_LadspaCleanupPorts(&MPorts);
    if (MLadspaDescriptor)  KODE_Free(MLadspaDescriptor);
  }
  
//------------------------------
public:
//------------------------------

  /*
    - at the moment, we only support index == 0
    - carla calles ladspa_descriptor a second time before deleting the first..
  */

  const LADSPA_Descriptor* entrypoint(unsigned long Index) {
    if (Index > 0) return KODE_NULL;
    if (MLadspaDescriptor) return MLadspaDescriptor;
    MDescriptor = KODE_New DESC();
    MLadspaDescriptor = (LADSPA_Descriptor*)KODE_Malloc(sizeof(LADSPA_Descriptor));
    KODE_Memset(MLadspaDescriptor,0,sizeof(LADSPA_Descriptor));
    //setDefaultParamValues();
    
    MNameBuffer       = (char*)KODE_Malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MLabelBuffer      = (char*)KODE_Malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MMakerBuffer      = (char*)KODE_Malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MCopyrightBuffer  = (char*)KODE_Malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);

    KODE_Strncpy(MNameBuffer,      MDescriptor->getName(),        KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    KODE_Strncpy(MLabelBuffer,     MDescriptor->getName(),        KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1); // todo: make valid c symbol (see lv2)
    KODE_Strncpy(MMakerBuffer,     MDescriptor->getAuthor(),      KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    KODE_Strncpy(MCopyrightBuffer, MDescriptor->getLicenseText(), KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    
    uint32_t numports = KODE_LadspaSetupPorts(MDescriptor,&MPorts);
    //KODE_Assert(port == numports);

    MLadspaDescriptor->UniqueID            = MDescriptor->getShortId();
    MLadspaDescriptor->Label               = MLabelBuffer;
    MLadspaDescriptor->Properties          = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN
    MLadspaDescriptor->Name                = MNameBuffer;
    MLadspaDescriptor->Maker               = MMakerBuffer;
    MLadspaDescriptor->Copyright           = MCopyrightBuffer;
    MLadspaDescriptor->PortCount           = numports;
    MLadspaDescriptor->PortDescriptors     = MPorts.descriptors;
    MLadspaDescriptor->PortNames           = (const char* const*)MPorts.names;
    MLadspaDescriptor->PortRangeHints      = MPorts.rangeHints;
    MLadspaDescriptor->ImplementationData  = this;//KODE_NULL;
    MLadspaDescriptor->instantiate         = ladspa_instantiate_callback;
    MLadspaDescriptor->connect_port        = ladspa_connect_port_callback;
    MLadspaDescriptor->activate            = ladspa_activate_callback;
    MLadspaDescriptor->run                 = ladspa_run_callback;
    MLadspaDescriptor->run_adding          = ladspa_run_adding_callback;
    MLadspaDescriptor->set_run_adding_gain = ladspa_set_run_adding_gain_callback;
    MLadspaDescriptor->deactivate          = ladspa_deactivate_callback;
    MLadspaDescriptor->cleanup             = ladspa_cleanup_callback;

    return MLadspaDescriptor;

  }
  
  //----------
  
  LADSPA_Handle instantiate(unsigned long SampleRate) {
    // instance deleted in ~KODE_LadspaInstance()
    KODE_Instance* instance = KODE_New INST(MDescriptor);
    instance->on_open();
    // ladspa_instance deleted in ladspa_cleanup_callback()
    KODE_LadspaInstance* ladspa_instance = KODE_New KODE_LadspaInstance(instance,SampleRate);
    return (LADSPA_Handle)ladspa_instance;
  }
  
//------------------------------
private: // ladspa callbacks
//------------------------------

  static
  LADSPA_Handle ladspa_instantiate_callback(const struct _LADSPA_Descriptor* Descriptor, unsigned long SampleRate) {
    KODE_LadspaPlugin* plugin = (KODE_LadspaPlugin*)Descriptor->ImplementationData;
    LADSPA_Handle instance = KODE_NULL;
    if (plugin) instance = plugin->instantiate(SampleRate);
    return (LADSPA_Handle)instance;    
  }

  //----------

  static
  void ladspa_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {
    //LADSPA_Trace("ladspa: ladspa_connect_port_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_connect_port(Port,DataLocation);
  }

  //----------

  static
  void ladspa_activate_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_activate_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_activate();
  }

  //----------

  static
  void ladspa_run_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: ladspa_run_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_run(SampleCount);
  }

  //----------

  static
  void ladspa_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: ladspa_run_adding_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_run_adding(SampleCount);
  }

  //----------

  static
  void ladspa_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain) {
    //LADSPA_Trace("ladspa: ladspa_set_run_adding_gain_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_set_run_adding_gain(Gain);
  }

  //----------

  static
  void ladspa_deactivate_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_deactivate_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_deactivate();
  }

  //----------

  static
  void ladspa_cleanup_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_cleanup_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_cleanup();
    //LADSPA_Trace("ladspa: cleanup -> deleting instance\n");
    delete ladspa_instance;
  }
  
//------------------------------

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_LADSPA_MAIN_SYMBOL asm ("ladspa_descriptor");
const LADSPA_Descriptor* kode_ladspa_entrypoint(unsigned long Index) KODE_LADSPA_MAIN_SYMBOL

//----------

#define KODE_LADSPA_MAIN(DESC,INST)                                       \
                                                                          \
  KODE_LadspaPlugin<DESC,INST> _LADSPA_PLUGIN;                            \
                                                                          \
  __KODE_DLLEXPORT                                                        \
  const LADSPA_Descriptor* kode_ladspa_entrypoint(unsigned long Index) {  \
    return _LADSPA_PLUGIN.entrypoint(Index);                              \
  }

//----------------------------------------------------------------------
#endif
