#ifndef kode_exe_instance_included
#define kode_exe_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_editor_listener.h"
#include "plugin/base/kode_base_instance.h"
#include "plugin/exe/kode_exe.h"

#ifndef KODE_NO_GUI
  #include "plugin/kode_editor.h"
#endif

//----------------------------------------------------------------------

class KODE_ExeInstance
: public KODE_BaseInstance
, public KODE_EditorListener {

//------------------------------
private:
//------------------------------

  #ifndef KODE_NO_GUI
    KODE_Editor* MEditor = KODE_NULL;
  #endif

//------------------------------
public:
//------------------------------

  KODE_ExeInstance(KODE_Descriptor* ADescriptor)
  : KODE_BaseInstance(ADescriptor) {
    KODE_EXEPRINT;
  }

  //----------

  virtual ~KODE_ExeInstance() {
    KODE_EXEPRINT;
  }

//------------------------------
public: // editor listener
//------------------------------

  void updateParameterFromEditor(uint32_t AIndex, float AValue) final {
    //KODE_PRINT;
    on_parameter(AIndex,AValue,0);
  }

  void resizeWindowFromEditor(uint32_t AWidth, uint32_t AHeight) final {
    //KODE_PRINT;
  }


//------------------------------
public:
//------------------------------

  void run() {
    KODE_EXEPRINT;

    setDefaultParameterValues();
    //updateAllParameters();
    notifyAllParameters();

    on_initialize();
    on_prepare(44100);
    on_activate();
    #ifndef KODE_NO_GUI
    if (MDescriptor->hasEditor()) {
      //MEditor->updateAllParameters();
      MEditor = on_openEditor();
      KODE_Window* window = (KODE_Window*)MEditor;//->getWindow();
      window->open();
      window->eventLoop();
      window->close();
      on_closeEditor(MEditor);
    }
    #endif
    on_deactivate();
    on_terminate();
  }


};

//----------------------------------------------------------------------
#endif

