#ifndef vst3_instance_included
#define vst3_instance_included
//----------------------------------------------------------------------

// needs kode_vst3.h, which is gpl3
// so this file also needs to be gpl3

//----------------------------------------------------------------------

#ifndef KODE_NO_GUI
  #include "plugin/kode_editor.h"
#endif

#include "plugin/base/kode_base_instance.h"
#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_utils.h"

//----------------------------------------------------------------------

#define KODE_VST3_MAX_INPUTS        8
#define KODE_VST3_MAX_OUTPUTS       8
#define KODE_VST3_PARAM_AFTERTOUCH  0x10000 // kode_vst3_AfterTouch (128)
#define KODE_VST3_PARAM_PITCHBEND   0x20000 // kode_vst3_PitchBend (129)
#define KODE_VST3_PARAM_BRIGHTNESS  0x30000 // kode_vst3_CtrlFilterResonance (74)
#define KODE_VST3_QUEUE_SIZE        1024
#define KODE_VST3_TIMER_MS          30

typedef KODE_Queue<uint32_t,KODE_VST3_QUEUE_SIZE> KODE_Vst3UpdateQueue;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class KODE_Vst3Instance

: public KODE_Vst3IComponent
, public KODE_Vst3IAudioProcessor
, public KODE_Vst3IUnitInfo
, public KODE_Vst3IConnectionPoint
, public KODE_Vst3IMidiMapping
, public KODE_Vst3IKeyswitchController
//, public KODE_Vst3INoteExpressionController
, public KODE_Vst3IEditController
, public KODE_Vst3IEditController2
, public KODE_Vst3IPlugView
, public KODE_Vst3ITimerHandler
, public KODE_Vst3IEventHandler
, public KODE_BaseInstance {

//------------------------------
private:
//------------------------------

  uint32_t                      MRefCount               = 1;
  KODE_Vst3IComponentHandler*   MComponentHandler       = KODE_NULL;
  KODE_Vst3IComponentHandler2*  MComponentHandler2      = KODE_NULL;
  KODE_Vst3IPlugFrame*          MPlugFrame              = KODE_NULL;
  KODE_Vst3IHostApplication*    MHostApp                = KODE_NULL;
  KODE_Vst3ParameterInfo*       MParamInfos             = KODE_NULL;
  KODE_Vst3IRunLoop*            MRunLoop                = KODE_NULL;
  uint32_t                      MIoMode                 = 0;
  uint32_t                      MProcessMode            = 0;
  uint32_t                      MSampleSize             = 0;
  uint32_t                      MBlockSize              = 0;
  float                         MSampleRate             = 0.0f;
  bool                          MIsProcessing           = false;
  char                          MHostName[129]          = {0};
  KODE_ProcessContext           MProcessContext         = {0};
  KODE_Descriptor*              MDescriptor             = KODE_NULL;
  KODE_Vst3UpdateQueue          MHostParameterQueue;
  float*                        MHostParameterValues    = KODE_NULL;
  float*                        MParameterValues        = KODE_NULL;
//float*                        MEditorParameterValues  = KODE_NULL;
  #ifndef KODE_NO_GUI
  KODE_Editor*                  MEditor                 = KODE_NULL;
  #endif



//------------------------------
public:
//------------------------------

  KODE_Vst3Instance(KODE_Descriptor* ADescriptor)
  : KODE_BaseInstance(ADescriptor) {
    KODE_VST3PRINT;
    MRefCount = 1;
    MDescriptor = ADescriptor;
    createParameterBuffers();
    createParameterInfo();
  }

  //----------

  virtual ~KODE_Vst3Instance() {
    KODE_VST3PRINT;
    deleteParameterInfo();
    destroyParameterBuffers();
  }

//------------------------------
public:
//------------------------------

  KODE_Descriptor* getDescriptor() override {
    KODE_VST3PRINT;
    return MDescriptor;
  }

//------------------------------
public:
//------------------------------

  void setDefaultParameterValues() override {
    KODE_VST3PRINT;
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float value = parameter->getDefValue();
      MParameterValues[i] = value;
    }
  }

  //----------

  void updateAllParameters() override {
    KODE_VST3PRINT;
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* parameter = MDescriptor->getParameter(i);
      float value = MParameterValues[i];
      float v = parameter->from01(value);
      on_plugin_parameter(0,i,v);
      // if editor is open ...
    }
  }

  //----------

  #ifndef KODE_NO_GUI
  void updateAllEditorParameters(KODE_BaseEditor* AEditor, bool ARedraw=true) override {
    KODE_Vst3Print("%s\n",ARedraw?"(redraw=true)":"");
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      float value = MParameterValues[i];
      //KODE_Parameter* parameter = MDescriptor->getParameter(i);
      //float v = parameter->from01(value);
      float v = value;
      //on_plugin_parameter(0,i,v);
      AEditor->updateParameterFromHost(i,v,ARedraw);
    }
  }
  #endif

  //----------

  /*
    we run our gui in its own thread, but need to communicate with the
    host on its gui thread.. so, when we tweak a parameter, we queue the
    parameter (index), and send all updated ones to the host during onTimer
    (see bottom)
    the host should communicate the parameter change (audio thread) during
    next process()
  */

  #ifndef KODE_NO_GUI
  void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
    KODE_Vst3Print("index: %i value: %.3f\n",AIndex,AValue);
    //MEditorParameterValues[AIndex] = AValue;
    MParameterValues[AIndex] = AValue;
    queueParameterToHost(AIndex,AValue);
  }
  #endif

