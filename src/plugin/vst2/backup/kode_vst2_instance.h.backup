#ifndef kode_vst2_instance_included
#define kode_vst2_instance_included
//----------------------------------------------------------------------

#define KODE_PLUGIN_MAX_MIDI_SEND   256

//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_instance_listener.h"

//----------------------------------------------------------------------

struct KODE_VstEvents {
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[KODE_PLUGIN_MAX_MIDI_SEND];
};

//----------------------------------------------------------------------

class KODE_Vst2Instance
: public KODE_InstanceListener {
  
//------------------------------
private:
//------------------------------

  AEffect             MAEffect        = {0};
  audioMasterCallback MAudioMaster    = KODE_NULL;
  KODE_Descriptor*    MDescriptor     = KODE_NULL;
  KODE_Instance*      MInstance       = KODE_NULL;
  
  uint32_t            MCurrentProgram = 0;
  float               MSampleRate     = 0.0f;
  uint32_t            MMaxBlockSize   = 0;
  ERect               MVstRect        = {0};
  uint32_t            MKnobMode       = 0;
  
  KODE_VstEvents      MVstEvents      = {0};
  VstMidiEvent        MVstMidiSendEvents[KODE_PLUGIN_MAX_MIDI_SEND] = {0};

  float               MTempo          = 0.0f;
  uint32_t            MTimeSigNum     = 0;
  uint32_t            MTimeSigDenom   = 0;
  uint32_t            MSamplePos      = 0;
  uint32_t            MPlayState      = 0;
  uint32_t            MPrevPlayState  = 0;
  float               MBeatPos        = 0.0f;
  
  bool                MIsOpen         = false;
  bool                MIsSuspended    = false;
  bool                MIsProcessing   = false;
//bool                MIsInitialized  = false;
//bool                MNeedToInitializeParameters = true;
  
  #ifndef KODE_NO_GUI
    KODE_Editor*        MEditor         = KODE_NULL;
    bool                MIsEditorOpen   = false;
  #endif // KODE_NO_GUI

//------------------------------
public:
//------------------------------

  KODE_Vst2Instance(KODE_Instance* AInstance, audioMasterCallback AAudioMaster) {
    KODE_VST2TRACE;
    MAudioMaster = AAudioMaster;
    MInstance = AInstance;
    MDescriptor = MInstance->getDescriptor();
    //memset(&MVstEvents,0,sizeof(KODE_VstEvents));
    memset(&MVstMidiSendEvents,0,sizeof(MVstMidiSendEvents));
    for (uint32_t i=0; i<KODE_PLUGIN_MAX_MIDI_SEND; i++) {
      MVstEvents.events[i] = (VstEvent*)&MVstMidiSendEvents[i];
    }
  }
  
  //----------
  
  virtual ~KODE_Vst2Instance() {
    KODE_VST2TRACE;
    if (MInstance) KODE_Delete(MInstance);
  }
  
//------------------------------
public:
//------------------------------

  AEffect* getAEffect() { return &MAEffect; }

//------------------------------
public: // instance listener
//------------------------------

  uint32_t getPluginFormat() override {
    //KODE_Trace("VST2\n");
    return KODE_PLUGIN_FORMAT_VST2;
  }

  float getSamplerate() override {
    //KODE_Trace("VST2\n");
    return MSampleRate;
  }

  void notifyHostUpdateParameter(uint32_t AIndex, float AValue) override {
    //KODE_TRACE;
    host_updateParameter(AIndex,AValue);
  }
  
  //----------
    
  void notifyHostResizeWindow(uint32_t AWidth, uint32_t AHeight) override {
    host_resizeEditor(AWidth,AHeight);
  }

  //----------
    
  // see: host_flushMidi();

  void notifyHostMidiOutput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
    if (MAudioMaster) {
      // see flushMidi
      int32_t       num   = MVstEvents.numEvents;
      VstMidiEvent* event = &MVstMidiSendEvents[num];
      event->type         = kVstMidiType;
      event->deltaFrames  = AOffset;
      event->midiData[0]  = AMsg1;
      event->midiData[1]  = AMsg2;
      event->midiData[2]  = AMsg3;
      event->midiData[3]  = 0;
      event->byteSize     = sizeof(VstMidiEvent) - 2 * sizeof(int32_t);
      event->flags        = 0; // kVstMidiEventIsRealtime;
      event->noteLength   = 0;
      event->noteOffset   = 0;
      event->detune       = 0;
      num += 1;
      MVstEvents.numEvents = num;
    }
  }

//------------------------------
private:
//------------------------------

  void updateParametersInProcess() {
    MInstance->flushParamsToProcess();
  }
  
  //----------
  
  #ifndef KODE_NO_GUI
  void updateEditorInIdle() {
    //KODE_TRACE;
    MInstance->flushParamsToEditor();
  }
  #endif // KODE_NO_GUI

