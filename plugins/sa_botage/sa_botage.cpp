
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

//typedef KODE_Queue<uint32_t,256> KODE_WritePosQueue;

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

  // buffer

  float               MBuffer[BUFFERSIZE]         = {0};
  //uint32_t            MBufferSize                 = 0;
  bool                MBufferSizeUpdated          = false;

  //float               MReadPos                    = 0.0;
  uint32_t            MWritePos                   = 0;
  uint32_t            MNumSlices                  = 0;

  // state

  bool                MWasPlaying                 = false;

  // editor

  sa_botage_editor*   MEditor                     = KODE_NULL;
//float               MEditorBuffer[BUFFERSIZE]   = {0};
//KODE_WritePosQueue  MWritePosQueue;
//uint32_t            MWritePosPrev               = 0;

  // set bu process(), read by editor
  float               MGuiBufferLength            = 0.0;
  float               MGuiReadPos                 = 0.0;
  float               MGuiWritePos                = 0.0;
  uint32_t            MGuiCurrentSlice            = 0;

  uint32_t            MGuiRangeStartSlice         = 0;
  uint32_t            MGuiRangeNumSlices          = 0;
  uint32_t            MGuiLoopDivisions           = 0;

  // slices

  uint32_t            MCurrentSlice               = 0;
  uint32_t            MSliceLength                = 0;
  uint32_t            MSliceCounter               = 0;

  // range

  bool                MHasRange                   = false;
  uint32_t            MRangeCounter               = 0;
//  uint32_t            MRangeStartSlice            = 0;
//  uint32_t            MRangeLength                = 0;
//  uint32_t            MRangeCounter               = 0;

  // loop

  bool                MHasLoop                    = false;
  uint32_t            MLoopStart                  = 0;
  float               MLoopCounter                = 0.0;
  float               MLoopLength                 = 0.0;
  float               MLoopSpeed                  = 0.0;

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

//  bool copy_buffer_editor(int32_t writepos) {
//    int32_t diff = writepos - MWritePosPrev;
//    if (diff == 0) return false;
//    if (diff > 0) {
//      //float* dst = MEditorBuffer  + MWritePosPrev;
//      //float* src = MBuffer + MWritePosPrev;
//      //memcpy(dst,src,diff);
//    }
//    else {
//      //int32_t left = MBufferSize - MWritePosPrev;
//      //float*  dst  = MEditorBuffer  + MWritePosPrev;
//      //float*  src  = MBuffer + MWritePosPrev;
//      //memcpy(dst,src,left);
//      //int32_t newpos  = MWritePosPrev + diff;
//      //memcpy(MEditorBuffer,MBuffer,newpos);
//    }
//    return true;
//  }

  //----------

  bool update_waveform() {
    MEditor->set_waveform_buffer_size(MGuiBufferLength);
    MEditor->set_waveform_read_pos(MGuiReadPos);
    MEditor->set_waveform_write_pos(MGuiWritePos);
    //return copy_buffer_editor(writepos);
    return true;
  }

  //----------

  bool update_waveform_grid(bool ARedraw=false) {
    uint32_t num_slices = PNumBeats * PBeatSubdiv;
    if (MEditor) {
      uint32_t old_num = MEditor->get_waveform_grid();
      if (num_slices != old_num) {
        MEditor->set_waveform_grid(num_slices,PBeatSubdiv);
        if (ARedraw) MEditor->redraw_waveform();
        return true;
      }
    }
    return false;
  }

  //----------

  bool update_waveform_slice(bool ARedraw=false) {
    //uint32_t num_slices = PNumBeats * PBeatSubdiv;
    //MEditor->set_waveform_slice(MGuiCurrentSlice,num_slices);
    //if (ARedraw) MEditor->redraw_waveform();
    //return true;
    return false;
  }

  //----------

  bool update_waveform_range() {
    uint32_t num_slices = PNumBeats * PBeatSubdiv;
    if (MHasRange) {
      MEditor->set_waveform_range(MGuiRangeStartSlice,MGuiRangeNumSlices,num_slices);
      return true;
    }
    else {
      MEditor->set_waveform_range(0,0,num_slices);
      return true;
    }
  return false;
  }

  //----------

  bool update_waveform_loop() {
    uint32_t num_slices = PNumBeats * PBeatSubdiv;
    if (MHasLoop) {
      MEditor->set_waveform_loop(MGuiRangeStartSlice,MGuiRangeNumSlices,num_slices,MGuiLoopDivisions);
    }
    else {
      MEditor->set_waveform_loop(0,0,num_slices,0);
      return true;
    }
    return false;
  }

//------------------------------
private:
//------------------------------




//------------------------------
public:
//------------------------------

  //void on_plugin_open() final {}
  //void on_plugin_close() final {}
  //void on_plugin_initialize() final {}
  //void on_plugin_terminate() final {}
  //void on_plugin_activate() final {}
  //void on_plugin_deactivate() final {}

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
  }

  //uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final { *ABuffer = KODE_NULL; return 0; }
  //void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {}
  //void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {}

