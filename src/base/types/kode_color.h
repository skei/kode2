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

  operator uint32_t () {
    uint8_t rr = r * 255.0f;
    uint8_t gg = g * 255.0f;
    uint8_t bb = b * 255.0f;
    uint8_t aa = a * 255.0f;
    uint32_t color = (aa << 24) + (rr << 16) + (gg << 8) + bb;
    return color;
  }

  KODE_Color& operator = (uint32_t color) {
    float aa = (color & 0xff000000) >> 24;
    float rr = (color & 0x00ff0000) >> 16;
    float gg = (color & 0x0000ff00) >> 8;
    float bb = (color & 0x000000ff);
    r = rr * KODE_INV255F;
    g = gg * KODE_INV255F;
    b = bb * KODE_INV255F;
    a = aa * KODE_INV255F;
    return *this;
  }

  KODE_Color& operator += (uint32_t color) {
    return *this;
  }

  //bool operator == (const char* str) {
  //  return (strcmp(MBuffer,str) == 0) ? true : false;
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

  void mul(KODE_Color color) {
    r *= color.r;
    g *= color.g;
    b *= color.b;
    a *= color.a;
  }

  void blend(KODE_Color color1, KODE_Color color2, float blend) {
    KODE_Color result;
    result.r = (color1.r * blend) + (color2.r * (1.0 - blend));
    result.g = (color1.g * blend) + (color2.g * (1.0 - blend));
    result.b = (color1.b * blend) + (color2.b * (1.0 - blend));
    result.a = (color1.a * blend) + (color2.a * (1.0 - blend));
  }

  // color2 on top of color1 (alpha from color2)

  void blend(KODE_Color color1, KODE_Color color2) {
    KODE_Color result;
    float blend = color2.a;
    result.r = (color1.r * blend) + (color2.r * (1.0 - blend));
    result.g = (color1.g * blend) + (color2.g * (1.0 - blend));
    result.b = (color1.b * blend) + (color2.b * (1.0 - blend));
    result.a = color1.a;// * blend) + (color2.a * (1.0 - blend));
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

const KODE_Color KODE_COLOR_DARK_RED        = KODE_Color( 0.25, 0.00, 0.00 );
const KODE_Color KODE_COLOR_RED             = KODE_Color( 0.50, 0.00, 0.00 );
const KODE_Color KODE_COLOR_LIGHT_RED       = KODE_Color( 0.75, 0.00, 0.00 );
const KODE_Color KODE_COLOR_BRIGHT_RED      = KODE_Color( 1.00, 0.00, 0.00 );

const KODE_Color KODE_COLOR_DARK_ORANGE     = KODE_Color( 0.25, 0.12, 0.00 );
const KODE_Color KODE_COLOR_ORANGE          = KODE_Color( 0.50, 0.25, 0.00 );
const KODE_Color KODE_COLOR_LIGHT_ORANGE    = KODE_Color( 0.75, 0.37, 0.00 );
const KODE_Color KODE_COLOR_BRIGHT_ORANGE   = KODE_Color( 1.00, 0.50, 0.00 );

const KODE_Color KODE_COLOR_DARK_YELLOW     = KODE_Color( 0.25, 0.25, 0.00 );
const KODE_Color KODE_COLOR_YELLOW          = KODE_Color( 0.50, 0.50, 0.00 );
const KODE_Color KODE_COLOR_LIGHT_YELLOW    = KODE_Color( 0.75, 0.75, 0.00 );
const KODE_Color KODE_COLOR_BRIGHT_YELLOW   = KODE_Color( 1.00, 1.00, 0.00 );

const KODE_Color KODE_COLOR_DARK_LIME       = KODE_Color( 0.12, 0.25, 0.00 );
const KODE_Color KODE_COLOR_LIME            = KODE_Color( 0.25, 0.50, 0.00 );
const KODE_Color KODE_COLOR_LIGHT_LIME      = KODE_Color( 0.37, 0.75, 0.00 );
const KODE_Color KODE_COLOR_BRIGHT_LIME     = KODE_Color( 0.50, 1.00, 0.00 );


const KODE_Color KODE_COLOR_DARK_GREEN      = KODE_Color( 0.00, 0.25, 0.00 );
const KODE_Color KODE_COLOR_GREEN           = KODE_Color( 0.00, 0.50, 0.00 );
const KODE_Color KODE_COLOR_LIGHT_GREEN     = KODE_Color( 0.00, 0.75, 0.00 );
const KODE_Color KODE_COLOR_BRIGHT_GREEN    = KODE_Color( 0.00, 1.00, 0.00 );

const KODE_Color KODE_COLOR_DARK_GREEN2     = KODE_Color( 0.00, 0.25, 0.12 );
const KODE_Color KODE_COLOR_GREEN2          = KODE_Color( 0.00, 0.50, 0.35 );
const KODE_Color KODE_COLOR_LIGHT_GREEN2    = KODE_Color( 0.00, 0.75, 0.37 );
const KODE_Color KODE_COLOR_BRIGHT_GREEN2   = KODE_Color( 0.00, 1.00, 0.50 );

const KODE_Color KODE_COLOR_DARK_CYAN       = KODE_Color( 0.00, 0.25, 0.25 );
const KODE_Color KODE_COLOR_CYAN            = KODE_Color( 0.00, 0.50, 0.50 );
const KODE_Color KODE_COLOR_LIGHT_CYAN      = KODE_Color( 0.00, 0.75, 0.75 );
const KODE_Color KODE_COLOR_BRIGHT_CYAN     = KODE_Color( 0.00, 1.00, 1.00 );

const KODE_Color KODE_COLOR_DARK_CYAN2       = KODE_Color( 0.00, 0.18, 0.25 );
const KODE_Color KODE_COLOR_CYAN2            = KODE_Color( 0.00, 0.37, 0.50 );
const KODE_Color KODE_COLOR_LIGHT_CYAN2      = KODE_Color( 0.00, 0.50, 0.75 );
const KODE_Color KODE_COLOR_BRIGHT_CYAN2     = KODE_Color( 0.00, 0.75, 1.00 );

const KODE_Color KODE_COLOR_DARK_BLUE1      = KODE_Color( 0.00, 0.12, 0.25 );
const KODE_Color KODE_COLOR_BLUE1           = KODE_Color( 0.00, 0.25, 0.50 );
const KODE_Color KODE_COLOR_LIGHT_BLUE1     = KODE_Color( 0.00, 0.37, 0.75 );
const KODE_Color KODE_COLOR_BRIGHT_BLUE1    = KODE_Color( 0.00, 0.50, 1.00 );

const KODE_Color KODE_COLOR_DARK_BLUE       = KODE_Color( 0.00, 0.00, 0.25 );
const KODE_Color KODE_COLOR_BLUE            = KODE_Color( 0.00, 0.00, 0.50 );
const KODE_Color KODE_COLOR_LIGHT_BLUE      = KODE_Color( 0.00, 0.00, 0.75 );
const KODE_Color KODE_COLOR_BRIGHT_BLUE     = KODE_Color( 0.00, 0.00, 1.00 );

const KODE_Color KODE_COLOR_DARK_BLUE2      = KODE_Color( 0.12, 0.00, 0.25 );
const KODE_Color KODE_COLOR_BLUE2           = KODE_Color( 0.25, 0.00, 0.50 );
const KODE_Color KODE_COLOR_LIGHT_BLUE2     = KODE_Color( 0.37, 0.00, 0.75 );
const KODE_Color KODE_COLOR_BRIGHT_BLUE2    = KODE_Color( 0.50, 0.00, 1.00 );

const KODE_Color KODE_COLOR_DARK_MAGENTA    = KODE_Color( 0.25, 0.00, 0.25 );
const KODE_Color KODE_COLOR_MAGENTA         = KODE_Color( 0.50, 0.00, 0.50 );
const KODE_Color KODE_COLOR_LIGHT_MAGENTA   = KODE_Color( 0.75, 0.00, 0.75 );
const KODE_Color KODE_COLOR_BRIGHT_MAGENTA  = KODE_Color( 1.00, 0.00, 1.00 );

const KODE_Color KODE_COLOR_DARK_RED2       = KODE_Color( 0.25, 0.00, 0.12 );
const KODE_Color KODE_COLOR_RED2            = KODE_Color( 0.50, 0.00, 0.25 );
const KODE_Color KODE_COLOR_LIGHT_RED2      = KODE_Color( 0.75, 0.00, 0.37 );
const KODE_Color KODE_COLOR_BRIGHT_RED2     = KODE_Color( 1.00, 0.00, 0.50 );


/*
const KODE_Color KODE_COLOR_                = KODE_Color( 0.00, 0.00, 0.00 );
*/

const KODE_Color KODE_COLOR_BLACK           = KODE_Color( 0.00 );
const KODE_Color KODE_COLOR_DARK_GRAY       = KODE_Color( 0.25 );
const KODE_Color KODE_COLOR_GRAY            = KODE_Color( 0.50 );
const KODE_Color KODE_COLOR_LIGHT_GRAY      = KODE_Color( 0.75 );
const KODE_Color KODE_COLOR_WHITE           = KODE_Color( 1.00 );

//----------------------------------------------------------------------
#endif