//------------------------------
public:
//------------------------------

  /*
    assumption: parameter changes can come in at any time, even when we're
    in the middle of on_processBlock.. so, we just stuff the parameter value
    into an array, and pushes the index onto a queue.. this queue is then
    read at the beginning of process(), and parameter changes are properly
    dispatches (now in a safe place)..
  */

  /*
    note that this can be called from any thread, at any time, also concurrent
    with audio processing, etc..
  */


  void vst2_setParameter(VstInt32 index, float parameter) {
    KODE_Vst2Trace("index %i value %.3f\n",index,parameter);
    //KODE_Trace("index %i value %.3f\n",index,parameter);
    //MInstance->updateProcessValue(index,parameter);
    //MInstance->updateEditorValue(index,parameter);
    MInstance->setParamValue(index,parameter);
    MInstance->queueParamToProcess(index);
    #ifndef KODE_NO_GUI
      if (MIsEditorOpen) MInstance->queueParamToEditor(index);
    #endif // KODE_NO_GUI
  }
  
  //----------
  
  /*
    we just return the last value we received from the host
    (we fill this up with default values when we create our instance)
  */

  float vst2_getParameter(VstInt32 index) {
    KODE_Vst2Trace("index %i\n",index);
    return MInstance->getParamValue(index);
  }
  
  //----------
  
  void vst2_process(float** inputs, float** outputs, VstInt32 sampleFrames) {
    //KODE_VST2TRACE;
    //if (MNeedToInitializeParameters) {
    //  MInstance->updateAllParameters();
    //  MNeedToInitializeParameters = false;
    //}
    updateParametersInProcess();
    host_updateTime();
    KODE_ProcessContext context;// = {0};
    uint32_t i;
    for (i=0; i<MDescriptor->getNumInputs(); i++)  { context.inputs[i]  = inputs[i]; }
    for (i=0; i<MDescriptor->getNumOutputs(); i++) { context.outputs[i] = outputs[i]; }
    context.numSamples    = sampleFrames;
    //context.oversample    = 1;
    context.samplerate    = MSampleRate;
    context.samplepos     = MSamplePos;    // getSamplePos();
    context.beatpos       = MBeatPos;
    context.tempo         = MTempo;        // getTempo();
    context.timesig_num   = MTimeSigNum;   // getTimeSigNum();
    context.timesig_denom = MTimeSigDenom; // getTimeSigDenom();
    context.playstate     = MPlayState;
    MInstance->on_process(&context);

    /*
    #ifdef KODE_PLUGIN_TICKS
      //float* out0 = outputs[0];
      //float* out1 = outputs[1];
      int32_t remaining = sampleFrames;
      int32_t offset = 0;
      //MEvents.preProcess();
      while (remaining>0) {
        if (remaining >= KODE_PLUGIN_PROCESS_TICKSIZE) {
          //MEvents.process(offset);
          //processTick();
          //copyBuffer(out0+offset,MTickBuffer);
          //copyBuffer(out1+offset,MTickBuffer);
          remaining -= KODE_PLUGIN_PROCESS_TICKSIZE;
          offset += KODE_PLUGIN_PROCESS_TICKSIZE;
        }
        else {
          //MEvents.process(offset,remaining);
          //processTick(remaining);
          //copyBuffer(out0+offset,MTickBuffer,remaining);
          //copyBuffer(out1+offset,MTickBuffer,remaining);
          remaining = 0;
        }
      }
      //MEvents.postProcess();
    #endif
    */
    
    /*
    #ifdef KODE_PLUGIN_PER_SAMPLE
      float* ins[2];
      float* outs[2];
      ins[0] = inputs[0];
      ins[1] = inputs[1];
      outs[0] = outputs[0];
      outs[1] = outputs[1];
      uint32_t len = sampleFrames;
      while (len > 0) {
        MInstance->on_processSample(ins,outs);
        //MInstance->on_processSample(&MProcessContext);
        ins[0]++;
        ins[1]++;
        outs[0]++;
        outs[1]++;
        len--;
      }
    #endif
    */
    
    //on_postProcess();
    if (MDescriptor->hasFlag(KODE_PLUGIN_SEND_MIDI)) host_flushMidi();
    
  }
  
  //----------
  
  void vst2_processDouble(double** inputs, double** outputs, VstInt32 sampleFrames) {
    //KODE_VST2TRACE;
  }
  
  //----------
  
  VstIntPtr vst2_dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
    //KODE_Vst2Trace("opcode %i index %i value %i ptr %p opt %.3f\n",opcode,index,value,opt);
    
    VstIntPtr result    = 0;
    char*     cptr      = (char*)ptr;
    char      str[256]  = {0};
    
    switch (opcode) {
      
      //----------

      /*
        called when plug-in is initialized
        initialise this plugin instance
      */

      case effOpen: // 0
        KODE_Vst2Trace("vst2: dispatcher/effOpen\n");
        MIsOpen = true;
        //updateHostInfo();
        //MInstance->on_open();
        MInstance->on_initialize();
        break;

      //----------

      /*
        called when plug-in will be released
        exit this plugin instance, release all memory and other resources
      */

      case effClose: // 1
        KODE_Vst2Trace("vst2: dispatcher/effClose\n");
        MIsOpen = false;
        MInstance->on_terminate();
        
        MInstance->on_close();
        
        break;

      //----------

      case effSetProgram: { // 2
        KODE_Vst2Trace("vst2: dispatcher/effSetProgram %i\n",(int)value);
        uint32_t program = (uint32_t)value;
        //if (program != MCurrentProgram) {
        //  on_programChange(program);
          MCurrentProgram = program;
        //}
        break;
      }

      //----------

      /*
        host requests the current program number
        return: current program number
        * reaper calls this repeatedly, 4-5 times per second (if editor open?)
        * EnergyXt calls this before/after each effEditIdle (even if we don't have an editor)
      */

      case effGetProgram: // 3
        //KODE_Vst2Trace("vst2: dispatcher/effGetProgram\n");
        return MCurrentProgram;

      //----------

      /*
        host has changed the current program name
        ptr: pointer to new program name (max 24 char + 0)
        stuff the name field of the current program with name.
        Limited to kVstMaxProgNameLen.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 4: {
      #else
      case effSetProgramName: { // 4
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetProgramName '%s'\n",cptr);
        //KODE_Strcpy(MProgramName,txt);
        break;
      }

      //----------

      /*
        host requests the current program name
        ptr: returns string-pointer to program name (max 24 char + 0)
        stuff name with the name of the current program.
        Limited to kVstMaxProgNameLen.
      */

      case effGetProgramName: // 5
        KODE_Vst2Trace("vst2: dispatcher/effGetProgramName\n");
        *cptr = 0;
        break;
        //KODE_Strcpy((char*)ptr,MProgramName);
        //return 1;

      //----------

      /*
        host requests parameter label
        index: parameter index
        ptr: returns string-pointer to parameter label (max 8 char + 0),
             examples: sec, dB, type
        Stuff label with the units in which parameter index is displayed
        (i.e. "sec", "dB", "type", etc...).
        Limited to kVstMaxParamStrLen.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 6: {
      #else
      case effGetParamLabel: { // 6
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetParamLabel %i\n",index);
        KODE_Parameter* pa = MDescriptor->getParameter(index);
        strcpy((char*)ptr,pa->getLabel());
        return 1;
        break;
      }

      //----------

      /*
        stuff text with a string representation of the value of parameter index.
        ("0.5", "-3", "PLATE", etc...)
        Limited to kVstMaxParamStrLen.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 7: {
      #else
      case effGetParamDisplay: { // 7
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetParamDisplay %i\n",index);
        KODE_Parameter* pa = MDescriptor->getParameter(index);
        float v = MInstance->getParamValue(index);
        char str[32];
        pa->getDisplayText(v,str);
        strcpy((char*)ptr,(char*)str);
        return 1;
        break;
      }

      //----------

      /*
        stuff text with the name of parameter index.
        ("Time", "Gain", "RoomType", etc...)
        Limited to kVstMaxParamStrLen.
      */

      case effGetParamName: { // 8
        KODE_Vst2Trace("vst2: dispatcher/effGetParamName %i\n",index);
        KODE_Parameter* pa = MDescriptor->getParameter(index);
        strcpy((char*)ptr,pa->getName());
        return 1;
        break;
      }

      //----------

      /*
        called when the sample rate changes
        host sets SampleRate
        opt: float value in Hz, for example 44100.0Hz
        (always in a suspend state).
      */

      case effSetSampleRate: // 10
        KODE_Vst2Trace("vst2: dispatcher/effSetSampleRate %.3f\n",opt);
        MSampleRate = opt;
        MInstance->on_sampleRate(MSampleRate);
        break;

      //----------

      /*
        called when the maximun block size changes (always in a suspend state).
        note that the sampleFrames in process calls could be smaller than
        this block size, but NOT bigger.
      */

      case effSetBlockSize: // 11
        KODE_Vst2Trace("vst2: dispatcher/effSetBlockSize %i\n",(int)value);
        MMaxBlockSize = (VstInt32)value;
        break;

      //----------

      /*
        the host has switched the 'power on' button. This only switches audio
        processing on/off.
        value: 0 = OFF, the plugin must call Resume() which in turn must flush
                   any delay buffers etc. here because otherwise pending data
                   would sound again when the effect is switched on next time.
               1 = ON, the plug must call Suspend()

        This method is called when the effect is turned off by the user.
        ex: The buffer gets flushed here because otherwise pending data would
            sound again when the effect is switched on next time.

        called from non-rt thread..

        I believe resume() is called from a non-real-time thread, so you can
        allocate memory and stuff. startProcessing() is called from a real-time
        thread, so it's supposed to be "fast" in each plug-in. Thus, you can do
        things like taking initial timer measurements, spin up hardware buffers
        if needed, etc. startProcessing() gives you a chance to do the "last half"
        of your process() functionality, if you're interfacing with something that
        needs double-buffering, for example.
        -
        Ah, right, now that I checked it, the documentation says startProcess()/
        stopProcess() are supposed to be called in RT thread, and resume()/
        suspend() are not in the list. Ok fine, that makes sense.
        -
        flush event list(s) on resume?

        suspend: called when plug-in is switched to off
        resume:  called when plug-in is switched to on
      */

      case effMainsChanged: // 12
        KODE_Vst2Trace("vst2: dispatcher/effMainsChanged %i\n",(int)value);
        if (value == 0) { // suspend
          MIsProcessing = false;
          MIsSuspended = true;
          MInstance->on_deactivate();
        }
        else { // resume
          //if (!MIsInitialized) {
          //  //MInstance->on_prepare();
          //  MIsInitialized = true;
          //}
          MIsProcessing = true;
          MIsSuspended = false;
          MInstance->on_activate();
        }
        break;

      //----------

      /*
        ptr: pointer to rect*(top, left, bottom, right)
        return: 1 = successful
        called repeatedly by reaper (4-5 times per second)
        NOTE:
          ERect has 16 bit integers.. does it matter?
      */

      case effEditGetRect: // 13
        KODE_Vst2Trace("vst2: dispatcher/effEditGetRect\n");
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          uint32_t w = MDescriptor->getEditorDefaultWidth();
          uint32_t h = MDescriptor->getEditorDefaultHeight();
          //if (w == 0) w = MInstance->getDefaultEditorWidth();
          //if (h == 0) h = MInstance->getDefaultEditorHeight();
          MVstRect.left     = 0;
          MVstRect.top      = 0;
          MVstRect.right    = w;
          MVstRect.bottom   = h;
          *(ERect**)ptr     = &MVstRect;
          return 1;
        }
        break;

      //----------

      /*
        ptr: contains system dependant Window handle/pointer
        return: 1 = successful
        ext2/linux:  display ptr (32-bit?)
      */

      case effEditOpen: // 14
        KODE_Vst2Trace("vst2: dispatcher/effEditOpen\n");
        #ifndef KODE_NO_GUI
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          if (!MIsEditorOpen) {
            MIsEditorOpen = true;
            //MEditor = (KODE_Editor*)MInstance->on_openEditor(ptr);
            MEditor = MInstance->openEditor(ptr);
            //MInstance->copyParameterValuesToEditor(MEditor);
            
//            MEditor->on_realign(true);
            
            MEditor->open();
            return 1;
          }
        }
        #endif // KODE_NO_GUI
        break;

      //----------

      /*
      */

      case effEditClose: // 15
        KODE_Vst2Trace("vst2: dispatcher/effEditClose\n");
        #ifndef KODE_NO_GUI
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          if (MIsEditorOpen) {
            MIsEditorOpen = false;
            if (MEditor) {
              MEditor->close();
              //MInstance->on_closeEditor(MEditor);
              MInstance->closeEditor(MEditor);
              MEditor = nullptr;
              return 1;
            }
          }
        }
        #endif // KODE_NO_GUI
        break;

      //----------

      /*
        no arguments. Be gentle!
        (it would be better if this call could be avoided)
        don't push stuff or changes to editor,
        notify editor that we want to change, and do it in the editor
        NOTE:
        EnergyXt calls this even if we don't have our own editor!
        FL studio --"--
        reaper, also in 'pause' state (when processing is not being called)
      */

      case effEditIdle: // 19
        KODE_Vst2Trace("vst2: dispatcher/effEditIdle\n");
        #ifndef KODE_NO_GUI
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          if (MIsEditorOpen) {
            //KODE_Assert(MEditor);
            MInstance->on_updateEditor(MEditor);
            updateEditorInIdle();
          }
        }
        #endif // KODE_NO_GUI
        break;

      //----------

      /*
        host requests pointer to a Bank/Program chunk,
        plugin is responsible for memory allocation/de-allocation of chunk
        index: 0 = isBank, 1 = isProgram
        ptr: pointer to chunk memory-block
        return: byteSize

        VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
        data - should point to the newly allocated memory block containg state
        data. You can savely release it in next suspend/resume call.
        isPreset - true when saving a single program, false for all programs
        note: If your plug-in is configured to use chunks (see
        AudioEffect::programsAreChunks) the Host will ask for a block of memory
        describing the current plug-in state for saving.
        To restore the state at a later stage, the same data is passed back to
        AudioEffect::setChunk. Alternatively, when not using chunk, the Host
        will simply save all parameter values.

        SONAR is sending the setChunks on separated cores. (dual-core machine
        here).. Or on separated threads..
        i think: 
        - reaper asks for chunk repeatedly (1.5 sec intervals?)
        - bitwig per 5 sec ??
        see:
          "proper way to load and save presets with getChunk setChunk"
          https://www.kvraudio.com/forum/viewtopic.php?p=6477202
          "Correctly restoring a VST plugin current program on host saved project"
          https://www.kvraudio.com/forum/viewtopic.php?p=6330607
          "getChunk question"
          http://www.kvraudio.com/forum/viewtopic.php?p=5530705
          sonigen:
          can you just assign the new chunk and delete the previous one in the
          getChunk call? The host is just going to keep one "chunk" pointer
          isnt it? So as long as that points to the last chunk created
          everything is ok?
          very angry mobster:
          I only use the one chunk and have not noticed any problems yet.
          mystran:
          Yeah, same here.. my chunk is simply an std::string and I just
          replace the contents on every new call to getChunk(), then pass the
          data() and length() back to the host. Never heard of any issues
          anywhere.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 23:
      #else
      case effGetChunk: // 23
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetChunk %i\n",index);
        //if (index==0) return MInstance->on_saveBank((void**)ptr);
        //else return MInstance->on_saveProgram((void**)ptr);
        {
          void* buffer = KODE_NULL;
          uint32_t size = MInstance->on_saveState(&buffer,0);
          if ((size == 0) && (buffer == KODE_NULL)) {
            buffer = MInstance->getParamValues();
            size = MDescriptor->getNumParameters() * sizeof(float);
          }
          *(void**)ptr = buffer;
          return size;
        }
        break;

      //----------

      /*
        plug-in receives a Bank/Program chunk,
        (host restores plug-in state)
        host is responsible for memory allocation/de-allocation of chunk
        index: 0 = isBank, 1 = isProgram
        value: byteSize
        ptr: pointer to chunk memory-block

        VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
        data - pointer to state data (owned by Host)
        byteSize - size of state data
        isPreset - true when restoring a single program, false for all programs
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 24:
      #else
      case effSetChunk: // 24
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetChunk %i %i\n",index,(int)value);
        {
          //if (index==0) return MInstance->on_loadBank(ptr,value); // was not retrurn
          //else  /*if (index==1)*/ return MInstance->on_loadProgram(ptr,value);
          float* buffer = (float*)ptr;
          uint32_t num_params = MDescriptor->getNumParameters();
          for (uint32_t i=0; i<num_params; i++) {
            float v = buffer[i];
            MInstance->setParamValue(i,v);
          }
          MInstance->updateAllParameters();
        }
        break;

      //case effGetChunk: v = getChunk ((void**)ptr, index ? true : false); break;
      //case effSetChunk: v = setChunk (ptr, (VstInt32)value, index ? true : false); break;

      //----------

  // vst 2

      //----------

      /*
        ptr: pointer to VstEvents*, host usually call ProcessEvents just before
             calling ProcessReplacing
        return: VST2.4: host should ignore return value, <=VST2.3: 0 = 'wants no
                more', else return=1

        assumptions:
          * called just before process(), from the same thread,
            non-overlapping (non-concurrent?)
          * sorted by offset
        todo:
          * check: negative offsets, offsets past buffer
                   concurrency (what happens IF it is called during process()

        Are you taking deltaFrames offsets into account? (Including the fact that
        they can be negative, indicating the event should have happened in the
        last sample buffer. As will necessarily be the case with any real-time
        MIDI input.)

        called repeatedly! by fl studio, even if nothing happens???
        energyxt calls this repeatedly (with num = 0), before each block?
      */

      case effProcessEvents: { // 25
        KODE_Vst2Trace("vst2: dispatcher/effProcessEvents\n");
        if (MDescriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
          VstEvents* ev = (VstEvents*)ptr;
          int num_events = ev->numEvents;
          for (int32_t i=0; i<num_events; i++) {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            if (event->type == kVstMidiType) {

              // todo: buffer, handle all in process..
              MInstance->on_midiInput(event->deltaFrames,event->midiData[0],event->midiData[1],event->midiData[2]);

            }
          }
        }
        // todo: sort?
        return 1;
      }

      //----------

      /*
        hosts asks if parameter can be automated, this should be called each time
        before attempting to call SetParameter()
        index: parameter index
        return: 1 = parameter can be automated
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 26: {
      #else
      case effCanBeAutomated: { // 26
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effCanBeAutomated %i\n",index);
        uint32_t res = 0;
        KODE_Parameter* param = MDescriptor->getParameter(index);
        if (param->hasFlag(KODE_PARAMETER_AUTOMATE)) res = 1;
        return res;
      }

      //----------

      /*
        host must call this two-fold:
        1) host checks the capability of plugin if it can convert a string into a parameter float value
            index: parameter index
            ptr: NULL
            return: 1 = plug CAN convert string2Parameter,
                    0 = plug CANNOT convert string2Parameter
        2) host requests plug to set a parameter from the parameter textual representation into a parameter
           float value (range 0.0 to 1.0), if this conversion is successful then internally the plugin must
           call setParameter as part of this call
            index: parameter index
            ptr: string-pointer to textual representation of parameter
            return: 1 = conversion successful,
                    0 = fail
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 27:
      #else
      case effString2Parameter: // 27
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effString2Parameter %i\n",index);
        break;

      //----------

      /*
        host requests program name
        index: program index
        value: deprecated in VST2.4
        ptr: returns string-pointer to program name (max 24 char + 0)
        return: 1 = OK, 0 = fail
        energy xt 2 calls this before effClose ??? (investigate)
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 29:
      #else
      case effGetProgramNameIndexed: // 29
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetProgramNameIndexed %i\n",index);
        //KStrcpy((char*)ptr,MProgramName);
        //return (VstIntPtr)MPrograms[index]->getName();
        strncpy((char*)ptr,"default",24);
        break;

      //----------

      /*
        struct VstPinProperties {
          char label[kVstMaxLabelLen];	          // pin name
          VstInt32 flags;					                // @see VstPinPropertiesFlags
          VstInt32 arrangementType;		            // @see VstSpeakerArrangementType
          char shortLabel[kVstMaxShortLabelLen];  // short name (recommended: 6 + delimiter)
          char future[48];				                // reserved for future use
        };
        enum VstPinPropertiesFlags {
          kVstPinIsActive   = 1 << 0,   // pin is active, ignored by Host
          kVstPinIsStereo   = 1 << 1,   // pin is first of a stereo pair
          kVstPinUseSpeaker = 1 << 2    // #VstPinProperties::arrangementType is valid and can be used to get the wanted arrangement
        };
        enum VstSpeakerArrangementType {
          kSpeakerArrUserDefined = -2,  // user defined
          kSpeakerArrEmpty = -1,        // empty arrangement
          kSpeakerArrMono  =  0,        // M
          kSpeakerArrStereo,            // L R
          kSpeakerArrStereoSurround,    // Ls Rs
          ...
        };
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 33:
        break;
      #else
      case effGetInputProperties: { // 33
        KODE_Vst2Trace("vst2: dispatcher/effGetInputProperties %i\n",index);
        VstPinProperties* pin = (VstPinProperties*)ptr;
        char* pc = strcpy(pin->label,"input "); // returns ptr to end of string? ptr to the '\0', or after the 0?
        *pc++ = KODE_HEX_TABLE[index&0x0f];
        *pc = 0;
        pin->flags = 1; // active
        if ( (index&1) == 0) pin->flags = pin->flags | 2; // first of stereo pair
        return 1;
      }
      #endif

      //----------

      /*
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 34:
        break;
      #else
      case effGetOutputProperties: { // 34
        KODE_Vst2Trace("vst2: dispatcher/effGetOutputProperties %i\n",index);
        VstPinProperties* pin = (VstPinProperties*)ptr;
        char* pc = strcpy(pin->label,"output ");
        *pc++ = KODE_HEX_TABLE[index&0x0f];
        *pc = 0;
        pin->flags = 1; // active
        if ( (index&1) == 0) pin->flags = pin->flags | 2; // first of stereo pair
        return 1;
        break;
      }
      #endif

      //----------

      /*
        host requests plugin category
        return: category number:
           0 = kPlugCategUnknown        Unknown, category not implemented
           1 = kPlugCategEffect         Simple Effect,
           2 = kPlugCategSynth          VST Instrument (Synths, samplers,...)
           3 = kPlugCategAnalysis       Analysis. Scope, Tuner, ...
           4 = kPlugCategMastering      Mastering, Dynamics, ...
           5 = kPlugCategSpacializer    Spacializer (panners),
           6 = kPlugCategRoomFx         RoomFx (delays and reverbs),
           7 = kPlugSurroundFx          SurroundFx (dedicated surround processors),
           8 = kPlugCategRestoration    Restoration, denoiser
           9 = kPlugCategOfflineProcess OfflineProcess,
          10 = kPlugCategShell          Shell (is only a container of plugins), @see effShellGetNextPlugin
          11 = kPlugCategGenerator      Generator, TonGenerator, ..
      */

      case effGetPlugCategory: { // 35
        KODE_Vst2Trace("vst2: dispatcher/effGetPlugCategory\n");
        uint32_t res = 0;
        #ifdef KODE_PLUGIN_VST2_VESTIGE
          res = (MDescriptor->hasFlag(KODE_PLUGIN_IS_SYNTH)) ? 2 : 1;
        #else
          res = (MDescriptor->hasFlag(KODE_PLUGIN_IS_SYNTH)) ? kPlugCategSynth : kPlugCategEffect;
        #endif
        //if (MPlugin->hasFlag(kpf_tool)) res = kPlugCategGenerator;
        return res;
      }

      //----------

      /*
        index: start flag
        value: count
        ptr: pointer to VstAudioFile array
        return: 1 = successful
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 38:
      #else
      case effOfflineNotify: // 38
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effOfflineNotify\n");
        break;

      //----------

      /*
        value: count
        ptr: pointer to VstOfflineTask array
        return: 1 = successful
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 39:
      #else
      case effOfflinePrepare: // 39
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effOfflinePrepare\n");
        break;

      //----------

      /*
        value: count
        ptr: pointer to VstOfflineTask array
        return: 1 = successful
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 40:
      #else
      case effOfflineRun: // 40
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effOfflineRun\n");
        break;

      //----------

      /*
        ptr: pointer to VariableIO*
        return: 1 = successful
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 41:
      #else
      case effProcessVarIo: // 41
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effProcessVarIo\n");
        break;

      //----------

      /*
        value: pointer to VstSpeakerArrangement* pluginInput
        ptr: pointer to VstSpeakerArrangement* pluginOutput
        return: 1 = successful
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 42:
      #else
      case effSetSpeakerArrangement: // 42
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetSpeakerArrangement\n");
        break;

      //----------

      /*
        the host has switched the 'softBypass' state
        value: onOff (0 = off, 1 = On)
        return: 1 = successful
        ---
        returns true if the plug-in supports SoftBypass, which allows the process
        to be called, even if the plug-in was bypassed. This is extremely useful
        if the plug-in should be able to maintain a processing state even when
        turned off, e.g. surround decoders/encoders. Moreover, this feature may be
        automated by the host.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 44:
      #else
      case effSetBypass: // 44
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetBypass %i\n",(int)value);
        return 1;

      //----------

      /*
        ptr: returns string-pointer to a string with effect name (max 32 bytes)
        return: 1 = successful
      */

      case effGetEffectName: // 45
        KODE_Vst2Trace("vst2: dispatcher/effGetEffectName\n");
        strcpy(str,MDescriptor->getName());
