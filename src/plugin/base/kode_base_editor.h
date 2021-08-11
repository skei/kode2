#ifndef kode_base_editor_included
#define kode_base_editor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widget.h"

// not #include, to avoid cyclic dependencies..
class KODE_BaseInstance;

//----------------------------------------------------------------------

class KODE_BaseEditor {
public:
  KODE_BaseEditor(KODE_BaseInstance* AInstance) {}
  virtual ~KODE_BaseEditor() {}
public:
  virtual void  connectParameter(KODE_Widget* AWidget, uint32_t AParameter, uint32_t AParamIndex=0) {}
  virtual void  updateParameterFromHost(uint32_t AIndex, float AValue, bool ARedraw=true) {}
};

//----------------------------------------------------------------------
#endif

