
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

#include "sa_botage_descriptor.h"

#ifndef KODE_NO_GUI
  #include "sa_botage_editor.h"
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

  //

  float               MBuffer[BUFFERSIZE]         = {0};
  uint32_t            MBufferSize                 = 0;
  uint32_t            MWritePos                   = 0;
  uint32_t            MReadPos                    = 0.0;

  // state

  int32_t             MNumSlices                  = 0;

  bool                MWasPlaying                 = false;
  float               MSamplesPerBeat             = 0.0;  // calculated at the start of process
  float               MSamplesPerSlice            = 0.0;  // --"--

  float               MSliceCounter               = 0.0;
  float               MSliceLength                = 0.0;
  int32_t             MCurrentSlice               = 0;

  bool                MHasRange                   = false;
  uint32_t            MRangeSliceStart            = 0;
  uint32_t            MRangeSliceLength           = 0;
  uint32_t            MRangeCurrentSlice          = 0;
  //uint32_t            MRangeSliceCounter          = 0;

  bool                MHasLoop                    = false;
  uint32_t            MLoopNum                    = 0;
  float               MLoopSpeed                  = 1.0;
  uint32_t            MLoopStart                  = 0;    // samples (writepos restart)
  float               MLoopLength                 = 0.0;  // in samples, counted
  float               MLoopCounter                = 0.0;  // in samples, counted

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
private: // update_editor
//------------------------------

  bool copy_buffer_editor(int32_t writepos) {
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
    return true;
  }

  //----------

  bool update_waveform(int32_t writepos) {
    MEditor->set_waveform_buffer_size(MBufferSize);
    if (MBufferSize > 0) {
      MEditor->set_waveform_read_pos(MReadPos/2);
      MEditor->set_waveform_write_pos(MWritePos/2);
    }
    else {
      MEditor->set_waveform_read_pos(0);
      MEditor->set_waveform_write_pos(0);
    }
    return copy_buffer_editor(writepos);

  }

  //----------

  bool update_waveform_grid(bool ARedraw=false) {
    if (MEditor) {
      int32_t old_num = MEditor->get_waveform_grid();
      //uint32_t new_num = PNumBeats * PBeatSubdiv;
      if (MNumSlices != old_num) {
        MEditor->set_waveform_grid(MNumSlices,PBeatSubdiv);
        if (ARedraw) MEditor->redraw_waveform();
        return true;
      }
    }
    return false;
  }

  //----------

  bool update_waveform_slice(bool ARedraw=false) {
    //MEditor->set_waveform_range(MCurrentSlice,1,(PNumBeats * PBeatSubdiv));
    //if (ARedraw) MEditor->redraw_waveform();
    //return true;
    return false;
  }

  //----------

  bool update_waveform_range() {
    if (MHasRange) {
      //MEditor->set_waveform_range(MRangeSliceStart,MRangeSliceLength,(PNumBeats * PBeatSubdiv));
      MEditor->set_waveform_range(MRangeSliceStart,MRangeSliceLength,MNumSlices);
      return true;
    }
    else {
      //MEditor->set_waveform_range(0,0,(PNumBeats * PBeatSubdiv));
      MEditor->set_waveform_range(0,0,MNumSlices);
      return true;
    }
    return false;
  }

  //----------

  bool update_waveform_loop() {
    if (MHasLoop) {
      //MEditor->set_waveform_loop(MRangeSliceStart,MRangeSliceLength,(PNumBeats * PBeatSubdiv),MLoopNum);
      MEditor->set_waveform_loop(MRangeSliceStart,MRangeSliceLength,MNumSlices,MLoopNum);
    }
    else {
      //MEditor->set_waveform_loop(0,0,(PNumBeats * PBeatSubdiv),0);
      MEditor->set_waveform_loop(0,0,MNumSlices,0);
      return true;
    }
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
    //MEditor->set_waveform_grid(PNumBeats * PBeatSubdiv,PBeatSubdiv);
    MEditor->set_waveform_grid(MNumSlices,PBeatSubdiv);
    MEditor->set_waveform_buffer(MBuffer);
    MEditor->set_waveform_buffer_size(MBufferSize);
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
      bool wave_changed   = update_waveform(writepos);
      bool grid_changed   = update_waveform_grid();
      bool range_changed  = update_waveform_range();
      bool loop_changed   = update_waveform_loop();
      bool slice_changed  = update_waveform_slice();
      MWritePosPrev = writepos;
      if (wave_changed || grid_changed || range_changed || loop_changed || slice_changed) {
        MEditor->redraw_waveform();
      }
    }
  }

  #endif

