#ifndef kode_dssi_plugin_included
#define kode_dssi_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/dssi/kode_dssi.h"

//----------------------------------------------------------------------

#define KODE_DSSI_MAIN_SYMBOL asm ("dssi_descriptor");
const DSSI_Descriptor* kode_dssi_entryPoint(unsigned long Index) KODE_DSSI_MAIN_SYMBOL

//----------

__KODE_DLLEXPORT
const DSSI_Descriptor* kode_dssi_entryPoint(unsigned long Index) {
  return nullptr;
}

//----------------------------------------------------------------------
#endif











#if 0

/*
  A DSSI synth plugin consists of a LADSPA plugin plus an additional framework
  for controlling program settings and transmitting MIDI events.
  A plugin must fully implement the LADSPA descriptor fields as well as the
  required LADSPA functions including instantiate() and (de)activate().
  It should also implement run(), with the same behaviour as if run_synth()
  were called with no synth events.

  In order to instantiate a synth the host calls the LADSPA instantiate
  function, passing in this LADSPA_Descriptor pointer.  The returned
  LADSPA_Handle is used as the argument for the DSSI functions below as well as
  for the LADSPA ones.
*/

//----------------------------------------------------------------------

#include "core/kode.h"
#include "extern/kode_dssi.h"
#include "plugin/base/kode_base_plugin.h"
#include "plugin/base/kode_base_instance.h"
#include "plugin/dssi/kode_dssi_instance.h"
#include "plugin/dssi/kode_dssi_utils.h"

#include "plugin/ladspa/kode_ladspa_utils.h"

//#include "plugin/ladspa/kode_ladspa_instance.h"
//#include "plugin/kode_parameter.h"


//----------------------------------------------------------------------

const DSSI_Descriptor*          kode_dssi_entryPoint(unsigned long Index);
LADSPA_Handle                   kode_dssi_createInstance(unsigned long SampleRate);

LADSPA_Handle                   dssi_instantiate_callback(const struct _LADSPA_Descriptor * Descriptor, unsigned long SampleRate);
void                            dssi_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data* DataLocation);
void                            dssi_activate_callback(LADSPA_Handle Instance);
void                            dssi_run_callback(LADSPA_Handle Instance, unsigned long SampleCount);
//void                            dssi_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount);
//void                            dssi_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain);
void                            dssi_deactivate_callback(LADSPA_Handle Instance);
void                            dssi_cleanup_callback(LADSPA_Handle Instance);

char*                           dssi_configure(LADSPA_Handle Instance, const char* Key, const char* Value);
const DSSI_Program_Descriptor*  dssi_get_program(LADSPA_Handle Instance, unsigned long Index);
void                            dssi_select_program(LADSPA_Handle Instance, unsigned long Bank, unsigned long Program);
int                             dssi_get_midi_controller_for_port(LADSPA_Handle Instance, unsigned long Port);
void                            dssi_run_synth(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t* Events, unsigned long EventCount);
void                            dssi_run_synth_adding(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t *Events, unsigned long EventCount);
//void                          dssi_run_multiple_synths(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts);
//void                          dssi_run_multiple_synths_adding(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts);

//----------------------------------------------------------------------

