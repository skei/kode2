#ifndef kode_convert_included
#define kode_convert_included
//----------------------------------------------------------------------

#include <stdio.h>  // sprintf
#include <stdlib.h> // atof

//----------------------------------------------------------------------

//------------------------------
#ifdef KODE_NO_STDLIB
//------------------------------

  #error not implemented

//------------------------------
#else // !KODE_NO_STDLIB
//------------------------------

char* KODE_FloatToString(char* AString, float AValue, uint32_t ADigits=3) {
  char format_string[16];
  format_string[0] = '%';
  format_string[1] = '.';
  format_string[2] = 48 + ADigits;
  format_string[3] = 'f';
  format_string[4] = 0;
  sprintf(AString,format_string,AValue);
  return AString;
}

//----------

char* KODE_IntToString(char* AString, int32_t AValue) {
  char format_string[16];
  format_string[0] = '%';
  format_string[1] = 'i';
  format_string[2] = 0;
  sprintf(AString,format_string,AValue);
  return AString;
}

//----------

float KODE_StringToFloat(char* AString) {
  return atof(AString);
}

//----------

int32_t KODE_StringToInt(char* AString) {
  return atoi(AString);
}

//------------------------------
#endif // KODE_NO_STDLIB
//------------------------------

//----------------------------------------------------------------------
#endif
