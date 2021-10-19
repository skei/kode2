#ifndef kode_clap_instance_included
#define kode_clap_instance_included
//----------------------------------------------------------------------

/*
  https://github.com/free-audio/clap
  https://www.kvraudio.com/forum/viewtopic.php?p=8240683#p8240683
  https://news.ycombinator.com/item?id=8809659
*/

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_process_context.h"
#include "plugin/clap/kode_clap.h"

// not #include, to avoid cyclic dependencies..
class KODE_BaseEditor;

//----------------------------------------------------------------------

class KODE_BaseInstance {
public:
  KODE_BaseInstance(KODE_Descriptor* ADescriptor) {}
  virtual ~KODE_BaseInstance() {}
public:
  virtual KODE_Descriptor*  getDescriptor() { return KODE_NULL; }
  virtual const char*       getHostName() { return ""; }
public:
  virtual void              setDefaultParameterValues() {}
  virtual void              updateAllParameters() {}
  virtual void              updateAllEditorParameters(KODE_BaseEditor* AEditor, bool ARedraw=true) {}
  virtual void              updateParameterFromEditor(uint32_t AIndex, float AValue) {}
  //virtual KODE_BaseEditor*  getEditor() { return KODE_NULL; }
public:
  virtual void              on_plugin_open() {}
  virtual void              on_plugin_close() {}
  virtual void              on_plugin_initialize() {}
  virtual void              on_plugin_terminate() {}
  virtual void              on_plugin_activate() {}
  virtual void              on_plugin_deactivate() {}
  virtual void              on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) {}
  virtual void              on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) {}
  virtual void              on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) {}
  virtual void              on_plugin_process(KODE_ProcessContext* AContext) {}
  virtual uint32_t          on_plugin_saveState(void** ABuffer, uint32_t AMode) { *ABuffer = KODE_NULL; return 0; }
  virtual void              on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) {}
  virtual KODE_BaseEditor*  on_plugin_openEditor(void* AParent) { return KODE_NULL; }
  virtual void              on_plugin_closeEditor(KODE_BaseEditor* AEditor) {}
  virtual void              on_plugin_updateEditor(KODE_BaseEditor* AEditor) {}
};

  const clap_plugin clap_create_plugin(const clap_host *host, const char *plugin_id) {
    DESC* descriptor = new DESC();
    INST* instance = new INST(descriptor);
    return instance->getClapPlugin(host,plugin_id);


//----------------------------------------------------------------------
#endif






















/* bitfield
 * This gives an hint to the host what the plugin might do. */
enum {
   /* Instruments can play notes, and generate audio */
   CLAP_PLUGIN_INSTRUMENT = (1 << 0),

   /* Audio effects, process audio input and produces audio.
    * Exemple: delay, reverb, compressor. */
   CLAP_PLUGIN_AUDIO_EFFECT = (1 << 1),

   /* Event effects, takes events as input and produces events.
    * Exemple: arpegiator */
   CLAP_PLUGIN_EVENT_EFFECT = (1 << 2), // can be seen as midi effect

   // Analyze audio and/or events.
   // If this is the only type reported by the plugin, the host can assume that it wont change the
   // audio and event signal.
   CLAP_PLUGIN_ANALYZER = (1 << 3),
};
typedef int32_t clap_plugin_type;

typedef struct clap_plugin_descriptor {
   clap_version clap_version; // initialized to CLAP_VERSION

   const char *id;          // eg: "com.u-he.diva"
   const char *name;        // eg: "Diva"
   const char *vendor;      // eg: "u-he"
   const char *url;         // eg: "https://u-he.com/products/diva/"
   const char *manual_url;  // eg: "https://dl.u-he.com/manuals/plugins/diva/Diva-user-guide.pdf"
   const char *support_url; // eg: "https://u-he.com/support/"
   const char *version;     // eg: "1.4.4"
   const char *description; // eg: "The spirit of analogue"

   // Arbitrary list of keywords, separated by `;'
   // They can be matched by the host search engine and used to classify the plugin.
   // Some examples:
   // "master;eq;spectrum"
   // "compressor;analog;character"
   // "reverb;plate;cathedral"
   // "kick;analog;808;roland"
   // "analog;character;roland;moog"
   // "chip;chiptune;gameboy;nintendo;sega"
   const char *keywords;

   uint64_t plugin_type; // bitfield of clap_plugin_type
} clap_plugin_descriptor;

typedef struct clap_plugin {
   const clap_plugin_descriptor *desc;

   void *plugin_data; // reserved pointer for the plugin

   // Must be called after creating the plugin.
   // If init returns false, the host must destroy the plugin instance.
   bool (*init)(const struct clap_plugin *plugin);

   /* Free the plugin and its resources.
    * It is not required to deactivate the plugin prior to this call. */
   void (*destroy)(const struct clap_plugin *plugin);

   /* activation/deactivation
    * [main-thread] */
   bool (*activate)(const struct clap_plugin *plugin, double sample_rate);
   void (*deactivate)(const struct clap_plugin *plugin);

   // Set to true before processing, and to false before sending the plugin to sleep.
   // [audio-thread]
   bool (*start_processing)(const struct clap_plugin *plugin);
   void (*stop_processing)(const struct clap_plugin *plugin);

   /* process audio, events, ...
    * [audio-thread] */
   clap_process_status (*process)(const struct clap_plugin *plugin, const clap_process *process);

   /* Query an extension.
    * The returned pointer is owned by the plugin.
    * [thread-safe] */
   const void *(*get_extension)(const struct clap_plugin *plugin, const char *id);

   // Called by the host on the main thread in response to a previous call to:
   //   host->request_callback(host);
   // [main-thread]
   void (*on_main_thread)(const struct clap_plugin *plugin);
} clap_plugin;
