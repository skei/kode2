#ifndef kode_vst2_instance_included
#define kode_vst2_instance_included
//----------------------------------------------------------------------

//#define KODE_MAX_PATH_LENGTH      512
//#define KODE_MAX_PROCESS_EVENTS   1024

//----------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/vst2/kode_vst2.h"
#include "plugin/vst2/kode_vst2_host.h"

//----------------------------------------------------------------------

class KODE_Vst2Instance {

//------------------------------
private:
//------------------------------

  AEffect               MAEffect          = {0};
  KODE_Vst2Host*        MHost             = nullptr;
  KODE_Descriptor*      MDescriptor       = nullptr;
  KODE_Instance*        MInstance         = nullptr;
  KODE_Editor*          MEditor           = nullptr;
  ERect                 MVstRect          = {0};
  uint32_t              MCurrentProgram   = 0;
  float                 MSampleRate       = 0.0f;
  uint32_t              MBlockSize        = 0;
  uint32_t              MKnobMode         = 0;
  bool                  MIsOpen           = false;
  bool                  MIsInitialized    = false;
  bool                  MIsSuspended      = false;
  bool                  MIsProcessing     = false;
  bool                  MIsEditorOpen     = false;

  bool MNeedToInitializeParameters = true;

//------------------------------
public:
//------------------------------

  KODE_Vst2Instance(KODE_Descriptor* ADescriptor, audioMasterCallback AMaster) {
    MHost       = new KODE_Vst2Host(ADescriptor,AMaster,&MAEffect);
    MDescriptor = ADescriptor;
    MInstance   = KODE_CreateInstance(MDescriptor,MHost);
  }

  //----------

  ~KODE_Vst2Instance() {
    if (MHost) delete MHost;
    if (MInstance) delete MInstance;
  }

//------------------------------
public:
//------------------------------

  AEffect*        getAEffect()  { return &MAEffect; }
  KODE_Vst2Host*  getHost()     { return MHost; }
  KODE_Instance*  getInstance() { return MInstance; }

//------------------------------
public:
//------------------------------

  VstIntPtr vst2_dispatcher(VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
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
        VST2_Trace("vst2: dispatcher/effOpen\n");
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
        VST2_Trace("vst2: dispatcher/effClose\n");
        MIsOpen = false;
        MInstance->on_terminate();
        //MInstance->on_close();
        break;

      //----------

      case effSetProgram: { // 2
        VST2_Trace("vst2: dispatcher/effSetProgram %i\n",(int)value);
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
        //VST2_Trace("vst2: dispatcher/effGetProgram\n");
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
        VST2_Trace("vst2: dispatcher/effSetProgramName '%s'\n",cptr);
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
        VST2_Trace("vst2: dispatcher/effGetProgramName\n");
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
        VST2_Trace("vst2: dispatcher/effGetParamLabel %i\n",index);
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
        VST2_Trace("vst2: dispatcher/effGetParamDisplay %i\n",index);
        KODE_Parameter* pa = MDescriptor->getParameter(index);
        float v = MInstance->getProcessValue(index);
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
        VST2_Trace("vst2: dispatcher/effGetParamName %i\n",index);
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
        VST2_Trace("vst2: dispatcher/effSetSampleRate %.3f\n",opt);
        MSampleRate = opt;
        break;

      //----------

      /*
        called when the maximun block size changes (always in a suspend state).
        note that the sampleFrames in process calls could be smaller than
        this block size, but NOT bigger.
      */

      case effSetBlockSize: // 11
        VST2_Trace("vst2: dispatcher/effSetBlockSize %i\n",(int)value);
        MBlockSize = (VstInt32)value;
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
        VST2_Trace("vst2: dispatcher/effMainsChanged %i\n",(int)value);
        if (value == 0) { // suspend
          //MIsProcessing = false;
          MIsSuspended = true;
          MInstance->on_deactivate();
        }
        else { // resume
          if (!MIsInitialized) {
            //MInstance->on_prepare();
            MIsInitialized = true;
          }
          //MIsProcessing = true;
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
        VST2_Trace("vst2: dispatcher/effEditGetRect\n");
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          uint32_t w = MDescriptor->getEditorWidth();
          uint32_t h = MDescriptor->getEditorHeight();
          if (w == 0) w = MInstance->getDefaultEditorWidth();
          if (h == 0) h = MInstance->getDefaultEditorHeight();
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
        VST2_Trace("vst2: dispatcher/effEditOpen\n");
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          if (!MIsEditorOpen) {
            MIsEditorOpen = true;
            MEditor = (KODE_Editor*)MInstance->on_openEditor(ptr);
            MInstance->copyParameterValuesToEditor(MEditor);
            //MEditor->on_realign(true);
            MEditor->open();
            return 1;
          }
        }
        break;

      //----------

      /*
      */

      case effEditClose: // 15
        VST2_Trace("vst2: dispatcher/effEditClose\n");
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          if (MIsEditorOpen) {
            MIsEditorOpen = false;
            if (MEditor) {
              MEditor->close();
              MInstance->on_closeEditor();
              MEditor = nullptr;
              return 1;
            }
          }
        }
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
        VST2_Trace("vst2: dispatcher/effEditIdle\n");
        if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
          if (MIsEditorOpen) {
            KODE_Assert(MEditor);
            MInstance->on_idleEditor();
            MInstance->updateEditorInIdle(MEditor);
          }
        }
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
        AudioEffect::programsAreChunks)
        the Host will ask for a block of memory describing the current plug-in
        state for saving.
        To restore the state at a later stage, the same data is passed back to
        AudioEffect::setChunk. Alternatively, when not using chunk, the Host
        will simply save all parameter values.

        SONAR is sending the setChunks on separated cores. (dual-core machine here)
        Or on separated threads.
        i think:
        reaper asks for chunk repeatedly (1.5 sec intervals?)
        bitwig per 5 sec ??
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
        VST2_Trace("vst2: dispatcher/effGetChunk %i\n",index);

        //#ifdef KODE_PLUGIN_SIMPLE_STATE
        //#else
        //#endif

        //#ifdef KODE_PLUGIN_PRESET_BUFFER
