
//#define KODE_NO_GUI
//#define KODE_GUI_XCB
#define KODE_GUI_CAIRO

//----------

//#define KODE_DEBUG_PRINT_TIME
//#define KODE_DEBUG_PRINT_THREAD
#define KODE_DEBUG_PRINT_SOCKET

// nc -U -l -k /tmp/kode.socket

//----------

#include "kode.h"
#include "audio/kode_audio_utils.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_voice_manager.h"

#include "sa_botage/sa_botage_descriptor.h"

#ifndef KODE_NO_GUI
  #include "sa_botage/sa_botage_editor.h"
#endif // KODE_NO_GUI

//----------------------------------------------------------------------

typedef KODE_Queue<uint32_t,256> KODE_WritePosQueue;

//----------

//class myVoice;
//typedef KODE_VoiceManager<myVoice,16> myVoices;

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

/*

  max buffer size:

  num_beats           8
  samples_per_second  192000
  beats_per_minute    30

  samples_per_minute = samples_per_second * 60
  samples_per_beat = samples_per_minute / beats_per_minute
  buffersize = samples_per_beat * num_beats

  srate * 60 * beats   192k * 60 * 8   92160k
  ------------------ = ------------- = ------ = 3072k -> 4mb
          bpm                 30         30

*/

// 4mb stereo
#define BUFFERSIZE (1024 * 1024 * 4 * 2)

class myInstance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  // parameters

  uint32_t            PNumBeats                   = 4;
  uint32_t            PBeatSubdiv                 = 4;
  float               PRepeatProb                 = 0;
  uint32_t            PRangeMinSlices             = 0;
  uint32_t            PRangeMaxSlices             = 0;
  uint32_t            PRangeMinSubdiv             = 0;
  uint32_t            PRangeMaxSubdiv             = 0;
  float               PLoopSizeProb               = 0;
  float               PLoopSizeAmt                = 0;
  float               PLoopSpeedProb              = 0;
  float               PLoopSpeedAmt               = 0;
  uint32_t            PXFadeMode                  = 0;
  float               PXFadeAmt                   = 0;

  // editor

  sa_botage_editor*   MEditor                     = KODE_NULL;
  float               MEditorBuffer[BUFFERSIZE]   = {0};
  KODE_WritePosQueue  MWritePosQueue;
  uint32_t            MWritePosPrev               = 0;

  // buffer
  float               MBuffer[BUFFERSIZE]         = {0};
  uint32_t            MBufferSize                 = 0;
  uint32_t            MWritePos                   = 0;
  float               MInvBufferSize              = 1.0;
  float               MReadPos                    = 0.0;
  float               MReadSpeed                  = 1.0;

  // state
  bool                MWasPlaying                 = false;
  float               MSamplesPerBeat             = 0.0;  // calculated at the start of process
  float               MSamplesPerSlice            = 0.0;  // --"--

  bool                MHasRange         = false;
  uint32_t            MRangeCounter     = 0;
  uint32_t            MRangeStart       = 0;
  uint32_t            MRangeLength      = 0;

  uint32_t            MRangeSliceStart  = 0;
  uint32_t            MRangeSliceLength = 0;


  uint32_t            MCurrentSlice     = 0;
  uint32_t            MSliceCounter     = 0;
  uint32_t            MSliceLength      = 0;

  uint32_t            MLoopCounter      = 0;
  uint32_t            MLoopStart        = 0;
  uint32_t            MLoopLength       = 0;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    wrap_buffer();
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
private:
//------------------------------

  void set_waveform_buffer(float* ABuffer) {
    MEditor->WWaveform->setBuffer(ABuffer);
  }

  //----------

  void set_waveform_buffer_size(uint32_t ABufferSize) {
    MEditor->WWaveform->setBufferSize(ABufferSize / 2);
  }

  //----------

  void set_waveform_grid(uint32_t ASlices, uint32_t AMajor) {
    MEditor->WWaveform->setNumGrid(ASlices);
    MEditor->WWaveform->setNumGridMajor(AMajor);
  }

  //----------

  void set_waveform_write_pos(uint32_t AWritePos) {
    float writepos = (float)AWritePos * MInvBufferSize;
    MEditor->WWaveform->setMarkerPos(0,writepos);
  }

  //----------

  void set_waveform_read_pos(uint32_t AReadPos) {
    float readpos = (float)AReadPos * MInvBufferSize;
    MEditor->WWaveform->setMarkerPos(1,readpos);
  }

  //----------

  void set_waveform_range(uint32_t AStartSlice, uint32_t ANumSlices) {
    float num_slices = (float)(PNumBeats * PBeatSubdiv);
    float pos  = AStartSlice / num_slices;
    float size = ANumSlices / num_slices;
    MEditor->WWaveform->setAreaPosSize(0,pos,size);
  }

  //----------

  //void set_waveform_loop(uint32_t ASubdiv) {
  //}

  //----------

  //void wrapWaveformLoop() {
  //}

  //----------

  void redraw_waveform() {
    MEditor->WWaveform->redraw();
  }

