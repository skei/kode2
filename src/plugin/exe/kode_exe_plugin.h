#ifndef kode_exe_plugin_included
#define kode_exe_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "audio/io/kode_portaudio.h"
#include "base/system/kode_time.h"

#include "plugin/kode_descriptor.h"
//#include "plugin/kode_editor.h"
#include "plugin/kode_instance.h"
#include "plugin/exe/kode_exe.h"

#ifndef KODE_NO_GUI
  #include "gui/kode_window.h"
#endif


//----------------------------------------------------------------------

template <class DESC, class INST>
class KODE_ExePlugin
: public KODE_PortAudioListener {

//------------------------------
private:
//------------------------------

  KODE_Descriptor*    MDescriptor = KODE_NULL;
  KODE_Instance*      MInstance   = KODE_NULL;

  KODE_PortAudio      MPortAudio;
  KODE_ProcessContext MProcessContext;

  #ifndef KODE_NO_GUI
  KODE_Editor*        MEditor     = KODE_NULL;
  #endif

//------------------------------
public:
//------------------------------

  KODE_ExePlugin() {
    KODE_EXEPRINT;
    //MPortAudio.initialize();
    //MPortAudio.printDevices();
  }

  //----------

  virtual ~KODE_ExePlugin() {
    KODE_EXEPRINT;
    //MPortAudio.terminate();
  }

//------------------------------
private:
//------------------------------

  void portaudio_callback(KODE_PortAudioContext* AContext) final {
    // set up MProcessContext..
    MInstance->on_processBlock(&MProcessContext);
  }

//------------------------------
private:
//------------------------------

  void startAudio() {
    //MPortAudio.openDefault(44100,256,KODE_NULL);
    //MPortAudio.start();
  }

  //----------

  void stopAudio() {
    //MPortAudio.stop();
    //MPortAudio.close();
  }

//------------------------------
public:
//------------------------------

  virtual int main(int argc, char** argv) {
    KODE_EXEPRINT;
    startAudio();
    MDescriptor = KODE_New DESC();
    MInstance = KODE_New INST(MDescriptor);
    MInstance->on_create();
    MInstance->run();

//    MInstance->on_initialize();
//    MInstance->on_prepare(44100);
//    MInstance->on_activate();
//    if (MDescriptor->hasEditor()) {
//      MEditor = MInstance->on_openEditor();
//      KODE_Window* window = MEditor->getWindow();
//      window->open();
//      window->eventLoop();
//      window->close();
//      MInstance->on_closeEditor(MEditor);
//    }
//    MInstance->on_deactivate();
//    MInstance->on_terminate();

    MInstance->on_destroy();
    KODE_Delete MInstance;
    KODE_Delete MDescriptor;
    stopAudio();
    return 0;
  }

//------------------------------

};

//----------------------------------------------------------------------

#define KODE_EXE_PLUGIN_ENTRYPOINT(DESC,INST)   \
                                                \
KODE_ExePlugin<DESC,INST> _KODE_EXE_PLUGIN;     \
                                                \
int main(int argc, char** argv) {               \
  KODE_EXEPRINT;                                \
  return _KODE_EXE_PLUGIN.main(argc,argv);      \
}

//----------------------------------------------------------------------
#endif

