#ifndef kode_lv2_plugin_included
#define kode_lv2_plugin_included
//----------------------------------------------------------------------

#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_instance.h"
#include "plugin/lv2/kode_lv2_utils.h"

//#define KODE_PLUGIN_LV2_DUMPTTL
#define KODE_PLUGIN_LV2_MAX_URI_LENGTH 256

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

KODE_Descriptor* kode_lv2_get_descriptor();

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

template <class DESC, class INST>
class KODE_Lv2Plugin {

  friend const LV2_Descriptor* kode_lv2_entryPoint(unsigned long Index);
  friend void kode_lv2_dumpttl();

//------------------------------
private:
//------------------------------

  /*
    static because the lv2_instantiate_callback accesses it..
  */

  DESC MDescriptor;
  //static DESC MDescriptor;


//------------------------------
private:
//------------------------------

  LV2_Descriptor  MLv2Descriptor                          = {0};
  char            MLv2Uri[KODE_PLUGIN_LV2_MAX_URI_LENGTH] = {0};

//------------------------------
public:
//------------------------------

  KODE_Lv2Plugin() {
    KODE_Print("\n");
    setup_lv2_uri();
    setup_lv2_descriptor();
  }

  //----------

  virtual ~KODE_Lv2Plugin() {
    KODE_Print("\n");
  }

//------------------------------
public:
//------------------------------

  const LV2_Descriptor* entrypoint(unsigned long Index) {
    KODE_Print("Index %i\n",Index);
    if (Index > 0) return KODE_NULL;
    return &MLv2Descriptor;
  }

  KODE_Descriptor* getDescriptor() {
    return &MDescriptor;
  }

  //----------

  //void dump_ttl(void) {
  //  #ifdef KODE_PLUGIN_LV2_DUMPTTL
  //  KODE_Descriptor* descriptor = &MDescriptor;
  //  _kode_lv2_dump_ttl(descriptor);
  //  #endif
  //}

//------------------------------
private:
//------------------------------

  void setup_lv2_uri() {
    KODE_Print("\n");
    memset(MLv2Uri,0,KODE_PLUGIN_LV2_MAX_URI_LENGTH+1);
    strcpy(MLv2Uri,"urn:");
    strcat(MLv2Uri,MDescriptor.getAuthor());
    strcat(MLv2Uri,"/");
    strcat(MLv2Uri,MDescriptor.getName());
  }

  //----------

