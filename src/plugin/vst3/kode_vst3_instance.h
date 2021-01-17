#ifndef kode_vst3_instance_included
#define kode_vst3_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_queue.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor_listener.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_process_context.h"
#include "plugin/base/kode_base_instance.h"
#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_utils.h"

#define KODE_PLUGIN_VST3_TIMER_MS 50

typedef KODE_Queue<uint32_t,1024> KODE_HostParameterQueue;

//----------------------------------------------------------------------

class KODE_Vst3Instance
: public VST3_IComponent
, public VST3_IAudioProcessor
, public VST3_IUnitInfo
, public VST3_IConnectionPoint
, public VST3_IMidiMapping
, public VST3_IKeyswitchController
//, public VST3_INoteExpressionController
, public VST3_IEditController
, public VST3_IEditController2
, public VST3_IPlugView
, public VST3_ITimerHandler
, public KODE_BaseInstance
, public KODE_EditorListener {

//------------------------------
private: // vst3
//------------------------------

  uint32_t                  MRefCount           = 1;
  VST3_IComponentHandler*   MComponentHandler   = KODE_NULL;
  VST3_IComponentHandler2*  MComponentHandler2  = KODE_NULL;
  VST3_IPlugFrame*          MPlugFrame          = KODE_NULL;
  VST3_IHostApplication*    MHostApp            = KODE_NULL;
  VST3_ParameterInfo*       MParamInfos         = KODE_NULL;
  VST3_IRunLoop*            MRunLoop            = KODE_NULL;
  uint32_t                  MIoMode             = 0;
  uint32_t                  MProcessMode        = 0;
  uint32_t                  MSampleSize         = 0;
  uint32_t                  MBlockSize          = 0;
  float                     MSampleRate         = 0.0f;
  bool                      MIsProcessing       = false;


//------------------------------
private:
//------------------------------

  #ifndef KODE_NO_GUI
  KODE_Editor*  MEditor       = KODE_NULL;
  uint32_t      MEditorWidth  = 0;
  uint32_t      MEditorHeight = 0;
  #endif

  float* MHostParameterValues = KODE_NULL;
  KODE_HostParameterQueue MHostParameterQueue;

//------------------------------
protected:
//------------------------------

  char MHostName[129] = {0};

//------------------------------
public:
//------------------------------

  KODE_Vst3Instance(KODE_Descriptor* ADescriptor)
  : KODE_BaseInstance(ADescriptor) {
    VST3_Print("\n");
    MRefCount   = 1;
    createHostParameterBuffers();
    createParameterInfo();
  }

  //----------

  virtual ~KODE_Vst3Instance() {
    VST3_Print("\n");
    deleteParameterInfo();
    destroyHostParameterBuffers();
  }

//------------------------------
public: // editor listener
//------------------------------

  // flushed in onTimer (calling flushParametersToHost)
  // (see bottom of this file)

  /*
    will host update the parameter for us?
  */

  void updateParameterFromEditor(uint32_t AIndex, float AValue) final {
    //KODE_PRINT;
    //KODE_Parameter* parameter = MDescriptor->getParameter(AIndex);
    //if (parameter) {
    //  float value = parameter->from01(AValue);
    //  on_parameter(AIndex,value,0);
    //}

    MParameterValues[AIndex] = AValue;
    queueParameterToHost(AIndex,AValue);

  }

  //----------

  //void resizeWindowFromEditor(uint32_t AWidth, uint32_t AHeight) final {
  //  //host_resizeEditor(AWidth,AHeight);
  //  if (MEditor) {
  //    MEditor->on_window_resize(AWidth,AHeight);
  //  }
  //}

  //----------

  //void sendMidiFromEditor() final {
  //}

//------------------------------
private: // host
//------------------------------

  /*
    enum RestartFlags {
      kReloadComponent            // The Component should be reloaded
      kIoChanged                  // Input and/or Output Bus configuration has changed
      kParamValuesChanged         // Multiple parameter values have changed (as result of a program change for example)
      kLatencyChanged             // Latency has changed (IAudioProcessor.getLatencySamples)
      kParamTitlesChanged         // Parameter titles or default values or flags have changed
      kMidiCCAssignmentChanged    // MIDI Controller Assignments have changed
      kNoteExpressionChanged      // Note Expression has changed (info, count...)
      kIoTitlesChanged            // Input and/or Output bus titles have changed
      kPrefetchableSupportChanged // Prefetch support has changed (\see IPrefetchableSupport)
      kRoutingInfoChanged         // RoutingInfo has changed (\see IComponent)
    };
    MComponentHandler->restartComponent(int32 flags)
  */

  /*
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

  //----------

//  void notifyHostAboutUpdatedParameters() {
//    uint32_t index;
//    while (MUpdatedHostParameters.read(&index)) {
//      float value = MHostValues[index];
//      KODE_Trace("%i %.2f\n",index,value);
//      if (MComponentHandler) {
//        //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
//        MComponentHandler->beginEdit(index);          // click
//        MComponentHandler->performEdit(index,value);  // drag
//        MComponentHandler->endEdit(index);            // release
//        //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
//      }
//    }
//  }

  //----------

  void host_updateParameter(uint32_t AIndex, float AValue) {
    //KODE_Print("index %i value %.3f\n",AIndex,AValue);
    if (MComponentHandler) {
      //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
      MComponentHandler->beginEdit(AIndex);          // click
      MComponentHandler->performEdit(AIndex,AValue);  // drag
      MComponentHandler->endEdit(AIndex);            // release
      //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
    }
  }

  //----------

  void host_resizeWindow(uint32_t AWidth, uint32_t AHeight) {
  }

  //----------

  void host_midiOut(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
  }


//------------------------------
private: // parameter to host
//------------------------------

  void createHostParameterBuffers() {
    uint32_t num = MDescriptor->getNumParameters();
    MHostParameterValues = (float*)KODE_Malloc(num * sizeof(float));
    KODE_Memset(MHostParameterValues,0,num * sizeof(float));
    //setDefaultParameterValues();
  }

  //----------

  void destroyHostParameterBuffers() {
    KODE_Free(MHostParameterValues);
  }

  //----------

  void queueParameterToHost(uint32_t AIndex, float AValue) {
    //KODE_Print("i %i v %.3f\n",AIndex,AValue);
    MHostParameterValues[AIndex] = AValue;
    MHostParameterQueue.write(AIndex);
  }

  //----------

  // called from onTimer
  // (see bottom of this file)

  void flushParametersToHost() {
    uint32_t index = 0;
    while (MHostParameterQueue.read(&index)) {
      float value = MHostParameterValues[index];
      host_updateParameter(index,value);
    }
  }

//----------------------------------------
private:
//----------------------------------------

  /*
    struct ParameterInfo {
      uint32_t id;                         // unique identifier of this parameter (named tag too)
      vst3str_t title;                    // parameter title (e.g. "Volume")
      vst3str_t shortTitle;               // parameter shortTitle (e.g. "Vol")
      vst3str_t units;                    // parameter unit (e.g. "dB")
      int32 stepCount;                    // number of discrete steps (0: continuous, 1: toggle, discrete value otherwise
                                          // (corresponding to max - min, for example: 127 for a min = 0 and a max = 127) - see \ref vst3parameterIntro)
      double defaultNormalizedValue;  // default normalized value [0,1] (in case of discrete value: defaultNormalizedValue = defDiscreteValue / stepCount)
      int32_t unitId;                      // id of unit this parameter belongs to (see vst3UnitsIntro)
      int32 flags;                        // ParameterFlags (see below)
      enum ParameterFlags {
        kCanAutomate      = 1 << 0,   // parameter can be automated
        kIsReadOnly       = 1 << 1,   // parameter cannot be changed from outside (implies that kCanAutomate is false)
        kIsWrapAround     = 1 << 2,   // attempts to set the parameter value out of the limits will result in a wrap around [SDK 3.0.2]
        kIsList           = 1 << 3,   // parameter should be displayed as list in generic editor or automation editing [SDK 3.1.0]
        kIsProgramChange  = 1 << 15,  // parameter is a program change (unitId gives info about associated unit
                                      // - see \ref vst3UnitPrograms)
        kIsBypass         = 1 << 16   // special bypass parameter (only one allowed): Plug-in can handle bypass
                                      // (highly recommended to export a bypass parameter for effect Plug-in)
      };
    };
  */

  /*
    called from
      KODE_Vst3Instance constructor
  */

  void createParameterInfo() {
    if (!MParamInfos) {
      uint32_t num = MDescriptor->getNumParameters();
      MParamInfos = (VST3_ParameterInfo*)KODE_Malloc( num * sizeof(VST3_ParameterInfo) );
      for (uint32_t i=0; i<num; i++) {
        KODE_Parameter* param = MDescriptor->getParameter(i);
        MParamInfos[i].id = i;
        vst3_char_to_utf16((char*)param->getName(),MParamInfos[i].title);
        vst3_char_to_utf16((char*)param->getShortName(),MParamInfos[i].shortTitle);
        vst3_char_to_utf16((char*)param->getLabel(),MParamInfos[i].units);
        if (param->isStepped() == true) MParamInfos[i].stepCount = param->getNumSteps() - 1;
        else MParamInfos[i].stepCount = 0;
        MParamInfos[i].defaultNormalizedValue = param->getDefValue();
        MParamInfos[i].unitId = vst3_RootUnitId;
        int32_t flags = 0;
        if (param->canAutomate()) flags += VST3_ParameterInfo::kCanAutomate;
        else flags += VST3_ParameterInfo::kIsReadOnly;
        MParamInfos[i].flags = flags;
      }
    }
  }

  //----------

  void deleteParameterInfo() {
    if (MParamInfos) KODE_Free(MParamInfos);
  }

  //----------

  /*
    class IParameterChanges: public FUnknown {
      virtual int32             VST3_PLUGIN_API getParameterCount();
      virtual IParamValueQueue* VST3_PLUGIN_API getParameterData(int32 index);
      virtual IParamValueQueue* VST3_PLUGIN_API addParameterData(const Vst::uint32_t& id, int32& index);
    };

    class IParamValueQueue {
      virtual uint32_t VST3_PLUGIN_API getParameterId();
      virtual int32   VST3_PLUGIN_API getPointCount();
      virtual int32_t VST3_PLUGIN_API getPoint (int32 index, int32& sampleOffset, double& value);
      virtual int32_t VST3_PLUGIN_API addPoint (int32 sampleOffset, double value, int32& index);
    };
  */

  void handleParametersInProcess(VST3_ProcessData& data) {
    VST3_IParameterChanges* paramChanges = data.inputParameterChanges;
    if (paramChanges) {
      int32_t num_param_changes = paramChanges->getParameterCount();
      if (num_param_changes > 0) {
        for (int32_t i=0; i<num_param_changes; i++) {
          VST3_IParamValueQueue* paramQueue = paramChanges->getParameterData(i);
          if (paramQueue) {
            uint32_t id = paramQueue->getParameterId();
            if (id < MDescriptor->getNumParameters()) {
              //for (int32_t j=0; j<paramQueue->getPointCount(); j++) {
                int32_t offset = 0;
                double value = 0;
                int32_t pointcount = paramQueue->getPointCount();
                paramQueue->getPoint(pointcount-1,offset,value); // last point
                KODE_Parameter* param = MDescriptor->getParameter(id);
                if (param) value = param->from01(value);
                on_parameter(id,value,KODE_PARAMETER_UPDATE_NORMAL);
              //}
            } // id < param
            else {
              // if KODE_PLUGIN_RECEIVE_MIDI
              // IMidiMapping
              int32_t offset = 0;
              double value = 0.5;
              int32_t pointcount = paramQueue->getPointCount();
              paramQueue->getPoint(pointcount-1,offset,value);      // last point
              uint32_t midi_ev = (id & 0xffff0000);
              uint32_t midi_ch = (id & 0x0000ffff);
              switch(midi_ev) {
                case VST3_PARAM_AFTERTOUCH: {
                  //if (offset != 0)
                  on_midi(offset,KODE_MIDI_CHANNEL_AFTERTOUCH+midi_ch,value*127.0f,0);
                  break;
                } // at
                case VST3_PARAM_PITCHBEND: {
                  float v2 = value * 16383.0f;
                  uint32_t i2 = (uint32_t)v2;
                  //if (midi_ch != 0) {
                    uint32_t m2 = i2 & 0x7f;
                    uint32_t m3 = i2 >> 7;
                    //if (offset != 0)
                    on_midi(offset,KODE_MIDI_PITCHBEND+midi_ch,m2,m3);
                  //}
                  break;
                } // pb
                case VST3_PARAM_BRIGHTNESS: {
                  //if (offset != 0)
                  on_midi(offset,KODE_MIDI_CONTROL_CHANGE+midi_ch,74,value*127.0f);
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

  /*
    class IEventList: public FUnknown {
      virtual int32   VST3_PLUGIN_API getEventCount();
      virtual int32_t VST3_PLUGIN_API getEvent(int32 index, Event& e);
      virtual int32_t VST3_PLUGIN_API addEvent(Event& e);
    };
  */

  void handleEventsInProcess(VST3_ProcessData& data) {
    //if KODE_PLUGIN_RECEIVE_MIDI
    VST3_IEventList* inputEvents = data.inputEvents;
    if (inputEvents) {
      int32_t num = inputEvents->getEventCount();
      for (int32_t i=0; i<num; i++) {
        VST3_Event event;
        inputEvents->getEvent(i,event);
        uint32_t offset  = 0;
        uint8_t  msg1    = 0;
        uint8_t  msg2    = 0;
        uint8_t  msg3    = 0;
        //uint32_t type    = 0;//kve_none;
        //uint32_t noteid  = 0;
        //float    value   = 0.0f;
        switch (event.type) {
          case VST3_Event::kNoteOnEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_NOTE_ON + event.noteOn.channel;
            msg2    = event.noteOn.pitch;
            msg3    = event.noteOn.velocity * 127;
            //noteid  = event.noteOn.noteId;
            on_midi(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case VST3_Event::kNoteOffEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_NOTE_OFF + event.noteOff.channel;
            msg2    = event.noteOff.pitch;
            msg3    = event.noteOff.velocity * 127;
            //noteid  = event.noteOff.noteId;
            on_midi(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case VST3_Event::kDataEvent:
            break;
          case VST3_Event::kPolyPressureEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_POLY_AFTERTOUCH + event.polyPressure.channel;
            msg2    = event.polyPressure.pitch;
            msg3    = event.polyPressure.pressure * 127;
            //noteid  = event.polyPressure.noteId;
            on_midi(offset,msg1,msg2,msg3);
            break;
          case VST3_Event::kNoteExpressionValueEvent:
            //offset = event.sampleOffset;
            //noteid = event.noteExpressionValue.noteId;
            //value  = event.noteExpressionValue.value;
            //switch(event.noteExpressionValue.typeId) {
            //  case kTuningTypeID:     type = kve_bend; break;
            //  case kBrightnessTypeID: type = kve_slide; break;
            //  //case kVolumeTypeID:     type = kve_none;  break;
            //  //case kPanTypeID:        type = kve_none;  break;
            //  //case kVibratoTypeID:    type = kve_none;  break;
            //  //case kExpressionTypeID: type = kve_none;  break;
            //}polyPressure
            //on_noteExpression(offset,type,noteid,value);
            break;
          case VST3_Event::kNoteExpressionTextEvent:
            break;
          case VST3_Event::kChordEvent:
            break;
          case VST3_Event::kScaleEvent:
            break;
          default:
            break;
        } // switch
      } // for all events
    } // if input events
  }

//----------------------------------------
public:
//----------------------------------------

  //--------------------
  // FUnknown
  //--------------------

  /*
    Adds a reference and return the new reference count.
    Remarks: The initial reference count after creating an object is 1.
  */

  uint32_t VST3_PLUGIN_API addRef() final {
    //VST3_Print("-> %i\n",MRefCount+1);
    MRefCount++;
    return MRefCount;
  }

  //----------

  /*
    Releases a reference and return the new reference count.
    If the reference count reaches zero, the object will be destroyed in
    memory.
  */

  uint32_t VST3_PLUGIN_API release() final {
    //VST3_Print("-> %i\n",MRefCount-1);
    const uint32_t r = --MRefCount; // const uint32 ?
    if (r == 0) {
      on_destroy();
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

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    Although it is not recommended, it is possible to implement both, the
    processing part and the controller part in one component class. The host
    tries to query the IEditController interface after creating an
    IAudioProcessor and on success uses it as controller.

    reaper asks for:
      C3B17BC0-2C174494-80293402-FBC4BBF8 (IContextInfoHandler,   plsextensions)
      31E29A7A-E55043AD-8B95B9B8-DA1FBE1E (IContextInfoHandler2,  pslextensions)

    bitwig asks for:
      6C389611-D391455D-B870B833-94A0EFDD (IUnitData)
      8683B01F-7B354F70-A2651DEC-353AF4FF (IProgramListData)
      7F4EFE59-F3204967-AC27A3AE-AFB63038 (IEditController2)
      65ED9690-8AC44525-8AADEF7A-72EA703F (IPlugViewContentScaleSupport)

    ( 65ed9690-8ac44525-8aadef7a-72ea703f (IPlugInViewScaling - pslextensions)

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

  int32_t VST3_PLUGIN_API queryInterface(const VST3_IID _iid, void** obj) final {

    VST3_Print("iid: ");
    VST3_PrintIID(_iid);

    *obj = nullptr;
    if ( vst3_iid_equal(IAudioProcessor_iid,_iid) ) {
      VST3_Print("= IAudioProcessor\n");
      *obj = (VST3_IAudioProcessor*)this;
      addRef();
      return kResultOk;
    }
    if ( vst3_iid_equal(IEditController_iid,_iid) ) {
      VST3_Print("= IEditController\n");
      *obj = (VST3_IEditController*)this;
      addRef();
      return kResultOk;
    }
    if ( vst3_iid_equal(IMidiMapping_iid,_iid) ) {
      VST3_Print("= IMidiMapping\n");
      *obj = (VST3_IMidiMapping*)this;
      addRef();
      return kResultOk;
    }
    if ( vst3_iid_equal(IUnitInfo_iid,_iid) ) {
      // reaper asks for this 3-4 times per second
      //VST3_Print("= IUnitInfo\n");
      *obj = (VST3_IUnitInfo*)this;
      addRef();
      return kResultOk;
    }
//    if ( vst3_iid_equal(INoteExpressionController_iid,_iid) ) {
//      VST3_Print("(INoteExpressionController)\n");
//      //*obj = (INoteExpressionController*)this;
//      //addRef();
//      //return kResultOk;
//      return kNoInterface;
//    }
    if ( vst3_iid_equal(IKeyswitchController_iid,_iid) ) {
      VST3_Print("(IKeyswitchController)\n");
      //*obj = (IKeyswitchController*)this;
      //addRef();
      //return kResultOk;
      return kNoInterface;
    }
    if ( vst3_iid_equal(IConnectionPoint_iid,_iid) ) {
      VST3_Print("(IConnectionPoint)\n");
      *obj = (VST3_IConnectionPoint*)this;
      addRef();
      return kResultOk;
    }
    //if ( vst3_iid_equal(ITimerHandler_iid,_iid) ) {
    //  VST3_Print("ITimerHandler\n");
    //  *obj = (ITimerHandler*)this;
    //  addRef();
    //  return kResultOk;
    //}
      VST3_Print("(unknown)\n");
    return kNoInterface;
  }

  //--------------------
  // IPluginBase
  //--------------------

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
  */

  /*
    class IHostApplication: public FUnknown {
      // Gets host application name.
      virtual int32_t VST3_PLUGIN_API getName (vst3str_t name) = 0;
      // Creates host object (e.g. Vst::IMessage).
      virtual int32_t VST3_PLUGIN_API createInstance (vst3iid_t cid, vst3iid_t _iid, void** obj) = 0;
  */

  /*
    3.6.12
    Allow a Plug-in to ask the host if a given Plug-in interface is
    supported/used by the host. It is implemented by the hostContext given
    when the component is initialized.
    int32_t VST3_PLUGIN_API MyPluginController::initialize (FUnknown* context) {
      // ...
      FUnknownPtr<IPlugInterfaceSupport> plugInterfaceSupport (context);
      if (plugInterfaceSupport) {
        if (plugInterfaceSupport->isPlugInterfaceSupported (IMidiMapping::iid) == kResultTrue)
          // IMidiMapping is used by the host
      }
      // ...
    }
  */

  /*
    IHostApplication: passed as 'context' in to IPluginBase::initialize()
  */

  int32_t VST3_PLUGIN_API initialize(VST3_FUnknown* context) final {
    //VST3_Print("\n");
    MHostApp = (VST3_IHostApplication*)context;
    //context->queryInterface(IHostApplication_iid, (void**)&MHostApp);
    if (MHostApp) {
      VST3_UTF16STR u;
      MHostApp->getName(u);
      vst3_utf16_to_char(u,MHostName);
      VST3_Print("host name: %s\n",MHostName);
    }
    else {
      VST3_Print("\n");
    }
    on_initialize();
    return kResultOk;
  }

  //----------

  /*
    This function is called before the Plug-in is unloaded and can be used
    for cleanups. You have to release all references to any host application
    interfaces.
  */

  int32_t VST3_PLUGIN_API terminate() final {
    VST3_Print("\n");
    on_terminate();
    return kResultOk;
  }

  //--------------------
  // IComponent
  //--------------------

  /*
    Called before initializing the component to get information about the
    controller class.

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    In order to enable the host to create a corresponding edit controller the
    processing component has to provide the matching class-ID. The host uses
    the module's class factory to create the controller component.
    (IComponent::getControllerClassId)
  */

  int32_t VST3_PLUGIN_API getControllerClassId(VST3_IID classId) final {
    VST3_Print("\n");
    if (MDescriptor->hasEditor()) {
      KODE_Memcpy(classId,MDescriptor->getLongEditorId(),16);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Called before 'initialize' to set the component usage (optional).
    See IoModes
  */

  int32_t VST3_PLUGIN_API setIoMode(int32_t mode) final {
    VST3_Print("mode %i\n",mode);
    //switch (mode) {
    //  case kSimple:             VST3_Trace("(kSimple)\n"); break;
    //  case kAdvanced:           VST3_Trace("(kAdvanced)\n"); break;
    //  case kOfflineProcessing:  VST3_Trace("(kOfflineProcessing)\n"); break;
    //  default:                  VST3_Trace("(unknown)\n"); break;
    //}
    MIoMode = mode;
    return kResultOk;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    enum MediaTypes {
      kAudio = 0,   // audio
      kEvent,       // events
      kNumMediaTypes
    };
    enum BusDirections {
      kInput = 0,   //input bus
      kOutput       // output bus
    };
  */

  // reaper calls this for each audio block?

  int32_t VST3_PLUGIN_API getBusCount(int32_t type, int32_t dir) final {
    //VST3_Print("type %i dir %i\n",type,dir);
    if (type == kAudio) {
      if ((dir == kOutput) && (MDescriptor->getNumOutputs() > 0)) { return 1; }
      if ((dir == kInput)  && (MDescriptor->getNumInputs()  > 0)) { return 1; };
    }
    if (type == kEvent) {
      //if (dir==kOutput) return 1;
      //else
      if (MDescriptor->canReceiveMidi()) {
        if (dir == kInput) { return 1; }
      }
    }
    return 0;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    struct BusInfo {
      int32_t mediaType;    // Media type - has to be a value of \ref MediaTypes
      int32_t direction; // input or output \ref BusDirections
      int32 channelCount;     // number of channels (if used then need to be recheck after \ref IAudioProcessor::setBusArrangements is called)
      vst3str_t name;         // name of the bus
      BusType busType;        // main or aux - has to be a value of \ref BusTypes
      uint32 flags;           // flags - a combination of \ref BusFlags
      enum BusFlags {
        kDefaultActive = 1 << 0	///< bus active per default
      };
    };
  */

  int32_t VST3_PLUGIN_API getBusInfo(int32_t type, int32_t dir, int32_t index, VST3_BusInfo& bus) final {
    VST3_Print("type %i dir %i index %i\n",type,dir,index);
    if (type == kAudio) {
      bus.mediaType = kAudio;
      if (dir == kInput) {
        bus.direction = kInput;
        bus.channelCount = MDescriptor->getNumInputs();
        vst3_char_to_utf16/*KODE_CharToUtf16*/("Audio In",bus.name);
      }
      else {
        bus.direction = kOutput;
        bus.channelCount = MDescriptor->getNumOutputs();
        vst3_char_to_utf16/*KODE_CharToUtf16*/("Audio Out",bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }
    else if (type == kEvent) {
      bus.mediaType = kEvent;
      if (dir == kInput) {
        bus.direction = kInput;
        bus.channelCount = 1;
        vst3_char_to_utf16/*KODE_CharToUtf16*/("Midi In",bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }

    return kResultFalse;
  }

  //----------

  /*
    Retrieves routing information (to be implemented when more than one
    regular input or output bus exists).
    The inInfo always refers to an input bus while the returned outInfo must
    refer to an output bus!

    struct RoutingInfo
    {
      int32_t mediaType;  // media type see \ref MediaTypes
      int32 busIndex;       // bus index
      int32 channel;        // channel (-1 for all channels)
    };
  */

  int32_t VST3_PLUGIN_API getRoutingInfo(VST3_RoutingInfo& inInfo, VST3_RoutingInfo& outInfo) final {
    VST3_Print("\n");
    outInfo.mediaType = inInfo.mediaType; // MediaTypes::kAudio;
    outInfo.busIndex  = inInfo.busIndex;  // 0;
    outInfo.channel   = -1;
    return kResultOk;
  }

  //----------

  /*
    Called upon (de-)activating a bus in the host application. The Plug-in
    should only processed an activated bus, the host could provide less
    AudioBusBuffers in the process call (see IAudioProcessor::process) if
    last buses are not activated
  */

  int32_t VST3_PLUGIN_API activateBus(int32_t type, int32_t dir, int32_t index, uint8_t state) final {
    VST3_Print("\n");
    return kResultOk;
  }

  //----------

  /*
    Activates / deactivates the component.
  */

  int32_t VST3_PLUGIN_API setActive(uint8_t state) final {
    VST3_Print("%i\n",state);
    if (state) on_activate();
    else on_deactivate();
    return kResultOk;
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
  */

  //----------

  /*
    Restore: When the states are restored, the host passes the processor state
    to both the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  /*
    IBStream->read(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesRead=0)     // how many bytes have been read from stream (set to 0 if this is of no interest)
    )

    IBStream->write(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesWritten=0)  // how many bytes have been written to stream (set to 0 if this is of no interest)
    )

  */

  /*
    Sets complete state of component.
    //state->read(MParamValues,MNumParameters*sizeof(float));
  */

  int32_t VST3_PLUGIN_API setState(VST3_IBStream* state) final {
    VST3_Print("\n");
    uint32_t version = 0;
    uint32_t mode = 0;
    int32_t size = 0;
    uint32_t num_params = 0;
    void*    ptr = KODE_NULL;
    int32_t num_read = 0;
    state->read(&version,sizeof(uint32_t),&num_read); //  KODE_Assert( num_read == sizeof(uint32_t) );
    state->read(&mode,sizeof(uint32_t),&num_read);    //  KODE_Assert( num_read == sizeof(uint32_t) );
    state->read(&size,sizeof(int32_t),&num_read);     //  KODE_Assert( num_read == sizeof(int32_t) );
    switch (mode) {
      case 0:

        // is it safe to malloc/free here?
        // TODO: use static, pre malloc'd buffer?

        ptr = KODE_Malloc(size);
        state->read(&ptr,size,&num_read);
        on_restoreState(size,ptr,0);
        KODE_Free(ptr);

        break;
      case 1:
        num_params = MDescriptor->getNumParameters();
        //KODE_Assert( (uint32_t)size == (num_params * sizeof(float)) );
        for (uint32_t i=0; i<num_params; i++) {
          float v = 0.f;
          state->read(&v,sizeof(float),&num_read);
          MParameterValues[i] = v;
        }
        updateAllParameters();
        break;
    }
    return kResultOk;
  }

  //----------

  /*
    Retrieves complete state of component.
    //state->write(MParamValues,MNumParameters*sizeof(float));

    mode
    0 = user
    1 = simple (all params)
  */

  int32_t VST3_PLUGIN_API getState(VST3_IBStream* state) final {
    VST3_Print("\n");
    uint32_t  version = MDescriptor->getVersion();
    uint32_t  mode    = 0;
    void*     ptr     = KODE_NULL;
    uint32_t  size    = on_saveState(&ptr,0);
    if ((size == 0) && (ptr == KODE_NULL)) {

      ptr = MParameterValues;//MInstance->getParamValues();
      size = MDescriptor->getNumParameters() * sizeof(float);
      mode = 1;
    }
    int num_written  = 0;
    state->write(&version,sizeof(uint32_t),&num_written); //  KODE_Assert( num_written == sizeof(uint32_t) );
    state->write(&mode,sizeof(uint32_t),&num_written);    //  KODE_Assert( num_written == sizeof(uint32_t) );
    state->write(&size,sizeof(uint32_t),&num_written);    //  KODE_Assert( num_written == sizeof(uint32_t) );
    state->write(ptr,size,&num_written);                  //  KODE_Assert( num_written == size );
    return kResultOk;
  }

  //--------------------
  // IAudioProcessor
  //--------------------

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

  // const SpeakerArrangement kStereo = kSpeakerL | kSpeakerR;

  int32_t VST3_PLUGIN_API setBusArrangements(uint64_t* inputs, int32_t numIns, uint64_t* outputs, int32_t numOuts) final {
    VST3_Print("\n");
    return kResultTrue;
  }

  //----------

  /*
    Gets the bus arrangement for a given direction (input/output) and index.
    Note: IComponent::getInfo() and IAudioProcessor::getBusArrangement()
    should be always return the same information about the buses arrangements.
  */

  int32_t VST3_PLUGIN_API getBusArrangement(int32_t dir, int32_t index, uint64_t& arr) final {
    VST3_Print("dir %i index %i\n",dir,index);
    if ((dir==kOutput) && (index==0)) {
      arr = vst3_SpeakerL | vst3_SpeakerR;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Asks if a given sample size is supported.
    see SymbolicSampleSizes.

    enum SymbolicSampleSizes {
      kSample32,    // 32-bit precision
      kSample64     // 64-bit precision
    };
  */

  int32_t VST3_PLUGIN_API canProcessSampleSize(int32_t symbolicSampleSize) final {
    VST3_Print("size %i\n",symbolicSampleSize);
    if (symbolicSampleSize==kSample32) {
      return kResultTrue;
    }
    return kResultFalse;// kResultFalse;// kNotImplemented;
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

  uint32_t VST3_PLUGIN_API getLatencySamples() final {
    //VST3_Print("\n");
    return 0;
  }

  //----------

  /*
    Called in disable state (not active) before processing will begin.
  */

  /*
    enum ProcessModes {
      kRealtime,  // realtime processing
      kPrefetch,  // prefetch processing
      kOffline    // offline processing
    };
    enum SymbolicSampleSizes {
      kSample32,  // 32-bit precision
      kSample64   // 64-bit precision
    };
    struct ProcessSetup {
      int32 processMode;        // ProcessModes
      int32 symbolicSampleSize; // \ref SymbolicSampleSizes
      int32 maxSamplesPerBlock; // maximum number of samples per audio block
      SampleRate sampleRate;    // sample rate (double)
    };
  */

  int32_t VST3_PLUGIN_API setupProcessing(VST3_ProcessSetup& setup) final {
    VST3_Print("samplerate %.2f\n",setup.sampleRate);
    MProcessMode  = setup.processMode;        // kRealtime, kPrefetch, kOffline
    MSampleSize   = setup.symbolicSampleSize; // kSample32, kSample64
    MBlockSize    = setup.maxSamplesPerBlock;
    MSampleRate   = setup.sampleRate;

    on_prepare(MSampleRate);

    return kResultOk;
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

  int32_t VST3_PLUGIN_API setProcessing(uint8_t state) final {
    VST3_Print("state %i\n",state);
    MIsProcessing = state;
//    if (MIsProcessing) on_prepare(MSampleRate);
    return kResultOk;
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
  */

  /*
    The Process call, where all information (parameter changes, event,
    audio buffer) are passed.

    struct AudioBusBuffers {
      int32   numChannels;            // number of audio channels in bus
      uint64  silenceFlags;           // Bitset of silence state per channel
      union {
        Sample32**  channelBuffers32; // sample buffers to process with 32-bit precision
        Sample64**  channelBuffers64; // sample buffers to process with 64-bit precision
      };
    };

    class IParamValueQueue {
      virtual uint32_t VST3_PLUGIN_API getParameterId();
      virtual int32   VST3_PLUGIN_API getPointCount();
      virtual int32_t VST3_PLUGIN_API getPoint (int32 index, int32& sampleOffset, double& value);
      virtual int32_t VST3_PLUGIN_API addPoint (int32 sampleOffset, double value, int32& index);
    };

    class IParameterChanges: public FUnknown {
      virtual int32             VST3_PLUGIN_API getParameterCount();
      virtual IParamValueQueue* VST3_PLUGIN_API getParameterData(int32 index);
      virtual IParamValueQueue* VST3_PLUGIN_API addParameterData(const Vst::uint32_t& id, int32& index);
    };

    class IEventList: public FUnknown {
      virtual int32   VST3_PLUGIN_API getEventCount();
      virtual int32_t VST3_PLUGIN_API getEvent(int32 index, Event& e);
      virtual int32_t VST3_PLUGIN_API addEvent(Event& e);
    };

    struct ProcessContext {
      // Transport state & other flags
      enum StatesAndFlags {
        kPlaying                = 1 << 1,   // currently playing
        kCycleActive            = 1 << 2,   // cycle is active
        kRecording              = 1 << 3,   // currently recording
        kSystemTimeValid        = 1 << 8,   // systemTime contains valid information
        kContTimeValid          = 1 << 17,  // continousTimeSamples contains valid information
        kProjectTimeMusicValid  = 1 << 9,   // projectTimeMusic contains valid information
        kBarPositionValid       = 1 << 11,  // barPositionMusic contains valid information
        kCycleValid             = 1 << 12,  // cycleStartMusic and barPositionMusic contain valid information
        kTempoValid             = 1 << 10,  // tempo contains valid information
        kTimeSigValid           = 1 << 13,  // timeSigNumerator and timeSigDenominator contain valid information
        kChordValid             = 1 << 18,  // chord contains valid information
        kSmpteValid             = 1 << 14,  // smpteOffset and frameRate contain valid information
        kClockValid             = 1 << 15   // samplesToNextClock valid
      };
      uint32        state;                // a combination of the values from \ref StatesAndFlags
      double        sampleRate;           // current sample rate (always valid)
      TSamples      projectTimeSamples;   // project time in samples (always valid)
      int64         systemTime;           // system time in nanoseconds (optional)
      TSamples      continousTimeSamples; // project time, without loop (optional)
      TQuarterNotes projectTimeMusic;     // musical position in quarter notes (1.0 equals 1 quarter note)
      TQuarterNotes barPositionMusic;     // last bar start position, in quarter notes
      TQuarterNotes cycleStartMusic;      // cycle start in quarter notes
      TQuarterNotes cycleEndMusic;        // cycle end in quarter notes
      double        tempo;                // tempo in BPM (Beats Per Minute)
      int32         timeSigNumerator;     // time signature numerator (e.g. 3 for 3/4)
      int32         timeSigDenominator;   // time signature denominator (e.g. 4 for 3/4)
      Chord         chord;                // musical info
      int32         smpteOffsetSubframes; // SMPTE (sync) offset in subframes (1/80 of frame)
      FrameRate     frameRate;            // frame rate
      int32         samplesToNextClock;   // MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest)
    };

    struct ProcessData {
      int32               processMode;            // processing mode - value of \ref ProcessModes
      int32               symbolicSampleSize;     // sample size - value of \ref SymbolicSampleSizes
      int32               numSamples;             // number of samples to process
      int32               numInputs;              // number of audio input buses
      int32               numOutputs;             // number of audio output buses
      AudioBusBuffers*    inputs;                 // buffers of input buses
      AudioBusBuffers*    outputs;                // buffers of output buses
      IParameterChanges*  inputParameterChanges;	// incoming parameter changes for this block
      IParameterChanges*  outputParameterChanges; // outgoing parameter changes for this block (optional)
      IEventList*         inputEvents;            // incoming events for this block (optional)
      IEventList*         outputEvents;           // outgoing events for this block (optional)
      ProcessContext*     processContext;         // processing context (optional, but most welcome)
    };

  */

  /*
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
  */

  /*
    http://www.noelborthwick.com/2014/01/22/developer-notes-sonar-x3-vst3-internals/
    The host may also send parameters to the plugin when no processing is
    active via a special “flush parameters” process call. This happens when
    the audio engine has been stopped in SONAR and the user manipulates an
    envelope or the user interface widget to change a plugin parameter.
    In this call numInputs and numOutputs of Steinberg::Vst::AudioBusBuffers
    are zeroed as well as numSamples. Plugin’s should expect and properly
    handle this call.
  */

  // assume only 1 input/output bus..

  int32_t VST3_PLUGIN_API process(VST3_ProcessData& data) final {
    //KODE_VST3PRINT;
    handleEventsInProcess(data);
    handleParametersInProcess(data);
    bool _flush = ( (data.numInputs == 0) && (data.numOutputs == 0) && (data.numSamples == 0) );
    if (!_flush) {
      KODE_ProcessContext context;// = {0};
      uint32_t i;
      for (i=0; i<MDescriptor->getNumInputs(); i++)   { context.inputs[i]  = data.inputs[0].channelBuffers32[i]; }
      for (i=0; i<MDescriptor->getNumOutputs(); i++)  { context.outputs[i] = data.outputs[0].channelBuffers32[i]; }
      context.numinputs     = MDescriptor->getNumInputs();
      context.numoutputs    = MDescriptor->getNumOutputs();
      context.numsamples    = data.numSamples;
      //context.oversample    = 1;
      context.samplerate    = data.processContext->sampleRate;
      context.samplepos     = data.processContext->continousTimeSamples;
      context.beatpos       = data.processContext->projectTimeMusic;
      context.tempo         = data.processContext->tempo;
      context.timesig_num   = data.processContext->timeSigNumerator;
      context.timesig_denom = data.processContext->timeSigDenominator;
      context.playstate     = KODE_PLUGIN_PLAYSTATE_NONE;
      if (data.processContext->state & VST3_ProcessContext::VST3_StatesAndFlags::kPlaying)      context.playstate |= KODE_PLUGIN_PLAYSTATE_PLAYING;
      if (data.processContext->state & VST3_ProcessContext::VST3_StatesAndFlags::kRecording)    context.playstate |= KODE_PLUGIN_PLAYSTATE_RECORDING;
      if (data.processContext->state & VST3_ProcessContext::VST3_StatesAndFlags::kCycleActive)  context.playstate |= KODE_PLUGIN_PLAYSTATE_LOOPING;
      on_processBlock(&context);
    }
    /*
      https://forum.juce.com/t/vst3-plugin-wrapper/12323/5
      I recall the Steinberg Validator complaining that process() should just
      return kResultTrue...
    */
    return kResultOk; // = kResulttrue (0)
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

  uint32_t VST3_PLUGIN_API getTailSamples() final {
    //KODE_VST3PRINT;
    return vst3_NoTail;
  }

  //--------------------
  // IMidiMapping
  //--------------------

  /*
    busIndex              index of Input Event Bus
    channel               channel of the bus
    midiControllerNumber  see \ref ControllerNumbers for expected values (could be bigger than 127)
    id                    return the associated ParamID to the given midiControllerNumber

    see ivstmidicontrollers.h

    kCtrlModWheel
    kCtrlFilterResonance (cc74) // Filter Resonance (Brightness)
    kAfterTouch
    kPitchBend

  */

  /*
    todo:
    ParamId, bitmask..
    0x0000.0x0000 'normal' parameter
    0x0001.0x0000 midi
    etc..
  */

  int32_t VST3_PLUGIN_API getMidiControllerAssignment(int32_t busIndex, int16_t channel, int16_t midiControllerNumber, uint32_t& id) final {
    //KODE_VST3PRINT;
    //if (busIndex == 0) {
      switch (midiControllerNumber) {
        case vst3_AfterTouch: // 128
          id = VST3_PARAM_AFTERTOUCH + channel;
          return kResultOk;
        case vst3_PitchBend: // 129
          id = VST3_PARAM_PITCHBEND + channel;
          return kResultOk;
        case vst3_CtrlFilterResonance: // cc 74 (slide)
          id = VST3_PARAM_BRIGHTNESS + channel;
          return kResultOk;
      }
    //}
    return kResultFalse;
  }

  //--------------------
  // INoteExpressionController
  //--------------------

      // Returns number of supported note change types for event bus index and channel.

  /*
  int32 VST3_PLUGIN_API getNoteExpressionCount(int32 busIndex, int16 channel) final {
    KODE_VST3PRINT;
    VST3_Trace("vst3: instance/getNoteExpressionCount busIndex:%i channel:%i\n",busIndex,channel);
    //if (busIndex==0) return 1;
    return 2;
  }

  //----------

  //flags: NoteExpressionTypeInfo::kIsBipolar, kIsOneShot, kIsAbsolute, kAssociatedParameterIDValid

  int32_t VST3_PLUGIN_API getNoteExpressionInfo(int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo& info) final {
    KODE_VST3PRINT;
    VST3_Trace("vst3: instance/getNoteExpressionInfo busIndex:%i channel:%i noteExpressionIndex:%i\n",busIndex,channel,noteExpressionIndex);
    //if (busIndex==0) {
      switch(noteExpressionIndex) {
        case 0:
          info.typeId                 = 0; // ??
          vst3_char_to_utf16("Tuning",info.title);
          vst3_char_to_utf16("Tun",info.shortTitle);
          vst3_char_to_utf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = NoteExpressionTypeInfo::kIsBipolar;
          return kResultOk;
        case 1:
          info.typeId                 = 1; // ??
          vst3_char_to_utf16("Brightness",info.title);
          vst3_char_to_utf16("Bri",info.shortTitle);
          vst3_char_to_utf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = 0;
          return kResultOk;
      }
    //}
    return kResultFalse;
  }

  //----------

  // Gets a user readable representation of the normalized note change value.

  int32_t VST3_PLUGIN_API getNoteExpressionStringByValue(int32 busIndex, int16 channel, NoteExpressionTypeID id, NoteExpressionValue valueNormalized, vst3str_t string) final {
    KODE_VST3PRINT;
    VST3_Trace("vst3: instance/getNoteExpressionStringByValue busIndex:%i channel:%i id:%i valueNormalized:%.3f\n",busIndex,channel,id,valueNormalized);
    char temp[100];
    KODE_FloatToString(temp,valueNormalized);
    vst3_char_to_utf16(temp,string);
    return kResultOk;
  }

  //----------

  // Converts the user readable representation to the normalized note change value.

  int32_t VST3_PLUGIN_API getNoteExpressionValueByString(int32 busIndex, int16 channel, NoteExpressionTypeID id, const char16_t* string, NoteExpressionValue& valueNormalized) final{
    KODE_VST3PRINT;
    VST3_Trace("vst3: instance/getNoteExpressionValueByString busIndex:%i  channel:%i id:%i string:%s\n",busIndex,channel,id,string);
    char temp[129];
    vst3_utf16_to_char(string,temp);
    float res = const char*ToFloat(temp);
    valueNormalized = res;
    return kResultOk;
  }

  */

  //--------------------
  // IKeyswitchController
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/keyswitch.html

  /*
    When a (instrument) Plugin supports such interface, the host could get
    from the Plug-in the current set of used key switches (megatrig/
    articulation) for a given channel of a event bus and then automatically use
    them (like in Cubase 6) to create VST Expression Map (allowing to
    associated symbol to a given articulation / key switch).
  */

  //----------

  // Returns number of supported key switches for event bus index and channel.

  int32_t VST3_PLUGIN_API getKeyswitchCount(int32_t busIndex, int16_t channel) final {
    //KODE_VST3PRINT;
    return 0;
  }

  //----------

  /*
  struct KeyswitchInfo {
    KeyswitchTypeID typeId; // see KeyswitchTypeID
    vst3str_t title;        // name of key switch (e.g. "Accentuation")
    vst3str_t shortTitle;   // short title (e.g. "Acc")
    int32 keyswitchMin;     // associated main key switch min (value between [0, 127])
    int32 keyswitchMax;     // associated main key switch max (value between [0, 127])
    int32 keyRemapped;      // optional remapped key switch (default -1), the Plug-in could provide one remapped key for a key switch (allowing better location on the keyboard of the key switches)
    int32 unitId;           // id of unit this key switch belongs to (see \ref vst3UnitsIntro), -1 means no unit used.
    int32 flags;            // not yet used (set to 0)
  };
  */

  // Returns key switch info.

  int32_t VST3_PLUGIN_API getKeyswitchInfo(int32_t busIndex, int16_t channel, int32_t keySwitchIndex, VST3_KeyswitchInfo& info) final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //--------------------
  // IConnectionPoint
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/index.html#vst3Communication

  /*
    This interface is used for the communication of separate components.
    Note that some hosts will place a proxy object between the components so
    that they are not directly connected.
  */

  //----------

  // Connects this instance with another connection point.

  /*
    class IConnectionPoint: public FUnknown {
    public:
      virtual int32_t VST3_PLUGIN_API connect (IConnectionPoint* other) = 0;
      virtual int32_t VST3_PLUGIN_API disconnect (IConnectionPoint* other) = 0;
      virtual int32_t VST3_PLUGIN_API notify (IMessage* message) = 0;
      static const FUID iid;
    };
  */

  int32_t VST3_PLUGIN_API connect(VST3_IConnectionPoint* other) final {
    //KODE_VST3PRINT;
    //IMessage* msg;
    //msg->setMessageID("test");
    //other->notify(msg);
    return kResultFalse;
  }

  //----------

  //Disconnects a given connection point from this.

  int32_t VST3_PLUGIN_API disconnect(VST3_IConnectionPoint* other)  final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //----------

  // Called when a message has been sent from the connection point to this.

  /*
  class IMessage: public FUnknown {
  public:
    virtual FIDString VST3_PLUGIN_API getMessageID () = 0;
    virtual void VST3_PLUGIN_API setMessageID (FIDString id) = 0;
    virtual IAttributeList* VST3_PLUGIN_API getAttributes () = 0;
    static const FUID iid;
  };
  */

  int32_t VST3_PLUGIN_API notify(VST3_IMessage* message) final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //--------------------
  // IUnitInfo
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Units.html

  /*
    IUnitInfo describes the internal structure of the Plug-in.
    - The root unit is the component itself, so getUnitCount must return 1 at least.
    - The root unit id has to be 0 (kRootUnitId).
    - Each unit can reference one program list - this reference must not change.
    - Each unit using a program list, references one program of the list.
  */

  //----------

  // Returns the flat count of units.

  int32_t VST3_PLUGIN_API getUnitCount() final {
    //KODE_VST3PRINT;
    return 1;
  }

  //----------

  // Gets UnitInfo for a given index in the flat list of unit.

  /*
  struct UnitInfo {
    int32_t id;                    // unit identifier
    int32_t parentUnitId;          // identifier of parent unit (kNoParentUnitId: does not apply, this unit is the root)
    vst3str_t name;               // name, optional for the root component, required otherwise
    int32_t programListId;  // id of program list used in unit (kNoProgramListId = no programs used in this unit)
  };
  */

  int32_t VST3_PLUGIN_API getUnitInfo(int32_t unitIndex, VST3_UnitInfo& info) final {
    //KODE_VST3PRINT;
    if (unitIndex==0) {
      info.id = vst3_RootUnitId; //0;
      info.parentUnitId = vst3_NoParentUnitId;
      vst3_char_to_utf16("root",info.name);
      info.programListId = vst3_NoProgramListId;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets the count of Program List.

  int32_t VST3_PLUGIN_API getProgramListCount() final {
    //KODE_VST3PRINT;
    return 0; // 1
  }

  //----------

  // Gets for a given index the Program List Info.

  /*
  struct ProgramListInfo {
    int32_t id;     // program list identifier
    vst3str_t name;       // name of program list
    int32 programCount;   // number of programs in this list
  };
  */

  int32_t VST3_PLUGIN_API getProgramListInfo(int32_t listIndex, VST3_ProgramListInfo& info) final {
    //KODE_VST3PRINT;
    if (listIndex == 0) {
      info.id = 0;
      vst3_char_to_utf16("program",info.name);
      info.programCount = 1;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets for a given program list ID and program index its program name.

  int32_t VST3_PLUGIN_API getProgramName(int32_t listId, int32_t programIndex, VST3_UTF16STR name) final {
    //KODE_VST3PRINT;
    if ((listId == 0) && (programIndex == 0)) {
      vst3_char_to_utf16("program",name);
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Gets for a given program list ID, program index and attributeId the
    associated attribute value.
  */

  //CString attributeId, vst3str_t attributeValue) {
  int32_t VST3_PLUGIN_API getProgramInfo(int32_t listId, int32_t programIndex, /*Steinberg::Vst::*/const char* attributeId, VST3_UTF16STR attributeValue) final {
    //KODE_VST3PRINT;
    ////attributeId = "";
    //if ((listId == 0) && (programIndex == 0) /* attributeId */) {
    //  vst3_char_to_utf16("",attributeValue);
    //  return kResultOk;
    //}
    return kResultFalse;
  }

  //----------

  // Returns kResultTrue if the given program index of a given program list ID supports PitchNames.

  int32_t VST3_PLUGIN_API hasProgramPitchNames(int32_t listId, int32_t programIndex) final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //----------

  /*
    Gets the PitchName for a given program list ID, program index and pitch.
    If PitchNames are changed the Plug-in should inform the host with
    IUnitHandler::notifyProgramListChange.
  */

  int32_t VST3_PLUGIN_API getProgramPitchName(int32_t listId, int32_t programIndex, int16_t midiPitch, VST3_UTF16STR name) final {
    //KODE_VST3PRINT;
    //vst3_char_to_utf16("pitch",name);
    return kResultFalse;
  }

  //----------

  // Gets the current selected unit.

  int32_t VST3_PLUGIN_API getSelectedUnit() final {
    //KODE_VST3PRINT;
    return 0;
  }

  //----------

  // Sets a new selected unit.

  int32_t VST3_PLUGIN_API selectUnit(int32_t unitId) final {
    //KODE_VST3PRINT;
    return kResultOk;
  }

  //----------

  /*
    Gets the according unit if there is an unambiguous relation between a
    channel or a bus and a unit.
    This method mainly is intended to find out which unit is related to a
    given MIDI input channel.
  */

  int32_t VST3_PLUGIN_API getUnitByBus(int32_t type, int32_t dir, int32_t busIndex, int32_t channel, int32_t& unitId) final {
    //KODE_VST3PRINT;
    unitId = 0;
    return kResultOk;//False;
  }

  //----------

  /*
    Receives a preset data stream.
    - If the component supports program list data (IProgramListData), the destination of the data
      stream is the program specified by list-Id and program index (first and second parameter)
    - If the component supports unit data (IUnitData), the destination is the unit specified by the first
      parameter - in this case parameter programIndex is < 0).
  */

  int32_t VST3_PLUGIN_API setUnitProgramData(int32_t listOrUnitId, int32_t programIndex, VST3_IBStream* data) final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //--------------------
  // IEditController
  //--------------------

    // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Automation.html

  //----------

  /*
    // https://forum.juce.com/t/automation-issue-in-cubase-nuendo/14566/5
    // Cubase and Nuendo need to inform the host of the current parameter values
    if (AudioProcessor* const pluginInstance = getPluginInstance()) {
      for (int i = 0; i < pluginInstance->getNumParameters(); ++i)
        setParamNormalized((Vst::uint32_t) i, (double) pluginInstance->getParameter (i));
    }
    return kResultFalse; // false?
  */

  int32_t VST3_PLUGIN_API setComponentState(VST3_IBStream* state) final {
    //KODE_VST3PRINT;
    /*
    // we receive the current state of the component (processor part)
    if (state == nullptr) return kResultFalse;
    // using helper to read the stream
    IBStreamer streamer(state, kLittleEndian);
    // ABSwitchParamID::kAudioSwitch
    float savedParam1 = 0.f;
    if (!streamer.readFloat(savedParam1)) return kResultFalse;
    setParamNormalized(ABSwitchParamID::kAudioSwitch, savedParam1);
    return kResultOk;
    */
    //return kResultOk;//False;

    /*
      https://forum.juce.com/t/possible-demo-plugin-bug/19131/9
      "..which in turn returns kNotImplemented, should you not be returning
      kResultOk here?"
      "That is intended. I can’t remember why but some DAWs needed that."

    */

    /*
    int num_read = 0;
    uint32_t test_int = 0;
    state->read(&test_int,4,&num_read);
    */

    // MEditor->updateParameterFromHost(param,index,value);
    return kResultOk;
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

  //int32_t VST3_PLUGIN_API setState(IBStream* state) {
  int32_t VST3_PLUGIN_API setEditorState(VST3_IBStream* state) final {
    //KODE_VST3PRINT;
    return kResultOk;
  }

  //----------

  //int32_t VST3_PLUGIN_API getState(IBStream* state) {
  int32_t VST3_PLUGIN_API getEditorState(VST3_IBStream* state) final {
    //KODE_VST3PRINT;
    return kResultOk;
  }

  //----------

  /*
    called repeatedly by bitwig, even if plugin is deactivated
  */

  int32_t VST3_PLUGIN_API getParameterCount() final {
    //KODE_VST3PRINT;
    return MDescriptor->getNumParameters();
  }

  //----------

  int32_t VST3_PLUGIN_API getParameterInfo(int32_t paramIndex, VST3_ParameterInfo& info) final {
    //KODE_VST3PRINT;
    if (paramIndex >= 0) {
      if (paramIndex < (int32_t)MDescriptor->getNumParameters()) {
        KODE_Parameter* param = MDescriptor->getParameter(paramIndex);
        if (param) {
          KODE_Memcpy(&info,&MParamInfos[paramIndex],sizeof(VST3_ParameterInfo));
          return kResultOk;
        }
      } // index < numparams
      else {
        //KODE_Trace("paramIndex (%08x) >= num.params\n",paramIndex);
        switch (paramIndex) {
          case vst3_AfterTouch: // 128
            break;
          case vst3_PitchBend: // 129
            break;
          case vst3_CtrlFilterResonance: // cc 74 (slide)
            break;
        }
        switch (paramIndex & 0xffff0000) {
          case VST3_PARAM_AFTERTOUCH:
            break;
          case VST3_PARAM_PITCHBEND:
            break;
          case VST3_PARAM_BRIGHTNESS:
            break;
        }
        //return kResultOk;
        return kResultFalse;
      }
    }
    return kResultFalse;
  }

  //----------

  int32_t VST3_PLUGIN_API getParamStringByValue(uint32_t id, double valueNormalized, VST3_UTF16STR string) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
      KODE_Parameter* param = MDescriptor->getParameter(id);
      char buffer[32]; // ???
      param->getDisplayText(valueNormalized,buffer);
      vst3_char_to_utf16/*KODE_CharToUtf16*/(buffer,string);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  int32_t VST3_PLUGIN_API getdoubleByString(uint32_t id, char16_t* string, double& valueNormalized) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
      KODE_Parameter* param = MDescriptor->getParameter(id);
      char temp[129];
      vst3_utf16_to_char/*KODE_Utf16ToChar*/(string,temp);
      float v = atoi(temp);
      float v2 = param->to01(v);
      valueNormalized = v2;
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  double VST3_PLUGIN_API normalizedParamToPlain(uint32_t id, double valueNormalized) final {
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

  double VST3_PLUGIN_API plainParamToNormalized(uint32_t id, double plainValue) final {
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

  double VST3_PLUGIN_API getParamNormalized(uint32_t id) final {
    //KODE_VST3PRINT;
    if (id < MDescriptor->getNumParameters()) {
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

  // bitwig sends a ParamID = 0x3f800000

  int32_t VST3_PLUGIN_API setParamNormalized(uint32_t id, double value) final {
    //KODE_VST3PRINT;
    if (id >= MDescriptor->getNumParameters()) {
      return kResultFalse; // ???
    }
    MParameterValues[id] = value;
    //MEditorValues[id] = value;

    #ifdef KODE_NO_GUI
      KODE_Parameter* parameter = MDescriptor->getParameter(id);
      float v = parameter->from01(value);
      on_parameter(id,v,0);
    #else
      if (MEditor) {
        //MEditor->setParameterValueAndRedraw(id,value);
        MEditor->parameterUpdatedFromHost(id,value);
      }
    #endif // KODE_NO_GUI
    //KODE_Parameter* parameter = MDescriptor->getParameter(id);
    //float v = parameter->from01(value);
    //on_updateParameter(id,v);
    return kResultOk;
  }

  //----------

  /*
    Gets from host a handler.
    aka host.. beginEdit, performEdit, endEdit, restartComponent
  */

  /*
  class IComponentHandler: public FUnknown {
  public:
    virtual int32_t VST3_PLUGIN_API beginEdit (uint32_t id) = 0;
    virtual int32_t VST3_PLUGIN_API performEdit (uint32_t id, double valueNormalized) = 0;
    virtual int32_t VST3_PLUGIN_API endEdit (uint32_t id) = 0;
    virtual int32_t VST3_PLUGIN_API restartComponent (int32 flags) = 0;
    static const FUID iid;
  };

  notifyHostParameterChanged
    MComponentHandler->begin/perform/endEdit

  */

  int32_t VST3_PLUGIN_API setComponentHandler(VST3_IComponentHandler* handler) final {
    //KODE_VST3PRINT;
    MComponentHandler = handler;
    //handler->queryInterface(IComponentHandler2::iid,(void**)&MComponentHandler2);
    //KODE_Vst3Host* host = (KODE_Vst3Host*)MInstance->getHost();
    //if (host) host->setComponentHandler(MComponentHandler,MComponentHandler2);
    return kResultOk;
  }

  //----------

  /*
    Creates the editor view of the Plug-in, currently only "editor" is
    supported, see ViewType.
    The life time of the editor view will never exceed the life time of
    this controller instance.
  */

  VST3_IPlugView* VST3_PLUGIN_API createView(const char* name) final {
    //KODE_VST3PRINT;
    if (MDescriptor->hasEditor()) {
      if (name && (KODE_Strcmp(name,vst3_Editor) == 0)) {
        addRef();
        return (VST3_IPlugView*)this;
      }
    }
    return nullptr;
  }

  //--------------------
  // IEditController2
  //--------------------

  int32_t VST3_PLUGIN_API setKnobMode(int32_t mode) final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //----------

  int32_t VST3_PLUGIN_API openHelp(uint8_t onlyCheck) final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //----------

  int32_t VST3_PLUGIN_API openAboutBox(uint8_t onlyCheck) final {
    //KODE_VST3PRINT;
    return kResultFalse;
  }

  //--------------------
  // IPlugView
  //--------------------

  int32_t VST3_PLUGIN_API isPlatformTypeSupported(const char* type) final {
    //KODE_VST3PRINT;
    // "X11EmbedWindowID"
    if (type && KODE_Strcmp(type,vst3_PlatformTypeX11EmbedWindowID) == 0) {
      return kResultOk;
    }
    return kResultFalse;
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

  int32_t VST3_PLUGIN_API attached(void* parent, const char* type) final {
    //KODE_VST3PRINT;

    #ifndef KODE_NO_GUI
      if (MDescriptor->hasEditor()) {
        if (MPlugFrame) {
          //uint32_t w = MDescriptor->getEditorWidth();
          //uint32_t h = MDescriptor->getEditorHeight();
          //if (w == 0) w = MInstance->getDefaultEditorWidth();
          //if (h == 0) h = MInstance->getDefaultEditorHeight();
          VST3_ViewRect r;
          r.left    = 0;
          r.top     = 0;
          r.right   = MEditorWidth;
          r.bottom  = MEditorHeight;
          MPlugFrame->resizeView(this,&r);
        }
        MEditor = on_openEditor(parent);

        MEditorWidth = MDescriptor->getEditorWidth();
        MEditorHeight = MDescriptor->getEditorHeight();
        //KODE_Print("MEditorWidth %i MEditorHeight %i\n",MEditorWidth,MEditorHeight);

// set widget values?

        MEditor->open();
        //if (MRunLoop)
        MRunLoop->registerTimer(this,KODE_PLUGIN_VST3_TIMER_MS);
        return kResultOk;
      }
    #endif // KODE_NO_GUI
    return kResultFalse;
  }

  //----------

  /*
    The parent window of the view is about to be destroyed.
    You have to remove all your own views from the parent window or view.
  */

  int32_t VST3_PLUGIN_API removed() final {
    //KODE_VST3PRINT;

      #ifndef KODE_NO_GUI
      if (MDescriptor->hasEditor()) {

        MEditorWidth = MEditor->getWindowWidth();
        MEditorHeight = MEditor->getWindowHeight();
        //KODE_Print("MEditorWidth %i MEditorHeight %i\n",MEditorWidth,MEditorHeight);

        //if (MRunLoop)
        MRunLoop->unregisterTimer(this);
        MEditor->close();
        on_closeEditor(MEditor);
        MEditor = nullptr;
        return kResultOk;
      }
      #endif // KODE_NO_GUI
    return kResultFalse;
  }

  //----------

  int32_t VST3_PLUGIN_API onWheel(float distance) final {
    //KODE_VST3PRINT;
    return kResultOk;
  }

  //----------

  int32_t VST3_PLUGIN_API onKeyDown(char16_t key, int16_t keyCode, int16_t modifiers) final {
    //KODE_VST3PRINT;
    return kResultOk;
  }

  //----------

  int32_t VST3_PLUGIN_API onKeyUp(char16_t key, int16_t keyCode, int16_t modifiers) final {
    //KODE_VST3PRINT;
    return kResultOk;
  }

  //----------

  /*
    Returns the size of the platform representation of the view.
  */

  int32_t VST3_PLUGIN_API getSize(VST3_ViewRect* size) final {
    //KODE_VST3PRINT;
    #ifndef KODE_NO_GUI
    if (MDescriptor->hasEditor()) {

      //KODE_Print("MEditorWidth %i MEditorHeight %i\n",MEditorWidth,MEditorHeight);

      size->left    = 0;
      size->top     = 0;
      size->right   = MEditorWidth;//MDescriptor->getEditorWidth();
      size->bottom  = MEditorHeight;//MDescriptor->getEditorHeight();
      return kResultOk;
    }
    #endif
    return kResultFalse;
  }

  //----------

  /*
    Resizes the platform representation of the view to the given rect.
    Note that if the Plug-in requests a resize (IPlugFrame::resizeView())
    onSize has to be called afterward.
  */

  int32_t VST3_PLUGIN_API onSize(VST3_ViewRect* newSize) final {
    //KODE_VST3PRINT;
    //KODE_PRINT;
    #ifndef KODE_NO_GUI
    if (MEditor) {

      MEditorWidth = newSize->getWidth();
      MEditorHeight = newSize->getHeight();
      //KODE_Print("MEditorWidth %i MEditorHeight %i\n",MEditorWidth,MEditorHeight);


      MEditor->on_window_resize(MEditorHeight,MEditorHeight);
      return kResultOk;
    }
    #endif
    return kResultFalse;
  }

  //----------

  /*
    Focus changed message.
  */

  int32_t VST3_PLUGIN_API onFocus(uint8_t state) final {
    //KODE_VST3PRINT;
    return kResultOk;
  }

  //----------

  /*
    Sets IPlugFrame object to allow the Plug-in to inform the host
    about resizing.
  */

  int32_t VST3_PLUGIN_API setFrame(VST3_IPlugFrame* frame) final {
    //KODE_VST3PRINT;

    if (MDescriptor->hasEditor()) {
      MPlugFrame = frame;
      //int32_t res =
      MPlugFrame->queryInterface(IRunLoop_iid, (void**)&MRunLoop);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Is view sizable by user?
  */

  int32_t VST3_PLUGIN_API canResize() final {
    //KODE_VST3PRINT;
    //KODE_PRINT;
    if (MDescriptor->hasEditor() && MDescriptor->canResizeEditor()) {
      return true;
    }
    return kResultFalse;
  }

  //----------

  /*
    On live resize this is called to check if the view can be resized to the
    given rect, if not adjust the rect to the allowed size.
  */

  int32_t VST3_PLUGIN_API checkSizeConstraint(VST3_ViewRect* rect) final {
    //KODE_VST3PRINT;
    int32_t left = rect->left;
    int32_t top = rect->top;
    int32_t right = rect->right;
    int32_t bottom = rect->bottom;
    KODE_Print("left %i top %i right %i bottom %i \n",left,top,right,bottom);
    return kResultOk;//False;
  }

  //--------------------
  // ITimerHandler
  //--------------------

  // tid = same as setComponentHandler..

  /*
    will be called afte we regisdter the timer:
      MRunLoop->registerTimer(this,KODE_PLUGIN_VST3_TIMER_MS);
    (in IEditView.attached)
  */

  void /*VST3_PLUGIN_API*/ onTimer() final {
    //KODE_VST3PRINT;
    #ifndef KODE_NO_GUI
      on_updateEditor(MEditor);
      flushParametersToHost();
    #endif // KODE_NO_GUI
  }

};

//----------------------------------------------------------------------
#endif
