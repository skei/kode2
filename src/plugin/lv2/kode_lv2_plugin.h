#ifndef kode_lv2_plugin_included
#define kode_lv2_plugin_included
//----------------------------------------------------------------------

#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_instance.h"
#include "plugin/lv2/kode_lv2_utils.h"

#define KODE_PLUGIN_LV2_MAX_URI_LENGTH 256

//----------

template <class DESC, class INST>
class KODE_Lv2Plugin
/*: public KODE_Vst3IPluginFactory3*/ {

  friend const LV2_Descriptor* kode_lv2_entryPoint(unsigned long Index);

//------------------------------
private:
//------------------------------

  /*
    static beacuse the lv2_instantiate_callback accesses it..
  */

  static
  DESC            MDescriptor;


//------------------------------
private:
//------------------------------

  LV2_Descriptor  MLv2Descriptor  = {0};
  char            MLv2Uri[KODE_PLUGIN_LV2_MAX_URI_LENGTH] = {0};

//------------------------------
public:
//------------------------------

  KODE_Lv2Plugin() {
    //MLv2Uri = (char*)malloc(KODE_PLUGIN_LV2_MAX_URI_LENGTH+1);
    setup_lv2_uri();
    setup_lv2_descriptor();
  }

  //----------

  virtual ~KODE_Lv2Plugin() {
    //if (MLv2Descriptor) free(MLv2Descriptor);
    //free(MLv2Uri);
  }

//------------------------------
private:
//------------------------------

  void setup_lv2_uri() {
    memset(MLv2Uri,0,KODE_PLUGIN_LV2_MAX_URI_LENGTH+1);
    strcpy(MLv2Uri,"urn:");
    strcat(MLv2Uri,MDescriptor.getAuthor());
    strcat(MLv2Uri,"/");
    strcat(MLv2Uri,MDescriptor.getName());
  }

  //----------

  void setup_lv2_descriptor() {
    memset(&MLv2Descriptor,0,sizeof(LV2_Descriptor));
    MLv2Descriptor.URI             = MLv2Uri;
    MLv2Descriptor.instantiate     = lv2_instantiate_callback;
    MLv2Descriptor.connect_port    = lv2_connect_port_callback;
    MLv2Descriptor.activate        = lv2_activate_callback;
    MLv2Descriptor.run             = lv2_run_callback;
    MLv2Descriptor.deactivate      = lv2_deactivate_callback;
    MLv2Descriptor.cleanup         = lv2_cleanup_callback;
    MLv2Descriptor.extension_data  = lv2_extension_data_callback;
  }

  //----------

//----------------------------------------------------------------------
private: // lv2 callbacks
//----------------------------------------------------------------------

  /*
    todo: new/separate descriptor per instance?
    (so we don't ned to keep the MDescriptor static..)
    we could return a struct...

  */

  static
  LV2_Handle lv2_instantiate_callback(const struct _LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {

    KODE_Lv2Instance* lv2_instance = new KODE_Lv2Instance(MDescriptor,sample_rate);

    /*
    #ifdef KODE_DEBUG_LV2
    LV2_DTrace("	features:\n");
    KODE_uint32 i = 0;
    while ( features[i] ) {
      KODE_pchar uri = features[i]->URI;
      LV2_DTrace("	%i: %s\n",i,uri);
      i++;
    }
    #endif
    */

    LV2_URID_Map* map = KODE_NULL;
    for (int i=0; features[i]; ++i) {
      if (!strcmp(features[i]->URI, LV2_URID__map)) {
        map = (LV2_URID_Map*)features[i]->data;
        break;
      }
    }
    if (!map) return nullptr;

    if (MDescriptor.canReceiveMidi()) {
      LV2_URID lv2_midi_input_urid = map->map(map->handle, LV2_MIDI__MidiEvent);
      lv2_instance->_setMidiInputUrid(lv2_midi_input_urid);

    }

    return (LV2_Handle)lv2_instance;
  }

  //----------

  static
  void lv2_connect_port_callback(LV2_Handle instance, uint32_t port, void* data_location) {
    //KODE_Trace("lv2: lv2_connect_port_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_connect_port(port,data_location);
  }

  //----------

  static
  void lv2_activate_callback(LV2_Handle instance) {
    //KODE_Trace("lv2: lv2_activate_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_activate();
  }

  //----------

  static
  void lv2_run_callback(LV2_Handle instance, uint32_t sample_count) {
    //KODE_Trace("lv2: lv2_run_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_run(sample_count);
  }

  //----------

  static
  void lv2_deactivate_callback(LV2_Handle instance) {
    //KODE_Trace("lv2: lv2_deactivate_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_deactivate();
  }

  //----------

  static
  void lv2_cleanup_callback(LV2_Handle instance) {
    //KODE_Trace("lv2: lv2_cleanup_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_cleanup();
    delete lv2_instance;
  }

  //----------

  static
  const void* lv2_extension_data_callback(const char* uri) {
    //KODE_Trace("lv2: lv2_extension_data_callback: %s\n",uri);
    return nullptr;
  }

  //----------

  /*
    at the moment, we only support index == 0
  */

  LV2_Descriptor* lv2_entryPoint(uint32_t AIndex) {
    if (AIndex > 0) return nullptr;
    return &MLv2Descriptor;
  }

//------------------------------
public:
//------------------------------

  const LV2_Descriptor* entrypoint(unsigned long Index) {
    //KODE_TRACE;
    return &MLv2Descriptor;
  }

};

/*
  - carla calls lv2_descriptor a second time before deleting the first
*/

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#define KODE_LV2_MAIN_SYMBOL asm ("lv2_descriptor");
const LV2_Descriptor* kode_lv2_entrypoint(unsigned long Index) KODE_LV2_MAIN_SYMBOL

//----------

#define KODE_LV2_MAIN(DESC,INST)                                    \
                                                                    \
  KODE_LV2Plugin<DESC,INST> _KODE_LV2_PLUGIN;                       \
                                                                    \
  __KODE_DLLEXPORT                                                  \
  const LV2_Descriptor* kode_lv2_entrypoint(unsigned long Index) {  \
    return _KODE_LV2_PLUGIN.entrypoint(Index);                      \
  }

//----------------------------------------------------------------------
#endif
