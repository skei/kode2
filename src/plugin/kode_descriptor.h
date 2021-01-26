#ifndef kode_descriptor_included
#define kode_descriptor_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_array.h"
#include "base/utils/kode_strutils.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

typedef KODE_Array<KODE_Parameter*> KODE_Parameters;

//----------------------------------------------------------------------

// cat /proc/sys/kernel/random/uuid

class KODE_Descriptor {

//------------------------------
protected:
//------------------------------

  #ifdef KODE_DEBUG
  const char*     MName             = "test_plugin_debug";
  #else
  const char*     MName             = "test_plugin";
  #endif

  const char*     MAuthor           = "skei.audio";
  const char*     MEmail            = "my@email.com";
  const char*     MUrl              = "https://www.website.com";
  const char*     MVersionText      = "0.0.0";
  uint32_t        MVersion          = 0;
  uint32_t        MShortId          = 0;
  const uint8_t*  MLongId           = KODE_NULL_ID;
  const uint8_t*  MLongEditorId     = KODE_NULL_ID;
  uint32_t        MNumInputs        = 2;
  uint32_t        MNumOutputs       = 2;
  uint32_t        MEditorWidth      = 0;
  uint32_t        MEditorHeight     = 0;

  bool            MHasEditor        = false;
  bool            MCanResizeEditor  = false;
  bool            MIsSynth          = false;
  bool            MCanSendMidi      = false;
  bool            MCanReceiveMidi   = false;

  KODE_Parameters MParameters;

  uint32_t   MIdBuffer[4]   = {0};
  uint32_t   MIdBuffer2[4]  = {0};

//------------------------------
private:
//------------------------------

  // 5f = _
  // ABCDEFGHIJKLMNOPQRSTUVWXYZ
  // 44444444444444455555555555
   //123456789abcdef0a123456789

  void create_uuid() {
    MIdBuffer[0] = 0x4b5f504c; // 'K_PL'
    MIdBuffer[1] = MVersion;
    MIdBuffer[2] = KODE_HashString((char*)MName);
    MIdBuffer[3] = KODE_HashString((char*)MAuthor);
    MIdBuffer2[0] = 0x4b5f4544; // 'K_ED'
    MIdBuffer2[1] = MVersion;
    MIdBuffer2[2] = KODE_HashString((char*)MName);
    MIdBuffer2[3] = KODE_HashString((char*)MAuthor);
  }


//------------------------------
public:
//------------------------------

  KODE_Descriptor() {
    /*
    KODE_Memcpy(MLongIdBuffer,"KODE_PLUGIN_",12);
    MLongIdBuffer[12] = (MVersion & 0x000000ff);
    MLongIdBuffer[13] = (MVersion & 0x0000ff00) >> 8;
    MLongIdBuffer[14] = (MVersion & 0x00ff0000) >> 16;
    MLongIdBuffer[15] = (MVersion & 0xff000000) >> 24;
    KODE_Memcpy(MLongEditorIdBuffer,"KODE_EDITOR_",12);
    MLongEditorIdBuffer[12] = (MVersion & 0x000000ff);
    MLongEditorIdBuffer[13] = (MVersion & 0x0000ff00) >> 8;
    MLongEditorIdBuffer[14] = (MVersion & 0x00ff0000) >> 16;
    MLongEditorIdBuffer[15] = (MVersion & 0xff000000) >> 24;
    MLongId = MLongIdBuffer;
    MLongEditorId = MLongEditorIdBuffer;
    */
  }

  //----------

  virtual ~KODE_Descriptor() {
    #ifndef KODE_NO_AUTODELETE
      deleteParameters();
    #endif
  }

//------------------------------
public:
//------------------------------

  const char*     getName()                     { return MName; }
  const char*     getAuthor()                   { return MAuthor; }
  const char*     getEmail()                    { return MEmail; }
  const char*     getUrl()                      { return MUrl; }
  const char*     getVersionText()              { return MVersionText; }
  const uint32_t  getVersion()                  { return MVersion; }
  const uint32_t  getShortId()                  { return MShortId; }

  const uint8_t*  getLongId()                   { return MLongId; }
  const uint8_t*  getLongEditorId()             { return MLongEditorId; }

  uint32_t        getNumParameters()            { return MParameters.size(); }
  KODE_Parameter* getParameter(uint32_t AIndex) { return MParameters[AIndex]; }

  uint32_t        getNumInputs()                { return MNumInputs; }
  uint32_t        getNumOutputs()               { return MNumOutputs; }

  uint32_t        getEditorWidth()              { return MEditorWidth; }
  uint32_t        getEditorHeight()             { return MEditorHeight; }

  bool            hasEditor()                   { return MHasEditor; }
  bool            canResizeEditor()             { return MCanResizeEditor; }
  bool            isSynth()                     { return MIsSynth; }
  bool            canSendMidi()                 { return MCanSendMidi; }
  bool            canReceiveMidi()              { return MCanReceiveMidi; }

//------------------------------
protected:
//------------------------------

  void appendParameter(KODE_Parameter* AParameter) {
    int32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
  }

  //----------

  void deleteParameters() {
    for (uint32_t i=0; i<MParameters.size(); i++) {
      KODE_Delete MParameters[i];
      //MParameters[i] = KODE_NULL;
    }
  }

  //----------


};

//----------------------------------------------------------------------
#endif
