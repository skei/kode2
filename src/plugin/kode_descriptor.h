#ifndef kode_descriptor_included
#define kode_descriptor_included
//----------------------------------------------------------------------

#include "plugin/kode_parameter.h"
#include "plugin/kode_program.h"

//----------------------------------------------------------------------

struct KODE_PluginOptions {
  bool isSynth = false;
  bool hasEditor = false;
  bool canSendMidi = false;
  bool canReceiveMidi = false;
};

//----------

struct KODE_PluginPort {
  bool        isAudio; // else ?
  bool        isInput; // else output
  const char* name;
};

typedef KODE_Array<KODE_PluginPort*> KODE_PluginPorts;

//----------------------------------------------------------------------

class KODE_Descriptor {

private:

  uint8_t             MLongId[16]       = {0};
  uint8_t             MLongEditorId[16] = {0};
  char                MVersionText[32]  = {0};

//------------------------------
protected:
//------------------------------

  const char*         MName     = "plugin";
  const char*         MAuthor   = "skei.audio";
  uint32_t            MVersion  = 0x00000000;
  KODE_PluginOptions  MOptions;
  KODE_Parameters     MParameters;
  KODE_Programs       MPrograms;
  KODE_PluginPorts    MInputs;
  KODE_PluginPorts    MOutputs;

//------------------------------
public:
//------------------------------

  KODE_Descriptor() {
    //MDefValue = to01(real-world-value)
  }

  //----------

  virtual ~KODE_Descriptor() {
    #ifndef KODE_NO_AUTODELETE
      deleteParameters();
      deletePrograms();
      deleteInputs();
      deleteOutputs();
    #endif
  }

//------------------------------
public:
//------------------------------

  const char*       getName()                 { return MName; }
  const char*       getAuthor()               { return MAuthor; }
  uint32_t          getVersion()              { return MVersion; }

  bool              isSynth()                 { return MOptions.isSynth; }
  bool              hasEditor()               { return MOptions.hasEditor; }
  bool              canSendMidi()             { return MOptions.canSendMidi; }
  bool              canReceiveMidi()          { return MOptions.canReceiveMidi; }

  uint32_t          getNumParameters()        { return MParameters.size(); }
  uint32_t          getNumPrograms()          { return MPrograms.size(); }
  uint32_t          getNumInputs()            { return MInputs.size(); }
  uint32_t          getNumOutputs()           { return MOutputs.size(); }

  KODE_Parameter*   getParameter(uint32_t i)  { return MParameters[i]; }
  KODE_Program*     getProgram(uint32_t i)    { return MPrograms[i]; }
  KODE_PluginPort*  getInput(uint32_t i)      { return MInputs[i]; }
  KODE_PluginPort*  getOutput(uint32_t i)     { return MOutputs[i]; }

//------------------------------
public:
//------------------------------

  void appendParameter(KODE_Parameter* AParameter) {
    //uint32_t index = MParameters.size();
    //AParameter->setIndex(index);
    MParameters.append(AParameter);
  }

  //----------

  void appendProgram(KODE_Program* AProgram) {
    MPrograms.append(AProgram);
  }

  //----------

  void appendInput(KODE_PluginPort* AInput) {
    MInputs.append(AInput);
  }

  //----------

  void appendOutput(KODE_PluginPort* AOutput) {
    MOutputs.append(AOutput);
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      if (MParameters[i]) {
        KODE_Delete MParameters[i];
        MParameters[i] = KODE_NULL;
      }
    }
  }

  //----------

  void deletePrograms() {
    for (uint32_t i=0; i<MPrograms.size(); i++) {
      if (MPrograms[i]) {
        KODE_Delete MPrograms[i];
        MPrograms[i] = KODE_NULL;
      }
    }
  }

  //----------

  void deleteInputs() {
    for (uint32_t i=0; i<MInputs.size(); i++) {
      if (MInputs[i]) {
        KODE_Delete MInputs[i];
        MInputs[i] = KODE_NULL;
      }
    }
  }

  //----------

  void deleteOutputs() {
    for (uint32_t i=0; i<MOutputs.size(); i++) {
      if (MOutputs[i]) {
        KODE_Delete MOutputs[i];
        MOutputs[i] = KODE_NULL;
      }
    }
  }

//------------------------------
public:
//------------------------------

  uint8_t* getLongId() {
    uint32_t* ptr = (uint32_t*)MLongId;
    ptr[0] = KODE_MAGIC_K_PL;
    ptr[1] = KODE_HashString(MName);
    ptr[2] = KODE_HashString(MAuthor);
    ptr[3] = MVersion;
    return MLongId;
  }

  //----------

  uint8_t* getLongEditorId() {
    uint32_t* ptr = (uint32_t*)MLongId;
    ptr[0] = KODE_MAGIC_K_ED;
    ptr[1] = KODE_HashString(MName);
    ptr[2] = KODE_HashString(MAuthor);
    ptr[3] = MVersion;
    return MLongEditorId;
  }

  //----------

  const char* getVersionText() {
    char temp[16];
    // 0x00030301 -> "3.3.1"
    uint32_t v1 = MVersion & 0xffff0000;
    uint32_t v2 = MVersion & 0x0000ff00;
    uint32_t v3 = MVersion & 0x000000ff;
    KODE_IntToString(temp,v1);
    KODE_Strcpy(MVersionText,temp);
    KODE_Strcat(MVersionText,".");
    KODE_IntToString(temp,v2);
    KODE_Strcat(MVersionText,temp);
    KODE_Strcat(MVersionText,".");
    KODE_IntToString(temp,v3);
    KODE_Strcat(MVersionText,temp);
    return MVersionText;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