class KODE_DssiPlugin
: public KODE_BasePlugin {

  friend const DSSI_Descriptor* kode_dssi_entryPoint(unsigned long Index);
  friend LADSPA_Handle          kode_dssi_createInstance(unsigned long SampleRate);

  //------------------------------
  //
  //------------------------------

private:

    bool              MInitialized = false;
    DSSI_Descriptor   MDssiDescriptor   = {0};
    LADSPA_Descriptor MLadspaDescriptor = {0};
    ladspa_port_data  MPortData;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_DssiPlugin()
    : KODE_BasePlugin() {
      //KODE_TRACE;
    }

    //----------

    /*
      free MDssiDescriptor (allocated in dssi_entryPoint)
      delete _PLUGIN (created in dssi_entryPoint)
    */

    ~KODE_DssiPlugin() {
      //KODE_TRACE;
      ladspa_cleanup_port_data(&MPortData);
      //if (MLadspaDescriptor) KODE_Free(MLadspaDescriptor);
      //if (MDssiDescriptor) KODE_Free(MDssiDescriptor);
      //if (MPlugin) KODE_Delete MPlugin;
    }

  //------------------------------
  //
  //------------------------------

  /*
    at the moment, we only support index == 0
    new _PLUGIN
    malloc MLadspaDescriptor
  */

  private:

    const DSSI_Descriptor* dssi_entryPoint(KODE_uint32 AIndex) {
      DSSI_Trace("dssi: entryPoint %i\n",AIndex);
      if (AIndex > 0) { KODE_Trace("AIndex > 0\n"); return nullptr; }

      if (!MInitialized) {

      //if (MDssiDescriptor) {
      //  KODE_Trace("MDssiDescriptor already allocated!\n");
      //  return MDssiDescriptor;
      //}
      //if (!MPlugin) {
      //  MPlugin = KODE_New _PLUGIN();
      //}
      //if (MPlugin) {
        //MDssiDescriptor = (DSSI_Descriptor*)KODE_Malloc(sizeof(DSSI_Descriptor));
        KODE_Memset(&MDssiDescriptor,0,sizeof(DSSI_Descriptor));
        //MLadspaDescriptor = (LADSPA_Descriptor*)KODE_Malloc(sizeof(LADSPA_Descriptor));
        KODE_Memset(&MLadspaDescriptor,0,sizeof(LADSPA_Descriptor));
        KODE_uint32 numports = ladspa_setup_port_data(this,&MLadspaDescriptor,&MPortData);
        //on_plugin_initialize();
        //MParameters.clear();
        //MPlugin->on_create();
        //setDefaultParamValues();
        MLadspaDescriptor.UniqueID                   = getShortId();
        MLadspaDescriptor.Label                      = MPortData.MLabelBuffer;
        MLadspaDescriptor.Properties                 = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN
        MLadspaDescriptor.Name                       = MPortData.MNameBuffer;
        MLadspaDescriptor.Maker                      = MPortData.MMakerBuffer;
        MLadspaDescriptor.Copyright                  = MPortData.MCopyrightBuffer;
        MLadspaDescriptor.PortCount                  = numports;
        MLadspaDescriptor.PortDescriptors            = MPortData.MPortDescriptors;
        MLadspaDescriptor.PortNames                  = (const char* const*)MPortData.MPortNames;
        MLadspaDescriptor.PortRangeHints             = MPortData.MPortRangeHints;
        MLadspaDescriptor.ImplementationData         = this;//nullptr;
        MLadspaDescriptor.instantiate                = dssi_instantiate_callback;
        MLadspaDescriptor.connect_port               = dssi_connect_port_callback;
        MLadspaDescriptor.activate                   = dssi_activate_callback;
        MLadspaDescriptor.run                        = dssi_run_callback;
        MLadspaDescriptor.run_adding                 = nullptr;//dssi_run_adding_callback;
        MLadspaDescriptor.set_run_adding_gain        = nullptr;//dssi_set_run_adding_gain_callback;
        MLadspaDescriptor.deactivate                 = dssi_deactivate_callback;
        MLadspaDescriptor.cleanup                    = dssi_cleanup_callback;

        MDssiDescriptor.DSSI_API_Version             = 1;
        MDssiDescriptor.LADSPA_Plugin                = &MLadspaDescriptor;
        MDssiDescriptor.configure                    = dssi_configure;
        MDssiDescriptor.get_program                  = dssi_get_program;
        MDssiDescriptor.select_program               = dssi_select_program;
        MDssiDescriptor.get_midi_controller_for_port = dssi_get_midi_controller_for_port;
        MDssiDescriptor.run_synth                    = dssi_run_synth;
        MDssiDescriptor.run_synth_adding             = nullptr;//dssi_run_synth_adding;
        MDssiDescriptor.run_multiple_synths          = nullptr;//dssi_run_multiple_synths;
        MDssiDescriptor.run_multiple_synths_adding   = nullptr;//dssi_run_multiple_synths_adding;
        MInitialized = true;
      }
      return &MDssiDescriptor;
    }

    //----------

    LADSPA_Handle dssi_createInstance(unsigned long SampleRate) {
      DSSI_Trace("dssi: createInstance %i\n",SampleRate);
      //KODE_Plugin* plugin = (KODE_Plugin*)MPlugin;
      //KODE_Instance* instance = (KODE_Instance*)KODE_New _INSTANCE(plugin); // deleted in KODE_DssiInstance destructor
      KODE_DssiInstance* instance = (KODE_DssiInstance*)createPluginInstance(nullptr);
      if (instance) {
        instance->setSampleRate(SampleRate);
        instance->on_stateChange(kps2_open);
        instance->on_stateChange(kps2_initialize);
      }
      //KODE_DssiInstance* dssiinstance = KODE_New KODE_DssiInstance(plugin,instance);
      //return (LADSPA_Handle)dssiinstance;
      return instance;
    }

};

