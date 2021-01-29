#ifndef kode_color_included
#define kode_color_included
//----------------------------------------------------------------------

#include "kode.h"

class KODE_Color {

public:

  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 1.0f;

//------------------------------
public:
//------------------------------

  KODE_Color() {}

  //----------

  KODE_Color(float v) {
    r = v;
    g = v;
    b = v;
    a = 1.0f;
  }

  //----------

  KODE_Color(float ar, float ag, float ab, float aa=1.0f) {
    r = ar;
    g = ag;
    b = ab;
    a = aa;
  }

//------------------------------
public:
//------------------------------

//  operator uint32_t () {
//    uint8_t rr = r * 255.0f;
//    uint8_t gg = g * 255.0f;
//    uint8_t bb = b * 255.0f;
//    uint8_t aa = a * 255.0f;
//    uint32_t color = (aa << 24) + (rr << 16) + (gg << 8) + bb;
//    return color;
//  }

//  KODE_Color& operator = (uint32_t color) {
//    float aa = (color & 0xff000000) >> 24;
//    float rr = (color & 0x00ff0000) >> 16;
//    float gg = (color & 0x0000ff00) >> 8;
//    float bb = (color & 0x000000ff);
//    r = rr * KODE_INV255F;
//    g = gg * KODE_INV255F;
//    b = bb * KODE_INV255F;
//    a = aa * KODE_INV255F;
//    return *this;
//  }

  KODE_Color& operator += (uint32_t color) {
    return *this;
  }

  //bool operator == (const char* str) {
  //  return (KODE_Strcmp(MBuffer,str) == 0) ? true : false;
  //}

//------------------------------
public:
//------------------------------

  void add(KODE_Color color) {
    r += color.r;
    g += color.g;
    b += color.b;
    a += color.a;
  }

};

//----------------------------------------------------------------------
#endif
