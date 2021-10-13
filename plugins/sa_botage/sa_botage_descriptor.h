#ifndef sa_botage_descriptor_included
#define sa_botage_descriptor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_parameters.h"

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

    setVersion(0x00000007);
    setAuthor("skei.audio");
    setUrl("https://torhelgeskei.com");
    setEmail("tor.helge.skei@gmail.com");

    //setIsSynth();
    //setCanSendMidi();
    //setCanReceiveMidi();

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(800,600);
    #endif

    appendInput(  new KODE_PluginPort("input_1")  );
    appendInput(  new KODE_PluginPort("input_2")  );
    appendOutput( new KODE_PluginPort("output_1") );
    appendOutput( new KODE_PluginPort("output_2") );

    //KODE_Parameter* parameter;
    /*parameter =*/ appendParameter( new KODE_IntParameter("Beats",4,1,8));
    /*parameter =*/ appendParameter( new KODE_IntParameter("Slices",2,1,8));
    //parameter->setLabel("db");
    //parameter = appendParameter( new KODE_Parameter("param3",0.4f) );
    //parameter->setLabel("%");

  }
};

//----------------------------------------------------------------------
#endif
