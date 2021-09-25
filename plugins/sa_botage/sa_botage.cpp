
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

//#include "audio/filters/kode_rc_filter.h"
//#include "audio/modulation/kode_envelope.h"

#include "sa_botage_descriptor.h"

#ifndef KODE_NO_GUI
  #include "sa_botage_editor.h"
  //#include "sa_botage_editor2.h"
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
  float               PLoopSizeMin                = 0;
  float               PLoopSizeMax                = 0;
  float               PLoopSpeedProb              = 0;
  float               PLoopSpeedMin               = 0;
  float               PLoopSpeedMax               = 0;
  float               PLoopReverseProb            = 0;

  float               PSliceEnvAtt                = 0;
  float               PSliceEnvDec                = 0;
  float               PLoopEnvAtt                 = 0;
  float               PLoopEnvDec                 = 0;

  //uint32_t            PXFadeMode                  = 0;
  //float               PXFadeAmt                   = 0;

  sa_botage_editor*   MEditor                     = KODE_NULL;
  //float               MEditorBuffer[BUFFERSIZE]   = {0};
  //uint32_t            MWritePosPrev               = 0;
  //KODE_WritePosQueue  MWritePosQueue;

  float               MSamplesPerMinute           = 0.0;
  float               MSamplesPerBeat             = 0.0;

  float               MBuffer[BUFFERSIZE]         = {0};
  float               MBufferLength               = 0.0;
  bool                MBufferSizeUpdated          = false;

  float               MReadPos                    = 0.0;
  uint32_t            MWritePos                   = 0;
  uint32_t            MNumSlices                  = 0;
  bool                MWasPlaying                 = false;

  bool                MHasRange                   = false;
  uint32_t            MRangeCounter               = 0;
  uint32_t            MRangeNumSlices             = 0;
  uint32_t            MRangeStartSlice            = 0;
  uint32_t            MLoopDivisions              = 0;
  uint32_t            MCurrentSlice               = 0;
  uint32_t            MSliceLength                = 0;
  uint32_t            MSliceCounter               = 0;

  bool                MHasLoop                    = false;
  uint32_t            MLoopStart                  = 0;
  float               MLoopCounter                = 0.0;
  float               MLoopLength                 = 0.0;
  float               MLoopSpeed                  = 0.0;
  bool                MLoopReverse                = false;

  //KODE_RcFilter       MSliceEnv;
  //KODE_RcFilter       MLoopEnv;
  //KODE_Envelope

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
private:
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

//------------------------------
private:
//------------------------------

  /*
    MBufferLength
    MReadPos
    MWritePos
  */

  bool update_editor_waveform_cursors() {
    MEditor->set_waveform_buffer_size(MBufferLength);
    float rp = MReadPos / MBufferLength;
    float wp = (float)MWritePos / MBufferLength;
    MEditor->set_waveform_read_pos(rp);
    MEditor->set_waveform_write_pos(wp);
    //return copy_buffer_editor(writepos);
    return true;
  }

  //----------

  /*
    MNumSlices
    PBeatSubdiv
  */

  bool update_editor_waveform_grid(bool ARedraw=false) {
    if (MEditor) {
      uint32_t old_num = MEditor->get_waveform_grid();
      if (MNumSlices != old_num) {
        MEditor->set_waveform_grid(MNumSlices,PBeatSubdiv);
        if (ARedraw) MEditor->redraw_waveform();
        return true;
      }
    }
    return false;
  }

  //----------

  /*
    MNumSlices
    MCurrentSlice
  */

  bool update_editor_waveform_slice(bool ARedraw=false) {
    //uint32_t num_slices = PNumBeats * PBeatSubdiv;
    //MEditor->set_waveform_slice(MGuiCurrentSlice,num_slices);
    //if (ARedraw) MEditor->redraw_waveform();
    //return true;
    return false;
  }

  //----------

  /*
    MHasRange
    MRangeStartSlice
    MRangeNumSlices
    MNumSlices
  */

  bool update_editor_waveform_range() {
    //uint32_t num_slices = PNumBeats * PBeatSubdiv;
    if (MHasRange) {
      MEditor->set_waveform_range(MRangeStartSlice,MRangeNumSlices,MNumSlices);
      return true;
    }
    else {
      MEditor->set_waveform_range(0,0,MNumSlices);
      return true;
    }
  return false;
  }

  //----------

  /*
    MHasLoop
    MLoopStart
    MLoopLength
    MBufferLength
  */

  bool update_editor_waveform_loop() {
    if (MHasLoop) {
      float pos = 0;
      float size = 0;
      if (MBufferLength > 0) {
        pos  = (float)MLoopStart / MBufferLength;
        size = (float)MLoopLength / MBufferLength;
      }
      else {
        pos = 0;
        size = 0;
      }
      MEditor->set_waveform_loop(pos,size);
      return false;
    }
    else {
      MEditor->set_waveform_loop(0,0);
      return true;
    }
    return false;
  }

