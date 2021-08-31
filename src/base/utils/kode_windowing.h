#ifndef kode_windowing_included
#define kode_windowing_included
//----------------------------------------------------------------------

#include <math.h>

/*
float KODE_Windowing_Rect(float pos);
float KODE_Windowing_Hann(float pos);
float KODE_Windowing_Hamming(float pos);
float KODE_Windowing_Blackman(float pos);
float KODE_Windowing_Gaussian(float pos);
float KODE_Windowing_Welch(float pos);
float KODE_Windowing_Bartlett(float pos);
*/

//----------------------------------------------------------------------

float KODE_Windowing_Rect(float pos) {
  return 1.0;
}

float KODE_Windowing_Hann(float pos) {
  float x = cosf(pos*KODE_PI/2.0);
  return x*x;
}

float KODE_Windowing_Hamming(float pos) {
  return 0.54 + 0.46 * cosf(KODE_PI*pos);
}

float KODE_Windowing_Blackman(float pos) {
  return 0.42 + 0.5 * cos(KODE_PI*pos) + 0.08 * cosf(2.0*KODE_PI*pos);
}

float KODE_Windowing_Gaussian(float pos) {
  float a = 4.0;
  float ax = a * pos;
  return exp(-0.5 * ax*ax);
}

float KODE_Windowing_Welch(float pos) {
  return 1.0 - pos*pos;
}

float KODE_Windowing_Bartlett(float pos) {
 if (pos == 0.0) return 1.0;
 else if (pos > -1.0 && pos < 0.0) return pos + 1.0;
 else if (pos > 0.0 && pos < 1.0) return 1.0 - pos;
 else return 0.0;
}

//----------------------------------------------------------------------
#endif
