
//#define KODE_NO_GUI
#define KODE_GUI_XCB

//#define KODE_DEBUG_PRINT_TIME
//#define KODE_DEBUG_PRINT_THREAD
//#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"
#include "audio/kode_audio_utils.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_voice_manager.h"
#include "gui/kode_widgets.h"

#define KODE_PLUGIN_MESSAGE_QUEUE_SIZE 32

typedef KODE_Queue<uint32_t,KODE_PLUGIN_MESSAGE_QUEUE_SIZE> KODE_Uint32Queue;

class myVoice;
typedef KODE_VoiceManager<myVoice,16> myVoices;

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
      setName("synth_template_debug");
    #else
      setName("synth_template");
    #endif

    setAuthor("skei.audio");
    setVersion(0x00000001);
    setIsSynth(true);

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    KODE_Parameter* parameter;
    parameter = appendParameter( KODE_New KODE_Parameter("param1",0.2f) );
    parameter = appendParameter( KODE_New KODE_Parameter("param2",0.7f) );
    parameter->setLabel("db");
    parameter = appendParameter( KODE_New KODE_FloatParameter("param3",0.0f,-1,2,0.25) );
    parameter->setLabel("%");

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,480);
    #endif

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

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
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

//------------------------------
private:
//------------------------------

  KODE_VoiceContext*  MContext      = KODE_NULL;
  float               MNote         = 0.0f;
  float               MBend         = 0.0f;
  float               MMasterBend   = 0.0f;
  float               MMasterPress  = 0.0f;

  float srate = 0.0f; // sample rate
  float ph    = 0.0f; // phase
  float phadd = 0.0f; // phase add

//------------------------------
public:
//------------------------------

  // content of AContext not valid yet..
  // just keep track of the ptr..

  void prepare(KODE_VoiceContext* AContext, float ASampleRate, uint32_t ABlockSize) {
    MContext = AContext;
    srate = ASampleRate;
  }

  //----------

  uint32_t strike(float note, float vel) {
    MNote = note;
    MBend = 0.0f;
    float hz = KODE_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    ph = 0.0f;
    phadd = hz / srate;
    return KODE_VOICE_PLAYING;
  }

  //----------

  uint32_t lift(float vel) {
    return KODE_VOICE_FINISHED;
  }

  //----------

  void bend(float b) {
    MBend = b;
    float hz = KODE_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / srate;
  }

  //----------

  void press(float p) {
  }

  //----------

  void slide(float s) {
  }

  //----------

  void ctrl(uint32_t i, uint32_t v) {
  }

  //----------

  void master_bend(float mb) {
    MMasterBend = mb;
    float hz = KODE_NoteToHz(MNote + (MMasterBend * 2.0f) + (MBend*48.0f));
    phadd = hz / srate;
  }

  //----------

  void master_press(float mp) {
    MMasterPress = mp;
  }

  //----------

  void master_ctrl(uint32_t i, float mc) {
  }

  //----------

  void parameter(uint32_t i, float c) {
  }

  //----------

  uint32_t process(uint32_t AMode) {
    uint32_t num = MContext->processContext->numsamples;
    float*out0 = MContext->processContext->outputs[0];
    float*out1 = MContext->processContext->outputs[1];
    for (uint32_t i=0; i<num; i++) {
      float v = (ph * 2.0f) - 1.0f;
      ph += phadd;
      ph = KODE_Fract(ph);
      *out0++ += v * 0.1f;
      *out1++ += v * 0.1f;
    }
    return AMode;
  }

  //----------

};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  myEditor*         MEditor       = KODE_NULL;
  myVoices          MVoices;

  //bool              MNeedRecalc   = false;
  //uint32_t          MCounter      = 0;

  //KODE_Uint32Queue  MProcessMessageQueue;
  //KODE_Uint32Queue  MGuiMessageQueue;


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
  }

  //----------

  void on_plugin_close() final {
  }

  //----------

  void on_plugin_initialize() final {
  }

  //----------

  void on_plugin_terminate() final {
  }

  //----------

  void on_plugin_activate() final {
    //MCounter = 0;
  }

  //----------

  void on_plugin_deactivate() final {
  }

  //----------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    MVoices.prepare(ASamplerate,ABlocksize);
  }

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("%i = %.3f\n",AIndex,AValue);
    //MNeedRecalc = true;
    //queueProcessMessage(AIndex);
    MVoices.parameter(AOffset,AIndex,AValue,AMode);
  }

  //----------

  void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {
    MVoices.midi(AOffset,AMsg1,AMsg2,AMsg3,AMode);
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    //flushProcessMessages();

    //if (MNeedRecalc) {
    //  recalc(AContext);
    //  MNeedRecalc = false;
    //}

    MVoices.processBlock(AContext);

    //#ifndef KODE_NO_GUI
    //if (MEditor) {
    //  queueGuiMessage(MCounter);
    //}
    //#endif
    //
    //MCounter += 1;

  }

  //----------

  uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {
    *ABuffer = KODE_NULL;
    return 0;
  }

  //----------

  void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
  }

  //----------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    MEditor = KODE_New myEditor(this,AParent);
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Assert(AEditor == MEditor);
    if (MEditor) {
      KODE_Delete MEditor;
      MEditor = KODE_NULL;
    }
  }

  //----------

  void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Assert(AEditor == MEditor);
    //if (MEditor) {
    //  flushGuiMessages();
    //}
  }

  #endif

//------------------------------
private:
//------------------------------

  //void queueProcessMessage(uint32_t AMessage) {
  //  MProcessMessageQueue.write(AMessage);
  //}

  //----------

  //void flushProcessMessages() {
  //  uint32_t message = 0;
  //  while (MProcessMessageQueue.read(&message)) {
  //    KODE_Print("%i\n",message);
  //  }
  //}

  //----------

  //void queueGuiMessage(uint32_t AMessage) {
  //  MGuiMessageQueue.write(AMessage);
  //}

  //----------

  //void flushGuiMessages() {
  //  uint32_t message = 0;
  //  while (MGuiMessageQueue.read(&message)) {
  //    KODE_Print("%i\n",message);
  //  }
  //}

  //----------

  //void recalc(KODE_ProcessContext* AContext) {
  //  KODE_PRINT;
  //}

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);









  //----------

  /*
    process-buffer = cache-aligned (64 bytes)
    ticksize = 16 floats = 4 simd instructions = 64 bytes
    hardcode all processing
    a) 16 samples.. optimized.. 4 simd.. unrolled, consts..
    b) 1..15 for the remaining (if 'uneven' buffer)
  */


  /*
  void process_tick(int ANumSamples, float* AResult) {
    int remaining = ANumSamples;
    int offset = 0;
    preprocess_events();
    while (remaining > 0) {
      if (remaining >= TICKSIZE) {
        process_events(offset);
        //MProcess.process();
        //copy_buffer(AResult + offset,MProcess.getBuffer());
        remaining -= TICKSIZE;
        offset += TICKSIZE;
      }
      else {
        process_events(offset,remaining);
        //MProcess.process(remaining);
        //copy_buffer(AResult + offset,MProcess.getBuffer(),remaining);
        remaining = 0;
      }
    }
    postprocess_events();
  }
  */
