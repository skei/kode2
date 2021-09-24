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
  //PAR_LOOP_SIZE_AMT,
  PAR_LOOP_SIZE_MIN,
  PAR_LOOP_SIZE_MAX,
  PAR_LOOP_SPEED_PROB,
  //PAR_LOOP_SPEED_AMT,
  PAR_LOOP_SPEED_MIN,
  PAR_LOOP_SPEED_MAX,
  PAR_XFADE_MODE,
  PAR_XFADE_AMT
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
    setVersion(0x00000002);
    setEmail("tor.helge.skei@gmail.com");
    setUrl("https://torhelgeskei.com");
    //setIsSynth(true);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    // NOTE: must be in same order as sa_botage_params

    //__KODE_UNUSED
    //KODE_Parameter* parameter;

    appendParameter( new KODE_IntParameter(   "Beats",            4,    1, 8    ));
    appendParameter( new KODE_IntParameter(   "Slices",           2,    1, 8    ));
    appendParameter( new KODE_IntParameter(   "Repeat",     "%",  30,   0, 100  ));
    appendParameter( new KODE_IntParameter(   "Min Slices",       1,    1, 8    ));
    appendParameter( new KODE_IntParameter(   "Max Slices",       3,    1, 8    ));
    appendParameter( new KODE_IntParameter(   "Min Div",          2,    1, 8    ));
    appendParameter( new KODE_IntParameter(   "Max Div",          4,    1, 8    ));
    appendParameter( new KODE_IntParameter(   "Loop Size",  "%",  0,    0, 100  ));
    appendParameter( new KODE_PowParameter(   "Min",        "%",  100,  2,  true, 50, 200, 1));
    appendParameter( new KODE_PowParameter(   "Max",        "%",  100,  2,  true, 50, 200, 1));
    appendParameter( new KODE_IntParameter(   "Loop Speed", "%",  0,    0, 100  ));
    appendParameter( new KODE_PowParameter(   "Min",        "%",  100,  2,  true, 50, 200, 1));
    appendParameter( new KODE_PowParameter(   "Max",        "%",  100,  2,  true, 50, 200, 1));
    appendParameter( new KODE_TextParameter(  "Xfade",            0,    4,  xfade_mode_txt));
    appendParameter( new KODE_FloatParameter( "Amount",           0     ));

    MParameters[PAR_LOOP_SIZE_MIN]->setNumDigits(0);
    MParameters[PAR_LOOP_SIZE_MAX]->setNumDigits(0);
    MParameters[PAR_LOOP_SPEED_MIN]->setNumDigits(0);
    MParameters[PAR_LOOP_SPEED_MAX]->setNumDigits(0);

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,480);
    #endif

  }
};


//----------------------------------------------------------------------
#endif
