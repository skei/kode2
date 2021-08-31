#ifndef kode_default_editor_included
#define kode_default_editor_included
//----------------------------------------------------------------------

#include "plugin/kode_editor.h"
//#include "plugin/base/kode_base_instance.h"

//----------------------------------------------------------------------

class KODE_DefaultEditor
: public KODE_Editor {

//------------------------------
private:
//------------------------------

  //KODE_Descriptor*    MDescriptor         = KODE_NULL;
  //KODE_BaseInstance*  MInstance           = KODE_NULL;
  //KODE_Widget**       MParameterToWidget  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_DefaultEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {
    //MDescriptor = AInstance->getDescriptor();
    //MInstance = AInstance;
    //uint32_t num = MDescriptor->getNumParameters();
    //uint32_t size = num * sizeof(KODE_Widget*);
    //MParameterToWidget = (KODE_Widget**)malloc(size);
    //memset(MParameterToWidget,0,size);
  }

  //----------

  virtual ~KODE_DefaultEditor() {
    //if (MParameterToWidget) free(MParameterToWidget);
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

  //void do_widget_update(KODE_Widget* AWidget) override {
  //  KODE_Parameter* parameter = AWidget->getParameterPtr();
  //  if (parameter) {
  //    int32_t index = parameter->getIndex();
  //    float value = AWidget->getValue();
  //    //if (value != MParameterValues[index]) {
  //    //  MParameterValues[index] = value;
  //      MInstance->updateParameterFromEditor(index,value);
  //    //}
  //  }
  //}

//------------------------------
public:
//------------------------------

  //void connectParameter(KODE_Widget* AWidget, uint32_t AParamIndex, uint32_t ASubIndex=0) override {
  //  //if (!AWidget) return;
  //  KODE_Assert(AWidget);
  //  KODE_Parameter* parameter = MDescriptor->getParameter(AParamIndex);
  //  if (parameter) {
  //    MParameterToWidget[AParamIndex] = AWidget;
  //    //AWidget->setText( parameter->getName() );
  //    AWidget->setParameterPtr(parameter);
  //    AWidget->MParameters[ASubIndex] = parameter;
  //    AWidget->on_widget_connect(parameter,ASubIndex);
  //  }
  //}

  //----------

  //void updateParameterFromHost(uint32_t AIndex, float AValue, bool ARedraw=true) override {
  //  //KODE_PRINT;
  //  KODE_Widget* widget= MParameterToWidget[AIndex];
  //  if (widget) {
  //    //KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
  //    //float v = parameter->from01(AValue);
  //    //MEditorParameterValues[AIndex] = AValue;
  //    //widget->setValue(AValue);
  //    widget->MValue = AValue;
  //    if (ARedraw) widget->redraw();
  //  }
  //}

};

//----------------------------------------------------------------------
#endif

