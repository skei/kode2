#ifndef kode_editor_included
#define kode_editor_included
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI
  #include "gui/kode_window.h"
#endif

#include "plugin/base/kode_base_editor.h"
#include "plugin/base/kode_base_instance.h"
#include "plugin/kode_instance.h"

//----------------------------------------------------------------------

class KODE_Editor
: public KODE_Window
, public KODE_BaseEditor {

//------------------------------
protected:
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

    //KODE_Print("AInstance %p AParent %p\n",AInstance,AParent);
    MDescriptor = AInstance->getDescriptor();
    MInstance = AInstance;
    uint32_t num = MDescriptor->getNumParameters();
    uint32_t size = num * sizeof(KODE_Widget*);
    MParameterToWidget = (KODE_Widget**)malloc(size);
    memset(MParameterToWidget,0,size);
  }

  //----------

  virtual ~KODE_Editor() {
    //KODE_PRINT;
    if (MParameterToWidget) free(MParameterToWidget);
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
    //KODE_Print("AWidget %p\n",AWidget);
    KODE_Parameter* parameter = AWidget->getParameter();
    if (parameter) {
      int32_t index = parameter->getIndex();
      float value = AWidget->getValue();
      //KODE_DPrint("  index %i value %.3f\n",index,value);
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
    //KODE_Print("AWidget %p AParamIndex %i ASubIndex %i\n",AWidget,AParamIndex,ASubIndex);
    //if (!AWidget) return;
    KODE_Assert(AWidget);
    KODE_Parameter* parameter = MDescriptor->getParameter(AParamIndex);
    if (parameter) {
      MParameterToWidget[AParamIndex] = AWidget;
      //AWidget->setText( parameter->getName() );
      AWidget->setParameter(parameter);
      //AWidget->MParameters[ASubIndex] = parameter;
      AWidget->on_widget_connect(parameter,ASubIndex);
    }
  }

  //----------

  void updateParameterFromHost(uint32_t AIndex, float AValue, bool ARedraw=true) override {
    //KODE_Print("AIndex %i AValue %.3f ARedraw %i\n",AIndex,AValue,ARedraw);
    KODE_Widget* widget= MParameterToWidget[AIndex];
    if (widget) {
      //KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
      //float v = parameter->from01(AValue);
      //MEditorParameterValues[AIndex] = AValue;
      //widget->setValue(AValue);

      //widget->MValue = AValue;
      widget->setValue(AValue);

      if (ARedraw) widget->redraw();
    }
  }

};

//----------------------------------------------------------------------
#endif
