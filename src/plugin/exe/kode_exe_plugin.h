#ifndef kode_exe_plugin_included
#define kode_exe_plugin_included
//----------------------------------------------------------------------

#include "plugin/exe/kode_exe.h"

//----------------------------------------------------------------------

template <typename DESC, typename INST>
class KODE_ExePlugin {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  KODE_ExePlugin() {
  }

  //----------

  ~KODE_ExePlugin() {
    //if (MDescriptor) KODE_Delete MDescriptor;
    //if (MInstance) KODE_Delete MInstance;
    //#ifndef KODE_NO_GUI
    //if (MEditor) KODE_Delete MEditor;
    //#endif
  }

//------------------------------
public:
//------------------------------

  int main(int argc, char** argv) {
    KODE_Descriptor* descriptor = KODE_New DESC();
    KODE_Instance* instance = KODE_New INST(descriptor);
    #ifndef KODE_NO_GUI
    if (descriptor->hasEditor()) {
      KODE_Editor* editor = (KODE_Editor*)instance->on_plugin_openEditor(KODE_NULL);
      editor->open();
      editor->eventLoop();
      editor->close();
      instance->on_plugin_closeEditor(editor);
    }
    #endif
    KODE_Delete instance;
    KODE_Delete descriptor;
    return 0;
  }

};


//----------------------------------------------------------------------

#define KODE_EXE_ENTRYPOINT(D,I)            \
                                            \
int main(int argc, char** argv) {           \
  KODE_ExePlugin<D,I> EXE_PLUGIN;           \
  int result = EXE_PLUGIN.main(argc,argv);  \
  return result;                            \
}

//----------------------------------------------------------------------
#endif
