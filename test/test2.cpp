
#define KODE_GUI_XCB

//----------------------------------------------------------------------

#include "kode.h"
//#include "plugin/kode_plugin_base.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

class myEditor : public KODE_Editor {
public:
  myEditor(KODE_BaseInstance* AInstance, uint32_t AWidth, uint32_t AHeight, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AWidth,AHeight,AParent) {
    setFillBackground();
  }
};


//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {
public:
  myDescriptor() {
    MName = "myPlugin";
    MAuthor = "skei";
    MOptions.hasEditor = true;
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
    myEditor* editor = (myEditor*)KODE_New myEditor(this,320,240,AParent);
    return editor;
  }

  void  on_plugin_closeEditor(void* AEditor) override {
    KODE_Delete (myEditor*)AEditor;
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
