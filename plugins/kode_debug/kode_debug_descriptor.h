#ifndef kode_debug_descriptor_included
#define kode_debug_descriptor_included
//----------------------------------------------------------------------

#include "plugin/kode_descriptor.h"

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor() {

    #ifdef KODE_DEBUG
      setName("kode_debug");
    #else
      setName("kode_debug");
    #endif

    setAuthor("skei.audio");
    setVersion(0x00000001);
    setIsSynth(true);

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    appendParameter( new KODE_Parameter("param1",0.2f) );
    appendParameter( new KODE_Parameter("param2",0.7f) );
    appendParameter( new KODE_FloatParameter("param3",0.0f,-1,2,0.25) );

    #ifndef KODE_NO_GUI
      setHasEditor(true);
      setEditorSize(640,480);
    #endif

  }
};

//----------------------------------------------------------------------
#endif
