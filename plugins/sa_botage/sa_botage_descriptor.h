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
  PAR_LOOP_SIZE_AMT,
  PAR_LOOP_SPEED_PROB,
  PAR_LOOP_SPEED_AMT,
  PAR_XFADE_MODE,
  PAR_XFADE_AMT
};

const char* xfade_mode_txt[4] = {
  "off", "fwd", "rev", "bi"
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
    setVersion(0x00000001);
    setIsSynth(true);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    // NOTE: must be in same order as sa_botage_params

    //__KODE_UNUSED
    //KODE_Parameter* parameter;

    /*parameter = */appendParameter( new KODE_IntParameter("Beats",       4,    1, 8) );
    /*parameter = */appendParameter( new KODE_IntParameter("Subdiv",      2,    1, 8) );
    /*parameter = */appendParameter( new KODE_IntParameter("Repeat", "%", 35,   0, 100) );
    /*parameter = */appendParameter( new KODE_IntParameter("Min slices",  1,    1, 8) );
    /*parameter = */appendParameter( new KODE_IntParameter("Max slices",  4,    1, 8) );
    /*parameter = */appendParameter( new KODE_IntParameter("Min subdiv",  1,    1, 8) );
    /*parameter = */appendParameter( new KODE_IntParameter("Max subdiv",  4,    1, 8) );
    /*parameter = */appendParameter( new KODE_IntParameter("Size", "%",   25,   0, 100) );
    /*parameter = */appendParameter( new KODE_PowParameter("amt", "%",    100,  2,  true, 50, 200, 1));
    /*parameter = */appendParameter( new KODE_IntParameter("Speed", "%",  25,   0, 100) );
    /*parameter = */appendParameter( new KODE_PowParameter("amt", "%",    100,  2,  true, 50, 200, 1));
    /*parameter = */appendParameter( new KODE_TextParameter("xfade",      0,    4,  xfade_mode_txt));
    /*parameter = */appendParameter( new KODE_FloatParameter("amt",       0     ));

    MParameters[PAR_LOOP_SIZE_AMT]->setNumDigits(0);
    MParameters[PAR_LOOP_SPEED_AMT]->setNumDigits(0);

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,480);
    #endif

  }
};


//----------------------------------------------------------------------
#endif
