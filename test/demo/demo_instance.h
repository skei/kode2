#ifndef demo_instance_included
#define demo_instance_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_voice_manager.h"

#include "demo_voice.h"

#ifndef KODE_NO_GUI
#include "demo_editor.h"
#endif


//#define KODE_PLUGIN_MESSAGE_QUEUE_SIZE 32
//typedef KODE_Queue<uint32_t,KODE_PLUGIN_MESSAGE_QUEUE_SIZE> KODE_Uint32Queue;

//----------------------------------------------------------------------

//class demo_voice;
typedef KODE_VoiceManager<demo_voice,16> demo_voices;

//----------------------------------------------------------------------

class demo_instance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  #ifndef KODE_NO_GUI
  demo_editor*         MEditor       = KODE_NULL;
  #endif
  demo_voices          MVoices;

  //bool              MNeedRecalc   = false;
  //uint32_t          MCounter      = 0;

  //KODE_Uint32Queue  MProcessMessageQueue;
  //KODE_Uint32Queue  MGuiMessageQueue;


//------------------------------
public:
//------------------------------

  demo_instance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
public:
//------------------------------

  void on_plugin_open() final {
  }

  //----------

  void on_plugin_close() final {
  }

  //----------

  void on_plugin_initialize() final {
  }

  //----------

  void on_plugin_terminate() final {
  }

  //----------

  void on_plugin_activate() final {
    //MCounter = 0;
  }

  //----------

  void on_plugin_deactivate() final {
  }

  //----------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    MVoices.prepare(ASamplerate,ABlocksize);
  }

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("%i = %.3f\n",AIndex,AValue);
    //MNeedRecalc = true;
    //queueProcessMessage(AIndex);
    MVoices.parameter(AOffset,AIndex,AValue,AMode);
  }

  //----------

  void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
    MVoices.midi(AOffset,AMsg1,AMsg2,AMsg3,AMode);
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    //flushProcessMessages();

    //if (MNeedRecalc) {
    //  recalc(AContext);
    //  MNeedRecalc = false;
    //}

    MVoices.processBlock(AContext);

    //#ifndef KODE_NO_GUI
    //if (MEditor) {
    //  queueGuiMessage(MCounter);
    //}
    //#endif
    //
    //MCounter += 1;

  }

  //----------

  uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {
    *ABuffer = KODE_NULL;
    return 0;
  }

  //----------

  void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
  }

  //----------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    MEditor = new demo_editor(this,AParent);
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Assert(AEditor == MEditor);
    if (MEditor) {
      delete MEditor;
      MEditor = KODE_NULL;
    }
  }

  //----------

  void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Assert(AEditor == MEditor);
    //if (MEditor) {
    //  flushGuiMessages();
    //}
  }

  #endif

//------------------------------
private:
//------------------------------

  //void queueProcessMessage(uint32_t AMessage) {
  //  MProcessMessageQueue.write(AMessage);
  //}

  //----------

  //void flushProcessMessages() {
  //  uint32_t message = 0;
  //  while (MProcessMessageQueue.read(&message)) {
  //    KODE_Print("%i\n",message);
  //  }
  //}

  //----------

  //void queueGuiMessage(uint32_t AMessage) {
  //  MGuiMessageQueue.write(AMessage);
  //}

  //----------

  //void flushGuiMessages() {
  //  uint32_t message = 0;
  //  while (MGuiMessageQueue.read(&message)) {
  //    KODE_Print("%i\n",message);
  //  }
  //}

  //----------

  //void recalc(KODE_ProcessContext* AContext) {
  //  KODE_PRINT;
  //}

};

//----------------------------------------------------------------------
#endif






















  //----------

  /*
    process-buffer = cache-aligned (64 bytes)
    ticksize = 16 floats = 4 simd instructions = 64 bytes
    hardcode all processing
    a) 16 samples.. optimized.. 4 simd.. unrolled, consts..
    b) 1..15 for the remaining (if 'uneven' buffer)
  */


  /*
  void process_tick(int ANumSamples, float* AResult) {
    int remaining = ANumSamples;
    int offset = 0;
    preprocess_events();
    while (remaining > 0) {
      if (remaining >= TICKSIZE) {
        process_events(offset);
        //MProcess.process();
        //copy_buffer(AResult + offset,MProcess.getBuffer());
        remaining -= TICKSIZE;
        offset += TICKSIZE;
      }
      else {
        process_events(offset,remaining);
        //MProcess.process(remaining);
        //copy_buffer(AResult + offset,MProcess.getBuffer(),remaining);
        remaining = 0;
      }
    }
    postprocess_events();
  }
  */

