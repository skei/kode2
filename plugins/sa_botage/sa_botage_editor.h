#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

#include "kode.h"
#include "plugin/base/kode_base_instance.h"
#include "plugin/kode_editor.h"
#include "gui/kode_widgets.h"

#include "kode_debug_watch_panel.h"

//----------------------------------------------------------------------

class myEditor
: public KODE_Editor {

//------------------------------
//private:
public:
//------------------------------

  KODE_DebugWatchPanel* MWatchPanel = KODE_NULL;
  KODE_WaveformWidget*  MWaveform   = KODE_NULL;

//  uint32_t  qwei = 666;
//  float     qwef = 3.14;
//  void*     qwep = this;

//------------------------------
public:
//------------------------------

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setFillBackground(true);
    setBackgroundColor(0.5f);

    setTitle("SA_BOTAGE v0.0.7");

    KODE_SliderWidget* widget;

    widget = (KODE_SliderWidget*)appendWidget(new KODE_SliderWidget( KODE_FRect(10,10,150,20) ));
    connectParameter(widget,0);

    widget = (KODE_SliderWidget*)appendWidget(new KODE_SliderWidget( KODE_FRect(10,35,150,20) ));
    //widget->setDrawLabel(true);
    connectParameter(widget,1);

    MWaveform = new KODE_WaveformWidget( KODE_FRect(200,100,400,80) );
    MWaveform->setBackgroundColor(KODE_Color(0.45));
    MWaveform->setWaveColor(KODE_Color(0.8));
    MWaveform->setGridColor(KODE_Color(0.35));
    MWaveform->setGridSubColor(KODE_Color(0.2));
    MWaveform->setZeroColor(KODE_Color(0.75));
    MWaveform->setNumMarkers(2);
    MWaveform->setMarker(0, 0.0, KODE_COLOR_BRIGHT_RED);    // writepos
    MWaveform->setMarker(1, 0.0, KODE_COLOR_BRIGHT_GREEN);  // readpos
    MWaveform->setNumAreas(3);
    MWaveform->setArea(0, 0.0, 0.0, KODE_Color(0.0, 0.5, 0.0)); // range
    MWaveform->setArea(1, 0.0, 0.0, KODE_Color(0.0, 0.4, 0.0)); // loop
    MWaveform->setArea(2, 0.0, 0.0, KODE_Color(0.4, 0.4, 0.4)); // slice
    appendWidget(MWaveform);

    MWatchPanel = new KODE_DebugWatchPanel(KODE_FRect(200,200));
    MWatchPanel->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    appendWidget(MWatchPanel);

//    MWatchPanel->appendWatch("qwei",KODE_DEBUG_WATCH_INT,&qwei);
//    MWatchPanel->appendWatch("qwef",KODE_DEBUG_WATCH_FLOAT,&qwef);
//    MWatchPanel->appendWatch("qwep",KODE_DEBUG_WATCH_PTR,&qwep);

    setScale(0.7,true);

  }

  //----------

  //virtual ~myEditor() {
  //}

//------------------------------
public:
//------------------------------

  KODE_DebugWatchPanel* getWatchPanel() {
    return MWatchPanel;
  }

  //----------

  void updateWatches() {
    MWatchPanel->updateWatches();
    MWatchPanel->redraw();
  }

  //----------

};

#endif

//----------------------------------------------------------------------
#endif
