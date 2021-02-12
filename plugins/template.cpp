
#define KODE_GUI_XCB
#define KODE_DEBUG_PRINT_TIME
#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"

#include "plugin/kode_plugin.h"
#include "gui/kode_widgets.h"

#include "plugin/kode_synth.h"
#include "plugin/kode_voice.h"


//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {

    #ifdef KODE_DEBUG
      setName("plugin_debug");
    #else
      setName("plugin");
    #endif

    setAuthor("author");
    setVersion(0x0101001);

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    KODE_Parameter* parameter;
    parameter = appendParameter( KODE_New KODE_Parameter("param1",0.2f) );
    parameter = appendParameter( KODE_New KODE_Parameter("param2",0.7f) );
    parameter->setLabel("db");
    parameter = appendParameter( KODE_New KODE_Parameter("param3",0.4f) );
    parameter->setLabel("%");

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,480);
    #endif

    // synth
    setIsSynth(true);
    setCanReceiveMidi(true);

  }
};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

class myEditor
: public KODE_Editor {

//------------------------------
public:
//------------------------------

  myEditor(KODE_IInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setFillBackground(true);
    setBackgroundColor(0.5f);

    KODE_ValueWidget* widget;
    widget = (KODE_ValueWidget*)appendWidget(KODE_New KODE_ValueWidget( KODE_FRect(10,10,150,20) ));
    connectParameter(widget,0);

    widget = (KODE_ValueWidget*)appendWidget(KODE_New KODE_ValueWidget( KODE_FRect(10,35,150,20) ));
    widget->setDrawLabel(true);
    connectParameter(widget,1);

    KODE_SliderWidget* slider;
    slider = (KODE_SliderWidget*)appendWidget(KODE_New KODE_SliderWidget( KODE_FRect(10,60,150,20) ));
    slider->setValueBarDirection(KODE_RIGHT);
    slider->setDrawLabel(true);
    connectParameter(slider,2);

  }

  //----------

  //virtual ~myEditor() {
  //}

};

#endif

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice {
public:

  void strike(float note, float vel) {
    //KODE_Print("strike: hz %.3f vel %.3f\n",hz,vel);
  }

  void lift(float vel) {
    //KODE_Print("lift: vel %.3f\n",vel);
  }

  void bend(float v, float mb) {
    //KODE_Print("bend: v %.3f\n",v);
  }

//  void master_bend(float v) {
//    //KODE_Print("bend: v %.3f\n",v);
//  }

  void press(float p, float mp) {
    //KODE_Print("press: v %.3f\n",v);
  }

  void slide(float s) {
    //KODE_Print("slide: v %.3f\n",v);
  }

  void ctrl(float v) {
    //KODE_Print("ctrl: v %.3f\n",v);
  }

};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

  KODE_VoiceManager<myVoice,16> MVoices;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
public:
//------------------------------

  void on_plugin_open() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_close() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_initialize() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_terminate() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_activate() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_deactivate() final {
    KODE_Print("\n");
  }

  //----------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    KODE_Print("sr %.3f bs %i\n",ASamplerate,ABlocksize);
    MVoices.prepare(ASamplerate,ABlocksize);
  }

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("ofs %i idx %i val %.3f mode %i\n",AOffset,AIndex,AValue,AMode);
    MVoices.parameter(AOffset,AIndex,AValue,AMode);
  }

  //----------

  void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
    //KODE_Print("ofs %i msg %02x %02x %02x mode %i\n",AOffset,AMsg1,AMsg2,AMsg3,AMode);
    MVoices.midi(AOffset,AMsg1,AMsg2,AMsg3,AMode);
  }

  //----------

  void on_plugin_processBlock(KODE_ProcessContext* AContext) final {
    //KODE_Print("\n");
  }

  //----------

  uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {
    //KODE_Print("\n");
    *ABuffer = KODE_NULL;
    return 0;
  }

  //----------

  void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
    //KODE_Print("\n");
  }

  //----------

  #ifndef KODE_NO_GUI

  KODE_IEditor* on_plugin_openEditor(void* AParent) final {
    KODE_Print("parent %p\n",AParent);
    myEditor* editor = (myEditor*)KODE_New myEditor(this,AParent);
    return editor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_IEditor* AEditor) final {
    KODE_Print("\n");
    KODE_Delete (myEditor*)AEditor;
  }

  //----------

  void on_plugin_updateEditor(KODE_IEditor* AEditor) final {
    //KODE_Print("\n");
  }

  #endif

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
