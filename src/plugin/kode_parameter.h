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
  int32_t               MIndex      = -1;

//------------------------------
public:
//------------------------------

  KODE_Parameter(const char* AName, float AValue=0.0f) {
    MName = AName;
    MDefValue = AValue;
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
  int32_t     getIndex()      { return MIndex; }

//------------------------------
public:
//------------------------------

  void setCanAutomate(bool ACanAutomate)  { MOptions.canAutomate = ACanAutomate; }
  void setName(const char* AName)         { MName = AName; }
  void setShortName(const char* AName)    { MShortName = AName; }
  void setLabel(const char* ALabel)       { MLabel = ALabel; }
  void setDefValue(float AValue)          { MDefValue = AValue; }
  void setMinValue(float AValue)          { MMinValue = AValue; }
  void setMaxValue(float AValue)          { MMaxValue = AValue; }
  void setNumSteps(uint32_t ANum)         { MNumSteps = ANum; }
  void setIndex(int32_t AIndex)           { MIndex = AIndex; }

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
