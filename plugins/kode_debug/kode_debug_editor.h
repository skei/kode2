#ifndef kode_debug_editor_included
#define kode_debug_editor_included
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_editor.h"
#include "gui/kode_widgets.h"

#include "kode_debug_gui_panel.h"
#include "kode_debug_process_panel.h"
#include "kode_debug_host_panel.h"

//----------------------------------------------------------------------

class myEditor
: public KODE_Editor {

//------------------------------
public:
//private:
//------------------------------

  char      in0_txt[32]       = {0};
  char      in1_txt[32]       = {0};
  char      out0_txt[32]      = {0};
  char      out1_txt[32]      = {0};
  pid_t     host_pid          = 0;
  pid_t     host_tid          = 0;
  pid_t     proc_pid          = 0;
  pid_t     proc_tid          = 0;
  pid_t     gui_pid           = 0;
  pid_t     gui_tid           = 0;
  pid_t     xcb_pid           = 0;
  pid_t     xcb_tid           = 0;

  uint32_t  ctx_mode          = 0;
  uint32_t  ctx_offset        = 0;
  uint32_t  ctx_numsamples    = 0;
  uint32_t  ctx_numinputs     = 0;
  uint32_t  ctx_numoutputs    = 0;
  uintptr_t ctx_in0           = 0;
  uintptr_t ctx_in1           = 0;
  uintptr_t ctx_out0          = 0;
  uintptr_t ctx_out1          = 0;
  float     ctx_samplerate    = 0.0;
  float     ctx_tempo         = 0.0;
  uint32_t  ctx_timesignum    = 0;
  uint32_t  ctx_timesigdenom  = 0;
  uint32_t  ctx_samplepos     = 0;
  float     ctx_beatpos       = 0.0;
  uint32_t  ctx_playstate     = KODE_PLUGIN_PLAYSTATE_NONE;

  //

  double                    _prev_time        = KODE_GetTimeMS();
  char                      hostname_txt[64]  = {0};
  kode_debug_gui_panel*     wdgGuiPanel       = KODE_NULL;
  kode_debug_process_panel* wdgProcessPanel   = KODE_NULL;
  kode_debug_host_panel*    wdgHostPanel      = KODE_NULL;
  KODE_BaseInstance*        MInstance         = KODE_NULL;

//------------------------------
public:
//------------------------------

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    MInstance = AInstance;

    setFillBackground(true);
    setBackgroundColor(0.5f);

    // GUI

    KODE_TextWidget* txt;

    txt = new KODE_TextWidget( KODE_FRect(10,10,200,20) );
    txt->setText("GUI");
    txt->setTextColor(0.8);
    txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
    txt->setFillBackground(true);
    txt->setBackgroundColor(0.3);
    appendWidget(txt);

    wdgGuiPanel = new kode_debug_gui_panel( KODE_FRect(10,40,200,400), this );
    appendWidget(wdgGuiPanel);

    // process

    txt = new KODE_TextWidget( KODE_FRect(220,10,200,20) );
    txt->setText("process()");
    txt->setTextColor(0.8);
    txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
    txt->setFillBackground(true);
    txt->setBackgroundColor(0.3);
    appendWidget(txt);

    wdgProcessPanel = new kode_debug_process_panel( KODE_FRect(220,40,200,400) );
    appendWidget(wdgProcessPanel);

    // host

    txt = new KODE_TextWidget( KODE_FRect(430,10,200,20) );
    txt->setText("Host");
    txt->setTextColor(0.8);
    txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
    txt->setFillBackground(true);
    txt->setBackgroundColor(0.3);
    appendWidget(txt);

    wdgHostPanel = new kode_debug_host_panel( KODE_FRect(430,40,200,400) );
    appendWidget(wdgHostPanel);

  }

  //----------

  //virtual ~myEditor() {
  //}

  //----------

  void param(uint32_t AIndex, float AValue) {
  }

  //----------


  void midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
  }

  //----------

  // called from process()
  // audio thread

  void process(KODE_ProcessContext* AContext) {
    proc_pid = getpid();
    proc_tid = gettid();
    if (AContext) {
      ctx_mode          = AContext->mode;
      ctx_offset        = AContext->offset;
      ctx_numsamples    = AContext->numsamples;
      ctx_numinputs     = AContext->numinputs;
      ctx_numoutputs    = AContext->numoutputs;
      if (AContext->inputs) {
        ctx_in0         = (intptr_t)AContext->inputs[0];
        ctx_in1         = (intptr_t)AContext->inputs[1];
      }
      if (AContext->outputs) {
        ctx_out0        = (intptr_t)AContext->outputs[0];
        ctx_out1        = (intptr_t)AContext->outputs[1];
      }
      ctx_samplerate    = AContext->samplerate;
      ctx_tempo         = AContext->tempo;
      ctx_timesignum    = AContext->timesignum;
      ctx_timesigdenom  = AContext->timesigdenom;
      ctx_samplepos     = AContext->samplepos;
      ctx_beatpos       = AContext->beatpos;
      ctx_playstate     = AContext->playstate;
    }
  }

  //----------

  // called from updateEditor()
  // gui thread

  void update() {

    if (wdgGuiPanel) {
      double current_time = KODE_GetTimeMS();
      double delta = current_time - _prev_time;
      _prev_time = current_time;
      wdgGuiPanel->wdgGuiDelta->setValue(delta);
      wdgGuiPanel->wdgPid->setValue(gui_pid);
      wdgGuiPanel->wdgTid->setValue(gui_tid);
      wdgGuiPanel->wdgXcbPid->setValue(xcb_pid);
      wdgGuiPanel->wdgXcbTid->setValue(xcb_tid);
      wdgGuiPanel->redraw();
    }

    if (wdgProcessPanel) {
      wdgProcessPanel->wdgMode->setValue(ctx_mode);
      wdgProcessPanel->wdgOffset->setValue(ctx_offset);
      wdgProcessPanel->wdgNumSamples->setValue(ctx_numsamples);
      wdgProcessPanel->wdgNumInputs->setValue(ctx_numinputs);
      wdgProcessPanel->wdgNumOutputs->setValue(ctx_numoutputs);
      sprintf(in0_txt, "in0:  %016x",(int)ctx_in0);
      sprintf(in1_txt, "in1:  %016x",(int)ctx_in1);
      sprintf(out0_txt,"out0: %016x",(int)ctx_out0);
      sprintf(out1_txt,"out1: %016x",(int)ctx_out1);
      wdgProcessPanel->wdgIn0->setText(in0_txt);
      wdgProcessPanel->wdgIn1->setText(in1_txt);
      wdgProcessPanel->wdgOut0->setText(out0_txt);
      wdgProcessPanel->wdgOut1->setText(out1_txt);
      wdgProcessPanel->wdgSampleRate->setValue(ctx_samplerate);
      wdgProcessPanel->wdgTempo->setValue(ctx_tempo);
      wdgProcessPanel->wdgTimeSigNum->setValue(ctx_timesignum);
      wdgProcessPanel->wdgTimeSigDenom->setValue(ctx_timesigdenom);
      wdgProcessPanel->wdgSamplePos->setValue(ctx_samplepos);
      wdgProcessPanel->wdgBeatPos->setValue(ctx_beatpos);
      wdgProcessPanel->wdgPlayState->setValue(ctx_playstate);
      wdgProcessPanel->wdgPid->setValue(proc_pid);
      wdgProcessPanel->wdgTid->setValue(proc_tid);
      wdgProcessPanel->redraw();
    }

    if (wdgHostPanel) {
      wdgHostPanel->wdgPid->setValue(host_pid);
      wdgHostPanel->wdgTid->setValue(host_tid);
      sprintf(hostname_txt,"host name: \"%s\"",MInstance->getHostName());
      wdgHostPanel->wdgHostName->setText(hostname_txt);
      wdgHostPanel->redraw();
    }

  }

};

#endif // KODE_NO_GUI

//----------------------------------------------------------------------
#endif
