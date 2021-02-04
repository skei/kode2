#ifndef vst3_instance_included
#define vst3_instance_included
//----------------------------------------------------------------------

/*
  * does not handle changing parameters or types
*/

//----------------------------------------------------------------------

#include "plugin/kode_editor.h"
#include "plugin/kode_plugin_base.h"
#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_utils.h"

//----------------------------------------------------------------------

#define KODE_VST3_MAX_INPUTS             8
#define KODE_VST3_MAX_OUTPUTS            8
#define KODE_VST3_PARAM_AFTERTOUCH       0x10000 // kode_vst3_AfterTouch (128)
#define KODE_VST3_PARAM_PITCHBEND        0x20000 // kode_vst3_PitchBend (129)
#define KODE_VST3_PARAM_BRIGHTNESS       0x30000 // kode_vst3_CtrlFilterResonance (74)
#define KODE_VST3_QUEUE_SIZE             1024
#define KODE_VST3_TIMER_MS               30

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

, public KODE_IInstance {

//------------------------------
private:
//------------------------------

  uint32_t                      MRefCount             = 1;
  KODE_Vst3IComponentHandler*   MComponentHandler     = nullptr;
  KODE_Vst3IComponentHandler2*  MComponentHandler2    = nullptr;
  KODE_Vst3IPlugFrame*          MPlugFrame            = nullptr;
  KODE_Vst3IHostApplication*    MHostApp              = nullptr;
  KODE_Vst3ParameterInfo*       MParamInfos           = nullptr;
  KODE_Vst3IRunLoop*            MRunLoop              = nullptr;
  uint32_t                      MIoMode               = 0;
  uint32_t                      MProcessMode          = 0;
  uint32_t                      MSampleSize           = 0;
  uint32_t                      MBlockSize            = 0;
  float                         MSampleRate           = 0.0f;
  bool                          MIsProcessing         = false;
  char                          MHostName[129]        = {0};
  KODE_Descriptor*              MDescriptor           = nullptr;
  KODE_Editor*                  MEditor               = nullptr;
  float*                        MParameterValues      = nullptr;
  float*                        MHostParameterValues  = nullptr;
  KODE_Vst3UpdateQueue          MHostParameterQueue;

//------------------------------
public:
//------------------------------

  KODE_Vst3Instance() {
    MRefCount = 1;
  }

  //----------

  virtual ~KODE_Vst3Instance() {
    deleteParameterInfo();
    destroyParameterBuffers();
  }

//------------------------------
public:
//------------------------------

  void setDescriptor(KODE_Descriptor* ADescriptor) override {
    MDescriptor = ADescriptor;
    createParameterBuffers();
    createParameterInfo();
  }

  KODE_Descriptor* getDescriptor() override {
    return MDescriptor;
  }

//------------------------------
public:
//------------------------------

  void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
    queueParameterToHost(AIndex,AValue);
  }

