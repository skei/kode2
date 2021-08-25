
#define KODE_GUI_XCB
//#define KODE_NO_WINDOW_BUFFERING

//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_plugin.h"
#include "plugin/vst3/kode_vst3_instance.h"

//----------------------------------------------------------------------

class myEditor : public KODE_Editor {

private:

  KODE_Bitmap* MBitmap = KODE_NULL;

public:

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {
    setFillBackground();
    //KODE_PRINT;

    appendWidget( KODE_New KODE_PanelWidget( KODE_FRect(10,10,100,20) ));

    appendWidget( KODE_New KODE_PanelWidget( KODE_FRect(10,35,100,20) ));

    KODE_ValueWidget* v;
    v = (KODE_ValueWidget*)appendWidget( KODE_New KODE_ValueWidget( KODE_FRect(10,60,100,20) ));
    v->setValue(0.5f);
    //v->setDrawLabel();
    v->setLabel("db");

    MBitmap = KODE_New KODE_Bitmap(100,100);
    MBitmap->fill(0x80);

    KODE_ImageWidget* i;
    i = (KODE_ImageWidget*)appendWidget( KODE_New KODE_ImageWidget( KODE_FRect(10,85,100,100) ));
    i->setBitmap(MBitmap);

    //

    KODE_TextWidget* tw;

    tw = (KODE_TextWidget*)appendWidget( KODE_New KODE_TextWidget(  KODE_FRect(115,10,100,35) ));
    //tw->setDrawBorder(false);
    tw->setTextAlignment(KODE_TEXT_ALIGN_CENTER);

    tw = (KODE_TextWidget*)appendWidget( KODE_New KODE_TextWidget(  KODE_FRect(115,50,100,35) ));
    //tw->setDrawBorder(false);
    tw->setTextAlignment(KODE_TEXT_ALIGN_LEFT);

    tw = (KODE_TextWidget*)appendWidget( KODE_New KODE_TextWidget(  KODE_FRect(115,90,100,35) ));
    //tw->setDrawBorder(false);
    tw->setTextAlignment(KODE_TEXT_ALIGN_RIGHT);

    tw = (KODE_TextWidget*)appendWidget( KODE_New KODE_TextWidget(  KODE_FRect(115,130,100,35) ));
    //tw->setDrawBorder(false);
    tw->setTextAlignment(KODE_TEXT_ALIGN_TOP);

    tw = (KODE_TextWidget*)appendWidget( KODE_New KODE_TextWidget(  KODE_FRect(115,170,100,35) ));
    //tw->setDrawBorder(false);
    tw->setTextAlignment(KODE_TEXT_ALIGN_BOTTOM);
  }

  virtual ~myEditor() {
    if (MBitmap) KODE_Delete MBitmap;
  }

  //void on_window_paint(uint32_t AXpos, uint32_t AYpos, uint32_t AWidth, uint32_t AHeight) final {
  //  KODE_PRINT
  //  KODE_Editor::on_window_paint(AXpos,AYpos,AWidth,AHeight);
  //}

  //void on_widget_paint(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) final {
  //  KODE_PRINT;
  //  KODE_Widget::on_widget_paint(APainter,ARect,AMode);
  //}

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
    appendParameter( KODE_New KODE_Parameter("param1",0.3f) );
    appendParameter( KODE_New KODE_Parameter("param2",0.9f) );
    appendParameter( KODE_New KODE_Parameter("param3",0.5f) );
  }
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("offset %i index %i value %.3f mode %i\n",AOffset,AIndex,AValue,AMode);
  }

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    //KODE_PRINT;
    myEditor* editor = (myEditor*)KODE_New myEditor(this,AParent);
    return editor;
  }

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    //KODE_PRINT;
    KODE_Delete (myEditor*)AEditor;
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