//------------------------------
public:
//------------------------------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("%i = %.3f\n",AIndex,AValue);
    switch (AIndex) {
      case PAR_NUM_BEATS:
        PNumBeats = AValue;
        update_buffersize();
        #ifdef KODE_PLUGIN_EXE
          update_waveform_grid(true);
        #endif
        break;
      case PAR_BEAT_SUBDIV:
        PBeatSubdiv = AValue;
        update_buffersize();
        #ifdef KODE_PLUGIN_EXE
          update_waveform_grid(true);
        #endif
        break;
      case PAR_REPEAT_PROB:       PRepeatProb     = AValue * 0.01;  break;
      case PAR_RANGE_MIN_SLICES:  PRangeMinSlices = AValue;         break;
      case PAR_RANGE_MAX_SLICES:  PRangeMaxSlices = AValue;         break;
      case PAR_RANGE_MIN_SUBDIV:  PRangeMinSubdiv = AValue;         break;
      case PAR_RANGE_MAX_SUBDIV:  PRangeMaxSubdiv = AValue;         break;
      case PAR_LOOP_SIZE_PROB:    PLoopSizeProb   = AValue * 0.01;  break;
      case PAR_LOOP_SIZE_AMT:     PLoopSizeAmt    = AValue * 0.01;  break;
      case PAR_LOOP_SPEED_PROB:   PLoopSpeedProb  = AValue * 0.01;  break;
      case PAR_LOOP_SPEED_AMT:    PLoopSpeedAmt   = AValue * 0.01;  break;
      case PAR_XFADE_MODE:        PXFadeMode      = AValue;         break;
      case PAR_XFADE_AMT:         PXFadeAmt       = AValue;         break;
    }
  }

//------------------------------
private:
//------------------------------

  void update_buffersize() {
    MNumSlices          = PNumBeats * PBeatSubdiv;
    MHasRange           = false;
    MHasLoop            = false;

    //MWritePos           = 0;
    //MRangeCurrentSlice  = 0;
    //MLoopCounter        = 0.0;
    //MSliceCounter       = 0.0;
    //MCurrentSlice       = KODE_MaxI(MNumSlices-1,MCurrentSlice);

  }

  //----------

// MWritePos points to first sample in coming slice

  void start_loop() {
    uint32_t num = 0;
    if (PRangeMinSlices > PRangeMaxSlices)
      num = KODE_RandomRangeInt(PRangeMaxSubdiv,PRangeMinSubdiv);
    else
      num = KODE_RandomRangeInt(PRangeMinSubdiv,PRangeMaxSubdiv);

    if (num > 0) {
      //KODE_DPrint("- start loop %i\n",num);
      MHasLoop      = true;
      MLoopNum      = num;
      MLoopStart    = MWritePos;
      MLoopLength   = MSamplesPerSlice * (float)MRangeSliceLength / (float)num;
      MLoopCounter  = 0.0;
      MLoopSpeed    = 1.0;
    }
  }

  //----------

  void start_range() {
    uint32_t len = 0;
    MHasRange = false;
    MHasLoop = false;
    float rnd = KODE_Random();
    if (rnd < PRepeatProb) {
      if (PRangeMinSlices > PRangeMaxSlices)
        len = KODE_RandomRangeInt(PRangeMaxSlices,PRangeMinSlices);
      else
        len = KODE_RandomRangeInt(PRangeMinSlices,PRangeMaxSlices);
      uint32_t left = MNumSlices - MCurrentSlice;
      len = KODE_MinI(left,len);
      if (len > 0) {
        MHasRange           = true;
        MRangeCurrentSlice  = 0;
        MRangeSliceStart    = MCurrentSlice;
        MRangeSliceLength   = len;
        start_loop();
      }
    }
  }

  //----------

  void next_slice() {
    //MSliceCounter = 0.0;
    //MLoopCounter = 0.0;
    MCurrentSlice += 1;
    if (MHasRange) {
      MRangeCurrentSlice += 1;
      if (MRangeCurrentSlice >= MRangeSliceLength) {
        start_range();
      }
    }
    else {
      start_range();
    }
    // fx per slice? envelope?
  }

  //----------

    // speed inc/dec
    // loopsize inc/dec

  void next_loop() {
    float rnd;
    rnd = KODE_Random();
    if (rnd < PLoopSpeedProb) MLoopSpeed *= PLoopSpeedAmt;
    MLoopSpeed = KODE_Clamp(MLoopSpeed,0.25,4.0);
    //rnd = KODE_Random();
    //if (rnd < PLoopSizeProb) MLoopLength *= PLoopSizeAmt;

  }

  //----------

  uint32_t calc_buffer_size(KODE_ProcessContext* AContext) {
    float samples_per_minute = AContext->samplerate * 60.0;
    MSamplesPerBeat = samples_per_minute / AContext->tempo;
    MSamplesPerSlice = MSamplesPerBeat / PBeatSubdiv;
    return MSamplesPerBeat * PNumBeats;
  }

