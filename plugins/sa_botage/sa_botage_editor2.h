#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_editor.h"
#include "gui/kode_widgets.h"

//----------

#include "sa_botage_header.h"

const char* txt_1to8[8] = {
  "1", "2", "3", "4", "5", "6", "7", "8"
};

//----------------------------------------------------------------------

class sa_botage_editor
: public KODE_Editor {

//------------------------------
private:
//------------------------------

  KODE_WaveformWidget*  MWaveformWidget         = KODE_NULL;
  KODE_ButtonRowWidget* MSlicesButtonRowWidget  = KODE_NULL;
  KODE_ButtonRowWidget* MSplitButtonRowWidget   = KODE_NULL;
  uint32_t              MWaveformBufferSize     = 0;

//------------------------------
public:
//------------------------------

  sa_botage_editor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setFillBackground(true);
    setBackgroundColor(0.5f);

    KODE_Widget*          widget;
    KODE_KnobWidget*      knob;
    KODE_TextWidget*      txt;
    KODE_ButtonWidget*    button;
    KODE_ImageWidget*     image;
    KODE_SliderWidget*    slider;

    // header

    image = new KODE_ImageWidget(KODE_FRect(10,10,565,60));
    image->setDrawBorder(true);
    image->setFillBackground(false);
    image->setBackgroundColor(0.5);
    image->setImage(this,(uint8_t*)sa_botage_header,sa_botage_header_size,KODE_Color(0.5));
    appendWidget(image);

//    // footer
//
//    WFooter = new KODE_TextWidget(KODE_FRect(0,0,20,20),"footer...");
//    WFooter->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
//    WFooter->setTextColor(KODE_Color(0.9));
//    WFooter->setDrawBorder(true);
//    WFooter->setBorderEdges(KODE_EDGE_TOP);
//    appendWidget(WFooter);

    // buffer

    slider = new KODE_SliderWidget(KODE_FRect(10,80,100,20));
    slider->setText("Beats");
    widget = appendWidget(slider);
    connectParameter(widget,PAR_NUM_BEATS);

    slider = new KODE_SliderWidget(KODE_FRect(10,110,100,20));
    slider->setText("Slices");
    widget = appendWidget(slider);
    connectParameter(widget,PAR_BEAT_SUBDIV);

    // waveform

    MWaveformWidget = new KODE_WaveformWidget(KODE_FRect(120,80,455,80));
    MWaveformWidget->setBackgroundColor(KODE_Color(0.4));
    MWaveformWidget->setNumMarkers(2);
    MWaveformWidget->setMarkerColor(0,KODE_COLOR_BRIGHT_RED);
    MWaveformWidget->setMarkerColor(1,KODE_COLOR_BRIGHT_GREEN);
    MWaveformWidget->setNumAreas(3);
    MWaveformWidget->setAreaColor(0,KODE_COLOR_GREEN);
    MWaveformWidget->setAreaColor(1,KODE_COLOR_DARK_GREEN);
    MWaveformWidget->setAreaColor(2,KODE_Color(0.37));
    appendWidget(MWaveformWidget);

    // ----- LOOP  -----

    KODE_GroupBoxWidget* loop_group = new KODE_GroupBoxWidget(KODE_FRect(10,170,280,120));
    loop_group->getTitleBar()->setText("LOOP","LOOP");
    loop_group->getTitleBar()->setDrawTriangle(false);
    loop_group->getTitleBar()->setCursor(KODE_CURSOR_DEFAULT);
    loop_group->getContainer()->setFillBackground(true);
    loop_group->getContainer()->setBackgroundColor(0.45);
    loop_group->setClosable(false);
    appendWidget(loop_group);

      knob = new KODE_KnobWidget(KODE_FRect(10,10,90,90));
      widget = loop_group->appendWidget(knob);
      connectParameter(widget,PAR_REPEAT_PROB);

      txt = new KODE_TextWidget(KODE_FRect(110,10,160,20),"Slices");
      loop_group->appendWidget(txt);

      MSlicesButtonRowWidget = new KODE_ButtonRowWidget(KODE_FRect(110,30,160,20),8,txt_1to8,KODE_BUTTON_ROW_MULTI);
      MSlicesButtonRowWidget->setValueIsBits(true,8);
      widget = loop_group->appendWidget(MSlicesButtonRowWidget);
      connectParameter(widget,PAR_RANGE_SLICE_BITS);

      txt = new KODE_TextWidget(KODE_FRect(110,50,160,20),"Split");
      loop_group->appendWidget(txt);

      MSplitButtonRowWidget = new KODE_ButtonRowWidget(KODE_FRect(110,70,160,20),8,txt_1to8,KODE_BUTTON_ROW_MULTI);
      MSplitButtonRowWidget->setValueIsBits(true,8);
      widget = loop_group->appendWidget(MSplitButtonRowWidget);
      connectParameter(widget,PAR_RANGE_SPLIT_BITS);

    // ----- ENV  -----

    KODE_GroupBoxWidget* env_group = new KODE_GroupBoxWidget(KODE_FRect(300,170,170,120));
    env_group->getTitleBar()->setText("ENV","ENV");
    env_group->getTitleBar()->setDrawTriangle(false);
    env_group->getTitleBar()->setCursor(KODE_CURSOR_DEFAULT);
    env_group->getContainer()->setFillBackground(true);
    env_group->getContainer()->setBackgroundColor(0.45);
    env_group->setClosable(false);
    appendWidget(env_group);

      txt = new KODE_TextWidget(KODE_FRect(10,5,70,15),"Slice");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      txt->setDrawBorder(true);
      txt->setBorderEdges(KODE_EDGE_BOTTOM);
      env_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(10,30,30,30));
      widget = env_group->appendWidget(knob);
      connectParameter(widget,PAR_SLICE_ENV_ATT);

      txt = new KODE_TextWidget(KODE_FRect(10,55,30,20),"Att");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      env_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(50,30,30,30));
      widget = env_group->appendWidget(knob);
      connectParameter(widget,PAR_SLICE_ENV_DEC);

      txt = new KODE_TextWidget(KODE_FRect(50,55,30,20),"Dec");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      env_group->appendWidget(txt);

      txt = new KODE_TextWidget(KODE_FRect(90,5,70,15),"Loop");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      txt->setDrawBorder(true);
      txt->setBorderEdges(KODE_EDGE_BOTTOM);
      env_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(90,30,30,30));
      widget = env_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_ENV_ATT);

      txt = new KODE_TextWidget(KODE_FRect(90,55,30,20),"Att");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      env_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(130,30,30,30));
      widget = env_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_ENV_DEC);

      txt = new KODE_TextWidget(KODE_FRect(130,55,30,20),"Dec");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      env_group->appendWidget(txt);

    // ----- CYCLE  -----

    KODE_GroupBoxWidget* cycle_group = new KODE_GroupBoxWidget(KODE_FRect(10,300,245,175));
    cycle_group->getTitleBar()->setText("CYCLE","CYCLE");
    cycle_group->getTitleBar()->setDrawTriangle(false);
    cycle_group->getTitleBar()->setCursor(KODE_CURSOR_DEFAULT);
    cycle_group->getContainer()->setFillBackground(true);
    cycle_group->getContainer()->setBackgroundColor(0.45);
    cycle_group->setClosable(false);
    appendWidget(cycle_group);

      // size

      txt = new KODE_TextWidget(KODE_FRect(10,5,65,20),"Size");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      txt->setDrawBorder(true);
      txt->setBorderEdges(KODE_EDGE_BOTTOM);
      cycle_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(10,35,65,65));
      widget = cycle_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_SIZE_PROB);

      knob = new KODE_KnobWidget(KODE_FRect(10,105,30,30));
      knob->setValue(0.5);
      knob->setBipolar(true);
      widget = cycle_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_SIZE_MIN);

      txt = new KODE_TextWidget(KODE_FRect(10,130,30,20),"Min");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      cycle_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(45,105,30,30));
      knob->setValue(0.5);
      knob->setBipolar(true);
      widget = cycle_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_SIZE_MAX);

      txt = new KODE_TextWidget(KODE_FRect(45,130,30,20),"Max");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      cycle_group->appendWidget(txt);

      // speed

      txt = new KODE_TextWidget(KODE_FRect(90,5,65,20),"Speed");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      txt->setDrawBorder(true);
      txt->setBorderEdges(KODE_EDGE_BOTTOM);
      cycle_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(90,35,65,65));
      widget = cycle_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_SPEED_PROB);

      knob = new KODE_KnobWidget(KODE_FRect(90,105,30,30));
      knob->setValue(0.5);
      knob->setBipolar(true);
      widget = cycle_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_SPEED_MIN);

      txt = new KODE_TextWidget(KODE_FRect(90,130,30,20),"Min");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      cycle_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(125,105,30,30));
      knob->setValue(0.5);
      knob->setBipolar(true);
      widget = cycle_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_SPEED_MAX);

      txt = new KODE_TextWidget(KODE_FRect(125,130,30,20),"Max");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      cycle_group->appendWidget(txt);

      // reverse

      txt = new KODE_TextWidget(KODE_FRect(170,5,65,20),"Reverse");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      txt->setDrawBorder(true);
      txt->setBorderEdges(KODE_EDGE_BOTTOM);
      cycle_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(170,35,65,65));
      widget = cycle_group->appendWidget(knob);
      connectParameter(widget,PAR_LOOP_REVERSE_PROB);

    // ----- FILTER  -----

    KODE_GroupBoxWidget* filter_group = new KODE_GroupBoxWidget(KODE_FRect(265,300,260,175));
    filter_group->getTitleBar()->setText("FILTER","FILTER");
    filter_group->getTitleBar()->setDrawTriangle(false);
    filter_group->getTitleBar()->setCursor(KODE_CURSOR_DEFAULT);
    filter_group->getContainer()->setFillBackground(true);
    filter_group->getContainer()->setBackgroundColor(0.45);
    filter_group->setClosable(false);
    appendWidget(filter_group);

      knob = new KODE_KnobWidget(KODE_FRect(10,10,65,65));
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_PROB);

      knob = new KODE_KnobWidget(KODE_FRect(10,80,30,30));
      knob->setValue(0.5);
      knob->setBipolar(true);
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_FREQ_MIN);

      txt = new KODE_TextWidget(KODE_FRect(10,105,30,20),"Min");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      filter_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(45,80,30,30));
      knob->setValue(0.5);
      knob->setBipolar(true);
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_FREQ_MAX);

      txt = new KODE_TextWidget(KODE_FRect(45,105,30,20),"Max");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      filter_group->appendWidget(txt);

      //

      button = new KODE_ButtonWidget(KODE_FRect(90,10,45,45));
      button->setText("LP","HP");
      button->setDrawTriangle(false);
      button->setIsToggle(true);
      widget = filter_group->appendWidget(button);
      connectParameter(widget,PAR_FX_FILTER_TYPE);

      //

      knob = new KODE_KnobWidget(KODE_FRect(145,10,50,50));
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_FREQ);

      txt = new KODE_TextWidget(KODE_FRect(145,55,50,20),"Freq");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      filter_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(200,10,50,50));
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_RES);

      txt = new KODE_TextWidget(KODE_FRect(200,55,50,20),"Res");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      filter_group->appendWidget(txt);

      //

      knob = new KODE_KnobWidget(KODE_FRect(145,80,30,30));
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_CLIP);

      txt = new KODE_TextWidget(KODE_FRect(145,110,30,20),"Clip");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      filter_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(180,80,30,30));
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_PRE);

      txt = new KODE_TextWidget(KODE_FRect(180,110,30,20),"Pre");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      filter_group->appendWidget(txt);

      knob = new KODE_KnobWidget(KODE_FRect(215,80,30,30));
      widget = filter_group->appendWidget(knob);
      connectParameter(widget,PAR_FX_FILTER_POST);

      txt = new KODE_TextWidget(KODE_FRect(215,110,30,20),"Post");
      txt->setTextAlignment(KODE_TEXT_ALIGN_CENTER);
      filter_group->appendWidget(txt);

      // filter_group

  }

  //----------

  //virtual ~sa_botage_editor() {
  //}

