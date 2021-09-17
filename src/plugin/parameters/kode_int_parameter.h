#ifndef kode_int_parameter_included
#define kode_int_parameter_included
//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_parameter.h"
#include "plugin/parameters/kode_float_parameter.h"

class KODE_IntParameter
: public KODE_FloatParameter {

public:

  KODE_IntParameter(const char* AName, float AVal, float AMin, float AMax)
  : KODE_FloatParameter(AName,AVal,AMin,AMax,1) {
  }

  KODE_IntParameter(const char* AName, const char* ALabel, float AVal, float AMin, float AMax)
  : KODE_FloatParameter(AName,ALabel,AVal,AMin,AMax,1) {
  }

public:

  // AValue = 0..1

  char* getDisplayString(float AValue, char* buffer) override {
    //KODE_PRINT;
    sprintf(buffer,"%i",(int)from01(AValue));
    return buffer;
  }

};

//----------------------------------------------------------------------
#endif