//------------------------------
public:
//------------------------------

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    // buffer

    MBufferSize = calc_buffer_size(AContext);
    MBufferSize *= 2; // stereo
    MSliceLength = MSamplesPerSlice;

    // state

    bool is_playing = (AContext->playstate & KODE_PLUGIN_PLAYSTATE_PLAYING);
    bool is_starting = (is_playing && !MWasPlaying);
    bool is_stopping = (MWasPlaying && !is_playing);
    MWasPlaying = is_playing;

    //-----

    if (is_starting) {
      MReadPos      = 0;
      MWritePos     = 0;
      MSliceCounter = 0.0;
      MCurrentSlice = -1;
      next_slice();
    }

    //-----

    if (is_stopping) {
    }

    //-----

    float*   input_0      = AContext->inputs[0];
    float*   input_1      = AContext->inputs[1];
    float*   output_0     = AContext->outputs[0];
    float*   output_1     = AContext->outputs[1];
    uint32_t num_samples  = AContext->numsamples;
    for (uint32_t i=0; i<num_samples; i++) {
      float in0 = *input_0++;
      float in1 = *input_1++;
      //-----
      if (is_playing) {
        MBuffer[MWritePos] = in0;
        MBuffer[MWritePos+1] = in1;

        if (MHasLoop) {
          int32_t ipos = (int32_t)MLoopCounter * 2; // stereo
          MReadPos = MLoopStart + ipos;
          MLoopCounter += MLoopSpeed;
          if (MLoopCounter >= MLoopLength) {
            MLoopCounter -= MLoopLength;
            next_loop();
          }
        }
        else {
          MReadPos = MWritePos;
        }

        MWritePos += 2;
        if (MWritePos >= MBufferSize) {
          MWritePos = 0;
          MHasRange = false;
          MHasLoop = false;
          MSliceCounter = 0.0;
          MLoopCounter = 0.0;
          MCurrentSlice = -1;
          next_slice();
        }
        else {
          MSliceCounter += 1.0;
          if (MSliceCounter >= MSliceLength) {
            MSliceCounter -= MSliceLength;
            next_slice();
          }
        } // buffer weap
      } // is_playing
      //-----
      float out0 = MBuffer[MReadPos];
      float out1 = MBuffer[MReadPos+1];
      // out
      *output_0++ = out0;
      *output_1++ = out1;
    } // for all samples

    if (is_playing) {
      MWritePosQueue.write(MWritePos);
    }

    //KODE_DPrint("readpos %i writepos %i buffersize % i\n",MReadPos,MWritePos,MBufferSize);
    //KODE_DPrint("MLoopCounter %.2f MSliceCOunter %.2f\n",MLoopCounter,MSliceCounter);

  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
