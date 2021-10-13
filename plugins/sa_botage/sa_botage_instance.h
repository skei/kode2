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
    KODE_WaveformWidget* waveform = MEditor->MWaveform;
    if (waveform) {
      waveform->setBuffer(MProcess.MBuffer);
      waveform->setBufferSize(MProcess.MBufferLength);
      waveform->setNumGrid(MProcess.MBufferNumBeats * MProcess.MBufferNumSlices);
      waveform->setNumGridSub(MProcess.MBufferNumSlices);
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
    MProcess.MRepeatProb          = 0.5;
    MProcess.MRepeatProbMinSlices = 1;
    MProcess.MRepeatProbMaxSlices = 4;
    MProcess.MRepeatProbMinDivide = 1;
    MProcess.MRepeatProbMaxDivide = 4;
    MProcess.start();
  }

  //----------

  //uint32_t on_plugin_saveState(void** ABuffer, uint32_t AMode) final {}
  //void on_plugin_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {}
  //void on_plugin_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3, uint32_t AMode=0) final {}

  //----------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    //KODE_Print("ofs %i idx %i val %.3f mode %i\n",AOffset,AIndex,AValue,AMode);
    switch (AIndex) {
      case 0: MProcess.MBufferNumBeats   = AValue; /*MProcess.MBufferChanged = true;*/ break;
      case 1: MProcess.MBufferNumSlices  = AValue; /*MProcess.MBufferChanged = true;*/ break;
    }
  }

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
    KODE_DebugWatchPanel* watchpanel = MEditor->getWatchPanel();
    if (watchpanel) {
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_BOOL,  "MIsPlaying",           &MProcess.MIsPlaying          );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MBeatsPerSecond",      &MProcess.MBeatsPerSecond     );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MBeatsPerSample",      &MProcess.MBeatsPerSample     );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MSamplesPerBeat",      &MProcess.MSamplesPerBeat     );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MSamplesPerSlice",     &MProcess.MSamplesPerSlice    );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_INT,   "MBufferNumBeats",      &MProcess.MBufferNumBeats     );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_INT,   "MBufferNumSlices",     &MProcess.MBufferNumSlices    );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MBufferPos",           &MProcess.MBufferPos          );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MBufferLength",        &MProcess.MBufferLength       );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MBeatPos",             &MProcess.MBeatPos            );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MBeatSpeed",           &MProcess.MBeatSpeed          );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_INT,   "MSliceCounter",        &MProcess.MSliceCounter       );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MSlicePos",            &MProcess.MSlicePos           );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MSliceLength",         &MProcess.MSliceLength        );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MSliceSpeed",          &MProcess.MSliceSpeed         );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_BOOL,  "MRangeActive",         &MProcess.MRangeActive        );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_INT,   "MRangeNumSlices",      &MProcess.MRangeNumSlices     );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MRangePos",            &MProcess.MRangePos           );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MRangeLength",         &MProcess.MRangeLength        );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      //watchpanel->appendWatch(  KODE_DEBUG_WATCH_SEPARATOR );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_BOOL,  "MLoopActive",          &MProcess.MLoopActive         );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_INT,   "MLoopNumDivide",       &MProcess.MLoopNumDivide      );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_INT,   "MLoopCounter",         &MProcess.MLoopCounter        );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MLoopPos",             &MProcess.MLoopPos            );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MLoopStart",           &MProcess.MLoopStart          );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MLoopLength",          &MProcess.MLoopLength         );
      watchpanel->appendWatch(  KODE_DEBUG_WATCH_FLOAT, "MLoopSpeed",           &MProcess.MLoopSpeed          );
    }
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
    //KODE_Print("\n");
    if (MEditor) {

      MEditor->updateWatches();
      update_waveform();

//      KODE_WaveformWidget* waveform = MEditor->MWaveform;
//      if (waveform) {
//        waveform->setBuffer(MProcess.MBuffer);
//        waveform->setBufferSize(MProcess.MBufferLength);
//        waveform->setNumGrid(MProcess.MBufferNumBeats * MProcess.MBufferNumSlices);
//        waveform->setNumGridSub(MProcess.MBufferNumSlices);
//
//        float bufferlength = MProcess.MBufferLength;
//
//        if (bufferlength > 0.0) {
//          float invbufferlength = 1.0 / bufferlength;
//
//          float readpos = 0.0;//MProcess.MBufferPos / MProcess.MBufferLength;
//          waveform->setMarkerPos(0,readpos);
//
//          float writepos = MProcess.MBufferPos * invbufferlength;
//          waveform->setMarkerPos(1,writepos);
//
//          if (MProcess.MRangeActive) {
//            float rangestart  = (MProcess.MRangeStart) * invbufferlength;
//            float rangelength = (MProcess.MRangeNumSlices * MProcess.MSliceLength) * invbufferlength;;
//            waveform->setAreaVisible(0,true);
//            waveform->setAreaPosSize(0,rangestart,rangelength);
//            if (MProcess.MLoopActive) {
//              float loopstart   = (MProcess.MLoopStart) * invbufferlength;
//              float looplength  = MProcess.MLoopLength * invbufferlength;
//              waveform->setAreaVisible(1,true);
//              waveform->setAreaPosSize(1,loopstart,looplength);
//            }
//            waveform->setAreaVisible(2,false);
//          }
//          else {
//            float slicestart = (MProcess.MSliceCounter * MProcess.MSliceLength) * invbufferlength;
//            float slicelength = MProcess.MSliceLength * invbufferlength;
//            waveform->setAreaVisible(0,false);
//            waveform->setAreaVisible(1,false);
//            waveform->setAreaVisible(2,true);
//            waveform->setAreaPosSize(2,slicestart,slicelength);
//          }
//
//        } // buffer length > 0
//
//        waveform->redraw();
//      } // waveform

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
