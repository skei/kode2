#ifndef kode_plugin_parameter_included
#define kode_plugin_parameter_included
//----------------------------------------------------------------------

/*

  * parameters must have the same index in all instances..

  todo:

  get rid of all the different version of  v = AValue*AValue, etc
  in plugin.on_parameterChange
  but could have an array of paramteres, and
  set them directly
      params[AIndex] = AValue

*/

//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_array.h"
#include "base/utils/kode_convert.h"

//class KODE_PluginParameter;
//typedef KODE_Array<KODE_Parameter*> KODE_Parameters;

//----------------------------------------------------------------------


class KODE_Parameter {

//------------------------------
protected:
//------------------------------

  int32_t       MIndex      = -1;

  const char*   MName       = "";
  const char*   MShortName  = "";
  const char*   MLabel      = "dB";
  float         MDefValue   = 0.0f;
  float         MMinValue   = 0.0f;
  float         MMaxValue   = 1.0f;
  bool          MIsStepped  = false;
  uint32_t      MNumSteps   = 0;
  float         MStepSize   = 0.0f;

  //uint32_t      MFlags      = 0;//KODE_PARAMETER_AUTOMATE;
  bool          MCanAutomate = true;


//------------------------------
public:
//------------------------------

  /*
    don't call virtual functions in the constructor?
    (to01).. hmm...
  */

  KODE_Parameter(const char* AName="", float AValue=0.0f) {
    MName = AName;
    MDefValue = this->to01(AValue);
  }

  //----------

  virtual ~KODE_Parameter() {
  }

//------------------------------
public:
//------------------------------

  virtual void          setIndex(int32_t AIndex)    { MIndex = AIndex; }

  virtual int32_t       getIndex()                  { return MIndex; }
  virtual const char*   getName()                   { return MName; }
  virtual const char*   getShortName()              { return MShortName; }
  virtual const char*   getLabel()                  { return MLabel; }
  virtual float         getDefValue()               { return MDefValue; }
  virtual float         getMinValue()               { return MMinValue; }
  virtual float         getMaxValue()               { return MMaxValue; }
  virtual uint32_t      getNumSteps()               { return MNumSteps; }
  virtual float         getStepSize()               { return MStepSize; }
  virtual bool          isStepped()                 { return MIsStepped; }

  //virtual bool          hasFlag(uint32_t AFlag)     { return (MFlags & AFlag); }
  //virtual void          setFlag(uint32_t AFlag)     { MFlags |= AFlag; }
  //virtual void          clearFlag(uint32_t AFlag)   { MFlags &= ~AFlag; }

  virtual float         canAutomate()               { return MCanAutomate; }


//------------------------------
public:
//------------------------------

  //virtual void normalizeDefValue() {
  //  MDefValue = this->to01(MDefValue);
  //}

  //----------

  virtual float from01(float AValue) {
    return AValue;
  }

  //----------

  virtual float to01(float AValue) {
    return AValue;
  }

  //----------

  // AValue = 0..1

  virtual char* getDisplayText(float AValue, char* ABuffer) {
    //sprintf(ABuffer,"%.2f",from01(AValue));
    float v = from01(AValue);
    KODE_FloatToString(ABuffer,v);
    return ABuffer;
  }

};

//----------------------------------------------------------------------
#endif