//----------------------------------------------------------------------
// ladspa callbacks
//----------------------------------------------------------------------

// new _INSTANCE

LADSPA_Handle dssi_instantiate_callback(const struct _LADSPA_Descriptor* Descriptor, unsigned long SampleRate) {
	KODE_DssiInstance* instance = (KODE_DssiInstance*)kode_dssi_createInstance(SampleRate);
	return (LADSPA_Handle)instance;
}

//----------

void dssi_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) instance->dssi_connect_port(Port,DataLocation);
}

//----------

void dssi_activate_callback(LADSPA_Handle Instance) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) instance->dssi_activate();
}

//----------

void dssi_run_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) instance->dssi_run(SampleCount);
}

//----------

//void dssi_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
//  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
//  if (instance) instance->dssi_run_adding(SampleCount);
//}

//----------

//void dssi_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain) {
//  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
//  if (instance) instance->dssi_set_run_adding_gain(Gain);
//}

//----------

void dssi_deactivate_callback(LADSPA_Handle Instance) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) instance->dssi_deactivate();
}

//----------

void dssi_cleanup_callback(LADSPA_Handle Instance) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) instance->dssi_cleanup();
  //DSSI_Trace("dssi: cleanup -> deleting instance\n");
  KODE_Delete instance;
}

//----------------------------------------------------------------------
// dssi callbacks
//----------------------------------------------------------------------

char* dssi_configure(LADSPA_Handle Instance, const char* Key, const char* Value) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) return instance->dssi_configure(Key,Value);
  return nullptr;
}

//----------

const DSSI_Program_Descriptor* dssi_get_program(LADSPA_Handle Instance, unsigned long Index) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) return instance->dssi_get_program(Index);
  return nullptr;
}

//----------

void dssi_select_program(LADSPA_Handle Instance, unsigned long Bank, unsigned long Program) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) instance->dssi_select_program(Bank,Program);
}

//----------

int dssi_get_midi_controller_for_port(LADSPA_Handle Instance, unsigned long Port) {
  return 0;
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) return instance->dssi_get_midi_controller_for_port(Port);
}

//----------

void dssi_run_synth(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t* Events, unsigned long EventCount) {
  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
  if (instance) instance->dssi_run_synth(SampleCount,Events,EventCount);
}

//----------

//void dssi_run_synth_adding(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t *Events, unsigned long EventCount) {
//  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
//  if (instance) instance->dssi_run_synth_adding(SampleCount,Events,EventCount);
//}

//----------

//void dssi_run_multiple_synths(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
//  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
//  if (instance) instance->dssi_run_multiple_synths(InstanceCount,Instances,SampleCount,Events,EventCounts);
//}

//----------

//void dssi_run_multiple_synths_adding(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
//  KODE_DssiInstance* instance = (KODE_DssiInstance*)Instance;
//  if (instance) instance->dssi_run_multiple_synths_adding(InstanceCount,Instances,SampleCount,Events,EventCounts);
//}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_DSSI_MAIN_SYMBOL asm ("dssi_descriptor");
const DSSI_Descriptor* kode_dssi_entryPoint(unsigned long Index) KODE_DSSI_MAIN_SYMBOL

//----------

#define KODE_MAIN(PLUG)                                           \
                                                                      \
  PLUG _DSSI_PLUGIN;                                  \
                                                                      \
  _KODE_DLLEXPORT                                                     \
  const DSSI_Descriptor* kode_dssi_entryPoint(unsigned long Index) {  \
    return _DSSI_PLUGIN.dssi_entryPoint(Index);                       \
  }                                                                   \
                                                                      \
  LADSPA_Handle kode_dssi_createInstance(unsigned long SampleRate) {  \
    return _DSSI_PLUGIN.dssi_createInstance(SampleRate);              \
  }


#endif // 0
