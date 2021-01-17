#ifndef kode_plugin_event_included
#define kode_plugin_event_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

// KODE_PLUGIN_EVENT_PARAMETER

struct KODE_ParameterEvent {
  uint32_t  index;
  float     value;
};

// KODE_PLUGIN_EVENT_MIDI

struct KODE_MidiEvent {
  uint8_t   msg1;
  uint8_t   msg2;
  uint8_t   msg3;
  uint8_t   dummy;
};

//----------------------------------------------------------------------

struct KODE_PluginEvent {
  uint32_t  offset;
  uint32_t  type;
  union {
    KODE_ParameterEvent parameter;
    KODE_MidiEvent      midi;
  };
};

//----------------------------------------------------------------------
#endif
