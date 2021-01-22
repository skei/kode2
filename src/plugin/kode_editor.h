#ifndef kode_editor_included
#define kode_editor_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor_listener.h"

#ifdef KODE_NO_GUI

  class KODE_Editor {
  };


//----------------------------------------------------------------------

#else // !KODE_NO_GUI

  #include "gui/kode_window.h"

  class KODE_Editor
  : public KODE_Window {

  //------------------------------
  private:
  //------------------------------

    KODE_Descriptor*      MDescriptor;
    KODE_EditorListener*  MListener;
    KODE_Widget**         MParameterToWidget;

  //------------------------------
  public:
  //------------------------------

    KODE_Editor(KODE_Descriptor* ADescriptor, KODE_EditorListener* AListener, void* AParent)
    : KODE_Window(ADescriptor->getEditorWidth(),ADescriptor->getEditorHeight(),"",AParent) {
      //KODE_PRINT;
      MDescriptor = ADescriptor;
      MListener = AListener;
      uint32_t num = ADescriptor->getNumParameters();
      MParameterToWidget = (KODE_Widget**)KODE_Malloc(num * sizeof(KODE_Widget*));
      KODE_Memset(MParameterToWidget,0,num * sizeof(KODE_Widget*));
    }

    //----------

    virtual ~KODE_Editor() {
      //KODE_PRINT;
      KODE_Free(MParameterToWidget);
    }

  //------------------------------
  public:
  //------------------------------

    //KODE_Window* getWindow() {
    //  return this;
    //}

  //------------------------------
  public:
  //------------------------------

    void connectParameter(KODE_Widget* AWidget, KODE_Parameter* AParameter) {
      AWidget->setParameter(AParameter);
      AWidget->on_widget_connect(AParameter);
      int32_t index = AParameter->getIndex();
      MParameterToWidget[index] = AWidget;
    }

    //----------

    void connectParameterIndex(KODE_Widget* AWidget, uint32_t AIndex) {
      KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
      AWidget->setParameter(parameter);
      AWidget->on_widget_connect(parameter);
      MParameterToWidget[AIndex] = AWidget;
    }

  //------------------------------
  public:
  //------------------------------

    void setParameterValue(uint32_t AIndex, float AValue) {
      KODE_Widget* widget = MParameterToWidget[AIndex];
      if (widget) {
        widget->setValue(AValue);
      }
    }

    //----------

    void setParameterValueAndRedraw(uint32_t AIndex, float AValue) {
      //KODE_Print("AIndex %i AValuer %.3f\n",AIndex,AValue);
      KODE_Widget* widget = MParameterToWidget[AIndex];
      if (widget) {
        //KODE_Print("index %i valuer %.3f\n",AIndex,AValue);
        widget->setValue(AValue);
        //paintWidget(widget);
        widget->do_widget_redraw(widget,widget->getRect(),0);
      }
    }

    //----------

    //void widgetUpdatedFromGui(KODE_Widget* AWidget) {
    void notifyInstanceParameterChanged(KODE_Widget* AWidget) {
      //KODE_PRINT;
      if (!MListener) return;
      KODE_Parameter* parameter = (KODE_Parameter*)AWidget->getParameter();
      if (parameter) {
        int32_t index = parameter->getIndex();
        float value = AWidget->getValue();
        MListener->updateParameterFromEditor(index,value);
      }
    }

  //------------------------------
  public:
  //------------------------------

  //------------------------------
  public: // "widget listener"
  //------------------------------

    //void do_widget_move(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) override {
    //}

    //----------

    //void do_widget_resize(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) override {
    //}

    //----------

    void do_widget_update(KODE_Widget* AWidget) override {
      //KODE_PRINT;
      //widgetUpdatedFromGui(AWidget);
      notifyInstanceParameterChanged(AWidget);
      KODE_Window::do_widget_update(AWidget);
    }

    //----------

    //void do_widget_redraw(KODE_Widget* AWidget, KODE_FRect ARect, uint32_t AMode=0) override {
    //  invalidate(ARect.x,ARect.y,ARect.w + 1,ARect.h + 1);
    //}

    //----------

    void do_widget_setHint(KODE_Widget* AWidget, const char* AHint) override {
      KODE_Widget::do_widget_setHint(AWidget,AHint);
    }

    //----------

    //void do_widget_setCursor(KODE_Widget* AWidget, uint32_t ACursor) override {
    //  setMouseCursor(ACursor);
    //}

    //----------

    //void do_widget_setModal(KODE_Widget* AWidget) override {
    //  MModalWidget = AWidget;
    //}

    //----------

    //void do_widget_keyCapture(KODE_Widget* AWidget) override {
    //  MKeyCaptureWidget = AWidget;
    //}

    //----------

    //void do_widget_mouseCapture(KODE_Widget* AWidget) override {
    //  MMouseCaptureWidget = AWidget;
    //}

    //----------

    //void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) override {
    //}

  //------------------------------
  public:
  //------------------------------

  };

#endif // !KODE_NO_GUI

//----------------------------------------------------------------------
#endif