//------------------------------
private:
//------------------------------

  /*
    waveform only updates if playing..
  */

  bool update_waveform(int32_t writepos, bool ARedraw=false) {
    if (MEditor) {
      set_waveform_buffer_size(MBufferSize);
      if (MBufferSize > 0) {
        set_waveform_write_pos(MWritePos);
        set_waveform_write_pos(MReadPos);
      }
      else {
        set_waveform_write_pos(0);
        set_waveform_write_pos(0);
      }
      int32_t diff = writepos - MWritePosPrev;
      if (diff == 0) return false;

      if (diff > 0) {
        //float* dst = MEditorBuffer  + MWritePosPrev;
        //float* src = MBuffer + MWritePosPrev;
        //memcpy(dst,src,diff);
      }
      else {
        //int32_t left = MBufferSize - MWritePosPrev;
        //float*  dst  = MEditorBuffer  + MWritePosPrev;
        //float*  src  = MBuffer + MWritePosPrev;
        //memcpy(dst,src,left);
        //int32_t newpos  = MWritePosPrev + diff;
        //memcpy(MEditorBuffer,MBuffer,newpos);
      }
      if (ARedraw) redraw_waveform();
    }
    return true;
  }

  //----------

  bool update_waveform_grid(bool ARedraw=false) {
    if (MEditor) {
      // grid
      uint32_t old_num = MEditor->WWaveform->getNumGrid();
      uint32_t new_num = PNumBeats * PBeatSubdiv;
      if (new_num != old_num) {
        set_waveform_grid(new_num,PBeatSubdiv);
        if (ARedraw) redraw_waveform();
        return true;
      }
    }
    return false;
  }

  //----------

  bool update_waveform_slice(bool ARedraw=false) {
    set_waveform_range(MCurrentSlice,1);
    if (ARedraw) redraw_waveform();
    return true;
  }

  //----------

  bool update_waveform_range(bool ARedraw=false) {
    if (MHasRange) {
      set_waveform_range(MRangeSliceStart,MRangeSliceLength);
      if (ARedraw) redraw_waveform();
      return true;
    }
    else {
      set_waveform_range(0,0);
    }
    return false;
  }

  //----------

  bool update_waveform_loop(bool ARedraw=false) {
    return false;
  }

//------------------------------
public:
//------------------------------

  //void on_plugin_open() final {}
  //void on_plugin_close() final {}
  //void on_plugin_initialize() final {}
  //void on_plugin_terminate() final {}
  //void on_plugin_activate() final {}
  //void on_plugin_deactivate() final {}
  //void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {}
  //uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final { *ABuffer = KODE_NULL; return 0; }
  //void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {}
  //void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {}

//------------------------------
public:
//------------------------------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    MEditor = new sa_botage_editor(this,AParent);
    set_waveform_grid(PNumBeats * PBeatSubdiv,PBeatSubdiv);
    set_waveform_buffer(MBuffer);
    set_waveform_buffer_size(MBufferSize);
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    if (MEditor) {
      delete MEditor;
      MEditor = KODE_NULL;
    }
  }

  //----------

  void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
    if (MEditor) {
      uint32_t writepos = MWritePosPrev;
      while (MWritePosQueue.read(&writepos)) { } // get last message
      bool wave_changed = update_waveform(writepos);
      bool grid_changed = update_waveform_grid();
      bool range_changed = update_waveform_range();
      bool loop_changed = update_waveform_loop();
      //bool slice_changed = update_waveform_slice();
      MWritePosPrev = writepos;
      if (wave_changed || grid_changed || range_changed || loop_changed) {
        redraw_waveform();
      }
    }
  }

  #endif

