#ifndef kode_vst2_host_included
#define kode_vst2_host_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_host.h"
#include "plugin/vst2/kode_vst2.h"

//----------------------------------------------------------------------

struct KODE_VstEvents {
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[KODE_PLUGIN_MAX_MIDI_SEND];
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_Vst2Host
: public KODE_Host {

  friend class KODE_Vst2Instance;

//------------------------------
private:
//------------------------------

  audioMasterCallback MAudioMaster  = nullptr;
  AEffect*            MAEffect      = nullptr;
  KODE_VstEvents      MVstEvents    = {0};
  VstMidiEvent        MVstMidiSendEvents[KODE_PLUGIN_MAX_MIDI_SEND] = {0};

  float               MTempo        = 0.0f;
  uint32_t            MTimeSigNum   = 0;
  uint32_t            MTimeSigDenom = 0;
  uint32_t            MSamplePos    = 0;
  uint32_t            MPlayState    = 0;
  float               MBeatPos      = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_Vst2Host(KODE_Descriptor* ADescriptor, audioMasterCallback AMaster, AEffect* AAEffect)
  : KODE_Host(ADescriptor) {
    MFormat = KODE_PLUGIN_FORMAT_VST2;
    MAudioMaster = AMaster;
    MAEffect = AAEffect;
    //memset(&MVstEvents,0,sizeof(KODE_VstEvents));
    memset(&MVstMidiSendEvents,0,sizeof(MVstMidiSendEvents));
    for (uint32_t i=0; i<KODE_PLUGIN_MAX_MIDI_SEND; i++) {
      MVstEvents.events[i] = (VstEvent*)&MVstMidiSendEvents[i];
    }
  }

  //----------

  virtual ~KODE_Vst2Host() {
    //KODE_Trace("~KODE_Vst2Host\n");
  }

//------------------------------
private:
//------------------------------

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

  void updateSync() {
    VstIntPtr mask = 0xffff;
    VstIntPtr result = MAudioMaster(MAEffect,audioMasterGetTime,0,mask,0,0);
    VstTimeInfo* timeinfo = (VstTimeInfo*)result;
    if (timeinfo) {
      //MNanoSeconds    = timeinfo->nanoSeconds;
      MSamplePos      = timeinfo->samplePos;
      //MSampleRate     = timeinfo->sampleRate;
      MTempo          = timeinfo->tempo;
      MTimeSigNum     = timeinfo->timeSigNumerator;
      MTimeSigDenom   = timeinfo->timeSigDenominator;
      MBeatPos        = timeinfo->ppqPos;
      //MPPQBarStart    = timeinfo->barStartPos;
      //MPPOCycleStart  = timeinfo->cycleStartPos;
      //MPPQCycleEnd    = timeinfo->cycleEndPos;
      //MPlayState      = timeinfo->flags & 127;
      MPlayState = KODE_PLUGIN_PLAYSTATE_NONE;
      if (timeinfo->flags & kVstTransportPlaying)     MPlayState |= KODE_PLUGIN_PLAYSTATE_PLAYING;
      if (timeinfo->flags & kVstTransportRecording)   MPlayState |= KODE_PLUGIN_PLAYSTATE_RECORDING;
      if (timeinfo->flags & kVstTransportCycleActive) MPlayState |= KODE_PLUGIN_PLAYSTATE_LOOPING;
    }
  }

//------------------------------
public:
//------------------------------

  void sendMidi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
    if (MAudioMaster) {
      //TODO: push event to array
      // see flushMidi
      int32_t      num   = MVstEvents.numEvents;
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

  //----------

  void flushMidi() override {
    if (MAudioMaster) {
      MAudioMaster(MAEffect,audioMasterProcessEvents,0,0,(VstEvents*)&MVstEvents,0);
      MVstEvents.numEvents = 0;
    }
  }

  //----------

  void updateParameter(uint32_t AIndex, float AValue) override {
    if (MAudioMaster) {
      MAudioMaster(MAEffect,audioMasterBeginEdit,AIndex,0,0,0);
      MAudioMaster(MAEffect,audioMasterAutomate,AIndex,0,0,AValue);
      MAudioMaster(MAEffect,audioMasterEndEdit,AIndex,0,0,0);
    }
  }

  //----------

  void resizeEditor(uint32_t AWidth, uint32_t AHeight) override {
    if (MAudioMaster) {
      MAudioMaster(MAEffect,audioMasterSizeWindow,AWidth,AHeight,0,0);
    }
  }


//------------------------------
public:
//------------------------------

  /*

  uint32_t getId() override {
    return MHostId;
  }

  //----------

  const char* getName() override {
    return MHostName;
  }

  //----------

  float getTempo() override {
    return MTempo;
  }

  //----------

  uint32_t getTimeSigNum() override {
    return MTimeSigNum;
  }

  //----------

  uint32_t getTimeSigDenom() override {
    return MTimeSigDenum;
  }

  //----------

  uint32_t getSamplePos() override {
    return 0;
  }

  */

//------------------------------
//
//------------------------------

    /*
    void updateHostInfo(void) { // override {
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
    }
    */

    /*
    KODE_HostInfo* host_getInfo() {
      MAudioMaster(MAEffect,audioMasterGetVendorString,0,0,MHostInfo.vendor,0);
      MAudioMaster(MAEffect,audioMasterGetProductString,0,0,MHostInfo.product,0);
      MHostInfo.version   = MAudioMaster(MAEffect,audioMasterGetVendorVersion,0,0,0,0);
      MHostInfo.language  = MAudioMaster(MAEffect,audioMasterGetLanguage,0,0,0,0);
      char* path = (char*)MAudioMaster(MAEffect,audioMasterGetDirectory,0,0,0,0);
      strcpy(MHostInfo.directory,path);
      //if (MAudioMaster(&MAEffect,audioMasterCanDo,0,0,(char*)"acceptIOChanges",0) == 1) MHostInfo.canDo += khc_acceptIoChange;
      return &MHostInfo;
    }
    */

    //----------

    /*
    KODE_HostTime* host_getTime() {
      memset(&MHostTime,0,sizeof(KODE_HostTime));
      VstIntPtr mask = 0xffff;
      VstIntPtr result = MAudioMaster(MAEffect,audioMasterGetTime,0,mask,0,0);
      VstTimeInfo* timeinfo = (VstTimeInfo*)result;
      if (timeinfo) {
        MHostTime.nano_seconds      = timeinfo->nanoSeconds;
        MHostTime.sample_pos        = timeinfo->samplePos;
        MHostTime.sample_rate       = timeinfo->sampleRate;
        MHostTime.tempo             = timeinfo->tempo;
        MHostTime.timesig_num       = timeinfo->timeSigNumerator;
        MHostTime.timesig_denom     = timeinfo->timeSigDenominator;
        MHostTime.ppq_pos           = timeinfo->ppqPos;
        MHostTime.ppq_bar_start     = timeinfo->barStartPos;
        MHostTime.ppq_cycle_start   = timeinfo->cycleStartPos;
        MHostTime.ppq_cycle_end     = timeinfo->cycleEndPos;
        MHostTime.flags = timeinfo->flags & 127;
        return &MHostTime;
      }
      return nullptr;
    }
    */

  //----------

  //void sendEvents(uint32_t ANumEvents, KODE_Event** MEvents)  override {
  //}

  //----------


  //----------

  /*

  bool host_ioChanged() {
    if (MAudioMaster(&MAEffect,audioMasterIOChanged,0,0,0,0) == 1) return true;;
    return false;
  }

  //----------

  uint32_t host_getSampleRate() {
    return MAudioMaster(&MAEffect,audioMasterGetSampleRate,0,0,0,0);
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

  uint32_t host_getAutomationState() {
    return MAudioMaster(&MAEffect,audioMasterGetAutomationState,0,0,0,0);
  }

  //----------

  void host_updateDisplay() {
    MAudioMaster(&MAEffect,audioMasterUpdateDisplay,0,0,0,0);
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

  */

};

//----------------------------------------------------------------------
#endif
