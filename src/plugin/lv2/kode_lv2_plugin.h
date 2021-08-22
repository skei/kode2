#ifndef kode_lv2_plugin_included
#define kode_lv2_plugin_included
//----------------------------------------------------------------------

#include "plugin/lv2/kode_lv2.h"
#include "plugin/lv2/kode_lv2_instance.h"
#include "plugin/lv2/kode_lv2_utils.h"

//----------------------------------------------------------------------

#ifdef KODE_DEBUG
  #define KODE_LV2PRINT
  #define KODE_Lv2Print  KODE_Print
  #define KODE_Lv2DPrint KODE_DPrint
#else
  #define KODE_LV2PRINT  KODE_PRINT
  #define KODE_Lv2Print  KODE_NoPrint
  #define KODE_Lv2DPrint KODE_NoPrint
#endif

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

  DESC            MDescriptor;

  LV2_Descriptor  MLv2Descriptor                          = {0};
  char            MLv2PluginUri[KODE_LV2_MAX_URI_LENGTH]  = {0};
  char            MLv2EditorUri[KODE_LV2_MAX_URI_LENGTH]  = {0};

//------------------------------
public:
//------------------------------

  KODE_Lv2Plugin() {
    KODE_LV2PRINT;
    setup_plugin_uri();
    #ifndef KODE_NO_GUI
    setup_editor_uri();
    #endif
    setup_lv2_descriptor();
  }

  //----------

  virtual ~KODE_Lv2Plugin() {
    KODE_LV2PRINT;
  }

//------------------------------
public:
//------------------------------

  const LV2_Descriptor* entrypoint(unsigned long Index) {
    KODE_Lv2Print("Index %i\n",Index);
    if (Index > 0) return KODE_NULL;
    return &MLv2Descriptor;
  }

  KODE_Descriptor* getDescriptor() {
    return &MDescriptor;
  }

  //----------

  void export_ttl(void) {
    KODE_LV2PRINT;
    #ifdef KODE_LV2_EXPORT_TTL
    KODE_Descriptor* descriptor = &MDescriptor;
    KODE_Lv2WriteManifest(descriptor);
    #endif
  }

//------------------------------
private:
//------------------------------

  //void setup_lv2_uri() {
  void setup_plugin_uri() {
    KODE_LV2PRINT;
    memset(MLv2PluginUri,0,KODE_LV2_MAX_URI_LENGTH+1);
    strcpy(MLv2PluginUri,"urn:");
    strcat(MLv2PluginUri,MDescriptor.getAuthor());
    strcat(MLv2PluginUri,"/");
    strcat(MLv2PluginUri,MDescriptor.getName());
  }

  //----------

  void setup_editor_uri() {
    KODE_LV2PRINT;
    memset(MLv2EditorUri,0,KODE_LV2_MAX_URI_LENGTH+1);
    strcpy(MLv2EditorUri,"urn:");
    strcat(MLv2EditorUri,MDescriptor.getAuthor());
    strcat(MLv2EditorUri,"/");
    strcat(MLv2EditorUri,MDescriptor.getName());
    strcat(MLv2EditorUri,"_editor");
  }

  //----------

  void setup_lv2_descriptor() {
    KODE_LV2PRINT;
    memset(&MLv2Descriptor,0,sizeof(LV2_Descriptor));
    MLv2Descriptor.URI             = MLv2PluginUri;
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
    KODE_Lv2Print("sample_rate %.2f bundle_path '%s' features %p\n",sample_rate,bundle_path,features);
    KODE_Lv2PrintFeatures(features);
    KODE_Descriptor* desc = kode_lv2_get_descriptor();
    KODE_Lv2Instance* lv2_instance = new KODE_Lv2Instance(desc);
    lv2_instance->lv2_setup(sample_rate,bundle_path,features);
    return (LV2_Handle)lv2_instance;
  }

  //----------

  static
  void lv2_connect_port_callback(LV2_Handle instance, uint32_t port, void* data_location) {
    KODE_Lv2Print("port %i data_location %p\n",port,data_location);
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_connect_port(port,data_location);
  }

  //----------

  static
  void lv2_activate_callback(LV2_Handle instance) {
    KODE_LV2PRINT;
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_activate();
  }

  //----------

  static
  void lv2_run_callback(LV2_Handle instance, uint32_t sample_count) {
    KODE_Lv2Print("sample_count %i\n",sample_count);
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_run(sample_count);
  }

  //----------

  static
  void lv2_deactivate_callback(LV2_Handle instance) {
    KODE_LV2PRINT;
    KODE_Lv2Instance* lv2_instance = (KODE_Lv2Instance*)instance;
    if (lv2_instance) lv2_instance->lv2_deactivate();
  }

  //----------

  static
  void lv2_cleanup_callback(LV2_Handle instance) {
    KODE_LV2PRINT;
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
    KODE_Lv2Print("uri %s\n",uri);
    return KODE_NULL;
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

#ifdef KODE_LV2_EXPORT_TTL
  #define KODE_LV2_EXPORT_TTL_SYMBOL asm ("lv2_export_ttl");
  void kode_lv2_export_ttl(void) KODE_LV2_EXPORT_TTL_SYMBOL
#endif

//----------

#define KODE_LV2_ENTRYPOINT(DESC,INST)                                \
                                                                      \
  KODE_Lv2Plugin<DESC,INST> _KODE_LV2_PLUGIN;                         \
                                                                      \
  __KODE_DLLEXPORT                                                    \
  const LV2_Descriptor* kode_lv2_entrypoint(unsigned long Index) {    \
    KODE_Lv2Print("Index %i\n",Index);                                \
    return _KODE_LV2_PLUGIN.entrypoint(Index);                        \
  }                                                                   \
                                                                      \
  __KODE_DLLEXPORT                                                    \
  void kode_lv2_export_ttl(void) {                                    \
    KODE_LV2PRINT;                                                    \
    _KODE_LV2_PLUGIN.export_ttl();                                    \
  }                                                                   \
                                                                      \
  /* called from lv2_instantiate_callback*/                           \
  KODE_Descriptor* kode_lv2_get_descriptor() {                        \
    KODE_LV2PRINT;                                                    \
    return _KODE_LV2_PLUGIN.getDescriptor();                          \
  }

//----------------------------------------------------------------------
#endif
