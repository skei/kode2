#ifndef kode_descriptor_included
#define kode_descriptor_included
//----------------------------------------------------------------------

#include "plugin/kode_parameter.h"
#include "plugin/kode_program.h"

//----------------------------------------------------------------------

struct KODE_PluginOptions {
  bool isSynth = false;
  bool hasEditor = false;
  bool canSendMidi = false;
  bool canReceiveMidi = false;
};

//----------

struct KODE_PluginPort {
  bool        isAudio; // else ?
  bool        isInput; // else output
  const char* name;
};

typedef KODE_Array<KODE_PluginPort*> KODE_PluginPorts;

//----------------------------------------------------------------------

class KODE_Descriptor {
public:
  const char*         name;
  const char*         author;
  uint32_t            version;
  KODE_PluginOptions  options;
  KODE_Parameters     parameters;
  KODE_Programs       programs;
  KODE_PluginPorts    inputs;
  KODE_PluginPorts    outputs;
public:
  virtual uint8_t* getLongId() { return KODE_NULL; }
};

//----------------------------------------------------------------------
#endif
