#ifndef kode_audio_math_included
#define kode_audio_math_included
//----------------------------------------------------------------------

#include <math.h>

//----------------------------------------------------------------------
// db
//----------------------------------------------------------------------

float KODE_DbToVolume(float db) {
  return powf(10.0f,0.05f*db);
}

float KODE_VolumeToDb(float volume) {
  return 20.0f * log10f(volume);
}

//----------

//float KODE_DbToAmp(float g) {
//  if (g > -144.0) return exp(g * 0.115129254);
//  else return 0;
//}

//double dbtoa(double db) {
//  return pow(2, db/6);
//}

//double atodb(double a) {
//  return 20*log10(a);
//}

//  converts linear value to decibel
//    float dc_signal = 1.f;
//    float amp_half = 0.5f;
//    dc_signal *= amp_half;                  // half the amplitude
//    float amp_db = KLin2DB(amp_half);      // amp_db = -6dbFS
//#define KODE_Lin2DB(lin) ( LOG2DB*KODE_Logf( (lin) ) )

//converts decibel value to linear
//  float amp_db = 20.f;         // 20 decibels
//  signal *= KDB2Lin(amp_db);  // *= ~10.079
//#define KODE_Db2Lin(dB) ( KODE_Expf( DB2LOG*(dB) ) )

//----------------------------------------------------------------------
// denormal
//----------------------------------------------------------------------

float KODE_KillDenormal(float n) {
  /*register*/ union
  {
    float f;
    uint32_t i; // unsigned int
  } u;
  u.f = n;
  if ( !(u.i & 0x7f800000) ) n = 0.f;
  return n;
}

// denormalization killing with dc as presented by Laurent de Soras in
// http://ldesoras.free.fr/doc/articles/denormal-en.pdf

float KODE_KillDenormal2(float val) {
  // static const float antidenormal_dc = 1e-20;
  static const float antidenormal_dc = 0.000000000000000001f;
  return val + antidenormal_dc;
}

//----------

//// http://www.kvraudio.com/forum/viewtopic.php?p=4817458#p4817458
//// kode_const.h: KODE_DENORM = 1.4012984e-45;
//
//const double _denorm = 1.0e-18;//1.0e-56;
//float KODE_KillDenormal(float v) {
//  v += _denorm;
//  v -= _denorm;
//  return v;
//}

//function KODE_KillDenormal(value : Single) : Single;
//begin
//  if (abs(value) < 1.0e-20) then Result := 0.0
//  else Result := value;
//end;

//----------

//----------------------------------------------------------------------
// hz
//----------------------------------------------------------------------

float KODE_NoteToHz(float ANote) {
  return 440 * pow(2.0,(ANote-69)*KODE_INV12F);
}

//----------

float KODE_HzToSamples(float AHertz, float ASampleRate) {
  KODE_Assert (AHertz > 0.0f);
  return ASampleRate / AHertz;
}

//----------------------------------------------------------------------
#endif