//------------------------------
public:
//------------------------------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    MEditor = new sa_botage_editor(this,AParent);
    MEditor->set_waveform_grid((PNumBeats * PBeatSubdiv), PBeatSubdiv);
    MEditor->set_waveform_buffer(MBuffer);
    MEditor->set_waveform_buffer_size(MGuiBufferLength);
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
//      uint32_t writepos = MWritePosPrev;
//      while (MWritePosQueue.read(&writepos)) { } // get last message
      bool changed = false;
      changed |= update_waveform();
      changed |= update_waveform_grid();
      changed |= update_waveform_range();
      changed |= update_waveform_loop();
      changed |= update_waveform_slice();
//      MWritePosPrev = writepos;
      if (changed) { MEditor->redraw_waveform(); }
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
        //MNumSlices = PNumBeats * PBeatSubdiv;
        //MBufferSizeUpdated = true;
        #ifdef KODE_PLUGIN_EXE
          update_waveform_grid(true);
        #endif
        break;
      case PAR_BEAT_SUBDIV:
        PBeatSubdiv = AValue;
        //MNumSlices = PNumBeats * PBeatSubdiv;
        //MBufferSizeUpdated = true;
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

  void next_slice(uint32_t ACurrentSlice, uint32_t ANumSlices, float ASliceLength) {
    if (ACurrentSlice >= ANumSlices) return;
    if (MHasRange) {
      MRangeCounter -= 1;
      if (MRangeCounter > 0) return;
      MHasRange = false;
      MHasLoop = false;
    }

    float prob = KODE_Random();
    if (prob < PRepeatProb) {
      int32_t  left = ANumSlices - ACurrentSlice - 1;
      uint32_t num  = KODE_RandomRangeInt(PRangeMinSlices,KODE_MinI(PRangeMaxSlices,left));
      uint32_t div  = KODE_RandomRangeInt(PRangeMinSubdiv,PRangeMaxSubdiv);
      MHasRange             = true;
      MRangeCounter         = num;
      MGuiRangeStartSlice   = MCurrentSlice;
      MGuiRangeNumSlices    = num;
      MHasLoop              = true;
      MLoopStart            = MWritePos;
      MLoopCounter          = 0.0;
      MLoopLength           = (ASliceLength * num) / div;
      MLoopSpeed            = 1.0;
      MGuiLoopDivisions     = div;
    }

  }

  //----------

//------------------------------
public:
//------------------------------

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    // num slices

    uint32_t num_slices = (PNumBeats * PBeatSubdiv);
    if (num_slices != MNumSlices) {
      //KODE_DPrint("MNumSlices changed\n");
      MNumSlices = num_slices;
    }

    // buffer length

    float samples_per_minute  = AContext->samplerate * 60.0;
    float samples_per_beat    = samples_per_minute / AContext->tempo;
    float samples_per_slice   = samples_per_beat / PBeatSubdiv;
    float buffer_length       = samples_per_beat * PNumBeats;

    while (MWritePos >= buffer_length) MWritePos -= buffer_length;
    MSliceLength = samples_per_slice;

    // state

    bool is_playing  = (AContext->playstate & KODE_PLUGIN_PLAYSTATE_PLAYING);
    //is_playing |= (MBufferSize > 0);
    bool is_starting = (is_playing && !MWasPlaying);
    bool is_stopping = (MWasPlaying && !is_playing);
    MWasPlaying = is_playing;

    // starting

    if (is_starting) {
      //KODE_DPrint("is_starting\n");
      MWritePos = 0;
      MSliceCounter = 0;
      MCurrentSlice = 0;
    }

    // stopping

    if (is_stopping) {
    }

    // per sample

    int32_t readpos = 0;//MWritePos;

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

      //-----

      if (is_playing) {

        if (MWritePos == 0) {
          next_slice(MCurrentSlice,num_slices,samples_per_slice);
        }

        // slice

        MSliceCounter += 1;
        if (MSliceCounter >= MSliceLength) {
          MSliceCounter = 0;
          //KODE_DPrint("slice %i end\n",MCurrentSlice);
          MCurrentSlice += 1;
          //if (MCurrentSlice < num_slices) {
          //  KODE_DPrint("slice %i\n",MCurrentSlice);
          //}
          next_slice(MCurrentSlice,num_slices,samples_per_slice);
        }

        // loop

        if (MHasLoop) {
          readpos = MLoopStart + (int32_t)MLoopCounter;
          MLoopCounter += MLoopSpeed;
          if (MLoopCounter >= MLoopLength) {
            MLoopCounter -= MLoopLength;
          }
        } // has loop
        else {
          readpos = MWritePos;
        }

        // write

        MBuffer[(MWritePos*2)  ] = in0;
        MBuffer[(MWritePos*2)+1] = in1;

        out0 = MBuffer[(readpos*2)  ];
        out1 = MBuffer[(readpos*2)+1];

        MWritePos += 1;
        if (MWritePos >= buffer_length) {
          //KODE_DPrint("end buffer\n");
          MWritePos     = 0;
          MCurrentSlice = 0;
          MSliceCounter = 0;
          MHasRange     = false;
          MHasLoop      = false;
        }


      } // is_playing

      //-----

      *output_0++ = out0;
      *output_1++ = out1;
    } // for all samples

    //

    //if (is_playing) {
    //  MWritePosQueue.write(MWritePos);
    //}

    // gui

    MGuiBufferLength      = buffer_length;
    MGuiReadPos           = readpos / buffer_length;
    MGuiWritePos          = (float)MWritePos / buffer_length;
    MGuiCurrentSlice      = MCurrentSlice;

  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
