#ifndef kode_parameter_included
#define kode_parameter_included
//----------------------------------------------------------------------

struct KODE_ParameterOptions {
  bool canAutomate = true;
};

//----------------------------------------------------------------------

class KODE_Parameter {
public:
  KODE_ParameterOptions options;
  const char* name = "";
  const char* label = "";
  float def_value = 0.0f;
  float min_value = 0.0f;
  float max_value = 1.0f;
  uint32_t num_steps = 0;
public: // implemented in KODE_*Parameter
  virtual float from01(float AValue) { return AValue; }
  virtual float to01(float AValue) { return AValue; }
  virtual void getDisplayString(float AValue, char* ABuffer) {}
};

//----------------------------------------------------------------------

typedef KODE_Array<KODE_Parameter*> KODE_Parameters;

//----------------------------------------------------------------------
#endif
