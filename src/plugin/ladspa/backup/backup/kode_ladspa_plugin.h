#ifndef kode_ladspa_plugin_included
#define kode_ladspa_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/ladspa/kode_ladspa.h"
#include "plugin/ladspa/kode_ladspa_host.h"
#include "plugin/ladspa/kode_ladspa_instance.h"

//----------------------------------------------------------------------


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_LadspaPlugin {

  friend const LADSPA_Descriptor* kode_ladspa_entryPoint(unsigned long Index);

//------------------------------
private:
//------------------------------

  //KODE_Descriptor*      MDescriptor       = nullptr;

  LADSPA_Descriptor*    MLadspaDescriptor = nullptr;
  int*                  MPortDescriptors  = nullptr;
  char**                MPortNames        = nullptr;
  char*                 MPortNamesBuffer  = nullptr;
  LADSPA_PortRangeHint* MPortRangeHints   = nullptr;
  char*                 MNameBuffer       = nullptr;
  char*                 MLabelBuffer      = nullptr;
  char*                 MMakerBuffer      = nullptr;
  char*                 MCopyrightBuffer  = nullptr;

//------------------------------
public:
//------------------------------

  KODE_LadspaPlugin() {
    //MDescriptor = KODE_CreateDescriptor();
  }

  //----------

  ~KODE_LadspaPlugin() {

    //if (MDescriptor)        delete MDescriptor;

    if (MNameBuffer)      free(MNameBuffer);
    if (MLabelBuffer)     free(MLabelBuffer);
    if (MMakerBuffer)     free(MMakerBuffer);
    if (MCopyrightBuffer) free(MCopyrightBuffer);

    if (MPortDescriptors)   free(MPortDescriptors);
    if (MPortNames)         free(MPortNames);
    if (MPortNamesBuffer)   free(MPortNamesBuffer);
    if (MPortRangeHints)    free(MPortRangeHints);

    if (MLadspaDescriptor)  free(MLadspaDescriptor);

  }

//------------------------------
private:
//------------------------------

  /*
    at the moment, we only support index == 0
  */

  const LADSPA_Descriptor* ladspa_entryPoint(unsigned long Index) {
    LADSPA_Trace("ladspa: entryPoint %i\n",Index);
    if (Index > 0) { KODE_Trace("Index > 0\n"); return nullptr; }
    // carla calles ladspa_descriptor a second time before deleting the first..
    if (MLadspaDescriptor) { KODE_Trace("MLadspaDescriptor already allocated!\n"); return MLadspaDescriptor; }
    KODE_Descriptor* descriptor = KODE_GetDescriptor();
    MLadspaDescriptor = (LADSPA_Descriptor*)malloc(sizeof(LADSPA_Descriptor));
    memset(MLadspaDescriptor,0,sizeof(LADSPA_Descriptor));
    //setDefaultParamValues();

    MNameBuffer       = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MLabelBuffer      = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MMakerBuffer      = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    MCopyrightBuffer  = (char*)malloc(KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);

    strncpy(MNameBuffer,      descriptor->getName(),     KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    strncpy(MLabelBuffer,     descriptor->getName(),     KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1); // todo: make valid c symbol (see lv2)
    strncpy(MMakerBuffer,     descriptor->getAuthor(),   KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);
    strncpy(MCopyrightBuffer, descriptor->getLicense(),  KODE_PLUGIN_LADSPA_MAX_NAME_LENGTH-1);

    uint32_t numin    = descriptor->getNumInputs();
    uint32_t numout   = descriptor->getNumOutputs();
    uint32_t numpar   = descriptor->getNumParameters();
    uint32_t numports = numin + numout + numpar;

    MPortDescriptors  = (LADSPA_PortDescriptor*)malloc(numports * sizeof(LADSPA_PortDescriptor));
    MPortNames        = (char**)malloc(numports * sizeof(char*));
    MPortNamesBuffer  = (char*)malloc(numports * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
    MPortRangeHints   = (LADSPA_PortRangeHint*)malloc(numports * sizeof(LADSPA_PortRangeHint));

    uint32_t i = 0;
    uint32_t port = 0;

    for (i=0; i<numin; i++) {
      //const char* port_name = "Audio Input";
      MPortDescriptors[port] = LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT;
      //char* ptr = MPortNamesBuffer + (port * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
      //KODE_Strncpy(ptr,port_name,KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
      //ptr[KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
      MPortNames[port] = (char*)"input";
      MPortRangeHints[port].HintDescriptor = 0;
      //MPortRangeHints[port].LowerBound = 0;
      //MPortRangeHints[port].UpperBound = 0;
      port++;
    }

    for (i=0; i<numout; i++) {
      //const char* port_name = "Audio Output";
      MPortDescriptors[port] = LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT;
      //char* ptr = MPortNames + (port * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
      //KODE_Strncpy(ptr,port_name,KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
      //ptr[KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
      MPortNames[port] = (char*)"output";
      MPortRangeHints[port].HintDescriptor = 0;
      //MPortRangeHints[port].LowerBound = 0;
      //MPortRangeHints[port].UpperBound = 0;
      port++;
    }

    for (i=0; i<numpar; i++) {
      KODE_Parameter* param = descriptor->getParameter(i);
      MPortDescriptors[port] = LADSPA_PORT_CONTROL | LADSPA_PORT_INPUT;
      //const char* pname = param->getName();
      //const char* port_name = pname;
      //KODE_Trace("%i %s\n",i,port_name);
      char* ptr = MPortNamesBuffer + (port * KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH);
      strncpy(ptr,param->getName(),KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH-1);
      ptr[KODE_PLUGIN_LADSPA_MAX_PORT_NAME_LENGTH] = 0;
      MPortNames[port] = ptr;//(char*)"param";
      // LADSPA_HINT_TOGGLED, LADSPA_HINT_SAMPLE_RATE, LADSPA_HINT_LOGARITHMIC
      // LADSPA_HINT_INTEGER,
      MPortRangeHints[port].HintDescriptor = LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      MPortRangeHints[port].LowerBound = param->getMinValue();
      MPortRangeHints[port].UpperBound = param->getMaxValue();
      port++;
    }

    KODE_Assert(port == numports);

    MLadspaDescriptor->UniqueID            = descriptor->getShortId();
    MLadspaDescriptor->Label               = MLabelBuffer;
    MLadspaDescriptor->Properties          = LADSPA_PROPERTY_HARD_RT_CAPABLE; // LADSPA_PROPERTY_REALTIME, LADSPA_PROPERTY_INPLACE_BROKEN
    MLadspaDescriptor->Name                = MNameBuffer;
    MLadspaDescriptor->Maker               = MMakerBuffer;
    MLadspaDescriptor->Copyright           = MCopyrightBuffer;
    MLadspaDescriptor->PortCount           = numports;
    MLadspaDescriptor->PortDescriptors     = MPortDescriptors;
    MLadspaDescriptor->PortNames           = (const char* const*)MPortNames;
    MLadspaDescriptor->PortRangeHints      = MPortRangeHints;
    MLadspaDescriptor->ImplementationData  = this;//nullptr;
    MLadspaDescriptor->instantiate         = ladspa_instantiate_callback;
    MLadspaDescriptor->connect_port        = ladspa_connect_port_callback;
    MLadspaDescriptor->activate            = ladspa_activate_callback;
    MLadspaDescriptor->run                 = ladspa_run_callback;
    MLadspaDescriptor->run_adding          = ladspa_run_adding_callback;
    MLadspaDescriptor->set_run_adding_gain = ladspa_set_run_adding_gain_callback;
    MLadspaDescriptor->deactivate          = ladspa_deactivate_callback;
    MLadspaDescriptor->cleanup             = ladspa_cleanup_callback;

    return MLadspaDescriptor;
  }

  //----------

  /*
  LADSPA_Handle ladspa_instantiate(const struct _LADSPA_Descriptor* Descriptor, unsigned long SampleRate) {
    KODE_LadspaInstance* ladspa_instance = new KODE_LadspaInstance(MDescriptor,SampleRate);
    //KODE_Instance* instance = ladspa_instance->getInstance();
    //instance->updateAllParameters();
    return (LADSPA_Handle)ladspa_instance;
  }
  */

//------------------------------
private: // ladspa callbacks
//------------------------------

  static
  LADSPA_Handle ladspa_instantiate_callback(const struct _LADSPA_Descriptor* Descriptor, unsigned long SampleRate) {
    KODE_Descriptor* descriptor = KODE_GetDescriptor();
    KODE_LadspaInstance* ladspa_instance = new KODE_LadspaInstance(descriptor,SampleRate);
    return (LADSPA_Handle)ladspa_instance;


  }

  //----------

  static
  void ladspa_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {
    //LADSPA_Trace("ladspa: ladspa_connect_port_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_connect_port(Port,DataLocation);
  }

  //----------

  static
  void ladspa_activate_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_activate_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_activate();
  }

  //----------

  static
  void ladspa_run_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: ladspa_run_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_run(SampleCount);
  }

  //----------

  static
  void ladspa_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount) {
    //LADSPA_Trace("ladspa: ladspa_run_adding_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_run_adding(SampleCount);
  }

  //----------

  static
  void ladspa_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain) {
    //LADSPA_Trace("ladspa: ladspa_set_run_adding_gain_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_set_run_adding_gain(Gain);
  }

  //----------

  static
  void ladspa_deactivate_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_deactivate_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_deactivate();
  }

  //----------

  static
  void ladspa_cleanup_callback(LADSPA_Handle Instance) {
    //LADSPA_Trace("ladspa: ladspa_cleanup_callback\n");
    KODE_LadspaInstance* ladspa_instance = (KODE_LadspaInstance*)Instance;
    if (ladspa_instance) ladspa_instance->ladspa_cleanup();
    //LADSPA_Trace("ladspa: cleanup -> deleting instance\n");
    delete ladspa_instance;
  }

};

//----------------------------------------------------------------------
// .so entry point
//----------------------------------------------------------------------

#define KODE_LADSPA_MAIN_SYMBOL asm ("ladspa_descriptor");
const LADSPA_Descriptor* kode_ladspa_entryPoint(unsigned long Index) KODE_LADSPA_MAIN_SYMBOL

//----------

KODE_LadspaPlugin LADSPA_PLUGIN;

//----------

__KODE_DLLEXPORT
const LADSPA_Descriptor* kode_ladspa_entryPoint(unsigned long Index) {
  const LADSPA_Descriptor* descriptor = LADSPA_PLUGIN.ladspa_entryPoint(Index);
  return descriptor;
}

//----------------------------------------------------------------------
#endif
