#ifndef kode_editor_included
#define kode_editor_included
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI
#include "gui/kode_window.h"
#endif

#include "plugin/kode_plugin_base.h"

//----------------------------------------------------------------------

class KODE_Editor
: public KODE_Window
, public KODE_BaseEditor {

//------------------------------
public:
//------------------------------

  KODE_Editor(KODE_BaseInstance* AInstance, /*uint32_t AWidth, uint32_t AHeight,*/ void* AParent=KODE_NULL)
  //: KODE_Window(AWidth,AHeight,"",AParent) {
  : KODE_Window(
      AInstance->getDescriptor()->getEditorWidth(),
      AInstance->getDescriptor()->getEditorHeight(),
      "",
      AParent
    ) {
  }

  //----------

  virtual ~KODE_Editor() {
  }

//------------------------------
public:
//------------------------------

  void setInstance(KODE_BaseInstance* AInstance) override {
  }

  void updateParameterFromHost(uint32_t AIndex, float AValue) override {
  }

};

//----------------------------------------------------------------------
#endif