//          if (index==0) return MInstance->on_saveBank((void**)ptr);
//          else /*if (index==1)*/ return MInstance->on_saveProgram((void**)ptr);
        //#endif
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
        VST2_Trace("vst2: dispatcher/effSetChunk %i %i\n",index,(int)value);

        //#ifdef KODE_PLUGIN_SIMPLE_STATE
        //#else
        //#endif

        //#ifdef KODE_PLUGIN_PRESET_BUFFER
//          if (index==0) return MInstance->on_loadBank(ptr,value); // was not retrurn
//          else  /*if (index==1)*/ return MInstance->on_loadProgram(ptr,value);
        //#endif
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
        VST2_Trace("vst2: dispatcher/effProcessEvents\n");
        if (MDescriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
          VstEvents* ev = (VstEvents*)ptr;
          int num_events = ev->numEvents;
          for (int32_t i=0; i<num_events; i++) {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            if (event->type == kVstMidiType) {

              // todo: buffer, handle all in process..
              MInstance->on_midiEvent(event->deltaFrames,event->midiData[0],event->midiData[1],event->midiData[2]);

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
        VST2_Trace("vst2: dispatcher/effCanBeAutomated %i\n",index);
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
        VST2_Trace("vst2: dispatcher/effString2Parameter %i\n",index);
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
        VST2_Trace("vst2: dispatcher/effGetProgramNameIndexed %i\n",index);
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
        VST2_Trace("vst2: dispatcher/effGetInputProperties %i\n",index);
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
        VST2_Trace("vst2: dispatcher/effGetOutputProperties %i\n",index);
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
        VST2_Trace("vst2: dispatcher/effGetPlugCategory\n");
        uint32_t res = 0;
        #ifdef KODE_PLUGIN_VST2_VESTIGE
          res = (MDescriptor->hasFlag(kpf_is_synth)) ? 2 : 1;
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
        VST2_Trace("vst2: dispatcher/effOfflineNotify\n");
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
        VST2_Trace("vst2: dispatcher/effOfflinePrepare\n");
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
        VST2_Trace("vst2: dispatcher/effOfflineRun\n");
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
        VST2_Trace("vst2: dispatcher/effProcessVarIo\n");
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
        VST2_Trace("vst2: dispatcher/effSetSpeakerArrangement\n");
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
        VST2_Trace("vst2: dispatcher/effSetBypass %i\n",(int)value);
        return 1;

      //----------

      /*
        ptr: returns string-pointer to a string with effect name (max 32 bytes)
        return: 1 = successful
      */

      case effGetEffectName: // 45
        VST2_Trace("vst2: dispatcher/effGetEffectName\n");
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
        VST2_Trace("vst2: dispatcher/effGetVendorString\n");
        strcpy((char*)ptr,(char*)MDescriptor->getAuthor());
        break;

      //----------

      /*
        ptr: returns string-pointer to a string with product name (max 64 char)
        return: 1 = successful
      */

      case effGetProductString: // 48
        VST2_Trace("vst2: dispatcher/effGetProductString\n");
        strcpy((char*)ptr,(char*)MDescriptor->getName());
        break;

      //----------

      /*
        return: vendor-specific version
      */

      case effGetVendorVersion: // 49
        VST2_Trace("vst2: dispatcher/effGetVendorVersion\n");
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
        VST2_Trace("vst2: dispatcher/effVendorSpecific\n");
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
        VST2_Trace("vst2: dispatcher/effCanDo '%s' ",cptr);
        char* p = (char*)ptr;

        // plug-in will send Vst/MIDI events to Host
        if (MDescriptor->hasFlag(KODE_PLUGIN_SEND_MIDI)) {
          if (!strcmp(p,"sendVstEvents"))    { VST2_Trace(" -> 1\n"); return 1; }
          if (!strcmp(p,"sendVstMidiEvent")) { VST2_Trace(" -> 1\n"); return 1; }
        }

        // plug-in can receive Vst/MIDI events to Host
        if (MDescriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
          if (!strcmp(p,"receiveVstEvents"))     { VST2_Trace(" -> 1\n"); return 1; }
          if (!strcmp(p,"receiveVstMidiEvent"))  { VST2_Trace(" -> 1\n"); return 1; }
        }

        // plug-in can receive Time info from Host
        if (!strcmp(p,"receiveVstTimeInfo")) {
          VST2_Trace(" -> 1\n");
          return 1;
        }

        // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
        if (!strcmp(p,"offline")) {
          VST2_Trace("-> 0\n");
          return 0;
        }

        // plug-in supports function #getMidiProgramName ()
        if (!strcmp(p,"midiProgramNames")) {
          VST2_Trace("-> 0\n");
          return 0;
        }

        // plug-in supports function setBypass()
        if (!strcmp(p,"bypass")) {
          VST2_Trace(" -> 1\n");
          return 1;
        }

        if (!strcmp(p,"MPE")) {
          //#ifdef KODE_DEBUG_VST
        //  #ifdef KODE_PLUGIN_MPE
        //    VST2_Trace("-> 1\n");
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

        if (!strcmp(p,"hasCockosExtensions")) { VST2_Trace("-> 0\n"); return 0; }

        VST2_Trace("-> 0\n");
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
        VST2_Trace("vst2: dispatcher/effGetTailSize\n");
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
      //  VST2_Trace("vst2: dispatcher/effIdle (deprecated)\n");
      //  break;
      //#endif

      //----------

      /*
        of parameter <index>, VstParameterProperties* in <ptr>
        return=1 when successful
      */

      case effGetParameterProperties: { // 56
        VST2_Trace("vst2: dispatcher/effGetParameterProperties %i\n",index);
        break;
      }

      //----------

      /*
        returns 2; older versions return 0
      */

      case effGetVstVersion: // 58
        VST2_Trace("vst2: dispatcher/effGetVstVersion\n");
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
        VST2_Trace("vst2: dispatcher/effEditKeyDown %i %i %i\n",index,(int)value,(int)opt);
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
        VST2_Trace("vst2: dispatcher/effEditKeyUp %i %i %i\n",index,(int)value,(int)opt);
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
        VST2_Trace("vst2: dispatcher/effSetEditKnobMode %i\n",(int)value);
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
        //VST2_Trace("vst2: dispatcher/effGetMidiProgramName\n");
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
        VST2_Trace("vst2: dispatcher/effGetCurrentMidiProgram\n");
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
        VST2_Trace("vst2: dispatcher/effGetMidiProgramCategory\n");
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
        VST2_Trace("vst2: dispatcher/effHasMidiProgramsChanged\n");
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
//          VST2_Trace("vst2: dispatcher/effGetMidiKeyName\n");
        break;

      //----------

      /*
        called before all setParameter calls when changing program..
        host calls this before a new program (effSetProgram) is loaded
      */

      case effBeginSetProgram: // 67
        VST2_Trace("vst2: dispatcher/effBeginSetProgram\n");
        break;

      //----------

      /*
        host calls this after the new program (effSetProgram) has been loaded
      */

      case effEndSetProgram: // 68
        VST2_Trace("vst2: dispatcher/effEndSetProgram\n");
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
        VST2_Trace("vst2: dispatcher/effGetSpeakerArrangement\n");
        break;

      //----------

      /*
        This opcode is only called, if plugin is of type kPlugCategShell.
        <ptr> points to a char buffer of size 64, which is to be filled with the
        name of the plugin including the terminating zero.
        return: the next plugin's uniqueID.
      */

      case effShellGetNextPlugin: // 70
        VST2_Trace("vst2: dispatcher/effShellGetNextPlugin\n");
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
        VST2_Trace("vst2: dispatcher/effStartProcess\n");
        MIsProcessing = true;
        break;

      //----------

      /*
        host calls this after the stop of process call and before e.g. suspend()
        - not called in energy xt2 linux
      */

      case effStopProcess: // 72
        VST2_Trace("vst2: dispatcher/effStopProcess\n");
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
        VST2_Trace("vst2: dispatcher/effSetTotalSampleToProcess\n");
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
        VST2_Trace("vst2: dispatcher/effSetPanLaw %i %.3f\n",(int)value,opt);
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
        VST2_Trace("vst2: dispatcher/effBeginLoadBank\n");
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
        VST2_Trace("vst2: dispatcher/effBeginLoadProgram\n");
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
        VST2_Trace("vst2: dispatcher/effSetProcessPrecision %i\n",(int)value);
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
        VST2_Trace("vst2: dispatcher/effGetNumMidiInputChannels\n");
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
        VST2_Trace("vst2: dispatcher/effGetNumMidiOutputChannels\n");
        break;

      //----------

      default:
        VST2_Trace("unhandled opcode: %i\n",opcode);
        break;

    }

    return result;

  }

  //----------

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
    MInstance->updateProcessValue(index,parameter);
    MInstance->updateEditorValue(index,parameter);
  }

  //----------

  /*
    we just return the last value we received from the host
    (we fill this up with default values when we create our instance)
  */

  float vst2_getParameter(VstInt32 index) {
    return MInstance->getProcessValue(index);
  }

  //----------

  //TODO:
  //MInstance->on_processBlock(inputs,outputs,sampleFrames,MProcessEventCount,MProcessEvents);

  void vst2_process(float** inputs, float** outputs, VstInt32 sampleFrames) {
    if (MNeedToInitializeParameters) {
      MInstance->initializeAllParameters();
      MNeedToInitializeParameters = false;
    }
    MInstance->updateParametersInProcess();
    MHost->updateSync();
    KODE_ProcessContext context;// = {0};
    uint32_t i;
    for (i=0; i<MDescriptor->getNumInputs(); i++)  { context.inputs[i]  = inputs[i]; }
    for (i=0; i<MDescriptor->getNumOutputs(); i++) { context.outputs[i] = outputs[i]; }
    context.length        = sampleFrames;
    context.oversample    = 1;
    context.samplerate    = MSampleRate;
    context.samplepos     = MHost->MSamplePos;    // getSamplePos();
    context.beatpos       = MHost->MBeatPos;
    context.tempo         = MHost->MTempo;        // getTempo();
    context.timesig_num   = MHost->MTimeSigNum;   // getTimeSigNum();
    context.timesig_denom = MHost->MTimeSigDenom; // getTimeSigDenom();
    MInstance->on_processBlock(&context);

    /*
    // process ticks
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
    // process samples
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
    // on_postProcess();
    //if hasFlag(KODE_PLUGIN_SEND_MIDI) MHost->flushMidi();
  }

  //----------

  void vst2_processDouble(double** inputs, double** outputs, VstInt32 sampleFrames) {
  }

  //----------

};

//----------------------------------------------------------------------
#endif