//------------------------------
private:
//------------------------------

  void next_slice() {
    if (MCurrentSlice >= MNumSlices) return;
    if (MHasRange) {
      MRangeCounter -= 1;
      if (MRangeCounter > 0) return;
      MHasRange = false;
      MHasLoop = false;
    }

    float prob = KODE_Random();
    if (prob < PRepeatProb) {
      int32_t  left     = MNumSlices - MCurrentSlice - 1;
      uint32_t num      = KODE_RandomRangeInt(PRangeMinSlices,KODE_MinI(PRangeMaxSlices,left));
      uint32_t div      = KODE_RandomRangeInt(PRangeMinSubdiv,PRangeMaxSubdiv);
      MHasRange         = true;
      MRangeCounter     = num;
      MRangeStartSlice  = MCurrentSlice;
      MRangeNumSlices   = num;
      MHasLoop          = true;
      MLoopStart        = MWritePos;
      MLoopCounter      = 0.0;
      MLoopLength       = (MSliceLength * num) / div;
      MLoopSpeed        = 1.0;
      MLoopReverse      = false;
      MLoopDivisions    = div;
    }

  }

  //----------

  void next_loop() {

    // speed

    float speed_prob = KODE_Random();
    if (speed_prob < PLoopSpeedProb) {
      float lsmin = PLoopSpeedMin;
      float lsmax = PLoopSpeedMax;
      if (lsmin >= lsmax) {
        float temp = lsmin;
        lsmin = lsmax;
        lsmax = temp;
      }
      float range = lsmax - lsmin;
      float amount = range * KODE_Random();
      MLoopSpeed *= (lsmin + amount);
      MLoopSpeed =  KODE_Clamp(MLoopSpeed, (1.0/16.0), 16.0);
    }

    // size

    float size_prob = KODE_Random();
    if (size_prob < PLoopSizeProb) {
      float lsmin = PLoopSizeMin;
      float lsmax = PLoopSizeMax;
      if (lsmin >= lsmax) {
        float temp = lsmin;
        lsmin = lsmax;
        lsmax = temp;
      }
      float range = lsmax - lsmin;
      float amount = range * KODE_Random();
      MLoopLength *= (lsmin + amount);
      float range_length = MNumSlices * MSliceLength;
      MLoopLength =  KODE_Clamp(MLoopLength,16,range_length);
    }

    // reverse

    float reverse_prob = KODE_Random();
    // toggle or set?
    if (reverse_prob < PLoopReverseProb) MLoopReverse = true;
    else MLoopReverse = false;

  }

  //----------

  void process(KODE_ProcessContext* AContext) {
    // num slices

    uint32_t num_slices = (PNumBeats * PBeatSubdiv);
    if (num_slices != MNumSlices) {
      MNumSlices = num_slices;
    }

    // buffer length

    MSamplesPerMinute = AContext->samplerate * 60.0;
    MSamplesPerBeat   = MSamplesPerMinute / AContext->tempo;
    MSliceLength      = MSamplesPerBeat / PBeatSubdiv;
    MBufferLength     = MSamplesPerBeat * PNumBeats;
    while (MWritePos >= MBufferLength) MWritePos -= MBufferLength;

    // state

    bool is_playing  = (AContext->playstate & KODE_PLUGIN_PLAYSTATE_PLAYING);
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

    uint32_t readpos = 0;

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
          next_slice();
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
          next_slice();
        }

        // loop

        if (MHasLoop) {
          readpos = MLoopStart + (int32_t)MLoopCounter;

          if (MLoopReverse) {
            MLoopCounter -= MLoopSpeed;
            if (MLoopCounter < 0.0) {
              MLoopCounter += MLoopLength;
              next_loop();
            }
          }
          else {
            MLoopCounter += MLoopSpeed;
            if (MLoopCounter >= MLoopLength) {
              MLoopCounter -= MLoopLength;
              next_loop();
            }
          }

        } // has loop
        else {
          readpos = MWritePos;
        }


        // write

        //readpos = KODE_Clamp(readpos,0,buffer_length);
        //MWritePos = KODE_Clamp(MWritePos,0,buffer_length);

        MBuffer[(MWritePos*2)  ] = in0;
        MBuffer[(MWritePos*2)+1] = in1;

        out0 = MBuffer[(readpos*2)  ];
        out1 = MBuffer[(readpos*2)+1];

        // env

        // where are we in slice?

        float env = 1.0f;
        float sa = MSliceLength * PSliceEnvAtt;
        float sd = MSliceLength * PSliceEnvDec;
        if (MSliceCounter < sa)                   env *= MSliceCounter / sa;
        if (MSliceCounter >= (MSliceLength - sd)) env *= (MSliceLength - MSliceCounter) / sd;
        if (MHasLoop) {
          float la = MLoopLength * PLoopEnvAtt;
          float ld = MLoopLength * PLoopEnvDec;
          if (MLoopCounter < la)                  env *= MLoopCounter / la;
          if (MLoopCounter >= (MLoopLength - ld)) env *= (MLoopLength - MLoopCounter) / ld;
        }

        out0 *= env;
        out1 *= env;

        //

        MWritePos += 1;
        if (MWritePos >= MBufferLength) {
          //KODE_DPrint("end buffer\n");
          MWritePos     = 0;
          MCurrentSlice = 0;
          MSliceCounter = 0;
          MHasRange     = false;
          MHasLoop      = false;
        }


      } // is_playing

      //----------

      *output_0++ = out0;
      *output_1++ = out1;

    } // for all samples

    //----------

    //if (is_playing) {
    //  MWritePosQueue.write(MWritePos);
    //}

    MReadPos = readpos;
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

  //void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
  //  MSliceEnv.setSampleRate(ASamplerate);
  //  MLoopEnv.setSampleRate(ASamplerate);
  //}

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
    MEditor->set_waveform_buffer_size(MBufferLength);
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
      //uint32_t writepos = MWritePosPrev;
      //while (MWritePosQueue.read(&writepos)) { } // get last message
      bool changed = false;
      changed |= update_editor_waveform_cursors();
      changed |= update_editor_waveform_grid();
      changed |= update_editor_waveform_range();
      changed |= update_editor_waveform_loop();
      changed |= update_editor_waveform_slice();
      //MWritePosPrev = writepos;
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
        MNumSlices = PNumBeats * PBeatSubdiv;
        #ifdef KODE_PLUGIN_EXE
          update_editor_waveform_grid(true);
        #endif
        break;
      case PAR_BEAT_SUBDIV:
        PBeatSubdiv = AValue;
        MNumSlices = PNumBeats * PBeatSubdiv;
        #ifdef KODE_PLUGIN_EXE
          update_editor_waveform_grid(true);
        #endif
        break;
      case PAR_REPEAT_PROB:       PRepeatProb       = AValue * 0.01;  break;
      case PAR_RANGE_MIN_SLICES:  PRangeMinSlices   = AValue;         break;
      case PAR_RANGE_MAX_SLICES:  PRangeMaxSlices   = AValue;         break;
      case PAR_RANGE_MIN_SUBDIV:  PRangeMinSubdiv   = AValue;         break;
      case PAR_RANGE_MAX_SUBDIV:  PRangeMaxSubdiv   = AValue;         break;
      case PAR_LOOP_SIZE_PROB:    PLoopSizeProb     = AValue * 0.01;  break;
      case PAR_LOOP_SIZE_MIN:     PLoopSizeMin      = AValue * 0.01;  break;
      case PAR_LOOP_SIZE_MAX:     PLoopSizeMax      = AValue * 0.01;  break;
      case PAR_LOOP_SPEED_PROB:   PLoopSpeedProb    = AValue * 0.01;  break;
      case PAR_LOOP_SPEED_MIN:    PLoopSpeedMin     = AValue * 0.01;  break;
      case PAR_LOOP_SPEED_MAX:    PLoopSpeedMax     = AValue * 0.01;  break;
      case PAR_LOOP_REVERSE_PROB: PLoopReverseProb  = AValue * 0.01;  break;
      case PAR_SLICE_ENV_ATT:     PSliceEnvAtt      = AValue * 0.01;  break;
      case PAR_SLICE_ENV_DEC:     PSliceEnvDec      = AValue * 0.01;  break;
      case PAR_LOOP_ENV_ATT:      PLoopEnvAtt       = AValue * 0.01;  break;
      case PAR_LOOP_ENV_DEC:      PLoopEnvDec       = AValue * 0.01;  break;

      //case PAR_XFADE_MODE:        PXFadeMode        = AValue;         break;
      //case PAR_XFADE_AMT:         PXFadeAmt         = AValue;         break;
    }
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    process(AContext);
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance);
