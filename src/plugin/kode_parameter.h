#ifndef kode_parameter_included
#define kode_parameter_included
//----------------------------------------------------------------------

struct KODE_ParameterOptions {
  bool canAutomate = true;
};

//----------------------------------------------------------------------

class KODE_Parameter {

//------------------------------
protected:
//------------------------------

  KODE_ParameterOptions MOptions;
  const char*           MName       = "";
  const char*           MShortName  = "";
  const char*           MLabel      = "";
  float                 MDefValue   = 0.0f;
  float                 MMinValue   = 0.0f;
  float                 MMaxValue   = 1.0f;
  uint32_t              MNumSteps   = 0;

//------------------------------
public:
//------------------------------

  KODE_Parameter() {
  }

  //----------

  virtual ~KODE_Parameter() {
  }

//------------------------------
public:
//------------------------------

  bool        canAutomate()   { return MOptions.canAutomate; }
  const char* getName()       { return MName; }
  const char* getShortName()  { return MShortName; }
  const char* getLabel()      { return MLabel; }
  float       getDefValue()   { return MDefValue; }
  float       getMinValue()   { return MMinValue; }
  float       getMaxValue()   { return MMaxValue; }
  uint32_t    getNumSteps()   { return MNumSteps; }

//------------------------------
public:
//------------------------------

  virtual float from01(float AValue) {
    return AValue;
  }

  //----------

  virtual float to01(float AValue) {
    return AValue;
  }

  //----------

  virtual void getDisplayString(float AValue, char* ABuffer) {
    float value = from01(AValue);
    KODE_FloatToString(ABuffer,value);
  }

};

//----------------------------------------------------------------------

typedef KODE_Array<KODE_Parameter*> KODE_Parameters;

//----------------------------------------------------------------------
#endif