  void setup_lv2_descriptor() {
    KODE_Print("\n");
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
     Instantiate the plugin.

     Note that instance initialisation should generally occur in activate()
     rather than here. If a host calls instantiate(), it MUST call cleanup()
     at some point in the future.

     @param descriptor Descriptor of the plugin to instantiate.

     @param sample_rate Sample rate, in Hz, for the new plugin instance.

     @param bundle_path Path to the LV2 bundle which contains this plugin
     binary. It MUST include the trailing directory separator so that simply
     appending a filename will yield the path to that file in the bundle.

     @param features A NULL terminated array of LV2_Feature structs which
     represent the features the host supports. Plugins may refuse to
     instantiate if required features are not found here. However, hosts MUST
     NOT use this as a discovery mechanism: instead, use the RDF data to
     determine which features are required and do not attempt to instantiate
     unsupported plugins at all. This parameter MUST NOT be NULL, i.e. a host
     that supports no features MUST pass a single element array containing
     NULL.

     @return A handle for the new plugin instance, or NULL if instantiation
     has failed.
  */

  //-----

  /*
    todo: new/separate descriptor per instance?
    (so we don't ned to keep the MDescriptor static..)
    we could return a struct...

    todo2: send the features pointer to the instance constructor, and
    let it sort it out itself?

  */

  static
  //LV2_Handle lv2_instantiate_callback(const struct _LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {
  LV2_Handle lv2_instantiate_callback(const LV2_Descriptor* descriptor, double sample_rate, const char* bundle_path, const LV2_Feature* const* features) {
    KODE_Print("\n");
    KODE_DPrint("  sample_rate %.2f\n  bundle_path '%s'\n  features %p\n",sample_rate,bundle_path,features);
    KODE_Print("sample_rate %.2f bundle_path '%s' features %p\n",sample_rate,bundle_path,features);
    kode_lv2_dump_features(features);
    KODE_Descriptor* desc = kode_lv2_get_descriptor();
    KODE_Lv2Instance* lv2_instance = new KODE_Lv2Instance(desc);
    lv2_instance->lv2_setup(sample_rate,bundle_path,features);
    return (LV2_Handle)lv2_instance;
  }

  //----------

  static
  void lv2_connect_port_callback(LV2_Handle instance, uint32_t port, void* data_location) {
    KODE_Print("port %i data_location %p\n",port,data_location);
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_connect_port(port,data_location);
  }

  //----------

  static
  void lv2_activate_callback(LV2_Handle instance) {
    KODE_Print("\n");
    //KODE_Print("lv2: lv2_activate_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_activate();
  }

  //----------

  static
  void lv2_run_callback(LV2_Handle instance, uint32_t sample_count) {
    KODE_Print("sample_count %i\n",sample_count);
    //KODE_Print("lv2: lv2_run_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_run(sample_count);
  }

  //----------

  static
  void lv2_deactivate_callback(LV2_Handle instance) {
    KODE_Print("\n");
    //KODE_Print("lv2: lv2_deactivate_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_deactivate();
  }

  //----------

  static
  void lv2_cleanup_callback(LV2_Handle instance) {
    KODE_Print("\n");
    //KODE_Print("lv2: lv2_cleanup_callback\n");
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_cleanup();
    delete lv2_instance;
  }

  //----------

  /*
     Return additional plugin data defined by some extension.

     A typical use of this facility is to return a struct containing function
     pointers to extend the LV2_Descriptor API.

     The actual type and meaning of the returned object MUST be specified
     precisely by the extension. This function MUST return NULL for any
     unsupported URI. If a plugin does not support any extension data, this
     field may be NULL.

     The host is never responsible for freeing the returned value.
  */

  static
  const void* lv2_extension_data_callback(const char* uri) {
    KODE_Print("uri %s\n",uri);
    return nullptr;
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

/*
   Prototype for plugin accessor function.

   Plugins are discovered by hosts using RDF data (not by loading libraries).
   See http://lv2plug.in for details on the discovery process, though most
   hosts should use an existing library to implement this functionality.

   This is the simple plugin discovery API, suitable for most statically
   defined plugins.  Advanced plugins that need access to their bundle during
   discovery can use lv2_lib_descriptor() instead.  Plugin libraries MUST
   include a function called "lv2_descriptor" or "lv2_lib_descriptor" with
   C-style linkage, but SHOULD provide "lv2_descriptor" wherever possible.

   When it is time to load a plugin (designated by its URI), the host loads the
   plugin's library, gets the lv2_descriptor() function from it, and uses this
   function to find the LV2_Descriptor for the desired plugin.  Plugins are
   accessed by index using values from 0 upwards.  This function MUST return
   NULL for out of range indices, so the host can enumerate plugins by
   increasing `index` until NULL is returned.

   Note that `index` has no meaning, hosts MUST NOT depend on it remaining
   consistent between loads of the plugin library.
*/

#define KODE_LV2_MAIN_SYMBOL asm ("lv2_descriptor");
const LV2_Descriptor* kode_lv2_entrypoint(unsigned long Index) KODE_LV2_MAIN_SYMBOL

//#define KODE_LV2_DUMPTTL_SYMBOL asm ("lv2_dumpttl");
//void kode_lv2_dumpttl(void) KODE_LV2_DUMPTTL_SYMBOL

//----------

#define KODE_LV2_ENTRYPOINT(DESC,INST)                                \
                                                                      \
  KODE_Lv2Plugin<DESC,INST> _KODE_LV2_PLUGIN;                         \
                                                                      \
  __KODE_DLLEXPORT                                                    \
  const LV2_Descriptor* kode_lv2_entrypoint(unsigned long Index) {    \
    KODE_Print("Index %i\n",Index);                                   \
    return _KODE_LV2_PLUGIN.entrypoint(Index);                        \
  }                                                                   \
                                                                      \
  KODE_Descriptor* kode_lv2_get_descriptor() {                        \
    return _KODE_LV2_PLUGIN.getDescriptor();                          \
  }                                                                   \


//__KODE_DLLEXPORT
//void kode_lv2_dumpttl(void) {
//  KODE_Print("");
//  _KODE_LV2_PLUGIN.dump_ttl();
//}


//----------------------------------------------------------------------
#endif
