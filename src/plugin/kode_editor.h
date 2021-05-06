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
private:
//------------------------------

  KODE_Descriptor*    MDescriptor         = KODE_NULL;
  KODE_BaseInstance*  MInstance           = KODE_NULL;
  KODE_Widget**       MParameterToWidget  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Editor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Window(AInstance->getDescriptor()->getEditorWidth(),AInstance->getDescriptor()->getEditorHeight(),"",AParent)
  , KODE_BaseEditor(AInstance) {
    MDescriptor = AInstance->getDescriptor();
    MInstance = AInstance;
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

  //uint32_t appendWidget(KODE_Widget* AWidget)

//------------------------------
public:
//------------------------------

  void do_widget_update(KODE_Widget* AWidget) override {
    KODE_Parameter* parameter = AWidget->getParameter();
    if (parameter) {
      int32_t index = parameter->getIndex();
      float value = AWidget->getValue();
      //if (value != MParameterValues[index]) {
      //  MParameterValues[index] = value;
        MInstance->updateParameterFromEditor(index,value);
      //}
    }
  }

//------------------------------
public:
//------------------------------

  void connectParameter(KODE_Widget* AWidget, uint32_t AParamIndex, uint32_t ASubIndex=0) override {
    //if (!AWidget) return;
    KODE_Assert(AWidget);
    KODE_Parameter* parameter = MDescriptor->getParameter(AParamIndex);
    if (parameter) {
      MParameterToWidget[AParamIndex] = AWidget;

      AWidget->setText( parameter->getName() );

      AWidget->MParameter = parameter;
      AWidget->MParameters[ASubIndex] = parameter;
      AWidget->on_widget_connect(parameter,ASubIndex);
    }
  }

  //----------

  void updateParameterFromHost(uint32_t AIndex, float AValue, bool ARedraw=true) override {
    //KODE_PRINT;
    KODE_Widget* widget= MParameterToWidget[AIndex];
    if (widget) {
      //KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
      //float v = parameter->from01(AValue);
      //MEditorParameterValues[AIndex] = AValue;
      //widget->setValue(AValue);
      widget->MValue = AValue;
      if (ARedraw) widget->redraw();
    }
  }

};

//----------------------------------------------------------------------
#endif
