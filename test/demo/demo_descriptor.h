#ifndef demo_descriptor_included
#define demo_descriptor_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"


class demo_descriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  demo_descriptor() {

    #ifdef KODE_DEBUG
      setName("demo_debug");
    #else
      setName("demo");
    #endif

    setAuthor("skei.audio");
    setVersion(0x00000001);
    setIsSynth(true);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    //KODE_Parameter* parameter;
    //parameter = appendParameter( new KODE_Parameter("param1",0.2f) );
    //parameter = appendParameter( new KODE_Parameter("param2",0.7f) );
    //parameter->setLabel("db");
    //parameter = appendParameter( new KODE_FloatParameter("param3",0.0f,-1,2,0.25) );
    //parameter->setLabel("%");

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(1024,768);
    #endif

  }
};

//----------------------------------------------------------------------
#endif
