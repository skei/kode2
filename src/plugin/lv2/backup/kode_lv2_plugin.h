#ifndef kode_lv2_plugin_included
#define kode_lv2_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance_listener.h"
#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_instance.h"

//----------------------------------------------------------------------

template<class DESC, class INST>
class KODE_Lv2Plugin
: public KODE_InstanceListener {
  
private:
  
  KODE_Descriptor*  MDescriptor     = KODE_NULL;
  LV2_Descriptor    MLv2Descriptor  = {0};
  
public:

  KODE_Lv2Plugin() {
    //KODE_TRACE;
    MDescriptor = KODE_New DESC();
    //TODO: setup MLv2Descriptor
  }
  
  ~KODE_Lv2Plugin() {
    //KODE_TRACE;
    if (MDescriptor) KODE_Delete MDescriptor;
  }
  
public:

  const LV2_Descriptor* entrypoint(unsigned long Index) {
    KODE_TRACE;
    return &MLv2Descriptor;
  }
  
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_LV2_MAIN_SYMBOL asm ("lv2_descriptor");
const LV2_Descriptor* kode_lv2_entrypoint(unsigned long Index) KODE_LV2_MAIN_SYMBOL

//----------

#define KODE_LV2_MAIN(DESC,INST)          \
                                          \
  KODE_Lv2Plugin<DESC,INST> _LV2_PLUGIN;  \
                                          \
  __KODE_DLLEXPORT                                                    \
  const LV2_Descriptor* kode_lv2_entrypoint(unsigned long Index) {  \
    return _LV2_PLUGIN.entrypoint(Index);                            \
  }

//----------------------------------------------------------------------
#endif



















#if 0

#include "base/kode.h"
#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_instance.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_Lv2Plugin {

  friend const LV2_Descriptor* kode_lv2_entryPoint(unsigned long Index);

//------------------------------
private:
//------------------------------

  LV2_Descriptor* MLv2Descriptor  = nullptr;
  char*           MLv2Uri         = nullptr;

//------------------------------
public:
//------------------------------

  KODE_Lv2Plugin() {
    MLv2Uri = (char*)malloc(KODE_PLUGIN_LV2_MAX_URI_LENGTH+1);
    memset(MLv2Uri,0,KODE_PLUGIN_LV2_MAX_URI_LENGTH+1);
  }

  //----------

  ~KODE_Lv2Plugin() {
    if (MLv2Descriptor) free(MLv2Descriptor);
    free(MLv2Uri);
  }

//------------------------------
private:
//------------------------------

  /*
    at the moment, we only support index == 0
  */

  LV2_Descriptor* lv2_entryPoint(uint32_t AIndex) {
    if (AIndex > 0) return nullptr;
    // carla calles lv2_descriptor a second time before deleting the first..
    if (MLv2Descriptor) return MLv2Descriptor;

    KODE_Descriptor* descriptor = KODE_GetDescriptor();
    strcpy(MLv2Uri,"urn:");
    strcat(MLv2Uri,descriptor->getAuthor());
    strcat(MLv2Uri,"/");
    strcat(MLv2Uri,descriptor->getName());

    MLv2Descriptor = (LV2_Descriptor*)malloc(sizeof(LV2_Descriptor));
    memset(MLv2Descriptor,0,sizeof(LV2_Descriptor));

    MLv2Descriptor->URI             = MLv2Uri;
    MLv2Descriptor->instantiate     = lv2_instantiate_callback;
    MLv2Descriptor->connect_port    = lv2_connect_port_callback;
    MLv2Descriptor->activate        = lv2_activate_callback;
    MLv2Descriptor->run             = lv2_run_callback;
    MLv2Descriptor->deactivate      = lv2_deactivate_callback;
    MLv2Descriptor->cleanup         = lv2_cleanup_callback;
    MLv2Descriptor->extension_data  = lv2_extension_data_callback;

    return MLv2Descriptor;
  }

//----------------------------------------------------------------------
private: // lv2 callbacks
//----------------------------------------------------------------------

  static
  LV2_Handle lv2_instantiate_callback(const struct _LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {
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
    KODE_Descriptor* desc = KODE_GetDescriptor();
    KODE_Lv2Instance* lv2_instance = new KODE_Lv2Instance(desc,sample_rate);

    LV2_URID_Map* map = NULL;
    for (int i=0; features[i]; ++i) {
      if (!strcmp(features[i]->URI, LV2_URID__map)) {
        map = (LV2_URID_Map*)features[i]->data;
        break;
      }
    }
    if (!map) return nullptr;
    if (desc->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
      LV2_URID lv2_midi_input_urid = map->map(map->handle, LV2_MIDI__MidiEvent);
      lv2_instance->setMidiInputUrid(lv2_midi_input_urid);
    }

    return (LV2_Handle)lv2_instance;
  }

  //----------

  static
  void lv2_connect_port_callback(LV2_Handle instance, uint32_t port, void* data_location) {
    //LV2_Trace("lv2: lv2_connect_port_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_connect_port(port,data_location);
  }

  //----------

  static
  void lv2_activate_callback(LV2_Handle instance) {
    //LV2_Trace("lv2: lv2_activate_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_activate();
  }

  //----------

  static
  void lv2_run_callback(LV2_Handle instance, uint32_t sample_count) {
    //LV2_Trace("lv2: lv2_run_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_run(sample_count);
  }

  //----------

  static
  void lv2_deactivate_callback(LV2_Handle instance) {
    //LV2_Trace("lv2: lv2_deactivate_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_deactivate();
  }

  //----------

  static
  void lv2_cleanup_callback(LV2_Handle instance) {
    //LV2_Trace("lv2: lv2_cleanup_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_cleanup();
    delete lv2_instance;
  }

  //----------

  static
  const void* lv2_extension_data_callback(const char* uri) {
    //LV2_Trace("lv2: lv2_extension_data_callback\n");
    return nullptr;
  }

};

//----------------------------------------------------------------------
// .so entry point
//----------------------------------------------------------------------

#define KODE_LV2_MAIN_SYMBOL asm ("lv2_descriptor");
const LV2_Descriptor* kode_lv2_entryPoint(unsigned long Index) KODE_LV2_MAIN_SYMBOL

//----------

KODE_Lv2Plugin LV2_PLUGIN;

//----------

__KODE_DLLEXPORT
const LV2_Descriptor* kode_lv2_entryPoint(unsigned long Index) {
  const LV2_Descriptor* descriptor = LV2_PLUGIN.lv2_entryPoint(Index);
  return descriptor;
}


#endif // 0