//------------------------------
public:
//------------------------------

  int32_t get_waveform_grid() {
    return MWaveformWidget->getNumGrid();
  }

  void set_waveform_buffer(float* ABuffer) {
    MWaveformWidget->setBuffer(ABuffer);
  }

  //----------

  void set_waveform_buffer_size(uint32_t ABufferSize) {
    uint32_t size = ABufferSize;// / 2;
    MWaveformWidget->setBufferSize(size);
    MWaveformBufferSize = size;
    //MWaveformBufferSizeInv = 1.0 / (float)size;
  }

  //----------

  void set_waveform_grid(uint32_t ASlices, uint32_t AMajor) {
    MWaveformWidget->setNumGrid(ASlices);
    MWaveformWidget->setNumGridMajor(AMajor);
  }

  //----------

  // MInvBufferSize

  void set_waveform_write_pos(float AWritePos) {
    float writepos = (float)AWritePos;// * MWaveformBufferSizeInv;
    MWaveformWidget->setMarkerPos(0,writepos);
  }

  //----------

  // MInvBufferSize

  void set_waveform_read_pos(float AReadPos) {
    float readpos = (float)AReadPos;// * MWaveformBufferSizeInv;
    MWaveformWidget->setMarkerPos(1,readpos);
  }

  //----------

  void set_waveform_range(uint32_t AStartSlice, uint32_t ANumSlices, uint32_t AMaxSlices) {
    float max_slices = (float)AMaxSlices;
    float pos  = (float)AStartSlice / max_slices;
    float size = (float)ANumSlices / max_slices;
    MWaveformWidget->setAreaPosSize(0,pos,size);
  }

  //----------

  //void set_waveform_loop(uint32_t AStartSlice, uint32_t ANumSlices, uint32_t AMaxSlices, uint32_t ASubdiv, float AScale) {
  //  float max_slices = (float)AMaxSlices;
  //  float pos  = (float)AStartSlice / max_slices;
  //  float size = (float)ANumSlices / max_slices;
  //  if (ASubdiv > 0) size /= (float)ASubdiv;
  //  WWaveform->setAreaPosSize(1,pos,size*AScale);
  //}

  void set_waveform_loop(float AStart, float ALength) {
    MWaveformWidget->setAreaPosSize(1,AStart,ALength);
  }

  //----------

  void set_waveform_slice(uint32_t ASlice, uint32_t ANumSlices) {
    float pos  = (float)ASlice / (float)ANumSlices;
    float size = 1.0 / (float)ANumSlices;
    MWaveformWidget->setAreaPosSize(2,pos,size);
  }

  //----------

  void redraw_waveform() {
    MWaveformWidget->redraw();
  }


//------------------------------
public:
//------------------------------

// SlicesButtonRow : getButtonBits
// DivideButtonRow

// hmm, editor override version isn't called..

//  void do_widget_update(KODE_Widget* AWidget) override {
//    KODE_PRINT;
//    if (AWidget == MSlicesButtonRowWidget) {
//      uint32_t bits = MSlicesButtonRowWidget->getButtonBits();
//      KODE_Print("bits %08b\n",bits);
//    }
//  }


};

//----------------------------------------------------------------------
#endif

