#ifndef sa_botage_instance_included
#define sa_botage_instance_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/base/kode_base_editor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_process_context.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  myEditor*   MEditor = KODE_NULL;
  myProcess   MProcess;

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
public:
//------------------------------

  void update_waveform() {
    if (MEditor) {
      KODE_WaveformWidget* waveform = MEditor->getWaveformWidget();//wdg_Waveform;
      if (waveform) {
        waveform->setBuffer(MProcess.MBuffer);
        waveform->setBufferSize(MProcess.MBufferLength);
        waveform->setNumGrid(MProcess.par_BufferNumBeats * MProcess.par_BufferNumSlices);
        waveform->setNumGridSub(MProcess.par_BufferNumSlices);
        float bufferlength = MProcess.MBufferLength;
        if (bufferlength > 0.0) {
          float invbufferlength = 1.0 / bufferlength;

          float writepos = MProcess.MBufferPos * invbufferlength;
          waveform->setMarkerPos(0,writepos);

          float readpos = MProcess.MReadPos * invbufferlength;
          waveform->setMarkerPos(1,readpos);

          if (MProcess.MRangeActive) {
            float rangestart  = (MProcess.MRangeStart) * invbufferlength;
            float rangelength = (MProcess.MRangeNumSlices * MProcess.MSliceLength) * invbufferlength;;
            waveform->setAreaVisible(0,true);
            waveform->setAreaPosSize(0,rangestart,rangelength);
            if (MProcess.MLoopActive) {
              float loopstart   = (MProcess.MLoopStart) * invbufferlength;
              float looplength  = MProcess.MLoopLength * invbufferlength;
              waveform->setAreaVisible(1,true);
              waveform->setAreaPosSize(1,loopstart,looplength);
            }
            waveform->setAreaVisible(2,false);
          }
          else {
            waveform->setAreaVisible(0,false);
            waveform->setAreaVisible(1,false);
            float slicestart = (MProcess.MSliceCounter * MProcess.MSliceLength) * invbufferlength;
            float slicelength = MProcess.MSliceLength * invbufferlength;
            waveform->setAreaVisible(2,true);
            waveform->setAreaPosSize(2,slicestart,slicelength);
          }

        } // buffer length > 0
        waveform->redraw();
      } // waveform
    } // editor
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

  //----------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    //KODE_PRINT;
//    MProcess.par_RepeatProb       = 0.5;
//    MProcess.par_RepeatMinSlices  = 1;
//    MProcess.par_RepeatMaxSlices  = 4;
//    MProcess.par_RepeatMinDivide  = 1;
//    MProcess.par_RepeatMaxDivide  = 4;
    MProcess.start();
  }

  //----------

  //uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {}
  //void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {}
  //void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {}

  //----------

  #ifdef KODE_PLUGIN_EXE
    #define UPDATE update_waveform();
  #else
    #define UPDATE
  #endif

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("ofs %i idx %i val %.3f mode %i\n",AOffset,AIndex,AValue,AMode);
    switch (AIndex) {

      case P_BUFFER_NUM_BEATS:      MProcess.par_BufferNumBeats     = AValue; UPDATE; break;
      case P_BUFFER_NUM_SLICES:     MProcess.par_BufferNumSlices    = AValue; UPDATE; break;
      case P_REPEAT_PROB:           MProcess.par_RepeatProb         = AValue; break;
      case P_REPEAT_SLICE_BITS:     MProcess.par_RepeatSliceBits    = AValue; break;
      case P_REPEAT_SPLIT_BITS:     MProcess.par_RepeatSplitBits    = AValue; break;

      case P_LOOPSIZE_RANGE_PROB:   MProcess.par_LoopsizeRangeProb  = AValue; break;
      case P_LOOPSIZE_RANGE_MIN:    MProcess.par_LoopsizeRangeMin   = AValue * 0.01; break;
      case P_LOOPSIZE_RANGE_MAX:    MProcess.par_LoopsizeRangeMax   = AValue * 0.01; break;
      case P_LOOPSIZE_LOOP_PROB:    MProcess.par_LoopsizeLoopProb   = AValue; break;
      case P_LOOPSIZE_LOOP_MIN:     MProcess.par_LoopsizeLoopMin    = AValue * 0.01; break;
      case P_LOOPSIZE_LOOP_MAX:     MProcess.par_LoopsizeLoopMax    = AValue * 0.01; break;

      case P_LOOPSPEED_RANGE_PROB:  MProcess.par_LoopspeedRangeProb = AValue; break;
      case P_LOOPSPEED_RANGE_MIN:   MProcess.par_LoopspeedRangeMin  = AValue * 0.01; break;
      case P_LOOPSPEED_RANGE_MAX:   MProcess.par_LoopspeedRangeMax  = AValue * 0.01; break;
      case P_LOOPSPEED_LOOP_PROB:   MProcess.par_LoopspeedLoopProb  = AValue; break;
      case P_LOOPSPEED_LOOP_MIN:    MProcess.par_LoopspeedLoopMin   = AValue * 0.01; break;
      case P_LOOPSPEED_LOOP_MAX:    MProcess.par_LoopspeedLoopMax   = AValue * 0.01; break;

      case P_REVERSE_RANGE_PROB:    MProcess.par_ReverseRangeProb   = AValue; break;
      case P_REVERSE_LOOP_PROB:     MProcess.par_ReverseLoopProb    = AValue; break;

      case P_FX_MULTI:              MProcess.par_FXMulti            = AValue; break;
      case P_FX_RANGE_PROB:         MProcess.par_FXRangeProb        = AValue; break;
      case P_FX_RANGE_MIN:          MProcess.par_FXRangeMin         = AValue * 0.01; break;
      case P_FX_RANGE_MAX:          MProcess.par_FXRangeMax         = AValue * 0.01; break;
      case P_FX_LOOP_PROB:          MProcess.par_FXLoopProb         = AValue; break;
      case P_FX_LOOP_MIN:           MProcess.par_FXLoopMin          = AValue * 0.01; break;
      case P_FX_LOOP_MAX:           MProcess.par_FXLoopMax          = AValue * 0.01; break;

//      case P_ENV_LOOP_ATT:          MProcess.par_EnvLoopAtt         = AValue; break;
//      case P_ENV_LOOP_DEC:          MProcess.par_EnvLoopDec         = AValue; break;
//      case P_ENV_SLICE_ATT:         MProcess.par_EnvSliceAtt        = AValue * 0.01; break;
//      case P_ENV_SLICE_DEC:         MProcess.par_EnvSliceDec        = AValue * 0.01; break;

      case P_ENV_LOOP_ATT:          MProcess.par_EnvLoopAtt         = AValue * 0.01; break;
      case P_ENV_LOOP_DEC:          MProcess.par_EnvLoopDec         = AValue * 0.01; break;
      case P_ENV_SLICE_ATT:         MProcess.par_EnvSliceAtt        = AValue * 0.01; break;
      case P_ENV_SLICE_DEC:         MProcess.par_EnvSliceDec        = AValue * 0.01; break;


    }
  }

  #undef UPDATE

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    MProcess.process(AContext);
  }

//------------------------------
//
//------------------------------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    MEditor = new myEditor(this,AParent);
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
      update_waveform();
    } // editor
  }

  #endif

//------------------------------
private:
//------------------------------

  //void queueProcessMessage(uint32_t AMessage) {
  //  MProcessMessageQueue.write(AMessage);
  //}

  //----------

  //void queueGuiMessage(uint32_t AMessage) {
  //  MGuiMessageQueue.write(AMessage);
  //}

  //----------

  //void flushProcessMessages() {
  //  uint32_t message = 0;
  //  while (MProcessMessageQueue.read(&message)) {
  //    //KODE_Print("%i\n",message);
  //  }
  //}

  //----------

  //void flushGuiMessages() {
  //  uint32_t message = 0;
  //  while (MGuiMessageQueue.read(&message)) {
  //    //KODE_Print("%i\n",message);
  //  }
  //}

  //----------

  //void recalc(KODE_ProcessContext* AContext) {
  //  //KODE_PRINT;
  //}

  //----------

  //void processSample(KODE_ProcessContext* AContext, float spl0, float spl1) {
  //}

};

//----------------------------------------------------------------------
#endif
