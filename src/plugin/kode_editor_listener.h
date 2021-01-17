#ifndef kode_editor_listener_included
#define kode_editor_listener_included
//----------------------------------------------------------------------

// implemented in kode_instance

#include "base/kode.h"

class KODE_EditorListener {
public:
  virtual void updateParameterFromEditor(uint32_t AIndex, float AValue) {}
  virtual void resizeWindowFromEditor(uint32_t AWidth, uint32_t AHeight) {}
  //virtual void saveStateFromEditor(uint32_t ASize, void* ABuffer) {}
};

//----------------------------------------------------------------------
#endif
