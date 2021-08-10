#ifndef kode_descriptor_included
#define kode_descriptor_included
//----------------------------------------------------------------------

#include "plugin/kode_parameter.h"
#include "plugin/kode_program.h"

//----------------------------------------------------------------------

struct KODE_PluginOptions {
  bool isSynth        = false;
  bool hasEditor      = false;
  bool canSendMidi    = false;
  bool canReceiveMidi = false;
};

//----------

struct KODE_PluginPort {
  bool        isAudio = true;   // else ?
  bool        isInput = false;  // else output
  const char* name    = "";
  //
  KODE_PluginPort(const char* AName, bool AIsAudio=true) {
    name    = AName;
    isAudio = AIsAudio;
    //isInput = AIsInput;
  }
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
  const char*         MAuthor   = "author";
  const char*         MUrl      = "https://internet.com";
  const char*         MEmail    = "author@email.com";
  uint32_t            MVersion  = 0x00000000;
  KODE_PluginOptions  MOptions;
  KODE_Parameters     MParameters;
  KODE_Programs       MPrograms;
  KODE_PluginPorts    MInputs;
  KODE_PluginPorts    MOutputs;

  uint32_t            MEditorWidth = 0;
  uint32_t            MEditorHeight = 0;

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
  const char*       getUrl()                  { return MUrl; }
  const char*       getEmail()                { return MEmail; }
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
  uint32_t          getEditorWidth()          { return MEditorWidth; }
  uint32_t          getEditorHeight()         { return MEditorHeight; }

//------------------------------
public:
//------------------------------

  void setName(const char* AName)               { MName = AName; }
  void setAuthor(const char* AAuthor)           { MAuthor = AAuthor; }
  void setUrl(const char* AUrl)                 { MUrl = AUrl; }
  void setEmail(const char* AEmail)             { MEmail = AEmail; }
  void setVersion(uint32_t AVersion)            { MVersion = AVersion; }

  void setIsSynth(bool AIsSynth=true) {
    MOptions.isSynth = AIsSynth;
    if (AIsSynth) MOptions.canReceiveMidi = true;
  }

  void setHasEditor(bool AHasEditor=true)           { MOptions.hasEditor = AHasEditor; }
  void setCanSendMidi(bool ACanSendMidi=true)       { MOptions.canSendMidi = ACanSendMidi; }
  void setCanReceiveMidi(bool ACanReceiveMidi=true) { MOptions.canReceiveMidi = ACanReceiveMidi; }
  void setEditorWidth(uint32_t AWidth)              { MEditorWidth = AWidth; }
  void setEditorHeight(uint32_t AHeight)            { MEditorHeight = AHeight; }
  void setEditorSize(uint32_t W, uint32_t H)        { MEditorWidth = W; MEditorHeight = H; }

  //bool isSynth()                                    { return MOptions.isSynth; }
  //bool hasEditor()                                  { return MOptions.hasEditor; }
  //bool canSendMidi(bool ACanSendMidi=true)          { return MOptions.canSendMidi; }
  //bool canReceiveMidi(bool ACanReceiveMidi=true)    { return MOptions.canReceiveMidi; }

//------------------------------
public:
//------------------------------

  KODE_Parameter* appendParameter(KODE_Parameter* AParameter) {
    uint32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
    return AParameter;
  }

  //----------

  KODE_Program* appendProgram(KODE_Program* AProgram) {
    MPrograms.append(AProgram);
    return AProgram;
  }

  //----------

  KODE_PluginPort* appendInput(KODE_PluginPort* AInput) {
    AInput->isInput = true;
    MInputs.append(AInput);
    return AInput;
  }

  //----------

  KODE_PluginPort* appendOutput(KODE_PluginPort* AOutput) {
    AOutput->isInput = false;
    MOutputs.append(AOutput);
    return AOutput;
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
    uint32_t* ptr = (uint32_t*)MLongEditorId;
    ptr[0] = KODE_MAGIC_K_ED;
    ptr[1] = KODE_HashString(MName);
    ptr[2] = KODE_HashString(MAuthor);
    ptr[3] = MVersion;
    return MLongEditorId;
  }

  //----------

    // 0x03030001 -> "3.3.1"

  const char* getVersionText() {
    char temp[16];
    uint32_t major = MVersion & 0xff000000;
    uint32_t minor = MVersion & 0x00ff0000;
    uint32_t build = MVersion & 0x0000ffff;
    KODE_IntToString(temp,major);
    KODE_Strcpy(MVersionText,temp);
    KODE_Strcat(MVersionText,".");
    KODE_IntToString(temp,minor);
    KODE_Strcat(MVersionText,temp);
    KODE_Strcat(MVersionText,".");
    KODE_IntToString(temp,build);
    KODE_Strcat(MVersionText,temp);
    return MVersionText;
  }

  //----------

};

//----------------------------------------------------------------------
#endif