//------------------------------
public:
//------------------------------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch (AIndex) {
      case PAR_NUM_BEATS:
        PNumBeats = AValue;
        #ifdef KODE_PLUGIN_EXE
          update_waveform_grid(true);
        #endif
        break;
      case PAR_BEAT_SUBDIV:
        PBeatSubdiv = AValue;
        #ifdef KODE_PLUGIN_EXE
          update_waveform_grid(true);
        #endif
        break;
      case PAR_REPEAT_PROB:       PRepeatProb     = AValue;   break;
      case PAR_RANGE_MIN_SLICES:  PRangeMinSlices = AValue;   break;
      case PAR_RANGE_MAX_SLICES:  PRangeMaxSlices = AValue;   break;
      case PAR_RANGE_MIN_SUBDIV:  PRangeMinSubdiv = AValue;   break;
      case PAR_RANGE_MAX_SUBDIV:  PRangeMaxSubdiv = AValue;   break;
      case PAR_LOOP_SIZE_PROB:    PLoopSizeProb   = AValue;   break;
      case PAR_LOOP_SIZE_AMT:     PLoopSizeAmt    = AValue;   break;
      case PAR_LOOP_SPEED_PROB:   PLoopSpeedProb  = AValue;   break;
      case PAR_LOOP_SPEED_AMT:    PLoopSpeedAmt   = AValue;   break;
      case PAR_XFADE_MODE:        PXFadeMode      = AValue;   break;
      case PAR_XFADE_AMT:         PXFadeAmt       = AValue;   break;
    }
  }

//------------------------------
private:
//------------------------------

  uint32_t calc_buffer_size(KODE_ProcessContext* AContext) {
    float samples_per_minute = AContext->samplerate * 60.0;
    MSamplesPerBeat = samples_per_minute / AContext->tempo;
    MSamplesPerSlice = MSamplesPerBeat / PBeatSubdiv;
    return MSamplesPerBeat * PNumBeats;
  }

  //----------

  void start_slice(uint32_t ACurrentSlice, bool AHasRange) {
    KODE_DPrint("slice %i has_range %i \n",MCurrentSlice,MHasRange);
  }

  //----------

  void wrap_buffer() {
    MCurrentSlice = 0;
    MSliceCounter = 0;
    MHasRange = false;
  }

  //----------

  void wrap_slice() {
    MCurrentSlice += 1;
    MCurrentSlice %= (PNumBeats * PBeatSubdiv);
    start_slice(MCurrentSlice,MHasRange);
  }

  //----------

  void wrap_loop() {
  }

  //----------

  void wrap_range() {
  }

//------------------------------
public:
//------------------------------

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    // buffer

    MBufferSize = calc_buffer_size(AContext);
    MInvBufferSize = 1.0 / (float)MBufferSize;
    MBufferSize *= 2; // stereo

    MSliceLength = MSamplesPerSlice;

    // state

    bool is_playing = (AContext->playstate & KODE_PLUGIN_PLAYSTATE_PLAYING);
    bool is_starting = (is_playing && !MWasPlaying);
    bool is_stopping = (MWasPlaying && !is_playing);
    MWasPlaying = is_playing;

    //

    if (is_starting) {
      wrap_buffer();
      start_slice(0,false);
    }

    if (is_stopping) {
    }

    //

    float*   input_0      = AContext->inputs[0];
    float*   input_1      = AContext->inputs[1];
    float*   output_0     = AContext->outputs[0];
    float*   output_1     = AContext->outputs[1];
    uint32_t num_samples  = AContext->numsamples;

    for (uint32_t i=0; i<num_samples; i++) {
      float in0 = *input_0++;
      float in1 = *input_1++;
      float out0 = in0;
      float out1 = in1;

      MBuffer[MWritePos++] = in0;
      MBuffer[MWritePos++] = in1;

      if (MWritePos >= MBufferSize) {
        //MWritePos -= MBufferSize;
        MWritePos = 0;
        wrap_buffer();
      }

      MSliceCounter += 1;
      if (MSliceCounter >= MSliceLength) {
        MSliceCounter = 0;
        wrap_slice();
      }

      if (MHasRange) {
        MLoopCounter += 1;
        if (MLoopCounter >= MLoopLength) {
          MLoopCounter = 0;
          wrap_loop();
        }
        MRangeCounter += 1;
        if (MRangeCounter >= MRangeLength) {
          wrap_range();
        }
      }

      // out
      *output_0++ = out0;
      *output_1++ = out1;

    }

    MWritePosQueue.write(MWritePos);

  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
