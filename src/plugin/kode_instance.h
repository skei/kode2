#ifndef kode_instance_included
#define kode_instance_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_editor.h"

//----------

#ifdef KODE_PLUGIN_DSSI
  #include "plugin/dssi/kode_dssi_instance.h"
  typedef KODE_DssiInstance KODE_ImplementedInstance;
#endif

#ifdef KODE_PLUGIN_EXE
  #include "plugin/exe/kode_exe_instance.h"
  typedef KODE_ExeInstance KODE_ImplementedInstance;
#endif

#ifdef KODE_PLUGIN_LADSPA
  #include "plugin/ladspa/kode_ladspa_instance.h"
  typedef KODE_LadspaInstance KODE_ImplementedInstance;
#endif

#ifdef KODE_PLUGIN_LV2
  #include "plugin/lv2/kode_lv2_instance.h"
  typedef KODE_Lv2Instance KODE_ImplementedInstance;
#endif

#ifdef KODE_PLUGIN_VST2
  #include "plugin/vst2/kode_vst2_instance.h"
  typedef KODE_Vst2Instance KODE_ImplementedInstance;
#endif

#ifdef KODE_PLUGIN_VST3
  #include "plugin/vst3/kode_vst3_instance.h"
  typedef KODE_Vst3Instance KODE_ImplementedInstance;
#endif

//----------

//#define KODE_PLUGIN_MESSAGE_QUEUE_SIZE 32
//typedef KODE_Queue<uint32_t,KODE_PLUGIN_MESSAGE_QUEUE_SIZE> KODE_Uint32Queue;

//----------------------------------------------------------------------

class KODE_Instance
: public KODE_ImplementedInstance {

//------------------------------
private:
//------------------------------

//  KODE_Uint32Queue  MProcessMessageQueue;
//  KODE_Uint32Queue  MGuiMessageQueue;

//------------------------------
public:
//------------------------------

  KODE_Instance(KODE_Descriptor* ADescriptor)
  : KODE_ImplementedInstance(ADescriptor) {
  }

  //----------

  virtual ~KODE_Instance() {
  }

////------------------------------
//public:
////------------------------------
//
//  void queueProcessMessage(uint32_t AMessage) {
//    MProcessMessageQueue.write(AMessage);
//  }
//
//  void flushProcessMessages() {
//    uint32_t message = 0;
//    while (MProcessMessageQueue.read(&message)) {
//      // handle message
//      KODE_Print("%i\n",message);
//    }
//  }
//
//  void queueGuiMessage(uint32_t AMessage) {
//    MGuiMessageQueue.write(AMessage);
//  }
//
//  void flushGuiMessages() {
//    uint32_t message = 0;
//    while (MGuiMessageQueue.read(&message)) {
//      // handle message
//      KODE_Print("%i\n",message);
//    }
//  }

};


//----------------------------------------------------------------------
#endif