//------------------------------
private:
//------------------------------

  void createParameterBuffers() {
    uint32_t num = MDescriptor->getNumParameters();
    MParameterValues = (float*)malloc(num * sizeof(float));
    MHostParameterValues = (float*)malloc(num * sizeof(float));
    memset(MParameterValues,0,num * sizeof(float));
    memset(MHostParameterValues,0,num * sizeof(float));
  }

  //----------

  void destroyParameterBuffers() {
    if (MParameterValues) free(MParameterValues);
    if (MHostParameterValues) free(MHostParameterValues);
  }

  //----------

  void queueParameterToHost(uint32_t AIndex, float AValue) {
    MHostParameterValues[AIndex] = AValue;
    MHostParameterQueue.write(AIndex);
  }

  //----------

  void flushParametersToHost() {
    uint32_t index = 0;
    while (MHostParameterQueue.read(&index)) {
      float value = MHostParameterValues[index];
      //host_updateParameter(index,value);
      if (MComponentHandler) {
        //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
        MComponentHandler->beginEdit(index);          // click
        MComponentHandler->performEdit(index,value);  // drag
        MComponentHandler->endEdit(index);            // release
        //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
      }
    }
  }

  //----------

  void createParameterInfo() {
    if (!MParamInfos) {
      uint32_t num = MDescriptor->getNumParameters();
      MParamInfos = (KODE_Vst3ParameterInfo*)malloc( num * sizeof(KODE_Vst3ParameterInfo) );
      for (uint32_t i=0; i<num; i++) {
        KODE_Parameter* param = MDescriptor->getParameter(i);
        MParamInfos[i].id = i;
        KODE_CharToUtf16(param->getName(),&MParamInfos[i].title);
        KODE_CharToUtf16(param->getShortName(),&MParamInfos[i].shortTitle);
        KODE_CharToUtf16(param->getLabel(),&MParamInfos[i].units);
        MParamInfos[i].stepCount = param->getNumSteps();
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
    if (MParamInfos) free(MParamInfos);
  }

  //----------

  void handleParametersInProcess(KODE_Vst3ProcessData& data) {
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

//------------------------------
public: // FUnknown
//------------------------------

  uint32_t KODE_VST3_PLUGIN_API addRef() final {
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32_t KODE_VST3_PLUGIN_API release() final {
    const uint32_t r = --MRefCount; // const uint32_t ?
    if (r == 0) {
      on_plugin_close();
      delete this;
    };
    return r;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API queryInterface(const KODE_Vst3Id _iid, void** obj) final {
    *obj = nullptr;
    if (KODE_iidEqual(KODE_Vst3IAudioProcessor_iid,_iid) ) {
      *obj = (KODE_Vst3IAudioProcessor*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IEditController_iid,_iid) ) {
      *obj = (KODE_Vst3IEditController*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IMidiMapping_iid,_iid) ) {
      *obj = (KODE_Vst3IMidiMapping*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    if (KODE_iidEqual(KODE_Vst3IUnitInfo_iid,_iid) ) {
      *obj = (KODE_Vst3IUnitInfo*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    //if (KODE_iidEqual(KODE_Vst3INoteExpressionController_iid,_iid) ) {
    //  //*obj = (KODE_Vst3INoteExpressionController*)this;
    //  //addRef();
    //  //return kode_vst3_ResultOk;
    //  return kode_vst3_NoInterface;
    //}
    if (KODE_iidEqual(KODE_Vst3IKeyswitchController_iid,_iid) ) {
      //*obj = (KODE_Vst3IKeyswitchController*)this;
      //addRef();
      //return kode_vst3_ResultOk;
      return kode_vst3_NoInterface;
    }
    if (KODE_iidEqual(KODE_Vst3IConnectionPoint_iid,_iid) ) {
      *obj = (KODE_Vst3IConnectionPoint*)this;
      addRef();
      return kode_vst3_ResultOk;
    }
    //if (KODE_iidEqual(KODE_Vst3ITimerHandler_iid,_iid) ) {
    //  *obj = (KODE_Vst3ITimerHandler*)this;
    //  addRef();
    //  return kode_vst3_ResultOk;
    //}
    return kode_vst3_NoInterface;
  }

//------------------------------
public: // IPluginBase
//------------------------------

  int32_t KODE_VST3_PLUGIN_API initialize(KODE_Vst3FUnknown* context) final {
    MHostApp = (KODE_Vst3IHostApplication*)context;
    //context->queryInterface(IHostApplication_iid, (void**)&MHostApp);
    if (MHostApp) {
      KODE_Vst3String u;
      MHostApp->getName(u);
      KODE_Utf16ToChar(&u,MHostName);
    }
    else {
    }
    on_plugin_initialize();
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API terminate() final {
    on_plugin_terminate();
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getControllerClassId(KODE_Vst3Id classId) final {
    if (MDescriptor->hasEditor()) {
      memcpy(classId,MDescriptor->getLongEditorId(),16);
      return kode_vst3_ResultOk;
    }
    else {
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setIoMode(int32_t mode) final {
    MIoMode = mode;
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getBusCount(int32_t type, int32_t dir) final {
    if (type == kode_vst3_Audio) {
      if ((dir == kode_vst3_Output) && (MDescriptor->getNumOutputs() > 0)) { return 1; }
      if ((dir == kode_vst3_Input)  && (MDescriptor->getNumInputs()  > 0)) { return 1; };
    }
    if (type == kode_vst3_Event) {
      //if (dir==kode_vst3_Output) return 1;
      //else
      if (MDescriptor->canReceiveMidi() || MDescriptor->isSynth()) {
        if (dir == kode_vst3_Input) { return 1; }
      }
    }
    return 0;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getBusInfo(int32_t type, int32_t dir, int32_t index, KODE_Vst3BusInfo& bus) final {
    if (type == kode_vst3_Audio) {
      bus.mediaType = kode_vst3_Audio;
      if (dir == kode_vst3_Input) {
        bus.direction = kode_vst3_Input;
        bus.channelCount = MDescriptor->getNumInputs();
        KODE_CharToUtf16("Audio In",&bus.name);
      }
      else {
        bus.direction = kode_vst3_Output;
        bus.channelCount = MDescriptor->getNumOutputs();
        KODE_CharToUtf16("Audio Out",&bus.name);
      }
      bus.flags = 0;//kode_vst3_DefaultActive;
      return kode_vst3_ResultOk;
    }
    else if (type == kode_vst3_Event) {
      bus.mediaType = kode_vst3_Event;
      if (dir == kode_vst3_Input) {
        bus.direction = kode_vst3_Input;
        bus.channelCount = 1;
        KODE_CharToUtf16("Midi In",&bus.name);
      }
      bus.flags = 0;//kode_vst3_DefaultActive;
      return kode_vst3_ResultOk;
    }

    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getRoutingInfo(KODE_Vst3RoutingInfo& inInfo, KODE_Vst3RoutingInfo& outInfo) final {
    outInfo.mediaType = inInfo.mediaType; // MediaTypes::kode_vst3_Audio;
    outInfo.busIndex  = inInfo.busIndex;  // 0;
    outInfo.channel   = -1;
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API activateBus(int32_t type, int32_t dir, int32_t index, uint8_t state) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setActive(uint8_t state) final {
    if (state) on_plugin_activate();
    else on_plugin_deactivate();
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setState(KODE_Vst3IBStream* state) final {
    uint32_t  version = 0;
    uint32_t  mode = 0;
    int32_t   size = 0;
    uint32_t  num_params = 0;
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
        num_params = MDescriptor->getNumParameters();
        for (uint32_t i=0; i<num_params; i++) {
          float v = 0.f;
          state->read(&v,sizeof(float),&num_read);
          MParameterValues[i] = v;
          //on_parameter(i,v,0);
        }
        updateAllParameters();
        break;
      }
    }
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getState(KODE_Vst3IBStream* state) final {
    uint32_t  version = MDescriptor->getVersion();
    uint32_t  mode    = 0;
    void*     ptr     = nullptr;
    uint32_t  size    = 0;;
    size = on_plugin_saveState(&ptr,0);
    if ((size == 0) && (ptr == nullptr)) {
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

  int32_t KODE_VST3_PLUGIN_API setBusArrangements(uint64_t* inputs, int32_t numIns, uint64_t* outputs, int32_t numOuts) final {
    return kode_vst3_ResultTrue;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getBusArrangement(int32_t dir, int32_t index, uint64_t& arr) final {
    if ((dir==kode_vst3_Output) && (index==0)) {
      arr = kode_vst3_SpeakerL | kode_vst3_SpeakerR;
      return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API canProcessSampleSize(int32_t symbolicSampleSize) final {
    if (symbolicSampleSize==kode_vst3_Sample32) {
      return kode_vst3_ResultTrue;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  uint32_t KODE_VST3_PLUGIN_API getLatencySamples() final {
    return 0;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setupProcessing(KODE_Vst3ProcessSetup& setup) final {
    MProcessMode  = setup.processMode;        // kode_vst3_Realtime, kode_vst3_Prefetch, kode_vst3_Offline
    MSampleSize   = setup.symbolicSampleSize; // kode_vst3_Sample32, kode_vst3_Sample64
    MBlockSize    = setup.maxSamplesPerBlock;
    MSampleRate   = setup.sampleRate;
    on_plugin_prepare(MSampleRate,MBlockSize);
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setProcessing(uint8_t state) final {
    MIsProcessing = state;
    if (MIsProcessing) on_plugin_prepare(MSampleRate,MBlockSize);
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API process(KODE_Vst3ProcessData& data) final {
    handleEventsInProcess(data);
    handleParametersInProcess(data);
    bool _flush = ( (data.numInputs == 0) && (data.numOutputs == 0) && (data.numSamples == 0) );
    if (!_flush) {
      KODE_ProcessContext context;// = {0};
      uint32_t i;
      for (i=0; i<MDescriptor->getNumInputs(); i++)   { context.inputs[i]  = data.inputs[0].channelBuffers32[i]; }
      for (i=0; i<MDescriptor->getNumOutputs(); i++)  { context.outputs[i] = data.outputs[0].channelBuffers32[i]; }
      context.numinputs    = MDescriptor->getNumInputs();
      context.numoutputs   = MDescriptor->getNumOutputs();
      context.numsamples   = data.numSamples;
      //context.oversample    = 1;
      context.samplerate   = data.processContext->sampleRate;
      context.samplepos    = data.processContext->continousTimeSamples;
      context.beatpos      = data.processContext->projectTimeMusic;
      context.tempo        = data.processContext->tempo;
      context.timesignum   = data.processContext->timeSigNumerator;
      context.timesigdenom = data.processContext->timeSigDenominator;
      context.playstate    = KODE_PLUGIN_PLAYSTATE_NONE;
      if (data.processContext->state & KODE_Vst3ProcessContext::StatesAndFlags::kode_vst3_Playing)      context.playstate |= KODE_PLUGIN_PLAYSTATE_PLAYING;
      if (data.processContext->state & KODE_Vst3ProcessContext::StatesAndFlags::kode_vst3_Recording)    context.playstate |= KODE_PLUGIN_PLAYSTATE_RECORDING;
      if (data.processContext->state & KODE_Vst3ProcessContext::StatesAndFlags::kode_vst3_CycleActive)  context.playstate |= KODE_PLUGIN_PLAYSTATE_LOOPING;
      on_plugin_processBlock(&context);
    }
    return kode_vst3_ResultOk;
  }

  //----------

  uint32_t KODE_VST3_PLUGIN_API getTailSamples() final {
    return kode_vst3_NoTail;
  }

//------------------------------
public: // IMidiMapping
//------------------------------

  int32_t KODE_VST3_PLUGIN_API getMidiControllerAssignment(int32_t busIndex, int16_t channel, int16_t midiControllerNumber, uint32_t& id) final {
    //if (busIndex == 0) {
      switch (midiControllerNumber) {
        case kode_vst3_AfterTouch: // 128
          id = KODE_VST3_PARAM_AFTERTOUCH + channel;
          return kode_vst3_ResultOk;
        case kode_vst3_PitchBend: // 129
          id = KODE_VST3_PARAM_PITCHBEND + channel;
          return kode_vst3_ResultOk;
        case kode_vst3_CtrlFilterResonance: // cc 74 (slide)
          id = KODE_VST3_PARAM_BRIGHTNESS + channel;
          return kode_vst3_ResultOk;
      }
    //}
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // INoteExpressionController
//------------------------------

  //int32_t KODE_VST3_PLUGIN_API getNoteExpressionInfo(int32_t busIndex, int16 channel, int32_t noteExpressionIndex, NoteExpressionTypeInfo& info) final {
  //  VST3_Trace("vst3: instance/getNoteExpressionInfo busIndex:%i channel:%i noteExpressionIndex:%i\n",busIndex,channel,noteExpressionIndex);
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

  int32_t KODE_VST3_PLUGIN_API getKeyswitchCount(int32_t busIndex, int16_t channel) final {
    return 0;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getKeyswitchInfo(int32_t busIndex, int16_t channel, int32_t keySwitchIndex, KODE_Vst3KeyswitchInfo& info) final {
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IConnectionPoint
//------------------------------

  int32_t KODE_VST3_PLUGIN_API connect(KODE_Vst3IConnectionPoint* other) final {
    //IMessage* msg;
    //msg->setMessageID("test");
    //other->notify(msg);
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API disconnect(KODE_Vst3IConnectionPoint* other)  final {
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API notify(KODE_Vst3IMessage* message) final {
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IUnitInfo
//------------------------------

  int32_t KODE_VST3_PLUGIN_API getUnitCount() final {
    return 1;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getUnitInfo(int32_t unitIndex, KODE_Vst3UnitInfo& info) final {
    if (unitIndex==0) {
      info.id = kode_vst3_RootUnitId;
      info.parentUnitId = kode_vst3_NoParentUnitId;
      KODE_CharToUtf16("root",&info.name);
      info.programListId = kode_vst3_NoProgramListId;
      return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getProgramListCount() final {
    return 0; // 1
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getProgramListInfo(int32_t listIndex, KODE_Vst3ProgramListInfo& info) final {
    if (listIndex == 0) {
      info.id = 0;
      KODE_CharToUtf16("program",&info.name);
      info.programCount = 1;
      return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getProgramName(int32_t listId, int32_t programIndex, KODE_Vst3String name) final {
    if ((listId == 0) && (programIndex == 0)) {
      KODE_CharToUtf16("program",&name);
      return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getProgramInfo(int32_t listId, int32_t programIndex, const char* attributeId, KODE_Vst3String attributeValue) final {
    ////attributeId = "";
    //if ((listId == 0) && (programIndex == 0) /* attributeId */) {
    //  /*CharToUtf16*/ char_to_utf16("",attributeValue);
    //  return kode_vst3_ResultOk;
    //}
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API hasProgramPitchNames(int32_t listId, int32_t programIndex) final {
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getProgramPitchName(int32_t listId, int32_t programIndex, int16_t midiPitch, KODE_Vst3String name) final {
    // KODE_CharToUtf16("pitch",name);
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getSelectedUnit() final {
    return 0;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API selectUnit(int32_t unitId) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getUnitByBus(int32_t type, int32_t dir, int32_t busIndex, int32_t channel, int32_t& unitId) final {
    unitId = 0;
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setUnitProgramData(int32_t listOrUnitId, int32_t programIndex, KODE_Vst3IBStream* data) final {
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IEditController
//------------------------------

  int32_t KODE_VST3_PLUGIN_API setComponentState(KODE_Vst3IBStream* state) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setEditorState(KODE_Vst3IBStream* state) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getEditorState(KODE_Vst3IBStream* state) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getParameterCount() final {
    return MDescriptor->getNumParameters();
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getParameterInfo(int32_t paramIndex, KODE_Vst3ParameterInfo& info) final {
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
    if (id < MDescriptor->getNumParameters()) {
      float v = MParameterValues[id];
      return v;
    }
    else {
      return 0;
    }
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setParamNormalized(uint32_t id, double value) final {
    if (id >= MDescriptor->getNumParameters()) {
      return kode_vst3_ResultFalse; // ???
    }
    MParameterValues[id] = value;
    if (MEditor) {
      MEditor->updateParameterFromHost(id,value);
    }
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setComponentHandler(KODE_Vst3IComponentHandler* handler) final {
    MComponentHandler = handler;
    return kode_vst3_ResultOk;
  }

  //----------

  KODE_Vst3IPlugView* KODE_VST3_PLUGIN_API createView(const char* name) final {
    if (MDescriptor->hasEditor()) {
      if (name && (strcmp(name,kode_vst3_Editor) == 0)) {
        addRef();
        return (KODE_Vst3IPlugView*)this;
      }
    }
    return nullptr;
  }

//------------------------------
public: // IEditController2
//------------------------------

  int32_t KODE_VST3_PLUGIN_API setKnobMode(int32_t mode) final {
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API openHelp(uint8_t onlyCheck) final {
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API openAboutBox(uint8_t onlyCheck) final {
    return kode_vst3_ResultFalse;
  }

//------------------------------
public: // IPlugView
//------------------------------

  int32_t KODE_VST3_PLUGIN_API isPlatformTypeSupported(const char* type) final {
    #ifndef KODE_NO_GUI
      //#ifdef __gnu_linux__
      #ifdef KODE_LINUX
        //"X11EmbedWindowID"
        if (type && strcmp(type,kode_vst3_PlatformTypeX11EmbedWindowID) == 0) {
          return kode_vst3_ResultOk;
        }
      #endif // KODE_LINUX
    #endif // KODE_NO_GUI
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API attached(void* parent, const char* type) final {
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
        MEditor->open();
        //if (MRunLoop)
        MRunLoop->registerTimer(this,KODE_VST3_TIMER_MS);
        return kode_vst3_ResultOk;
      }
    #endif // KODE_NO_GUI
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API removed() final {
      #ifndef KODE_NO_GUI
      if (MDescriptor->hasEditor()) {
        //if (MRunLoop)
        MRunLoop->unregisterTimer(this);
        MEditor->close();
        on_plugin_closeEditor(MEditor);
        MEditor = nullptr;
        return kode_vst3_ResultOk;
      }
      #endif // KODE_NO_GUI
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onWheel(float distance) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onKeyDown(char16_t key, int16_t keyCode, int16_t modifiers) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onKeyUp(char16_t key, int16_t keyCode, int16_t modifiers) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API getSize(KODE_Vst3ViewRect* size) final {
    if (MDescriptor->hasEditor()) {
      size->left    = 0;
      size->top     = 0;
      size->right   = MDescriptor->getEditorWidth();
      size->bottom  = MDescriptor->getEditorHeight();
      return kode_vst3_ResultOk;
    }
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onSize(KODE_Vst3ViewRect* newSize) final {
    if (MDescriptor->hasEditor()) {
      //TODO: resize/redraw editor
      return kode_vst3_ResultOk;
    }
    else {
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API onFocus(uint8_t state) final {
    return kode_vst3_ResultOk;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API setFrame(KODE_Vst3IPlugFrame* frame) final {
    if (MDescriptor->hasEditor()) {
      MPlugFrame = frame;
      MPlugFrame->queryInterface(KODE_Vst3IRunLoop_iid, (void**)&MRunLoop);
      return kode_vst3_ResultOk;
    }
    else {
      return kode_vst3_ResultFalse;
    }
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API canResize() final {
    return kode_vst3_ResultFalse;
  }

  //----------

  int32_t KODE_VST3_PLUGIN_API checkSizeConstraint(KODE_Vst3ViewRect* rect) final {
    return kode_vst3_ResultOk;
  }

//------------------------------
public: // ITimerHandler
//------------------------------

  void onTimer() final {
    #ifndef KODE_NO_GUI
    on_plugin_updateEditor(MEditor);
    flushParametersToHost();
    #endif
  }

  //--------------------
  //
  //--------------------

};

//----------------------------------------------------------------------
#endif
