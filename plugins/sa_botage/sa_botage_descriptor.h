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

  PAR_RANGE_MIN_SLICES,
  PAR_RANGE_MAX_SLICES,
  PAR_RANGE_MIN_SUBDIV,
  PAR_RANGE_MAX_SUBDIV,

  PAR_LOOP_SIZE_PROB,
  PAR_LOOP_SIZE_MIN,
  PAR_LOOP_SIZE_MAX,

  PAR_LOOP_SPEED_PROB,
  PAR_LOOP_SPEED_MIN,
  PAR_LOOP_SPEED_MAX,

  PAR_LOOP_REVERSE_PROB,

  //PAR_XFADE_MODE,
  //PAR_XFADE_AMT

  PAR_SLICE_ENV_ATT,
  PAR_SLICE_ENV_DEC,
  PAR_LOOP_ENV_ATT,
  PAR_LOOP_ENV_DEC


};

const char* xfade_mode_txt[4] = {
  "off", "forward", "reverse", "bidirectional"
};

//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

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
    setVersion(0x00000004);
    setEmail("tor.helge.skei@gmail.com");
    setUrl("https://torhelgeskei.com");
    //setIsSynth(true);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    // same order as sa_botage_params

    appendParameter( new KODE_IntParameter(   "Beats",                4,    1,  8    ));
    appendParameter( new KODE_IntParameter(   "Slices",               2,    1,  8    ));
    appendParameter( new KODE_IntParameter(   "Repeat",       "%",    30,   0,  100  ));
    appendParameter( new KODE_IntParameter(   "min slices",           1,    1,  8    ));
    appendParameter( new KODE_IntParameter(   "max slices",           3,    1,  8    ));
    appendParameter( new KODE_IntParameter(   "Min Div",              2,    1,  8    ));
    appendParameter( new KODE_IntParameter(   "Max Div",              4,    1,  8    ));
    appendParameter( new KODE_IntParameter(   "Loop Size",    "%",    30,   0,  100  ));
    appendParameter( new KODE_PowParameter(   "Min",          "%",    50,   2,  true, 50, 200, 1));
    appendParameter( new KODE_PowParameter(   "Max",          "%",    200,  2,  true, 50, 200, 1));
    appendParameter( new KODE_IntParameter(   "Loop Speed",   "%",    30,   0,  100  ));
    appendParameter( new KODE_PowParameter(   "Min",          "%",    50,   2,  true, 50, 200, 1));
    appendParameter( new KODE_PowParameter(   "Max",          "%",    200,  2,  true, 50, 200, 1));
    appendParameter( new KODE_IntParameter(   "Loop Reverse", "%",    30,   0,  100  ));

    //appendParameter( new KODE_IntParameter(   "Range Att",    "%",  1,    1, 64  ));
    //appendParameter( new KODE_IntParameter(   "Range Dec",    "%",  1,    1, 64  ));
    //appendParameter( new KODE_IntParameter(   "Loop Att",     "%",  1,    1, 64  ));
    //appendParameter( new KODE_IntParameter(   "Loop Dec",     "%",  1,    1, 64  ));

    appendParameter( new KODE_PowParameter(   "Slice Att",      "%",  1,   3.0,  true, 1, 100));
    appendParameter( new KODE_PowParameter(   "Slice Dec",      "%",  1,   3.0,  true, 1, 100));
    appendParameter( new KODE_PowParameter(   "Loop Att",       "%",  1,   3.0,  true, 1, 100));
    appendParameter( new KODE_PowParameter(   "Loop Dec",       "%",  1,   3.0,  true, 1, 100));

    //appendParameter( new KODE_TextParameter(  "Xfade",              0,    4,  xfade_mode_txt));
    //appendParameter( new KODE_FloatParameter( "Amount",             0     ));

    MParameters[PAR_LOOP_SIZE_MIN]->setNumDigits(0);
    MParameters[PAR_LOOP_SIZE_MAX]->setNumDigits(0);
    MParameters[PAR_LOOP_SPEED_MIN]->setNumDigits(0);
    MParameters[PAR_LOOP_SPEED_MAX]->setNumDigits(0);
    MParameters[PAR_SLICE_ENV_ATT]->setNumDigits(2);
    MParameters[PAR_SLICE_ENV_DEC]->setNumDigits(2);
    MParameters[PAR_LOOP_ENV_ATT]->setNumDigits(2);
    MParameters[PAR_LOOP_ENV_DEC]->setNumDigits(2);

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,400);
    #endif

  }
};


//----------------------------------------------------------------------
#endif
