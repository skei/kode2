#ifndef sa_botage_descriptor_included
#define sa_botage_descriptor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"

//----------------------------------------------------------------------

enum sa_botage_params {

  PAR_NUM_BEATS,
  PAR_BEAT_SUBDIV,
  PAR_REPEAT_PROB,

//  PAR_RANGE_MIN_SLICES,
//  PAR_RANGE_MAX_SLICES,
//  PAR_RANGE_MIN_SUBDIV,
// PAR_RANGE_MAX_SUBDIV,

  PAR_RANGE_SLICE_BITS,
  PAR_RANGE_SPLIT_BITS,

  PAR_LOOP_SIZE_PROB,
  PAR_LOOP_SIZE_MIN,
  PAR_LOOP_SIZE_MAX,
  PAR_LOOP_SPEED_PROB,
  PAR_LOOP_SPEED_MIN,
  PAR_LOOP_SPEED_MAX,
  PAR_LOOP_REVERSE_PROB,
  PAR_SLICE_ENV_ATT,
  PAR_SLICE_ENV_DEC,
  PAR_LOOP_ENV_ATT,
  PAR_LOOP_ENV_DEC,

  PAR_FX_FILTER_PROB,
  PAR_FX_FILTER_TYPE,
  PAR_FX_FILTER_FREQ,
  PAR_FX_FILTER_RES,
  PAR_FX_FILTER_CLIP,
  PAR_FX_FILTER_PRE,
  PAR_FX_FILTER_POST,
  PAR_FX_FILTER_FREQ_MIN,
  PAR_FX_FILTER_FREQ_MAX,

};


//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

  const char* filter_type_txt[2] = {
    "Lowpass",
    "Highpass"
  };

//------------------------------
public:
//------------------------------

  myDescriptor() {

    #ifdef KODE_DEBUG
      setName("sa_botage_debug");
    #else
      setName("sa_botage");
    #endif

    setAuthor("skei.audio");
    setVersion(0x00000005);
    setEmail("tor.helge.skei@gmail.com");
    setUrl("https://torhelgeskei.com");
    //setIsSynth(true);
    //setCanReceiveMidi(true);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    // same order as sa_botage_params

    appendParameter( new KODE_IntParameter(   "Beats",                4,    1,    8     ));
    appendParameter( new KODE_IntParameter(   "Slices",               2,    1,    8     ));
    appendParameter( new KODE_IntParameter(   "Repeat",       "%",    30,   0,    100   ));

//    appendParameter( new KODE_IntParameter(   "min slices",           1,    1,    8     ));
//    appendParameter( new KODE_IntParameter(   "max slices",           3,    1,    8     ));
//    appendParameter( new KODE_IntParameter(   "Min Div",              2,    1,    8     ));
//    appendParameter( new KODE_IntParameter(   "Max Div",              4,    1,    8     ));

    appendParameter( new KODE_IntParameter(   "slice_bits",           0b00001011,   0, 255 ));
    appendParameter( new KODE_IntParameter(   "split_bits",           0b00001110,   0, 255 ));

    appendParameter( new KODE_IntParameter(   "Loop Size",    "%",    30,   0,    100   ));
    appendParameter( new KODE_PowParameter(   "Min",          "%",    50,   2,    true, 50, 200 ));
    appendParameter( new KODE_PowParameter(   "Max",          "%",    200,  2,    true, 50, 200 ));
    appendParameter( new KODE_IntParameter(   "Loop Speed",   "%",    30,   0,    100   ));
    appendParameter( new KODE_PowParameter(   "Min",          "%",    50,   2,    true, 50, 200 ));
    appendParameter( new KODE_PowParameter(   "Max",          "%",    200,  2,    true, 50, 200 ));
    appendParameter( new KODE_IntParameter(   "Loop Reverse", "%",    30,   0,    100   ));
    appendParameter( new KODE_PowParameter(   "Slice Att",    "%",    0,    3,    true, 0, 100 ));
    appendParameter( new KODE_PowParameter(   "Slice Dec",    "%",    0,    3,    true, 0, 100 ));
    appendParameter( new KODE_PowParameter(   "Loop Att",     "ms",   0,    3,    true, 0, 100 ));
    appendParameter( new KODE_PowParameter(   "Loop Dec",     "ms",   0,    3,    true, 0, 100 ));

    appendParameter( new KODE_IntParameter(   "Filter",       "%",    30,   0,    100   ));
    appendParameter( new KODE_TextParameter(  "Type",                 0,    2,    filter_type_txt ));
    appendParameter( new KODE_PowParameter(   "Frq",                  1,    4,    true  ));
    appendParameter( new KODE_FloatParameter( "Res",                  0.0,  0.0,  1.1   ));
    appendParameter( new KODE_FloatParameter( "Clip",                 1,    0.01, 4.0   ));
    appendParameter( new KODE_FloatParameter( "Pre",                  1,    0,    2     ));
    appendParameter( new KODE_FloatParameter( "Post",                 1,    0,    2     ));
    appendParameter( new KODE_PowParameter(   "Min",           "%",   50,   2,  true, 50, 200 ));
    appendParameter( new KODE_PowParameter(   "Max",           "%",   200,  2,  true, 50, 200 ));

    MParameters[PAR_LOOP_SIZE_MIN]->setNumDigits(0);
    MParameters[PAR_LOOP_SIZE_MAX]->setNumDigits(0);
    MParameters[PAR_LOOP_SPEED_MIN]->setNumDigits(0);
    MParameters[PAR_LOOP_SPEED_MAX]->setNumDigits(0);

    MParameters[PAR_FX_FILTER_FREQ]->setNumDigits(2);
    MParameters[PAR_FX_FILTER_RES]->setNumDigits(2);
    MParameters[PAR_FX_FILTER_FREQ_MIN]->setNumDigits(0);
    MParameters[PAR_FX_FILTER_FREQ_MAX]->setNumDigits(0);

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(585,485);
    #endif

  }
};


//----------------------------------------------------------------------
#endif
