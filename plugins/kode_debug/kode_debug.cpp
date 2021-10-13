
//#define KODE_NO_GUI
#define KODE_GUI_XCB

#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET

// nc -U -l -k /tmp/kode.socket

//----------

#include "kode.h"
#include "audio/kode_audio_utils.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

#include "kode_debug_descriptor.h"
#include "kode_debug_editor.h"
#include "kode_debug_voice.h"

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  myEditor* MEditor = KODE_NULL;
  myVoices  MVoices;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
public:
//------------------------------

  void on_plugin_open() final {
    KODE_PRINT;
  }

  //----------

  void on_plugin_close() final {
    KODE_PRINT;
  }

  //----------

  void on_plugin_initialize() final {
    KODE_PRINT;
  }

  //----------

  void on_plugin_terminate() final {
    KODE_PRINT;
  }

  //----------

  void on_plugin_activate() final {
    KODE_PRINT;
  }

  //----------

  void on_plugin_deactivate() final {
    KODE_PRINT;
  }

  //----------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    MVoices.prepare(ASamplerate,ABlocksize);
  }

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    MVoices.parameter(AOffset,AIndex,AValue,AMode);
  }

  //----------

  void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
    MVoices.midi(AOffset,AMsg1,AMsg2,AMsg3,AMode);
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    MVoices.processBlock(AContext);
    if (MEditor) MEditor->process(AContext);
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
    MEditor = new myEditor(this,AParent);
    #ifndef KODE_PLUGIN_EXE
      MEditor->host_pid = getHostPid();
      MEditor->host_tid = getHostTid();
    #endif
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    if (MEditor) {
      delete MEditor;
      MEditor = KODE_NULL;
    }
  }

  //----------

  void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
    if (MEditor) {
      MEditor->xcb_pid = _kode_xcb_event_thread_pid;
      MEditor->xcb_tid = _kode_xcb_event_thread_tid;
      MEditor->gui_pid = getpid();
      MEditor->gui_tid = gettid();
      MEditor->update();
    }
  }

  #endif // KODE_NO_GUI

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