//------------------------------
private:
//------------------------------

  void createParameterBuffers() {
    KODE_VST3PRINT;
    uint32_t size = MDescriptor->getNumParameters() * sizeof(float);
    //MNumParameters = MDescriptor->getNumParameters();
    MParameterValues        = (float*)malloc(size);
    //MEditorParameterValues  = (float*)malloc(size);
    MHostParameterValues    = (float*)malloc(size);
    memset(MParameterValues,       0,size);
    //memset(MEditorParameterValues, 0,size);
    memset(MHostParameterValues,   0,size);
  }

  //----------

  void destroyParameterBuffers() {
    KODE_VST3PRINT;
    if (MParameterValues)       free(MParameterValues);
    //if (MEditorParameterValues) free(MEditorParameterValues);
    if (MHostParameterValues)   free(MHostParameterValues);
  }

  //----------

  void queueParameterToHost(uint32_t AIndex, float AValue) {
    KODE_Print("index: %i value: %.3f\n",AIndex,AValue);
    MHostParameterValues[AIndex] = AValue;
    MHostParameterQueue.write(AIndex);
  }

  //----------

  /*
    must call the MComponentHandler from the same thread as we received it
    (same thread as setComponentHandler)
    IRunLoop

    Sets the normalized value to the parameter associated to the paramID.
    The controller must never pass this value-change back to the host via the
    IComponentHandler. It should update the according GUI element(s) only!

    https://github.com/soundradix/JUCE/commit/2e9e66cbc8c65e889be5232ffae83c0ca78f9c7e

    performEdit ((Vst::ParamID) index, (double) newValue);
    // call setParamNormalized too, as without it value will reset at endEdit in Cubase.
    // setParamNormalized does not replace performEdit as it does not record automation.

    setParamNormalized ((Vst::ParamID) index, (double) newValue);

    https://sdk.steinberg.net/viewtopic.php?t=693

    "Remember that everything in the edit controller domain must happen on the
    main thread also calls to the IComponentHandler instance of the host. So
    don't call beginEdit, endEdit or performEdit on a secondary thread."

    NB: Cubase has problems if performEdit is called without setParamNormalized
    EditController::setParamNormalized(AIndex,AValue);

    MComponentHandler->performEdit(AIndex,AValue); // drag
  */

  /*
    todo:
    beginEdit when mouseClick
    performEdit while mouseDrag
    endEdit when mouseRelease
  */

  void flushParametersToHost() {
    KODE_VST3PRINT;
    uint32_t index = 0;
    while (MHostParameterQueue.read(&index)) {
      float value = MHostParameterValues[index];
      KODE_Vst3DPrint("  index: %i value: %.3f\n",index,value);
      if (MComponentHandler) {
        //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
        MComponentHandler->beginEdit(index);
        MComponentHandler->performEdit(index,value);
        MComponentHandler->endEdit(index);
        //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
      }
    }
  }

  //----------

  /*
    reaper asks for this all the time, so we create (and cache) it, and
    return a ptr to the same buffer each time..
    (re-check, as i think i remember i saw something about this in a
    recent reaper update log)

    numsteps
    0 = continuous
    1 = toggle
    2.. = max value
    so, steps-1, or 0 if cont..
  */

  void createParameterInfo() {
    KODE_VST3PRINT;
    if (!MParamInfos) {
      uint32_t num = MDescriptor->getNumParameters();
      MParamInfos = (KODE_Vst3ParameterInfo*)malloc( num * sizeof(KODE_Vst3ParameterInfo) );
      for (uint32_t i=0; i<num; i++) {
        KODE_Parameter* param = MDescriptor->getParameter(i);
        MParamInfos[i].id = i;
        KODE_CharToUtf16(param->getName(),&MParamInfos[i].title);
        KODE_CharToUtf16(param->getShortName(),&MParamInfos[i].shortTitle);
        KODE_CharToUtf16(param->getLabel(),&MParamInfos[i].units);
        uint32_t numsteps = param->getNumSteps();
        if (numsteps > 1) numsteps -= 1;
        MParamInfos[i].stepCount = numsteps;
        MParamInfos[i].defaultNormalizedValue = param->getDefValue();
        MParamInfos[i].unitId = kode_vst3_RootUnitId; //-1;
        int32_t flags = 0;
        if (param->canAutomate()) flags += KODE_Vst3ParameterInfo::kode_vst3_CanAutomate;
        else flags += KODE_Vst3ParameterInfo::kode_vst3_IsReadOnly; // ??
        MParamInfos[i].flags = flags;
      }
    }
  }

  //----------

  void deleteParameterInfo() {
    KODE_VST3PRINT;
    if (MParamInfos) free(MParamInfos);
  }

  //----------

  /*
    no interpolation, or sample-accurate automation yet..
    we 'cheat', and only use the last value for each block
    (plugins can interpolate this)
    a) fill a buffer (blocksize) with parameter values?
    b) split process-buffer, and send parameter updates inbetween?
       (not interpolated)
    #define KODE_VST3_PARAMS_LAST, _STEPPED, _INTERPOLATED
  */

  void handleParametersInProcess(KODE_Vst3ProcessData& data) {
    //KODE_VST3PRINT;
    KODE_Vst3IParameterChanges* paramChanges = data.inputParameterChanges;
    if (paramChanges) {
      int32_t num_param_changes = paramChanges->getParameterCount();
      if (num_param_changes > 0) {
        for (int32_t i=0; i<num_param_changes; i++) {
          KODE_Vst3IParamValueQueue* paramQueue = paramChanges->getParameterData(i);
          if (paramQueue) {
            uint32_t id = paramQueue->getParameterId();
            if (id < MDescriptor->getNumParameters()) {
              //for (int32_t j=0; j<paramQueue->getPointCount(); j++) {
                int32_t offset = 0;
                double value = 0;
                int32_t pointcount = paramQueue->getPointCount();
                paramQueue->getPoint(pointcount-1,offset,value); // last point
                MParameterValues[id] = value;
                //KODE_Print("MParameterValues[%i] = %.3f\n",id,value);
                KODE_Parameter* param = MDescriptor->getParameter(id);
                if (param) value = param->from01(value);
                on_plugin_parameter(0,id,value);
              //}
            } // id < param
            else {
              // if PLUGIN_RECEIVE_MIDI
              // IMidiMapping
              int32_t offset = 0;
              double value = 0.5;
              int32_t pointcount = paramQueue->getPointCount();
              paramQueue->getPoint(pointcount-1,offset,value);      // last point
              uint32_t midi_ev = (id & 0xffff0000);
              uint32_t midi_ch = (id & 0x0000ffff);
              switch(midi_ev) {
                case KODE_VST3_PARAM_AFTERTOUCH: {
                  //if (offset != 0)
                  on_plugin_midi(offset,KODE_MIDI_CHANNEL_AFTERTOUCH+midi_ch,value*127.0f,0);
                  break;
                } // at
                case KODE_VST3_PARAM_PITCHBEND: {
                  float v2 = value * 16383.0f;
                  uint32_t i2 = (uint32_t)v2;
                  //if (midi_ch != 0) {
                    uint32_t m2 = i2 & 0x7f;
                    uint32_t m3 = i2 >> 7;
                    //if (offset != 0)
                    on_plugin_midi(offset,KODE_MIDI_PITCHBEND+midi_ch,m2,m3);
                  //}
                  break;
                } // pb
                case KODE_VST3_PARAM_BRIGHTNESS: {
                  //if (offset != 0)
                  on_plugin_midi(offset,KODE_MIDI_CONTROL_CHANGE+midi_ch,74,value*127.0f);
                  break;
                }
              } // switch (midi_ev)
            } // id < num params
          } // paramqueue
        } // for all params
      } // numparams > 0
    } // paramchanges
  }

  //----------

  void handleEventsInProcess(KODE_Vst3ProcessData& data) {
    //KODE_VST3PRINT;
    //if PLUGIN_RECEIVE_MIDI
    KODE_Vst3IEventList* inputEvents = data.inputEvents;
    if (inputEvents) {
      int32_t num = inputEvents->getEventCount();
      for (int32_t i=0; i<num; i++) {
        KODE_Vst3Event event;
        inputEvents->getEvent(i,event);
        uint32_t offset  = 0;
        uint8_t  msg1    = 0;
        uint8_t  msg2    = 0;
        uint8_t  msg3    = 0;
        //uint32_t type    = 0;//kve_none;
        //uint32_t noteid  = 0;
        //float    value   = 0.0f;
        switch (event.type) {
          case KODE_Vst3Event::kode_vst3_NoteOnEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_NOTE_ON + event.noteOn.channel;
            msg2    = event.noteOn.pitch;
            msg3    = event.noteOn.velocity * 127;
            //noteid  = event.noteOn.noteId;
            on_plugin_midi(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case KODE_Vst3Event::kode_vst3_NoteOffEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_NOTE_OFF + event.noteOff.channel;
            msg2    = event.noteOff.pitch;
            msg3    = event.noteOff.velocity * 127;
            //noteid  = event.noteOff.noteId;
            on_plugin_midi(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case KODE_Vst3Event::kode_vst3_DataEvent:
            break;
          case KODE_Vst3Event::kode_vst3_PolyPressureEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_POLY_AFTERTOUCH + event.polyPressure.channel;
            msg2    = event.polyPressure.pitch;
            msg3    = event.polyPressure.pressure * 127;
            //noteid  = event.polyPressure.noteId;
            on_plugin_midi(offset,msg1,msg2,msg3);
            break;
          case KODE_Vst3Event::kode_vst3_NoteExpressionValueEvent:
            //offset = event.sampleOffset;
            //noteid = event.noteExpressionValue.noteId;
            //value  = event.noteExpressionValue.value;
            //switch(event.noteExpressionValue.typeId) {
            //  case kode_vst3_TuningTypeID:     type = kve_bend; break;
            //  case kode_vst3_BrightnessTypeID: type = kve_slide; break;
            //  //case kode_vst3_VolumeTypeID:     type = kve_none;  break;
            //  //case kode_vst3_PanTypeID:        type = kve_none;  break;
            //  //case kode_vst3_VibratoTypeID:    type = kve_none;  break;
            //  //case kode_vst3_ExpressionTypeID: type = kve_none;  break;
            //}polyPressure
            //on_noteExpression(offset,type,noteid,value);
            break;
          case KODE_Vst3Event::kode_vst3_NoteExpressionTextEvent:
            break;
          case KODE_Vst3Event::kode_vst3_ChordEvent:
            break;
          case KODE_Vst3Event::kode_vst3_ScaleEvent:
            break;
          default:
            break;
        } // switch
      } // for all events
    } // if input events
  }

//----------------------------------------------------------------------
// vst3
//----------------------------------------------------------------------

//------------------------------
public: // FUnknown
//------------------------------

  /*
    Adds a reference and return the new reference count.
    Remarks: The initial reference count after creating an object is 1.
  */

  uint32_t KODE_VST3_PLUGIN_API addRef() final {
    MRefCount++;
    KODE_Vst3Print("-> %i\n",MRefCount);
    return MRefCount;
  }

  //----------

  /*
    Releases a reference and return the new reference count.
    If the reference count reaches zero, the object will be destroyed in
    memory.
  */

  uint32_t KODE_VST3_PLUGIN_API release() final {
    const uint32_t r = --MRefCount;
    KODE_Vst3Print("-> %i %s",r, (r==0) ? "(delete)\n" : "\n" );
    if (r == 0) {
      on_plugin_close();
      delete this;
    };
    return r;
  }

  //----------

  /*
    Query for a pointer to the specified interface.
    Returns kResultOk on success or kNoInterface if the object does not
    implement the interface.
    The object has to call addRef when returning an interface.
    _iid: (in)  16 Byte interface identifier (-> FUID)
    obj:  (out) On return, *obj points to the requested interface

    Although it is not recommended, it is possible to implement both, the
    processing part and the controller part in one component class. The host
    tries to query the IEditController interface after creating an
    IAudioProcessor and on success uses it as controller.

    reaper asks for:
      C3B17BC0-2C174494-80293402-FBC4BBF8 (IContextInfoHandler,   pslextensions)
      31E29A7A-E55043AD-8B95B9B8-DA1FBE1E (IContextInfoHandler2,  pslextensions)

    bitwig asks for:
      6C389611-D391455D-B870B833-94A0EFDD (IUnitData)
      8683B01F-7B354F70-A2651DEC-353AF4FF (IProgramListData)
      7F4EFE59-F3204967-AC27A3AE-AFB63038 (IEditController2)
      65ED9690-8AC44525-8AADEF7A-72EA703F (IPlugViewContentScaleSupport)

    ( 65ed9690-8ac44525-8aadef7a-72ea703f (IPlugInViewScaling, pslextensions) !! )

      633b9012-d19a4953-b477b436-9daaeedb
      8e8bbb11-73384477-ac6b1eeb-383df4f0
      ccc87233-accb4371-bc74a1f2-5ebba43c

      7743fe5e-f3224d6d-a729ada9-aab2333b
      cd291308-775e4492-b5db37be-62bb1a54
      41c5087e-4bbb4e8b-8da21bd2-97dc6679
      001a4b89-86924ed8-b7aacdee-cc12d6dd
      98e97900-c220484c-82a3edcd-d06cdd45

      6bec9099-83c14927-83a8ee72-79e97733

  */


  int32_t KODE_VST3_PLUGIN_API queryInterface(const KODE_Vst3Id _iid, void** obj) final {
    KODE_Vst3Print("iid: ");
    KODE_Vst3PrintIID(_iid);
    if (KODE_iidEqual(KODE_Vst3IAudioProcessor_iid,_iid) ) {
      KODE_Vst3DPrint(" (IAudioProcessor) -> Ok\n");
      *obj = (KODE_Vst3IAudioProcessor*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IEditController_iid,_iid) ) {
      KODE_Vst3DPrint(" (IEditController) -> Ok\n");
      *obj = (KODE_Vst3IEditController*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IMidiMapping_iid,_iid) ) {
      KODE_Vst3DPrint(" (IMidiMapping) -> Ok\n");
      *obj = (KODE_Vst3IMidiMapping*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IUnitInfo_iid,_iid) ) {
      KODE_Vst3DPrint(" (IUnitInfo) -> Ok\n");
      *obj = (KODE_Vst3IUnitInfo*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    //if (KODE_iidEqual(KODE_Vst3INoteExpressionController_iid,_iid) ) {
    //  KODE_Vst3DPrint(" (INoteExpressionController) -> NoInterface\n");
    //  //*obj = (KODE_Vst3INoteExpressionController*)this;
    //  //addRef();
    //  //return kode_vst3_ResultOk;
    //  return kode_vst3_NoInterface;
    //}
    if (KODE_iidEqual(KODE_Vst3IKeyswitchController_iid,_iid) ) {
      KODE_Vst3DPrint(" (IKeySwitchProcessor) -> Ok\n");
      //*obj = (KODE_Vst3IKeyswitchController*)this;
      //addRef();
      //return kode_vst3_ResultOk;
      return kode_vst3_NoInterface;
    }
    if (KODE_iidEqual(KODE_Vst3IConnectionPoint_iid,_iid) ) {
      KODE_Vst3DPrint(" (IConnectionPoint) -> Ok\n");
      *obj = (KODE_Vst3IConnectionPoint*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3ITimerHandler_iid,_iid) ) {
      KODE_Vst3DPrint(" (ITimerHandler) -> NoInterface\n");
      //*obj = (KODE_Vst3ITimerHandler*)this;
      //addRef();
      //return kode_vst3_ResultOk;
      return kode_vst3_NoInterface;
    }
    KODE_Vst3DPrint(" (unknown) -> NoInterface\n");
    *obj = KODE_NULL;
    return kode_vst3_NoInterface;

  }

//------------------------------
public: // IPluginBase
//------------------------------

/*
    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    The context parameter passed to IPluginBase::initialize is
    Vst::IHostApplication. Hosts should not call others functions before
    initialize is called!, except IComponent::setIoMode which need to be
    called before or IComponent::getControllerClassId which could be called
    before.

    note: Extensive memory allocations etc. should be performed in this
    method rather than in the class' constructor!
    If the method does NOT return kResultOk, the object is released
    immediately. In this case terminate is not called!

    ---

    3.6.12
    Allow a Plug-in to ask the host if a given Plug-in interface is
    supported/used by the host. It is implemented by the hostContext given
    when the component is initialized.
    tresult PLUGIN_API MyPluginController::initialize (FUnknown* context) {
      // ...
      FUnknownPtr<IPlugInterfaceSupport> plugInterfaceSupport (context);
      if (plugInterfaceSupport) {
        if (plugInterfaceSupport->isPlugInterfaceSupported (IMidiMapping::iid) == kResultTrue)
          // IMidiMapping is used by the host
      }
      //...
    }

  ---

    IHostApplication: passed as 'context' in to IPluginBase::initialize()
  */

  int32_t KODE_VST3_PLUGIN_API initialize(KODE_Vst3FUnknown* context) final {
    KODE_Vst3Print("contect: %p",context);
    MHostApp = (KODE_Vst3IHostApplication*)context;
    //context->queryInterface(IHostApplication_iid, (void**)&MHostApp);
    if (MHostApp) {
      KODE_Vst3String u;
      MHostApp->getName(u);
      KODE_Utf16ToChar(&u,MHostName);
      KODE_Vst3DPrint(" (hostname: '%s')",MHostName);
    }
    else {
    }
    KODE_Vst3DPrint(" -> Ok\n");
    on_plugin_initialize();
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    This function is called before the Plug-in is unloaded and can be used
    for cleanups. You have to release all references to any host application
    interfaces.
  */

  int32_t KODE_VST3_PLUGIN_API terminate() final {
    KODE_Vst3Print(" -> Ok\n");
    on_plugin_terminate();
    return kode_vst3_ResultOk;
  }

//------------------------------
public: // IComponent
//------------------------------

  /*
    Called before initializing the component to get information about the
    controller class.

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    In order to enable the host to create a corresponding edit controller the
    processing component has to provide the matching class-ID. The host uses
    the module's class factory to create the controller component.
    (IComponent::getControllerClassId)
  */

  int32_t KODE_VST3_PLUGIN_API getControllerClassId(KODE_Vst3Id classId) final {
    KODE_Vst3Print("classId: ");
    KODE_Vst3PrintIID(classId);
    if (MDescriptor->hasEditor()) {
      memcpy(classId,MDescriptor->getLongEditorId(),16);
      KODE_Vst3DPrint("-> ");
      KODE_Vst3PrintIID(classId);
      KODE_Vst3DPrint(" -> Ok\n");
      return kode_vst3_ResultOk;
    }
    else {
      KODE_Vst3DPrint(" -> False\n");
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  /*
    Called before 'initialize' to set the component usage (optional).
    See IoModes
  */

  int32_t KODE_VST3_PLUGIN_API setIoMode(int32_t mode) final {
    KODE_Vst3Print(" mode: %i",mode);
    MIoMode = mode;
    switch (mode) {
      case kode_vst3_Simple:            KODE_Vst3DPrint(" /simple"); break;
      case kode_vst3_Advanced:          KODE_Vst3DPrint(" /advanced"); break;
      case kode_vst3_OfflineProcessing: KODE_Vst3DPrint(" /offline"); break;
    }
    KODE_Vst3DPrint(" -> Ok\n");
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections
  */

  int32_t KODE_VST3_PLUGIN_API getBusCount(int32_t type, int32_t dir) final {
    KODE_Vst3Print("type: %s dir: %s",type?"1/event":"0/audio",dir?"1/output":"0/input");
    switch (type) {
      case kode_vst3_Audio:
        switch (dir) {
          case kode_vst3_Input:
            if (MDescriptor->getNumInputs() > 0) {
              KODE_Vst3DPrint(" -> 1\n");
              return 1;
            }
            else {
              KODE_Vst3DPrint(" -> 0\n");
              return 0;
            }
          case kode_vst3_Output:
            if (MDescriptor->getNumOutputs() > 0) {
              KODE_Vst3DPrint(" -> 1\n");
              return 1;
            }
            else {
              KODE_Vst3DPrint(" -> 0\n");
              return 0;
            }
        }
      case kode_vst3_Event:
        switch (dir) {
          case kode_vst3_Input:
            if (MDescriptor->canReceiveMidi() || MDescriptor->isSynth()) {
              KODE_Vst3DPrint(" -> 1\n");
              return 1;
            }
            else {
              KODE_Vst3DPrint(" -> 0\n");
              return 0;
            }
          case kode_vst3_Output:
              KODE_Vst3DPrint(" -> 0\n");
            return 0;
        }
    }
    KODE_Vst3DPrint("-> 0\n");
    return 0;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections
  */

  int32_t KODE_VST3_PLUGIN_API getBusInfo(int32_t type, int32_t dir, int32_t index, KODE_Vst3BusInfo& bus) final {
    KODE_Vst3Print("type: %s dir: %s index: %i",type?"1/event":"0/audio",dir?"1/output":"0/input",index);
    switch (type) {
      case kode_vst3_Audio:
        switch (dir) {
          case kode_vst3_Input:
            bus.mediaType = kode_vst3_Audio;
            bus.direction = kode_vst3_Input;
            bus.channelCount = MDescriptor->getNumInputs();
            KODE_CharToUtf16("Audio In",&bus.name);
            bus.busType = kode_vst3_Main;         // Aux
            bus.flags = KODE_Vst3BusInfo::kode_vst3_DefaultActive;  // 0
            KODE_Vst3DPrint(" -> Ok\n");
            KODE_Vst3DPrint(". type: %i (Audio)\n",bus.mediaType);
            KODE_Vst3DPrint(". dir: %i (Input)\n",bus.direction);
            KODE_Vst3DPrint(". chn.count: %i\n",bus.channelCount);
            KODE_Vst3DPrint(". name: '%s'\n","Audio In");
            KODE_Vst3DPrint(". busType: %i (Main)\n",bus.busType);
            KODE_Vst3DPrint(". flags: %i (DefaultActive)\n",bus.flags);
            return kode_vst3_ResultOk;
          case kode_vst3_Output:
            bus.mediaType = kode_vst3_Audio;
            bus.direction = kode_vst3_Output;
            bus.channelCount = MDescriptor->getNumOutputs();
            KODE_CharToUtf16("Audio Out",&bus.name);
            bus.busType = kode_vst3_Main;         // Aux
            bus.flags = KODE_Vst3BusInfo::kode_vst3_DefaultActive;  // 0
            KODE_Vst3DPrint(" -> Ok\n");
            KODE_Vst3DPrint(". type: %i (Audio)\n",bus.mediaType);
            KODE_Vst3DPrint(". dir: %i (Output)\n",bus.direction);
            KODE_Vst3DPrint(". chn.count: %i\n",bus.channelCount);
            KODE_Vst3DPrint(". name: '%s'\n","Audio Out");
            KODE_Vst3DPrint(". busType: %i (Main)\n",bus.busType);
            KODE_Vst3DPrint(". flags: %i (DefaultActive)\n",bus.flags);
            return kode_vst3_ResultOk;
        } // switch (dir)
      case kode_vst3_Event:
        switch (dir) {
          case kode_vst3_Input:
            bus.mediaType = kode_vst3_Event;
            bus.direction = kode_vst3_Input;
            bus.channelCount = 16;
            KODE_CharToUtf16("Midi In",&bus.name);
            bus.busType = kode_vst3_Main;         // Aux
            bus.flags = KODE_Vst3BusInfo::kode_vst3_DefaultActive;  // 0
            KODE_Vst3DPrint(" -> Ok\n");
            KODE_Vst3DPrint(". type: %i (Event)\n",bus.mediaType);
            KODE_Vst3DPrint(". dir: %i (Input)\n",bus.direction);
            KODE_Vst3DPrint(". chn.count: %i\n",bus.channelCount);
            KODE_Vst3DPrint(". name: '%s'\n","Midi In");
            KODE_Vst3DPrint(". busType: %i (Main)\n",bus.busType);
            KODE_Vst3DPrint(". flags: %i (DefaultActive)\n",bus.flags);
            return kode_vst3_ResultOk;
          case kode_vst3_Output:
            //bus.mediaType = kode_vst3_Event;
            ////bus.flags = 0;//kode_vst3_DefaultActive;
            //bus.direction = kode_vst3_Input;
            //bus.channelCount = 16;
            //KODE_CharToUtf16("Midi In",&bus.name);
            KODE_Vst3DPrint(" -> False\n");
            return kode_vst3_ResultFalse; // kode_vst3_ResultOk;
            //break;
        } // switch (dir)
    } // switch (type)
    KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    Retrieves routing information (to be implemented when more than one
    regular input or output bus exists).
    The inInfo always refers to an input bus while the returned outInfo must
    refer to an output bus!
  */


  int32_t KODE_VST3_PLUGIN_API getRoutingInfo(KODE_Vst3RoutingInfo& inInfo, KODE_Vst3RoutingInfo& outInfo) final {
    KODE_Vst3Print("\n");
    outInfo.mediaType = inInfo.mediaType; // MediaTypes::kode_vst3_Audio;
    outInfo.busIndex  = inInfo.busIndex;  // 0;
    outInfo.channel   = -1;
    KODE_Vst3DPrint(". inInfo.mediaType: \n",inInfo.mediaType);
    KODE_Vst3DPrint(". inInfo.busIndex: \n",inInfo.busIndex);
    KODE_Vst3DPrint(". inInfo.channel: \n",inInfo.channel);
    KODE_Vst3DPrint(". outInfo.mediaType: \n",outInfo.mediaType);
    KODE_Vst3DPrint(". outInfo.busIndex: \n",outInfo.busIndex);
    KODE_Vst3DPrint(". outInfo.channel: \n",outInfo.channel);
    KODE_Vst3Print("-> Ok\n");
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Called upon (de-)activating a bus in the host application. The Plug-in
    should only processed an activated bus, the host could provide less
    AudioBusBuffers in the process call (see IAudioProcessor::process) if
    last buses are not activated
  */

  int32_t KODE_VST3_PLUGIN_API activateBus(int32_t type, int32_t dir, int32_t index, uint8_t state) final {
    KODE_Vst3Print("type: %s dir: %s index: %i state: %i",type?"1/event":"0/audio",dir?"1/output":"0/input",index,state);
    KODE_Vst3DPrint(" -> Ok\n");
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Activates / deactivates the component.
  */

  int32_t KODE_VST3_PLUGIN_API setActive(uint8_t state) final {
    if (state) on_plugin_activate();
    else on_plugin_deactivate();
    KODE_Vst3Print("state: %i -> Ok\n",state);
    return kode_vst3_ResultOk;
  }

  //----------

  /*

    How does persistence work?

    An instantiated Plug-in often has state information that must be saved in
    order to properly re-instantiate that Plug-in at a later time. A VST 3
    Plug-in has two states which are saved and reloaded: its component state
    and its controller state.
    The sequence of actions for saving is:
      component->getState (compState)
      controller->getState (ctrlState)
    The sequence of actions for loading is:
      component->setState (compState)
      controller->setComponentState (compState)
      controller->setState (ctrlState)
    In the latter sequence you can see that the controller part will receive
    the component state. This allows the 2 parts to synchronize their states.

    What's the difference between IEditController::setComponentState and
    IEditController::setState?

    After a preset is loaded, the host calls IEditController::setComponentState
    and IComponent::setState. Both delivering the same information.
    IEditController::setState is called by the host so that the Plug-in is able
    to update its controller dependent parameters, eg. position of scroll bars.
    Prior to this, there should have been a call by the host to
    IEditController::getState, where the Plug-in writes these various
    parameters into the stream.

    Can IComponent::getState()/setState() could be called during processing?

    Yes, setState and getState are called normally from the UI Thread when the
    Plug-in is used in a realtime context, in an offline context set/getState
    could be called in the same thread than the process call. Check the
    workflow diagram Audio Processor Call Sequence for more info about in which
    state which interfaces are called.

  ---

    Restore: When the states are restored, the host passes the processor state
    to both the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  int32_t KODE_VST3_PLUGIN_API setState(KODE_Vst3IBStream* state) final {
    KODE_VST3PRINT;
    uint32_t  version = 0;
    uint32_t  mode = 0;
    int32_t   size = 0;
    int32_t   num_read = 0;
    state->read(&version, sizeof(uint32_t),&num_read);
    state->read(&mode, sizeof(uint32_t),&num_read);
    state->read(&size, sizeof(int32_t),&num_read);
    switch (mode) {
      case 0: {
        // is it safe to malloc/free here?
        // use static, pre malloc'd buffer?
        void* ptr = malloc(size);
        state->read(&ptr,size,&num_read);
        on_plugin_restoreState(size,ptr,0);
        free(ptr);
        break;
      }
      case 1: {
        uint32_t num_params = MDescriptor->getNumParameters();
        for (uint32_t i=0; i<num_params; i++) {
          float v = 0.f;
          state->read(&v,sizeof(float),&num_read);
          //setParameterValue(i,v);
          MParameterValues[i] = v;
          //on_plugin_parameter(i,v,0);
        }
        updateAllParameters();
        break;
      }
    }
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Retrieves complete state of component.
    //state->write(MParamValues,MNumParameters*sizeof(float));

    mode
    0 = user
    1 = simple (all params)
  */

  int32_t KODE_VST3_PLUGIN_API getState(KODE_Vst3IBStream* state) final {
    KODE_VST3PRINT;
    uint32_t  version = MDescriptor->getVersion();
    uint32_t  mode    = 0;
    void*     ptr     = KODE_NULL;
    uint32_t  size    = 0;;
    size = on_plugin_saveState(&ptr,0);
    if ((size == 0) && (ptr == KODE_NULL)) {
      //ptr = getParameterValues();
      ptr = MParameterValues;
      size = MDescriptor->getNumParameters() * sizeof(float);
      mode = 1;
    }
    int num_written  = 0;
    state->write(&version,sizeof(uint32_t), &num_written);
    state->write(&mode,sizeof(uint32_t), &num_written);
    state->write(&size,sizeof(uint32_t), &num_written);
    state->write(ptr,size,&num_written);
    return kode_vst3_ResultOk;
  }

//------------------------------
public: // IAudioProcessor
//------------------------------

  /*
    Try to set (from host) a predefined arrangement for inputs and outputs.
    The host should always deliver the same number of input and output buses
    than the Plug-in needs (see IComponent::getBusCount).
    The Plug-in returns kResultFalse if wanted arrangements are not supported.
    If the Plug-in accepts these arrangements, it should modify its buses to
    match the new arrangements
    (asked by the host with IComponent::getInfo() or
    IAudioProcessor::getBusArrangement ()) and then return kResultTrue.
    If the Plug-in does not accept these arrangements, but can adapt its
    current arrangements (according to the wanted ones), it should modify its
    buses arrangements and return kResultFalse.
  */

  //const uint64_t  kode_vst3_SpeakerL                      = 1 << 0;
  //const uint64_t  kode_vst3_SpeakerR                      = 1 << 1;
  // const SpeakerArrangement kStereo = kSpeakerL | kSpeakerR;


  int32_t KODE_VST3_PLUGIN_API setBusArrangements(uint64_t* inputs, int32_t numIns, uint64_t* outputs, int32_t numOuts) final {
    KODE_Vst3Print("numIns %i numOuts %i",numIns,numOuts);
    bool inputs_ok = false;
    bool outputs_ok = false;
    uint32_t num_inputs = MDescriptor->getNumInputs();
    uint32_t num_outputs = MDescriptor->getNumOutputs();
    if (numIns == 1) {
      if ((*inputs == kode_vst3_Mono) && (num_inputs == 1)) {
        KODE_Vst3DPrint(" {m_in}");
        inputs_ok = true;
      }
      if ((*inputs == kode_vst3_Stereo) && (num_inputs == 2)) {
        KODE_Vst3DPrint(" {st_in}");
        inputs_ok = true;
      }
    }
    if (numOuts == 1) {
      if ((*outputs == kode_vst3_Mono) && (num_outputs == 1)) {
        KODE_Vst3DPrint(" {m_out}");
        outputs_ok = true;
      }
      if ((*outputs == kode_vst3_Stereo) && (num_outputs == 2)) {
        KODE_Vst3DPrint(" {st_out}");
        outputs_ok = true;
      }
    }
    if (inputs_ok && outputs_ok) {
      KODE_Vst3DPrint(" -> True\n");
      return kode_vst3_ResultTrue;
    }
    else {
      KODE_Vst3DPrint(" -> False\n");
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  /*
    Gets the bus arrangement for a given direction (input/output) and index.
    Note: IComponent::getInfo() and IAudioProcessor::getBusArrangement()
    should be always return the same information about the buses arrangements.
  */

  int32_t KODE_VST3_PLUGIN_API getBusArrangement(int32_t dir, int32_t index, uint64_t& arr) final {
    KODE_Vst3Print("dir: %s index: %i",dir?"1/output":"0/input",index);
    if (index == 0) {
      switch (dir) {
        case kode_vst3_Input:
          switch (MDescriptor->getNumInputs()) {
            case 0:
              arr = kode_vst3_Empty;
              KODE_Vst3DPrint(" -> (Empty) -> False\n");
              return kode_vst3_ResultFalse;
            case 1:
              arr = kode_vst3_Mono;
              KODE_Vst3DPrint(" -> (Mono) -> Ok\n");
              return kode_vst3_ResultOk;
            case 2:
              KODE_Vst3DPrint(" -> (Stereo) -> Ok\n");
              arr = kode_vst3_Stereo;
              return kode_vst3_ResultOk;
          }
          break;
        case kode_vst3_Output:
          switch (MDescriptor->getNumOutputs()) {
            case 0:
              arr = kode_vst3_Empty;
              KODE_Vst3DPrint(" -> (Empty) -> False\n");
              return kode_vst3_ResultFalse;
            case 1:
              arr = kode_vst3_Mono;
              KODE_Vst3DPrint(" -> (Mono) -> Ok\n");
              return kode_vst3_ResultOk;
            case 2:
              KODE_Vst3DPrint(" -> (Stereo) -> Ok\n");
              arr = kode_vst3_Stereo;
              return kode_vst3_ResultOk;
          }
          break;
      } // switch dir
    } // index 0
    arr = kode_vst3_Empty;
    KODE_Vst3DPrint(" -> (Empty) -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    Asks if a given sample size is supported.
    see SymbolicSampleSizes.
  */

  int32_t KODE_VST3_PLUGIN_API canProcessSampleSize(int32_t symbolicSampleSize) final {
    //KODE_Vst3Print("size: %s",symbolicSampleSize?"1/Sample64":"0/Sample32");
    if (symbolicSampleSize==kode_vst3_Sample32) {
      //KODE_Vst3DPrint(" -> True\n");
      return kode_vst3_ResultTrue;
    }
    //KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    Gets the current Latency in samples.
    The returned value defines the group delay or the latency of the Plug-in.
    For example, if the Plug-in internally needs to look in advance (like
    compressors) 512 samples then this Plug-in should report 512 as latency.
    If during the use of the Plug-in this latency change, the Plug-in has to
    inform the host by using IComponentHandler::restartComponent(kLatencyChanged),
    this could lead to audio playback interruption because the host has to
    recompute its internal mixer delay compensation.
    Note that for player live recording this latency should be zero or small.
  */

  uint32_t KODE_VST3_PLUGIN_API getLatencySamples() final {
    //KODE_VST3PRINT;
    return 0;
  }

  //----------

  /*
    Called in disable state (not active) before processing will begin.
  */

  int32_t KODE_VST3_PLUGIN_API setupProcessing(KODE_Vst3ProcessSetup& setup) final {
    MProcessMode  = setup.processMode;        // kode_vst3_Realtime, kode_vst3_Prefetch, kode_vst3_Offline
    MSampleSize   = setup.symbolicSampleSize; // kode_vst3_Sample32, kode_vst3_Sample64
    MBlockSize    = setup.maxSamplesPerBlock;
    MSampleRate   = setup.sampleRate;
    on_plugin_prepare(MSampleRate,MBlockSize);
    KODE_Vst3Print("process_mode: %i sample_size %i block_size %i sample_rate %.2f -> ok\n",MProcessMode,MSampleSize,MBlockSize,MSampleRate);
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Informs the Plug-in about the processing state. This will be called
    before any process calls start with true and after with false.
    Note that setProcessing (false) may be called after setProcessing (true)
    without any process calls.
    In this call the Plug-in should do only light operation (no memory
    allocation or big setup reconfiguration), this could be used to reset
    some buffers (like Delay line or Reverb).
  */

  int32_t KODE_VST3_PLUGIN_API setProcessing(uint8_t state) final {
    MIsProcessing = state;
    //if (MIsProcessing) on_plugin_prepare(MSampleRate,MBlockSize);
    KODE_Vst3Print("state: %i -> Ok\n",state);
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    For any audio bus defined by the Plug-in the host must provide buffer data
    - even for inactive buses. Buses are addressed by index, so leaving out
    inactive buses will mix-up these indices. The actual data buffer can be
    null though. (see AudioBusBuffers).
    Note that channelBuffers32 (or channelBuffers64) buffers pointers could be
    the same or different for input and output: this has to be take into
    account in the process function (for example not reseting the output before
    processing if input and output buffers are the same!). It could be the same
    for multiple inputs or multiple outputs (case of instrument Plug-ins) all
    outputs (or inputs) could share the same buffer!
    Important: the host could call IAudioProcessor::process without buffers
    (numInputs and numOutputs of AudioBusBuffers are zeroed, numSamples too),
    in order to flush parameters (from host to Plug-in). Parameters flush could
    happen only when the host needs to send parameter changes and no processing
    is called.

    ---

    VST3_SDK/doc/vstsdk/faq.html#faqCommunication6
    Q: How my parameter changes (from UI interaction) are send to the
    processor if the host does not process?
    When a parameter is changed in the Plug-in UI by user action, the plug
    sends this change to the host with performEdit (do not forget to call
    beginEdit and endEdit), then the host has the responsibility to transfer
    this parameter change to the processor part: => if the audio engine is
    running (playing) this will be done in the next available process call =>
    if the audio engine is not running, the host has to flush parameter
    changes from time to time by sending them to processor by calling process
    (with audio buffer set to null), in this case the Plug-in should only
    update the parameters changes without processing any audio. This is very
    important that the host supports this flush mechanism else by saving
    Plug-ins state (project/preset) the host will not get the correct updated
    one.

    VST3_SDK/doc/vstinterfaces/index.html#vst3Editing
    Any change to a parameter that is caused by user interaction in the
    Plug-in GUI must be properly reported to the Steinberg::Vst::
    IComponentHandler. The host is responsible for transmitting the change to
    the processor. In order to make recording of automation work accordingly
    it is necessary to call beginEdit, performEdit and endEdit in the
    expected order!
    When the controller transmits a parameter change to the host, the host
    synchronizes the processor by passing the new values as Steinberg::Vst::
    IParameterChanges to the process call.

    ---

    http://www.noelborthwick.com/2014/01/22/developer-notes-sonar-x3-vst3-internals/
    The host may also send parameters to the plugin when no processing is
    active via a special “flush parameters” process call. This happens when
    the audio engine has been stopped in SONAR and the user manipulates an
    envelope or the user interface widget to change a plugin parameter.
    In this call numInputs and numOutputs of Steinberg::Vst::AudioBusBuffers
    are zeroed as well as numSamples. Plugin’s should expect and properly
    handle this call.
  */

  int32_t KODE_VST3_PLUGIN_API process(KODE_Vst3ProcessData& data) final {
    //KODE_VST3PRINT;
    //KODE_DPrint("data: %p in %i out %i spl %i\n",&data,data.numInputs,data.numOutputs,data.numSamples);
    handleEventsInProcess(data);
    handleParametersInProcess(data);
    //TODO: better error checking (num channels, ..)
    if ((data.numInputs == 0) && (data.numOutputs == 0)) {
      // no inpouts/outputs ???
      // something is wrong with the bus/channel stuff??
    }
    else {
      bool not_flush = (data.numSamples != 0);
      if (not_flush) {
        uint32_t i;
        for (i=0; i<MDescriptor->getNumInputs(); i++) {
          MProcessContext.inputs[i]  = data.inputs[0].channelBuffers32[i];
        }
        for (i=0; i<MDescriptor->getNumOutputs(); i++) {
          MProcessContext.outputs[i] = data.outputs[0].channelBuffers32[i];
        }
        MProcessContext.numinputs    = MDescriptor->getNumInputs();
        MProcessContext.numoutputs   = MDescriptor->getNumOutputs();
        MProcessContext.numsamples   = data.numSamples;
        //MProcessContext.oversample    = 1;
        MProcessContext.samplerate   = data.processContext->sampleRate;
        MProcessContext.samplepos    = data.processContext->continousTimeSamples;
        MProcessContext.beatpos      = data.processContext->projectTimeMusic;
        MProcessContext.tempo        = data.processContext->tempo;
        MProcessContext.timesignum   = data.processContext->timeSigNumerator;
        MProcessContext.timesigdenom = data.processContext->timeSigDenominator;
        MProcessContext.playstate    = KODE_PLUGIN_PLAYSTATE_NONE;
        if (data.processContext->state & KODE_Vst3ProcessContext::StatesAndFlags::kode_vst3_Playing) {
          MProcessContext.playstate |= KODE_PLUGIN_PLAYSTATE_PLAYING;
        }
        if (data.processContext->state & KODE_Vst3ProcessContext::StatesAndFlags::kode_vst3_Recording) {
          MProcessContext.playstate |= KODE_PLUGIN_PLAYSTATE_RECORDING;
        }
        if (data.processContext->state & KODE_Vst3ProcessContext::StatesAndFlags::kode_vst3_CycleActive) {
          MProcessContext.playstate |= KODE_PLUGIN_PLAYSTATE_LOOPING;
        }
        on_plugin_process(&MProcessContext);
      } // !flush
      //else {
      //  // flush?
      //  // already possibly done in handleEventsInProcess()
      //}
    } // in/out != 0
    /*
      https://forum.juce.com/t/vst3-plugin-wrapper/12323/5
      I recall the Steinberg Validator complaining that process() should just
      return kResultTrue...
    */
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Gets tail size in samples. For example, if the Plug-in is a Reverb
    Plug-in and it knows that the maximum length of the Reverb is 2sec, then
    it has to return in getTailSamples()
    (in VST2 it was getGetTailSize()) : 2*sampleRate.
    This information could be used by host for offline processing, process
    optimization and downmix (avoiding signal cut (clicks)).
    It should return:
    - kNoTail when no tail
    - x * sampleRate when x Sec tail.
    - kInfiniteTail when infinite tail.
  */

  uint32_t KODE_VST3_PLUGIN_API getTailSamples() final {
    //KODE_VST3PRINT;
    return kode_vst3_NoTail;
  }

//------------------------------
public: // IMidiMapping
//------------------------------

  /*
    busIndex              index of Input Event Bus
    channel               channel of the bus
    midiControllerNumber  see \ref ControllerNumbers for expected values (could be bigger than 127)
    id                    return the associated ParamID to the given midiControllerNumber

  todo:
    ParamId, bitmask..
    0x0000.0x0000 'normal' parameter
    0x0001.0x0000 midi
    etc..
  */


  int32_t KODE_VST3_PLUGIN_API getMidiControllerAssignment(int32_t busIndex, int16_t channel, int16_t midiControllerNumber, uint32_t& id) final {
    KODE_Vst3Print("bus: %i chn: %i ctrl: %i",busIndex,channel,midiControllerNumber);
    //if (busIndex == 0) {
      switch (midiControllerNumber) {
        case kode_vst3_AfterTouch: // 128
          id = KODE_VST3_PARAM_AFTERTOUCH + channel;
          KODE_Vst3DPrint(" -> (%i) Ok\n",id);
          return kode_vst3_ResultOk;
        case kode_vst3_PitchBend: // 129
          id = KODE_VST3_PARAM_PITCHBEND + channel;
          KODE_Vst3DPrint(" -> (%i) Ok\n",id);
          return kode_vst3_ResultOk;
        case kode_vst3_CtrlFilterResonance: // cc 74 (slide)
          id = KODE_VST3_PARAM_BRIGHTNESS + channel;
          KODE_Vst3DPrint(" -> (%i) Ok\n",id);
          return kode_vst3_ResultOk;
      }
    //}
    KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // INoteExpressionController
//------------------------------

  // Returns number of supported note change types for event bus index and channel.

  /*
  int32 PLUGIN_API getNoteExpressionCount(int32 busIndex, int16 channel) override {
    //if (busIndex==0) return 1;
    return 2;
  }
  */

  //

  //int32_t KODE_VST3_PLUGIN_API getNoteExpressionInfo(int32_t busIndex, int16 channel, int32_t noteExpressionIndex, NoteExpressionTypeInfo& info) final {
  //  //if (busIndex==0) {
  //    switch(noteExpressionIndex) {
  //      case 0:
  //        info.typeId                 = 0; // ??
  //        char_to_utf16("Tuning",info.title);
  //        char_to_utf16("Tun",info.shortTitle);
  //        char_to_utf16("",info.units);
  //        info.unitId                 = 0;
  //        info.valueDesc.defaultValue = 0.0;
  //        info.valueDesc.minimum      = 0;
  //        info.valueDesc.maximum      = 1;
  //        info.associatedParameterId  = -1;
  //        info.flags                  = NoteExpressionTypeInfo::kode_vst3_IsBipolar;
  //        return kode_vst3_ResultOk;
  //      case 1:
  //        info.typeId                 = 1; // ??
  //        char_to_utf16("Brightness",info.title);
  //        char_to_utf16("Bri",info.shortTitle);
  //        char_to_utf16("",info.units);
  //        info.unitId                 = 0;
  //        info.valueDesc.defaultValue = 0.0;
  //        info.valueDesc.minimum      = 0;
  //        info.valueDesc.maximum      = 1;
  //        info.associatedParameterId  = -1;
  //        info.flags                  = 0;
  //        return kode_vst3_ResultOk;
  //    }
  //  //}
  //  return kode_vst3_ResultFalse;
  //}

  //----------

  //int32_t KODE_VST3_PLUGIN_API getNoteExpressionStringByValue(int32_t busIndex, int16 channel, NoteExpressionTypeID id, NoteExpressionValue valueNormalized, KODE_Vst3String string) final {
  //  char temp[100];
  //  FloatToString(temp,valueNormalized);
  //  char_to_utf16(temp,string);
  //  return kode_vst3_ResultOk;
  //}

  //----------

  //int32_t KODE_VST3_PLUGIN_API getNoteExpressionValueByString(int32_t busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string, NoteExpressionValue& valueNormalized) final{
  //  char temp[129];
  //  utf16_to_char(string,temp);
  //  float res = const char*ToFloat(temp);
  //  valueNormalized = res;
  //  return kode_vst3_ResultOk;
  //}

//------------------------------
public: // IKeyswitchController
//------------------------------

  /*
    file:///WORK/code/VST3_SDK/doc/vstinterfaces/keyswitch.html

    ---

    When a (instrument) Plugin supports such interface, the host could get
    from the Plug-in the current set of used key switches (megatrig/
    articulation) for a given channel of a event bus and then automatically use
    them (like in Cubase 6) to create VST Expression Map (allowing to
    associated symbol to a given articulation / key switch).

    ---

    Returns number of supported key switches for event bus index and channel.
  */

  int32_t KODE_VST3_PLUGIN_API getKeyswitchCount(int32_t busIndex, int16_t channel) final {
    KODE_Vst3Print("bus: %i chn: %i -> 0\n",busIndex,channel);
    return 0;
  }

  //----------

  // Returns key switch info.

  int32_t KODE_VST3_PLUGIN_API getKeyswitchInfo(int32_t busIndex, int16_t channel, int32_t keySwitchIndex, KODE_Vst3KeyswitchInfo& info) final {
    KODE_Vst3Print("-> False\n");
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IConnectionPoint
//------------------------------

  /*
    file:///WORK/code/VST3_SDK/doc/vstinterfaces/index.html#vst3Communication

    ---

    This interface is used for the communication of separate components.
    Note that some hosts will place a proxy object between the components so
    that they are not directly connected.

    ---

    Connects this instance with another connection point.
  */

  int32_t KODE_VST3_PLUGIN_API connect(KODE_Vst3IConnectionPoint* other) final {
    //KODE_VST3PRINT;
    //IMessage* msg;
    //msg->setMessageID("test");
    //other->notify(msg);
    KODE_Vst3Print("-> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  // Disconnects a given connection point from this.

  int32_t KODE_VST3_PLUGIN_API disconnect(KODE_Vst3IConnectionPoint* other)  final {
    //KODE_VST3PRINT;
    KODE_Vst3Print("-> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  // Called when a message has been sent from the connection point to this.

  int32_t KODE_VST3_PLUGIN_API notify(KODE_Vst3IMessage* message) final {
    //KODE_VST3PRINT;
    KODE_Vst3Print("-> False\n");
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IUnitInfo
//------------------------------

  /*
    file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Units.html

    ---

    IUnitInfo describes the internal structure of the Plug-in.
    - The root unit is the component itself, so getUnitCount must return 1 at least.
    - The root unit id has to be 0 (kRootUnitId).
    - Each unit can reference one program list - this reference must not change.
    - Each unit using a program list, references one program of the list.

    --

    Returns the flat count of units.
  */

  int32_t KODE_VST3_PLUGIN_API getUnitCount() final {
    //KODE_VST3PRINT;
    KODE_Vst3Print("-> 1\n");
    return 1;
  }

  //----------

  // Gets UnitInfo for a given index in the flat list of unit.

  int32_t KODE_VST3_PLUGIN_API getUnitInfo(int32_t unitIndex, KODE_Vst3UnitInfo& info) final {
    KODE_Vst3Print("index: %i",unitIndex);
    if (unitIndex == 0) {
      info.id = kode_vst3_RootUnitId;
      info.parentUnitId = kode_vst3_NoParentUnitId;
      KODE_CharToUtf16("root",&info.name);
      info.programListId = kode_vst3_NoProgramListId;
      KODE_Vst3DPrint(" -> (id %i parent %i name '%s' progr: %i)",kode_vst3_RootUnitId,kode_vst3_NoParentUnitId,"root",kode_vst3_NoProgramListId);
      KODE_Vst3DPrint(" -> Ok\n");
      return kode_vst3_ResultOk;
    }
    KODE_Vst3DPrint("-> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  // Gets the count of Program List.

  int32_t KODE_VST3_PLUGIN_API getProgramListCount() final {
    KODE_Vst3Print("-> 0\n");
    return 0; // 1
  }

  //----------

  // Gets for a given index the Program List Info.

  int32_t KODE_VST3_PLUGIN_API getProgramListInfo(int32_t listIndex, KODE_Vst3ProgramListInfo& info) final {
    KODE_Vst3Print("index: %i\n",listIndex);
    if (listIndex == 0) {
      info.id = 0;
      KODE_CharToUtf16("program",&info.name);
      info.programCount = 1;
      KODE_Vst3DPrint(" -> (id %i name %s count: %i)",0,"program",1);
      KODE_Vst3DPrint(" -> Ok\n");
      return kode_vst3_ResultOk;
    }
      KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  // Gets for a given program list ID and program index its program name.

  int32_t KODE_VST3_PLUGIN_API getProgramName(int32_t listId, int32_t programIndex, KODE_Vst3String name) final {
    KODE_Vst3Print("list: %i program: %i",listId,programIndex);
    if ((listId == 0) && (programIndex == 0)) {
      KODE_CharToUtf16("program",&name);
      KODE_Vst3DPrint(" -> (%s)","program");
      KODE_Vst3DPrint(" -> Ok\n");
      return kode_vst3_ResultOk;
    }
      KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    Gets for a given program list ID, program index and attributeId the
    associated attribute value.
  */

  int32_t KODE_VST3_PLUGIN_API getProgramInfo(int32_t listId, int32_t programIndex, const char* attributeId, KODE_Vst3String attributeValue) final {
    //KODE_VST3PRINT;
    ////attributeId = "";
    //if ((listId == 0) && (programIndex == 0) /* attributeId */) {
    //  /*CharToUtf16*/ char_to_utf16("",attributeValue);
    //  return kode_vst3_ResultOk;
    //}
    return kode_vst3_ResultFalse;
  }

  //----------

  // Returns kResultTrue if the given program index of a given program list ID supports PitchNames.

  int32_t KODE_VST3_PLUGIN_API hasProgramPitchNames(int32_t listId, int32_t programIndex) final {
    //KODE_VST3PRINT;
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    Gets the PitchName for a given program list ID, program index and pitch.
    If PitchNames are changed the Plug-in should inform the host with
    IUnitHandler::notifyProgramListChange.
  */

  int32_t KODE_VST3_PLUGIN_API getProgramPitchName(int32_t listId, int32_t programIndex, int16_t midiPitch, KODE_Vst3String name) final {
    //KODE_VST3PRINT;
    //KODE_CharToUtf16("pitch",name);
    return kode_vst3_ResultFalse;
  }

  //----------

  // Gets the current selected unit.

  int32_t KODE_VST3_PLUGIN_API getSelectedUnit() final {
    //KODE_VST3PRINT;
    return 0;
  }

  //----------

  // Sets a new selected unit.

  int32_t KODE_VST3_PLUGIN_API selectUnit(int32_t unitId) final {
    //KODE_VST3PRINT;
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Gets the according unit if there is an unambiguous relation between a
    channel or a bus and a unit.
    This method mainly is intended to find out which unit is related to a
    given MIDI input channel.
  */

  int32_t KODE_VST3_PLUGIN_API getUnitByBus(int32_t type, int32_t dir, int32_t busIndex, int32_t channel, int32_t& unitId) final {
    //KODE_VST3PRINT;
    unitId = 0;
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Receives a preset data stream.
    - If the component supports program list data (IProgramListData), the destination of the data
      stream is the program specified by list-Id and program index (first and second parameter)
    - If the component supports unit data (IUnitData), the destination is the unit specified by the first
      parameter - in this case parameter programIndex is < 0).
  */

  int32_t KODE_VST3_PLUGIN_API setUnitProgramData(int32_t listOrUnitId, int32_t programIndex, KODE_Vst3IBStream* data) final {
    //KODE_VST3PRINT;
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IEditController
//------------------------------

  /*
    file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Automation.html

    ---

    // https://forum.juce.com/t/automation-issue-in-cubase-nuendo/14566/5
    // Cubase and Nuendo need to inform the host of the current parameter values
    if (AudioProcessor* const pluginInstance = getPluginInstance()) {
      for (int i = 0; i < pluginInstance->getNumParameters(); ++i)
        setParamNormalized((Vst::ParamID) i, (double) pluginInstance->getParameter (i));
    }
    return kResultFalse; // false?
  */

  int32_t KODE_VST3_PLUGIN_API setComponentState(KODE_Vst3IBStream* state) final {
    KODE_Vst3Print("state: %p -> Ok\n",state);
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    stores and restores any GUI settings that are not related to the processor
    (like scroll positions etc)

    When the states are restored, the host passes the processor state to both
    the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  int32_t KODE_VST3_PLUGIN_API setEditorState(KODE_Vst3IBStream* state) final {
    KODE_Vst3Print("state: %p -> Ok\n",state);
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getEditorState(KODE_Vst3IBStream* state) final {
    KODE_Vst3Print("state: %p -> Ok\n",state);
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    called repeatedly by bitwig, even if plugin is deactivated
  */

  int32_t KODE_VST3_PLUGIN_API getParameterCount() final {
    KODE_Vst3Print("-> %i\n",MDescriptor->getNumParameters());
    return MDescriptor->getNumParameters();
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getParameterInfo(int32_t paramIndex, KODE_Vst3ParameterInfo& info) final {
    //KODE_VST3PRINT;
    if (paramIndex >= 0) {
      if (paramIndex < (int32_t)MDescriptor->getNumParameters()) {
        //VST3_Parameter* param = MDescriptor->getParameter(paramIndex);
        //if (param) {
          memcpy(&info,&MParamInfos[paramIndex],sizeof(KODE_Vst3ParameterInfo));
          return kode_vst3_ResultOk;
        //}
      } // index < numparams
      else {
        switch (paramIndex) {
          case kode_vst3_AfterTouch: // 128
            break;
          case kode_vst3_PitchBend: // 129
            break;
          case kode_vst3_CtrlFilterResonance: // cc 74 (slide)
            break;
        }
        switch (paramIndex & 0xffff0000) {
          case KODE_VST3_PARAM_AFTERTOUCH:
            break;
          case KODE_VST3_PARAM_PITCHBEND:
            break;
          case KODE_VST3_PARAM_BRIGHTNESS:
            break;
        }
        return kode_vst3_ResultFalse;
      }
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getParamStringByValue(uint32_t id, double valueNormalized, KODE_Vst3String string) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
      char temp[129]; // ???
      KODE_Parameter* param = MDescriptor->getParameter(id);
      param->getDisplayString(valueNormalized,temp);
      KODE_CharToUtf16(temp,string);
      return kode_vst3_ResultOk;
    }
    else {
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getParamValueByString(uint32_t id, char16_t* string, double& valueNormalized) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
      char temp[129];
      KODE_Utf16ToChar(string,temp);
      float v = atoi(temp);
      KODE_Parameter* param = MDescriptor->getParameter(id);
      float v2 = param->to01(v);
      valueNormalized = v2;
      return kode_vst3_ResultOk;
    }
    else {
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  double KODE_VST3_PLUGIN_API normalizedParamToPlain(uint32_t id, double valueNormalized) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
      KODE_Parameter* param = MDescriptor->getParameter(id);
      float v = param->from01(valueNormalized);
      return v;
    }
    else {
      return valueNormalized;
    }
  }

  //----------

  double KODE_VST3_PLUGIN_API plainParamToNormalized(uint32_t id, double plainValue) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
      KODE_Parameter* param = MDescriptor->getParameter(id);
      float v = param->to01(plainValue);
      return v;
    }
    else {
      return plainValue;
    }
  }

  //----------

  double KODE_VST3_PLUGIN_API getParamNormalized(uint32_t id) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
      //float v = MEditorParameterValues[id];
      float v = MParameterValues[id];
      return v;
    }
    else {
      return 0;
    }
  }

  //----------

  /*
    The host is responsible for updating the Plug-in GUI when automation data
    is transmitted to the processor. This is realized by frequent calls of
    Steinberg::Vst::IEditController::setParamNormalized.
  */

  /*
    Sets the normalized value to the parameter associated to the paramID.
    The controller must never pass this value-change back to the host via the
    IComponentHandler. It should update the according GUI element(s) only!
  */


  int32_t KODE_VST3_PLUGIN_API setParamNormalized(uint32_t id, double value) final {
    //KODE_VST3PRINT;
    if (id >= MDescriptor->getNumParameters()) {
      return kode_vst3_ResultFalse; // ???
    }
    //MEditorParameterValues[id] = value;
    MParameterValues[id] = value;
    #ifndef KODE_NO_GUI
    if (MEditor) {
      MEditor->updateParameterFromHost(id,value);
    }
    #endif
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Gets from host a handler.
    aka host.. beginEdit, performEdit, endEdit, restartComponent
  */

  int32_t KODE_VST3_PLUGIN_API setComponentHandler(KODE_Vst3IComponentHandler* handler) final {
    KODE_Vst3Print("handler: %p -> Ok\n",handler);
    MComponentHandler = handler;
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Creates the editor view of the Plug-in, currently only "editor" is
    supported, see ViewType.
    The life time of the editor view will never exceed the life time of
    this controller instance.
  */

  KODE_Vst3IPlugView* KODE_VST3_PLUGIN_API createView(const char* name) final {
    KODE_Vst3Print("name: '%s'",name);
    if (MDescriptor->hasEditor()) {
      if (name && (strcmp(name,kode_vst3_Editor) == 0)) {
        addRef();
        KODE_Vst3DPrint(" -> %p\n",this);
        return (KODE_Vst3IPlugView*)this;
      }
    }
    KODE_Vst3DPrint(" -> NULL\n");
    return KODE_NULL;
  }

//------------------------------
public: // IEditController2
//------------------------------

  int32_t KODE_VST3_PLUGIN_API setKnobMode(int32_t mode) final {
    KODE_Vst3Print("mode: %i\n",mode);
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API openHelp(uint8_t onlyCheck) final {
    KODE_Vst3Print("only_check: %i -> False\n",onlyCheck);
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API openAboutBox(uint8_t onlyCheck) final {
    KODE_Vst3Print("only_check: %i -> False\n",onlyCheck);
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IPlugView
//------------------------------

  int32_t KODE_VST3_PLUGIN_API isPlatformTypeSupported(const char* type) final {
    KODE_Vst3Print("type: %s",type);
    #ifndef KODE_NO_GUI
      #ifdef KODE_LINUX
        //"X11EmbedWindowID"
        if (type && strcmp(type,kode_vst3_PlatformTypeX11EmbedWindowID) == 0) {
          KODE_Vst3DPrint(" -> Ok\n");
          return kode_vst3_ResultOk;
        }
      #endif // KODE_LINUX
    #endif // KODE_NO_GUI
    KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    The parent window of the view has been created, the (platform)
    representation of the view should now be created as well.

    Note that the parent is owned by the caller and you are not allowed
    to alter it in any way other than adding your own views. Note that
    in this call the Plug-in could call a IPlugFrame::resizeView ()!

    Parameters
      parent  : platform handle of the parent window or view
      type    : Platform UI Types which should be created
  */

  int32_t KODE_VST3_PLUGIN_API attached(void* parent, const char* type) final {
    KODE_Vst3Print("parent: %p type: %s",parent,type);
    #ifndef KODE_NO_GUI
      if (MDescriptor->hasEditor()) {
        if (MPlugFrame) {
          uint32_t w = MDescriptor->getEditorWidth();
          uint32_t h = MDescriptor->getEditorHeight();
          KODE_Vst3ViewRect r;
          r.left    = 0;
          r.top     = 0;
          r.right   = w;
          r.bottom  = h;
          MPlugFrame->resizeView(this,&r);
        }
        MEditor = (KODE_Editor*)on_plugin_openEditor(parent);
        updateAllEditorParameters(MEditor,false);
        MEditor->open();
        //if (MRunLoop)
        MRunLoop->registerTimer(this,KODE_VST3_TIMER_MS);
        //MRunLoop->registerEventHandler(this,0);
        KODE_Vst3DPrint(" -> Ok\n");
        return kode_vst3_ResultOk;
      }
    #endif // KODE_NO_GUI
    KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    The parent window of the view is about to be destroyed.
    You have to remove all your own views from the parent window or view.
  */

  int32_t KODE_VST3_PLUGIN_API removed() final {
    //KODE_VST3PRINT;
    #ifndef KODE_NO_GUI
    if (MDescriptor->hasEditor()) {
      //if (MRunLoop)
      MRunLoop->unregisterTimer(this);
      MEditor->close();
      on_plugin_closeEditor(MEditor);
      MEditor = KODE_NULL;
      KODE_Vst3Print(" -> Ok\n");
      return kode_vst3_ResultOk;
    }
    #endif // KODE_NO_GUI
    KODE_Vst3Print(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onWheel(float distance) final {
    KODE_Vst3Print("distance: %f -> False\n",distance);
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onKeyDown(char16_t key, int16_t keyCode, int16_t modifiers) final {
    KODE_Vst3Print("key: %i code: %i mod: %i -> Ok\n",key,keyCode,modifiers);
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onKeyUp(char16_t key, int16_t keyCode, int16_t modifiers) final {
    KODE_Vst3Print("key: %i code: %i mod: %i -> Ok\n",key,keyCode,modifiers);
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Returns the size of the platform representation of the view.
  */

  int32_t KODE_VST3_PLUGIN_API getSize(KODE_Vst3ViewRect* size) final {
    //KODE_VST3PRINT;
    if (MDescriptor->hasEditor()) {
      size->left    = 0;
      size->top     = 0;
      size->right   = MDescriptor->getEditorWidth();
      size->bottom  = MDescriptor->getEditorHeight();
      KODE_Vst3Print("%i,%i,%i,%i -> Ok\n",0,0,size->right,size->bottom);
      return kode_vst3_ResultOk;
    }
    KODE_Vst3Print("-> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    Resizes the platform representation of the view to the given rect.
    Note that if the Plug-in requests a resize (IPlugFrame::resizeView())
    onSize has to be called afterward.
  */

  int32_t KODE_VST3_PLUGIN_API onSize(KODE_Vst3ViewRect* newSize) final {
    KODE_Vst3Print("newsize: %i,%i,%i,%i\n",0,0,newSize->left,newSize->top,newSize->right,newSize->bottom);
    if (MDescriptor->hasEditor()) {
      //TODO: resize/redraw editor
      KODE_Vst3DPrint("-> Ok\n");
      return kode_vst3_ResultOk;
    }
    else {
      KODE_Vst3DPrint("-> False\n");
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  /*
    Focus changed message.
  */

  int32_t KODE_VST3_PLUGIN_API onFocus(uint8_t state) final {
    KODE_Vst3Print("state: %i -> Ok\n",state);
    return kode_vst3_ResultOk;
  }

  //----------

  /*
    Sets IPlugFrame object to allow the Plug-in to inform the host
    about resizing.
  */

  int32_t KODE_VST3_PLUGIN_API setFrame(KODE_Vst3IPlugFrame* frame) final {
    KODE_Vst3Print("frame: %p",frame);
    if (MDescriptor->hasEditor()) {
      MPlugFrame = frame;
      if (frame) {
        MPlugFrame->queryInterface(KODE_Vst3IRunLoop_iid, (void**)&MRunLoop);
      }
      KODE_Vst3DPrint(" -> Ok\n");
      return kode_vst3_ResultOk;
    }
    KODE_Vst3DPrint(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    Is view sizable by user?
  */

  int32_t KODE_VST3_PLUGIN_API canResize() final {
    KODE_Vst3Print(" -> False\n");
    return kode_vst3_ResultFalse;
  }

  //----------

  /*
    On live resize this is called to check if the view can be resized to the
    given rect, if not adjust the rect to the allowed size.
  */

  int32_t KODE_VST3_PLUGIN_API checkSizeConstraint(KODE_Vst3ViewRect* rect) final {
    KODE_Vst3Print(" -> Ok\n");
    return kode_vst3_ResultOk;
  }

//------------------------------
public: // ITimerHandler
//------------------------------

  // tid = same as setComponentHandler..

  /*
    will be called afte we register the timer:
      MRunLoop->registerTimer(this,KODE_PLUGIN_VST3_TIMER_MS);
    (in IEditView.attached)

  */

  void onTimer() final {
    //KODE_VST3PRINT;
    #ifndef KODE_NO_GUI
      if (MEditor) {
        on_plugin_updateEditor(MEditor);
      }
      flushParametersToHost();
      //}
    #endif
  }

//------------------------------
public: // IEventHandler
//------------------------------

  void KODE_VST3_PLUGIN_API onFDIsSet(int fd) {
    //KODE_VST3PRINT;
    KODE_Vst3Print("fd: %i\n",fd);
  }

};

//----------------------------------------------------------------------
#endif