//          #ifdef KODE_32BIT
//            str += "_32";
//          #endif
//          #ifdef KODE_64BIT
//            str += "_64";
//          #endif
//          #ifdef KODE_DEBUG
//            str += "_debug";
//          #endif
        strcpy((char*)ptr,(char*)str);
        return 1;

      //----------

      /*
        ptr: returns string-pointer to a string identifying the vendor (max 64 ch)
        return: 1 = successful
      */

      case effGetVendorString: // 47
        KODE_Vst2Trace("vst2: dispatcher/effGetVendorString\n");
        strcpy((char*)ptr,(char*)MDescriptor->getAuthor());
        break;

      //----------

      /*
        ptr: returns string-pointer to a string with product name (max 64 char)
        return: 1 = successful
      */

      case effGetProductString: // 48
        KODE_Vst2Trace("vst2: dispatcher/effGetProductString\n");
        strcpy((char*)ptr,(char*)MDescriptor->getName());
        break;

      //----------

      /*
        return: vendor-specific version
      */

      case effGetVendorVersion: // 49
        KODE_Vst2Trace("vst2: dispatcher/effGetVendorVersion\n");
        return MDescriptor->getVersion();

      //----------

      /*
        no definition, vendor specific handling
        The MouseWheel handling is a bit of an after thought, and has been
        implemented by Stenberg via the effVendorSpecific call as follows:
        index: 'stCA' / $73744341
        value: 'Whee' / $57686565
        ptr: 1.0 = wheel-up, -1.0 = wheel-down
        return: 1 = MouseWheel used

        case effVendorSpecific:
          if (index == effGetParamDisplay && ptr) {
            if (value>=0 && value<NUM_PARAMS) {
              sprintf(ptr,"%f",opt);
              return 0xbeef;
            }
          }
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 50:
      #else
      case effVendorSpecific: // 50
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effVendorSpecific\n");
        break;

      //----------

     /*
        host queries a plugin 'cando' capability
        ptr: string-pointer to cando string
        return: 1='cando', 0='don't know' (default), -1='No'

        Bitwig asks about:
          midiSingleNoteTuningChange
          midiKeyBasedInstrumentControl
          MPE

        Jost asks about:
          plugAsChannelInsert
          plugAsSend
          mixDryWet

        Reaper asks about:
          hasCockosNoScrollUI
          hasCockosExtensions

        see: MIDI specifications for Multidimensional Polyphonic Expression (MPE)
        https://docs.google.com/document/d/1-26r0pVtVBrZHM6VGA05hpF-ij5xT6aaXY9BfDzyTx8/edit?pli=1#

        reaper calls this after resume & start plugin..
      */

      case effCanDo: { // 51
        KODE_Vst2Trace("vst2: dispatcher/effCanDo '%s' ",cptr);
        char* p = (char*)ptr;

        // plug-in will send Vst/MIDI events to Host
        if (MDescriptor->hasFlag(KODE_PLUGIN_SEND_MIDI)) {
          if (!strcmp(p,"sendVstEvents"))    { KODE_Vst2Trace(" -> 1\n"); return 1; }
          if (!strcmp(p,"sendVstMidiEvent")) { KODE_Vst2Trace(" -> 1\n"); return 1; }
        }

        // plug-in can receive Vst/MIDI events to Host
        if (MDescriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
          if (!strcmp(p,"receiveVstEvents"))     { KODE_Vst2Trace(" -> 1\n"); return 1; }
          if (!strcmp(p,"receiveVstMidiEvent"))  { KODE_Vst2Trace(" -> 1\n"); return 1; }
        }

        // plug-in can receive Time info from Host
        if (!strcmp(p,"receiveVstTimeInfo")) {
          KODE_Vst2Trace(" -> 1\n");
          return 1;
        }

        // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
        if (!strcmp(p,"offline")) {
          KODE_Vst2Trace("-> 0\n");
          return 0;
        }

        // plug-in supports function #getMidiProgramName ()
        if (!strcmp(p,"midiProgramNames")) {
          KODE_Vst2Trace("-> 0\n");
          return 0;
        }

        // plug-in supports function setBypass()
        if (!strcmp(p,"bypass")) {
          KODE_Vst2Trace(" -> 1\n");
          return 1;
        }

        if (!strcmp(p,"MPE")) {
          //#ifdef KODE_DEBUG_VST
        //  #ifdef KODE_PLUGIN_MPE
        //    KODE_Vst2Trace("-> 1\n");
        //    return 1;

          //if (MDescriptor->hasFlag(kpf_mpe)) return 1;

          //#else
          //VST_TRACE("vst dispatcher: effCanDo '%s' >> 0",(char*)ptr);
        //  #endif
          //#endif // KODE_PLUGIN_MPE
          //#ifdef KODE_PLUGIN_MPE
          //MHostMPE = true;
          //// MVoices.setMPE(true);
          //on_setMode(kpm_mpe,MHostMPE);
          //return 1; // 0
          //#else
          //return 0;
          //#endif
        }

        /*
          http://www.reaper.fm/sdk/vst/vst_ext.php
          A REAPER aware VST plug-in can respond to effCanDo "hasCockosExtensions",
          with 0xbeef0000 (returning this value), which will inform the host that
          it can call certain extended calls. A plug-in that responds to
          "hasCockosExtensions" may choose to implement any subset of the extended
          calls.
        */

        //#ifdef KODE_PLUGIN_REAPER_EXT
        //if (MFlags&kpf_reaper) {
        //  // Warning: range check error while evaluating constants
        //  //and (Pos(ptr,'hasCockosExtensions') <> 0) then result := {%H-}$beef0000;
        //  if (!SStrcmp(p,"hasCockosExtensions")) return 0xbeef0000;
        //}
        //#endif // KODE_PLUGIN_REAPER_EXT

        if (!strcmp(p,"hasCockosExtensions")) { KODE_Vst2Trace("-> 0\n"); return 0; }

        KODE_Vst2Trace("-> 0\n");
        return 0;
      }

      //----------

      /*
        returns tail size; 0 is default (return 1 for 'no tail')
        called a lot by Bitwig
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 52:
      #else
      case effGetTailSize: // 52
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetTailSize\n");
        //if (MTail == -1) return 1;
        return -1;//0;
        //break;

      //----------

      /*
        host sends this idle call in response to plugin's 'audioMasterneedIdle'
        request
        return: 1 must be returned to keep idle calls being issued by host

        energyXt2 calls this
        reaper too, if editor is open?
        reaper calls this even if we don't have an editor..
      */

      //#ifndef KODE_PLUGIN_VST2_VESTIGE
      //case DECLARE_VST_DEPRECATED(effIdle): // 53
      //  KODE_Vst2Trace("vst2: dispatcher/effIdle (deprecated)\n");
      //  break;
      //#endif

      //----------

      /*
        of parameter <index>, VstParameterProperties* in <ptr>
        return=1 when successful
      */

      case effGetParameterProperties: { // 56
        KODE_Vst2Trace("vst2: dispatcher/effGetParameterProperties %i\n",index);
        break;
      }

      //----------

      /*
        returns 2; older versions return 0
      */

      case effGetVstVersion: // 58
        KODE_Vst2Trace("vst2: dispatcher/effGetVstVersion\n");
        return 2;
        //break;

      //----------

  // vst 2.1

      //----------

      /*
        index: character
        value: virtual key-code
        opt: key-modifiers
        return: 0 = KeyDown not used, 1 = KeyDown used
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 59:
      #else
      case effEditKeyDown: // 59
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effEditKeyDown %i %i %i\n",index,(int)value,(int)opt);
        break;

      //----------

      /*
        index: character
        value: virtual key-code
        opt: key-modifiers
        return: 0 = not KeyUp used, 1 = KeyUp used
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 60:
      #else
      case effEditKeyUp: // 60
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effEditKeyUp %i %i %i\n",index,(int)value,(int)opt);
        break;

      //----------

      /*
        value: knobmode 0=circular, 1=circular relativ, 2=linear
        return: 1 = successful
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 61:
      #else
      case effSetEditKnobMode: // 61
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetEditKnobMode %i\n",(int)value);
        MKnobMode = (uint32_t)value;
        //return 1;
        break;

      //----------

      /*
        ptr: passed points to MidiProgramName struct.
        ptr: struct will be filled with information for 'thisProgramIndex'.
        return: number of used programIndexes.
                if 0 is returned, no MidiProgramNames supported.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 62:
      #else
      case effGetMidiProgramName: // 62
      #endif
        //KODE_Vst2Trace("vst2: dispatcher/effGetMidiProgramName\n");
        break;

      //----------

      /*
        ptr: passed points to MidiProgramName struct.
        ptr: struct will be filled with information for the current program.
        return: the programIndex of the current program.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 63:
      #else
      case effGetCurrentMidiProgram: // 63
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetCurrentMidiProgram\n");
        break;

      //----------

      /*
        ptr: passed points to MidiProgramCategory struct.
        ptr: struct will be filled with information for 'thisCategoryIndex'.
        return: number of used categoryIndexes.
                if 0 is returned, no MidiProgramCategories supported.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 64:
      #else
      case effGetMidiProgramCategory: // 64
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetMidiProgramCategory\n");
        break;

      //----------

      /*
        return 1 if the MidiProgramNames or MidiKeyNames had changed on this
        channel, 0 otherwise. <ptr> ignored.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 65:
      #else
      case effHasMidiProgramsChanged: // 65
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effHasMidiProgramsChanged\n");
        break;

      //----------

      /*
        ptr: passed points to MidiKeyName struct.
        ptr: struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 66:
      #else
      case effGetMidiKeyName: // 66
      #endif
//          KODE_Vst2Trace("vst2: dispatcher/effGetMidiKeyName\n");
        break;

      //----------

      /*
        called before all setParameter calls when changing program..
        host calls this before a new program (effSetProgram) is loaded
      */

      case effBeginSetProgram: // 67
        KODE_Vst2Trace("vst2: dispatcher/effBeginSetProgram\n");
        break;

      //----------

      /*
        host calls this after the new program (effSetProgram) has been loaded
      */

      case effEndSetProgram: // 68
        KODE_Vst2Trace("vst2: dispatcher/effEndSetProgram\n");
        break;

      //----------

  // vst 2.3

      //----------

      /*
        value: pointer to VstSpeakerArrangement** pluginInput
        ptr: pointer to VstSpeakerArrangement** pluginOutput
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 69:
      #else
      case effGetSpeakerArrangement: // 69
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetSpeakerArrangement\n");
        break;

      //----------

      /*
        This opcode is only called, if plugin is of type kPlugCategShell.
        <ptr> points to a char buffer of size 64, which is to be filled with the
        name of the plugin including the terminating zero.
        return: the next plugin's uniqueID.
      */

      case effShellGetNextPlugin: // 70
        KODE_Vst2Trace("vst2: dispatcher/effShellGetNextPlugin\n");
        break;

      //----------

      /*
        host calls this before the start of process call and after e.g. resume()

        http://ygrabit.steinberg.de/~ygrabit/public_html/vstsdk/OnlineDoc/vstsdk2.3/html/plug/2.0/AudioEffectX.html#startProcess
        Called one time before the start of process call.
        example:
          resume () // the user activates the plugin (User Interface Thread)
          startProcess () // Audio Thread
          process () // or ProcessReplacing ()
          process ()
          process ()
          stopProcess () // something has stopped the engine...or delayed the call
          startProcess ()
          process ()
          process ()
          process ()
          stopProcess () // yet the user has really stopped the plugin (Off)
          suspend ()

        pre vst 2.3 hosts don't call this, do they?
        should we handle it in suspend/resume?

        ext2 2.5 doesn't call this
        http://www.kvraudio.com/forum/viewtopic.php?t=277899
      */

      case effStartProcess: //71
        KODE_Vst2Trace("vst2: dispatcher/effStartProcess\n");
        MIsProcessing = true;
        break;

      //----------

      /*
        host calls this after the stop of process call and before e.g. suspend()
        - not called in energy xt2 linux
      */

      case effStopProcess: // 72
        KODE_Vst2Trace("vst2: dispatcher/effStopProcess\n");
        MIsProcessing = false;
        break;

      //----------

      /*
        Called in offline (non RealTime) Process before process is called,
        indicates how many sample will be processed
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 73:
      #else
      case effSetTotalSampleToProcess: // 73
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetTotalSampleToProcess\n");
        break;

      //----------

      /*
        PanLaw:
          Type (Linear, Equal Power,.. see enum PanLaw Type) in <value>,
          Gain in <opt>: for Linear :
          [1.0 => 0dB PanLaw], [~0.58 => -4.5dB], [0.5 => -6.02dB]
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 74:
      #else
      case effSetPanLaw: // 74
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetPanLaw %i %.3f\n",(int)value,opt);
        break;

      //----------

      /*
        Called before a Bank is loaded, <ptr> points to VstPatchChunkInfo structure
        return: -1 if the Bank cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
        ala beginSetProgram
        called by: ableton live
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 75:
      #else
      case effBeginLoadBank: // 75
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effBeginLoadBank\n");
        break;

      //----------

      /*
        Called before a Program is loaded, <ptr> points to VstPatchChunkInfo structure
        return: -1 if the Program cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 76:
      #else
      case effBeginLoadProgram: // 76
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effBeginLoadProgram\n");
        break;

      //----------

    // vst 2.4

      //----------

      /*
        value
        Symbolic precision constants used for effSetProcessPrecision.
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 77:
      #else
      case effSetProcessPrecision: // 77
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effSetProcessPrecision %i\n",(int)value);
        break;

      //----------

      /*
        return: number of used MIDI input channels (1-15)
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 78:
      #else
      case effGetNumMidiInputChannels: // 78
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetNumMidiInputChannels\n");
        break;

      //----------

      /*
        return: number of used MIDI output channels (1-15)
      */

      #ifdef KODE_PLUGIN_VST2_VESTIGE
      case 79:
      #else
      case effGetNumMidiOutputChannels: // 79
      #endif
        KODE_Vst2Trace("vst2: dispatcher/effGetNumMidiOutputChannels\n");
        break;

      //----------

      default:
        KODE_Vst2Trace("unhandled opcode: %i\n",opcode);
        break;

    }
        
    return result;
  }
  
  //----------
  
