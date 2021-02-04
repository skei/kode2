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
, public KODE_IEditor {

//------------------------------
private:
//------------------------------

  KODE_Descriptor*  MDescriptor         = KODE_NULL;
  KODE_IInstance*   MInstance           = KODE_NULL;
  KODE_Widget**     MParameterToWidget  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Editor(KODE_IInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Window(AInstance->getDescriptor()->getEditorWidth(),
                AInstance->getDescriptor()->getEditorHeight(),
                "", AParent ) {
    MDescriptor = AInstance->getDescriptor();
    uint32_t num = MDescriptor->getNumParameters();
    uint32_t size = num * sizeof(KODE_Widget*);
    MParameterToWidget = (KODE_Widget**)KODE_Malloc(size);
    KODE_Memset(MParameterToWidget,0,size);
  }

  //----------

  virtual ~KODE_Editor() {
    if (MParameterToWidget) KODE_Free(MParameterToWidget);
  }

//------------------------------
public:
//------------------------------

  //void setInstance(KODE_IInstance* AInstance) override {
  //}

//------------------------------
public:
//------------------------------

  void connectParameter(KODE_Widget* AWidget, uint32_t AIndex) override {
    MParameterToWidget[AIndex] = AWidget;
    KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
    AWidget->setParameter(parameter);
  }

  //----------

  void updateParameterFromHost(uint32_t AIndex, float AValue) override {
    //KODE_PRINT;
    KODE_Widget* widget= MParameterToWidget[AIndex];
    if (widget) {
      //KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
      //float v = parameter->from01(AValue);
      widget->setValue(AValue);
      widget->redraw();
    }
  }

};

//----------------------------------------------------------------------
#endif
