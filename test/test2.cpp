
#define KODE_GUI_XCB
//#define KODE_NO_WINDOW_BUFFERING

//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_plugin.h"
#include "plugin/vst3/kode_vst3_instance.h"

//----------------------------------------------------------------------

class myEditor : public KODE_Editor {

public:

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {
    setFillBackground();
    KODE_PRINT;
  }

  void on_window_paint(uint32_t AXpos, uint32_t AYpos, uint32_t AWidth, uint32_t AHeight) final {
    KODE_PRINT
    KODE_Editor::on_window_paint(AXpos,AYpos,AWidth,AHeight);
  }

  void on_widget_paint(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    KODE_PRINT;
    KODE_Widget::on_widget_paint(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {
public:
  myDescriptor() {
    MName = "myPlugin";
    MAuthor = "skei";
    MOptions.hasEditor = true;
    MEditorWidth = 640;
    MEditorHeight = 480;
  }
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

public:

  void* on_plugin_openEditor(void* AParent) override {
    KODE_PRINT;
    myEditor* editor = (myEditor*)KODE_New myEditor(this,AParent);
    return editor;
  }

  void  on_plugin_closeEditor(void* AEditor) override {
    KODE_PRINT;
    KODE_Delete (myEditor*)AEditor;
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
