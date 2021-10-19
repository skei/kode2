#ifndef kode_clap_plugin_included
#define kode_clap_plugin_included
//----------------------------------------------------------------------

/*
  https://github.com/free-audio/clap
  https://www.kvraudio.com/forum/viewtopic.php?p=8240683#p8240683
  https://news.ycombinator.com/item?id=8809659
*/

#include "kode.h"
#include "plugin/clap/kode_clap.h"

//----------------------------------------------------------------------

template <class DESC, class INST>
class KODE_ClapPlugin {
};

//----------------------------------------------------------------------


//----------------------------------------------------------------------

bool clap_init(const char *plugin_path) {
  //_KODE_CLAP_PLUGIN.init();
  return true;
}

//----------

void clap_deinit(void) {
}

//----------

uint32_t clap_get_plugin_count(void) {
  return 1;
}

//----------

const clap_plugin_descriptor* clap_get_plugin_descriptor(uint32_t index) {
  clap_plugin_descriptor* descriptor = (clap_plugin_descriptor*)malloc(sizeof(clap_plugin_descriptor));
  return descriptor;
}

//----------

const clap_plugin* clap_create_plugin(const clap_host *host, const char *plugin_id) {
  clap_plugin* plugin = (clap_plugin*)malloc(sizeof(clap_plugin));
  return plugin;
}

//----------

uint32_t clap_get_invalidation_source_count(void) {
  return 0;
}

//----------

const clap_plugin_invalidation_source* clap_get_invalidation_source(uint32_t index) {
  //clap_plugin_invalidation_source* invalidation_source = (clap_plugin_invalidation_source*)malloc(sizeof(clap_plugin_invalidation_source));
  //return invalidation_source;
  return KODE_NULL;
}

//----------

void clap_refresh(void)  {
}


//----------------------------------------------------------------------

__KODE_DLLEXPORT

//CLAP_EXPORT
const struct clap_plugin_entry clap_plugin_entry = {
   CLAP_VERSION,
   clap_init,
   clap_deinit,
   clap_get_plugin_count,
   clap_get_plugin_descriptor,
   clap_create_plugin,
   clap_get_invalidation_source_count,
   clap_get_invalidation_source,
   clap_refresh,
};

//----------------------------------------------------------------------

#define KODE_LV2_ENTRYPOINT(DESC,INST)                \
                                                      \
  KODE_ClapPlugin<DESC,INST> _KODE_CLAP_PLUGIN;       \


//----------------------------------------------------------------------

//template <class DESC, class INST>
//class KODE_ClapPlugin {
//
////------------------------------
//private:
////------------------------------
//
////  DESC                            MDescriptor;
//
////  clap_plugin_descriptor          MClapDescriptor;
////  clap_plugin_invalidation_source MClapInvalidationSource;
////  // instance
////  clap_plugin                     MClapPlugin;
//
////------------------------------
//public:
////------------------------------
//
//  KODE_ClapPlugin() {
//    clap_plugin_entry.clap_version.major            = 0;
//    clap_plugin_entry.clap_version.minor            = 0;
//    clap_plugin_entry.clap_version.revision         = 0;
//    clap_plugin_entry.init                          = clap_init;
//    clap_plugin_entry.deinit                        = clap_deinit;
//    clap_plugin_entry.get_plugin_count              = clap_get_plugin_count;
//    clap_plugin_entry.get_plugin_descriptor         = clap_get_plugin_descriptor;
//    clap_plugin_entry.create_plugin                 = clap_create_plugin;
//    clap_plugin_entry.get_invalidation_source_count = clap_get_invalidation_source_count;
//    clap_plugin_entry.get_invalidation_source       = clap_get_invalidation_source;
//    clap_plugin_entry.refresh                       = clap_refresh;
//
//
//
//    //KODE_GLOBAL.appendModule(new DESC());
//  }
//
//  //----------
//
//  virtual ~KODE_ClapPlugin() {
//  }
//
////------------------------------
//private:
////------------------------------
//
//  static
//  bool clap_init(const char *plugin_path) {
//    return true;
//  }
//
//  //----------
//
//  static
//  void clap_deinit(void) {
//  }
//
//  //----------
//
//  static
//  uint32_t clap_get_plugin_count(void) {
//    return 1;
//  }
//
//  //----------
//
//  static
//  const clap_plugin_descriptor clap_get_plugin_descriptor(uint32_t index) {
//  }
//
//  //----------
//
//  static
//  const clap_plugin clap_create_plugin(const clap_host *host, const char *plugin_id) {
//  }
//
//  //----------
//
//  static
//  uint32_t clap_get_invalidation_source_count(void) {
//    return 0;
//  }
//
//  //----------
//
//  static
//  const clap_plugin_invalidation_source clap_get_invalidation_source(uint32_t index) {
//  }
//
//  //----------
//
//  static
//  void clap_refresh(void)  {
//  }
//
////------------------------------
//public:
////------------------------------
//
//  /*
//  bool clap_entrypoint() {
//    clap_plugin_entry.clap_version.major            = 0;
//    clap_plugin_entry.clap_version.minor            = 0;
//    clap_plugin_entry.clap_version.revision         = 0;
//    clap_plugin_entry.init                          = clap_init;
//    clap_plugin_entry.deinit                        = clap_deinit;
//    clap_plugin_entry.get_plugin_count              = clap_get_plugin_count;
//    clap_plugin_entry.get_plugin_descriptor         = clap_get_plugin_descriptor;
//    clap_plugin_entry.create_plugin                 = clap_create_plugin;
//    clap_plugin_entry.get_invalidation_source_count = clap_get_invalidation_source_count;
//    clap_plugin_entry.get_invalidation_source       = clap_get_invalidation_source;
//    clap_plugin_entry.refresh                       = clap_refresh;
//    return true;
//  }
//  */
//
//};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#if 0

/*
  CLAP_EXPORT extern const struct clap_plugin_entry clap_plugin_entry;
*/

#define KODE_CLAP_MAIN_SYMBOL asm ("clap_plugin_entry");
const struct clap_plugin_entry kode_clap_entrypoint(unsigned long Index) KODE_CLAP_MAIN_SYMBOL

//----------

#define KODE_LV2_ENTRYPOINT(DESC,INST)                        \
                                                              \
  KODE_ClapPlugin<DESC,INST> _KODE_CLAP_PLUGIN;               \
  /*bool initialized = _KODE_CLAP_PLUGIN.clap_entrypoint();*/ \
                                                              \
  __KODE_DLLEXPORT                                            \
  clap_plugin_entry _KODE_CLAP_PLUGIN_ENTRY;

#endif // 0

//----------------------------------------------------------------------
#endif