//------------------------------
private: // host
//------------------------------

  //bool host_processEvents(VstEvents* events) {
  //  if (MVstMaster) {
  //    if ((VstInt32)MVstMaster(&MVstEffect,audioMasterProcessEvents,0,0,events,0) == 1) return true;
  //  }
  //  return false;
  //}

  void host_flushMidi() {
    if (MAudioMaster) {
      MAudioMaster(&MAEffect,audioMasterProcessEvents,0,0,(VstEvents*)&MVstEvents,0);
      MVstEvents.numEvents = 0;
    }
  }

  //----------

  //bool host_sizeWindow(KODE_int32 AWidth, KODE_int32 AHeight) {
  //  if (MVstMaster) {
  //    if ((VstInt32)MVstMaster(&MVstEffect,audioMasterSizeWindow,AWidth,AHeight,KODE_NULL,0) == 1) return true;
  //  }
  //  return false;
  //}
    
  void host_resizeEditor(uint32_t AWidth, uint32_t AHeight) {
    if (MAudioMaster) {
      MAudioMaster(&MAEffect,audioMasterSizeWindow,AWidth,AHeight,0,0);
    }
  }
  
  //----------

  //bool host_canDo(const char* AText) {
  //  if (MVstMaster) {
  //    if ( (VstInt32)MVstMaster(&MVstEffect,audioMasterCanDo,0,0,(void*)AText,0) == 1) return true;
  //  }
  //  return false;
  //}

  //VstInt32 host_currentId(void) {
  //  if (MVstMaster) {
  //    return (VstInt32)MVstMaster(&MVstEffect,audioMasterCurrentId,0,0,KODE_NULL,0);
  //  }
  //  return 0;
  //}

  void host_updateInfo(void) {
    
    /*
    
    MHostCanDo = khc_none;
    if (host_canDo("acceptIOChanges"))                MHostCanDo += khc_acceptIOChanges;
  //if (host_canDo("asyncProcessing"))                MHostCanDo += khc_asyncProcessing;
    if (host_canDo("closeFileSelector"))              MHostCanDo += khc_closeFileSelector;
  //if (host_canDo("editFile"))                       MHostCanDo += khc_editFile;
  //if (host_canDo("getChunkFile"))                   MHostCanDo += khc_getChunkFile;
  //if (host_canDo("midiProgramNames"))               MHostCanDo += khc_midiProgramNames;
  //if (host_canDo("NIMKPIVendorSpecificCallbacks"))  MHostCanDo += khc_nimkpi;
    if (host_canDo("offline"))                        MHostCanDo += khc_offline;
    if (host_canDo("openFileSelector"))               MHostCanDo += khc_openFileSelector;
    if (host_canDo("receiveVstEvents"))               MHostCanDo += khc_receiveVstEvents;
    if (host_canDo("receiveVstMidiEvent"))            MHostCanDo += khc_receiveVstMidiEvent;
  //if (host_canDo("receiveVstTimeInfo"))             MHostCanDo += khc_receiveVstTimeInfo;
    if (host_canDo("reportConnectionChanges"))        MHostCanDo += khc_reportConnectionChanges;
    if (host_canDo("sendVstEvents"))                  MHostCanDo += khc_sendVstEvents;
    if (host_canDo("sendVstMidiEvent"))               MHostCanDo += khc_sendVstMidiEvent;
    if (host_canDo("sendVstMidiEventFlagIsRealtime")) MHostCanDo += khc_sendVstMidiEventFlagIsRealtime;
    if (host_canDo("sendVstTimeInfo"))                MHostCanDo += khc_sendVstTimeInfo;
    if (host_canDo("shellCategory"))                  MHostCanDo += khc_shellCategory;
  //if (host_canDo("shellCategorycurID"))             MHostCanDo += khc_shellCategoryCurId;
    if (host_canDo("sizeWindow"))                     MHostCanDo += khc_sizeWindow;
    if (host_canDo("startStopProcess"))               MHostCanDo += khc_startStopProcess;
  //if (host_canDo("supplyIdle"))                     MHostCanDo += khc_supplyIdle;
  //if (host_canDo("supportShell"))                   MHostCanDo += khc_supportShell;
  
    MHostId = khi_unknown;
    char buffer[256];
    buffer[0] = 0;
    host_getProductString(buffer);
  //if (KStrcmp("AudioMulch",buffer) == 0)              MHostId = khi_audiomulch;       // http://www.audiomulch.com/mulchnotes/mulchnote_2.htm
    if (KODE_Strcmp("Bitwig Studio",buffer) == 0)       MHostId = khi_bitwig;
    if (KODE_Strcmp("Carla",buffer) == 0)               MHostId = khi_carla;
    if (KODE_Strcmp("Carla_Discovery",buffer) == 0)     MHostId = khi_carla_discovery;  // 'Carla_Discovery' during scanning..
  //if (KStrcmp("DSSI-VST Plugin",buffer) == 0)         MHostId = khi_dssi_plugin;      // https://github.com/falkTX/dssi-vst/blob/master/dssi-vst-server.cpp
  //if (KStrcmp("DSSI-VST Scanner",buffer) == 0)        MHostId = khi_dssi_scanner;     // https://github.com/falkTX/dssi-vst/blob/master/dssi-vst-scanner.cpp
  //if (KStrcmp("DSSI VST Wrapper Plugin",buffer) == 0) MHostId = khi_linvst;           // https://github.com/osxmidi/LinVst/blob/master/lin-vst-server.cpp
    if (KODE_Strcmp("energyXT",buffer) == 0)            MHostId = khi_energyxt;
    if (KODE_Strcmp("Fruity Wrapper",buffer) == 0)      MHostId = khi_fruity;
    if (KODE_Strcmp("Live",buffer) == 0)                MHostId = khi_live;
    if (KODE_Strcmp("Multi Host 4 Linux",buffer) == 0)  MHostId = khi_jost;
    if (KODE_Strcmp("Qtractor",buffer) == 0)            MHostId = khi_qtractor;
    if (KODE_Strcmp("REAPER",buffer) == 0)              MHostId = khi_reaper;
  //if (KStrcmp("Tracktion",buffer) == 0)               MHostId = khi_tracktion;        // https://github.com/osxmidi/LinVst/blob/master/linvst.cpp
  //if (KStrcmp("Waveform",buffer) == 0)                MHostId = khi_waveform;
  
    MHostVersion = host_getVendorVersion();
    MHostVstVer = host_version();
    
    //KODE_HostInfo* host_info;    
    MAudioMaster(MAEffect,audioMasterGetVendorString,0,0,MHostInfo.vendor,0);
    MAudioMaster(MAEffect,audioMasterGetProductString,0,0,MHostInfo.product,0);
    MHostInfo.version   = MAudioMaster(MAEffect,audioMasterGetVendorVersion,0,0,0,0);
    MHostInfo.language  = MAudioMaster(MAEffect,audioMasterGetLanguage,0,0,0,0);
    char* path = (char*)MAudioMaster(MAEffect,audioMasterGetDirectory,0,0,0,0);
    strcpy(MHostInfo.directory,path);
    //if (MAudioMaster(&MAEffect,audioMasterCanDo,0,0,(char*)"acceptIOChanges",0) == 1) MHostInfo.canDo += khc_acceptIoChange;

    */
    
  }

  //----------

  //void host_automate(VstInt32 AIndex, float AValue) {
  //  if (MVstMaster) {
  //    //KODE_VST2TRACE;
  //    MVstMaster(&MVstEffect,audioMasterAutomate,AIndex,0,KODE_NULL,AValue);
  //  }
  //}

  //bool host_beginEdit(VstInt32 AIndex) {
  //  if (MVstMaster) {
  //    if ( (VstInt32)MVstMaster(&MVstEffect,audioMasterBeginEdit,AIndex,0,KODE_NULL,0) == 1) return true;
  //  }
  //  return false;
  //}

  //bool host_endEdit(VstInt32 AIndex) {
  //  if (MVstMaster) {
  //    if ((VstInt32)MVstMaster(&MVstEffect,audioMasterEndEdit,AIndex,0,KODE_NULL,0) == 1) return true;
  //  }
  //  return false;
  //}


  void host_updateParameter(uint32_t AIndex, float AValue) {
    if (MAudioMaster) {
      MAudioMaster(&MAEffect,audioMasterBeginEdit,AIndex,0,0,0);
      MAudioMaster(&MAEffect,audioMasterAutomate,AIndex,0,0,AValue);
      MAudioMaster(&MAEffect,audioMasterEndEdit,AIndex,0,0,0);
    }
  }

  //----------

  /*
    samplePos:          Current Position. It must always be valid, and should
                        not cost a lot to ask for. The sample position is ahead
                        of the time displayed to the user. In sequencer stop
                        mode, its value does not change. A 32 bit integer is
                        too small for sample positions, and it's a double to
                        make it easier to convert between ppq and samples.
    ppqPos:             At tempo 120, 1 quarter makes 1/2 second, so 2.0 ppq
                        translates to 48000 samples at 48kHz sample rate.
                        .25 ppq is one sixteenth note then. if you need
                        something like 480ppq, you simply multiply ppq by that
                        scaler.
    barStartPos:        Say we're at bars/beats readout 3.3.3. That's 2 bars +
                        2 q + 2 sixteenth, makes 2 * 4 + 2 + .25 = 10.25 ppq.
                        at tempo 120, that's 10.25 * .5 = 5.125 seconds, times
                        48000 = 246000 samples.
    samplesToNextClock: MIDI Clock Resolution (24 per Quarter Note), can be
                        negative the distance to the next midi clock (24 ppq,
                        pulses per quarter) in samples. unless samplePos falls
                        precicely on a midi clock, this will either be negative
                        such that the previous MIDI clock is addressed, or
                        positive when referencing the following (future) MIDI
                        clock.
  */

  /*
    struct VstTimeInfo {
      double samplePos;             // current Position in audio samples (always valid)
      double sampleRate;            // current Sample Rate in Herz (always valid)
      double nanoSeconds;           // System Time in nanoseconds (10^-9 second)
      double ppqPos;                // Musical Position, in Quarter Note (1.0 equals 1 Quarter Note)
      double tempo;                 // current Tempo in BPM (Beats Per Minute)
      double barStartPos;           // last Bar Start Position, in Quarter Note
      double cycleStartPos;         // Cycle Start (left locator), in Quarter Note
      double cycleEndPos;           // Cycle End (right locator), in Quarter Note
      VstInt32 timeSigNumerator;    // Time Signature Numerator (e.g. 3 for 3/4)
      VstInt32 timeSigDenominator;  // Time Signature Denominator (e.g. 4 for 3/4)
      VstInt32 smpteOffset;         // SMPTE offset (in SMPTE subframes (bits; 1/80 of a frame)). The current SMPTE position can be calculated using #samplePos, #sampleRate, and #smpteFrameRate.
      VstInt32 smpteFrameRate;      // @see VstSmpteFrameRate
      VstInt32 samplesToNextClock;  // MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest clock)
      VstInt32 flags;               // @see VstTimeInfoFlags
    };

    enum VstTimeInfoFlags {

      kVstTransportChanged      = 1,        // indicates that play, cycle or record state has changed
      kVstTransportPlaying      = 1 << 1,   // set if Host sequencer is currently playing
      kVstTransportCycleActive  = 1 << 2,   // set if Host sequencer is in cycle mode
      kVstTransportRecording    = 1 << 3,   // set if Host sequencer is in record mode
      kVstAutomationWriting     = 1 << 6,   // set if automation write mode active (record parameter changes)
      kVstAutomationReading     = 1 << 7,   // set if automation read mode active (play parameter changes)

      kVstNanosValid            = 1 << 8,   // VstTimeInfo::nanoSeconds valid
      kVstPpqPosValid           = 1 << 9,   // VstTimeInfo::ppqPos valid
      kVstTempoValid            = 1 << 10,  // VstTimeInfo::tempo valid
      kVstBarsValid             = 1 << 11,  // VstTimeInfo::barStartPos valid
      kVstCyclePosValid         = 1 << 12,  // VstTimeInfo::cycleStartPos and VstTimeInfo::cycleEndPos valid
      kVstTimeSigValid          = 1 << 13,  // VstTimeInfo::timeSigNumerator and VstTimeInfo::timeSigDenominator valid
      kVstSmpteValid            = 1 << 14,  // VstTimeInfo::smpteOffset and VstTimeInfo::smpteFrameRate valid
      kVstClockValid            = 1 << 15   // VstTimeInfo::samplesToNextClock valid
    };
  */
  
  void host_updateTime() {
    VstIntPtr mask = 0xffff;
    VstIntPtr result = MAudioMaster(&MAEffect,audioMasterGetTime,0,mask,0,0);
    VstTimeInfo* timeinfo = (VstTimeInfo*)result;
    if (timeinfo) {
      MTempo          = timeinfo->tempo;
      MTimeSigNum     = timeinfo->timeSigNumerator;
      MTimeSigDenom   = timeinfo->timeSigDenominator;
      MSamplePos      = timeinfo->samplePos;
      MBeatPos        = timeinfo->ppqPos;
      
      //MNanoSeconds    = timeinfo->nanoSeconds;
      //MSampleRate     = timeinfo->sampleRate;
      //MPPQBarStart    = timeinfo->barStartPos;
      //MPPOCycleStart  = timeinfo->cycleStartPos;
      //MPPQCycleEnd    = timeinfo->cycleEndPos;
      //MPlayState      = timeinfo->flags & 127;
      
      //if (timeinfo->flags & kVstTransportChanged) {
        MPlayState = KODE_PLUGIN_PLAYSTATE_NONE;
        
        if (timeinfo->flags & kVstTransportPlaying)     MPlayState |= KODE_PLUGIN_PLAYSTATE_PLAYING;
        if (timeinfo->flags & kVstTransportRecording)   MPlayState |= KODE_PLUGIN_PLAYSTATE_RECORDING;
        if (timeinfo->flags & kVstTransportCycleActive) MPlayState |= KODE_PLUGIN_PLAYSTATE_LOOPING;
        
        MPrevPlayState = MPlayState;
        
      //}
      
    }
  }

  //----------
  //----------
  //----------
  
  uint32_t host_getAutomationState() {
    return MAudioMaster(&MAEffect,audioMasterGetAutomationState,0,0,0,0);
  }

  //----------

  uint32_t host_getBlockSize() {
    return MAudioMaster(&MAEffect,audioMasterGetBlockSize,0,0,0,0);
  }

  //----------

  uint32_t host_getInputLatency() {
    return MAudioMaster(&MAEffect,audioMasterGetInputLatency,0,0,0,0);
  }

  //----------

  uint32_t host_getOutputLatency() {
    return MAudioMaster(&MAEffect,audioMasterGetOutputLatency,0,0,0,0);
  }

  //----------

  uint32_t host_getProcessLevel() {
    return MAudioMaster(&MAEffect,audioMasterGetCurrentProcessLevel,0,0,0,0);
  }

  //----------

  uint32_t host_getSampleRate() {
    return MAudioMaster(&MAEffect,audioMasterGetSampleRate,0,0,0,0);
  }

  //----------

  void host_idle(void) {
    if (MAudioMaster) MAudioMaster(&MAEffect,audioMasterIdle,0,0,KODE_NULL,0);
  }
  
  //----------

  bool host_ioChanged() {
    if (MAudioMaster(&MAEffect,audioMasterIOChanged,0,0,0,0) == 1) return true;
    return false;
  }

  //----------

  void host_updateDisplay() {
    MAudioMaster(&MAEffect,audioMasterUpdateDisplay,0,0,0,0);
  }

  //----------

  VstInt32 host_version(void) {
    VstInt32 result = 1;
    if (MAudioMaster) result = (VstInt32)MAudioMaster(&MAEffect,audioMasterVersion,0,0,KODE_NULL,0);
    if (result==0) result = 1; // old (!version)
    return result;
  }

  //----------

  //audioMasterOfflineStart,              // [index]: numNewAudioFiles [value]: numAudioFiles [ptr]: #VstAudioFile*  @see AudioEffectX::offlineStart
  //audioMasterOfflineRead,               // [index]: bool readSource [value]: #VstOfflineOption* @see VstOfflineOption [ptr]: #VstOfflineTask*  @see VstOfflineTask @see AudioEffectX::offlineRead
  //audioMasterOfflineWrite,              // @see audioMasterOfflineRead @see AudioEffectX::offlineRead
  //audioMasterOfflineGetCurrentPass,     // @see AudioEffectX::offlineGetCurrentPass
  //audioMasterOfflineGetCurrentMetaPass, // @see AudioEffectX::offlineGetCurrentMetaPass
  //audioMasterOpenFileSelector,          // [ptr]: VstFileSelect* [return value]: 1 if supported  @see AudioEffectX::openFileSelector
  //audioMasterCloseFileSelector,         // [ptr]: VstFileSelect*  @see AudioEffectX::closeFileSelector
  //audioMasterVendorSpecific,            // no definition, vendor specific handling  @see AudioEffectX::hostVendorSpecific
  
};

//----------------------------------------------------------------------
#endif
