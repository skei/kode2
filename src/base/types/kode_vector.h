#ifndef kode_vector_included
#define kode_vector_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/utils/kode_math.h"

#include <math.h>

//----------------------------------------------------------------------
// KODE_Vector2
//----------------------------------------------------------------------

struct KODE_Vector2 {

  union {
    float el[2];
    struct { float x,y; };
  };

  //----------

  KODE_Vector2()                    {}
  KODE_Vector2(float v)             { x=v; y=v; }
  KODE_Vector2(float ax, float ay)  { x=ax; y=ay; }

  KODE_Vector2  operator +  (const KODE_Vector2& rhs)  const { return KODE_Vector2(x+rhs.x,y+rhs.y); }
  KODE_Vector2  operator -  (const KODE_Vector2& rhs)  const { return KODE_Vector2(x-rhs.x,y-rhs.y); }
  KODE_Vector2  operator *  (const KODE_Vector2& rhs)  const { return KODE_Vector2(x*rhs.x,y*rhs.y); }
  KODE_Vector2  operator /  (const KODE_Vector2& rhs)  const { return KODE_Vector2(x/rhs.x,y/rhs.y); }
  KODE_Vector2& operator += (const KODE_Vector2& v)          { x+=v.x; y+=v.y; return *this; }
  KODE_Vector2& operator -= (const KODE_Vector2& v)          { x-=v.x; y-=v.y; return *this; }
  KODE_Vector2& operator *= (const KODE_Vector2& v)          { x*=v.x; y*=v.y; return *this; }
  KODE_Vector2& operator /= (const KODE_Vector2& v)          { x/=v.x; y/=v.y; return *this; }
  KODE_Vector2  operator +  (float s)          const { return KODE_Vector2(x+s,y+s); }
  KODE_Vector2  operator -  (float s)          const { return KODE_Vector2(x-s,y-s); }
  KODE_Vector2  operator *  (float s)          const { return KODE_Vector2(x*s,y*s); }
  KODE_Vector2  operator /  (float s)          const { return KODE_Vector2(x/s,y/s); }
  KODE_Vector2& operator += (const float& s)         { x+=s; y+=s; return *this; }
  KODE_Vector2& operator -= (const float& s)         { x-=s; y-=s; return *this; }
  KODE_Vector2& operator *= (const float& s)         { x*=s; y*=s; return *this; }
  KODE_Vector2& operator /= (const float& s)         { x/=s; y/=s; return *this; }
  KODE_Vector2  operator -  ()                 const { return KODE_Vector2(-x,-y); }

  float dot(KODE_Vector2 v1) {
    return x * v1.x + y * v1.y;
  }

  KODE_Vector2 cross(KODE_Vector2 v1) {
    return KODE_Vector2(
      0,
      0
    );
  }

  void cross(KODE_Vector2 v0, KODE_Vector2 v1) {
    x = 0;
    y = 0;
  }

  float length(void) {
    return sqrtf(x*x + y*y);
  }

  float length_sq(void) {
    return (x*x + y*y);
  }

  void normalize(void) {
    float il = 1.0f / length();
    x *= il;
    y *= il;
  }

  void rotate(float AAngle) {
    float sa = sin(AAngle);
    float ca = cos(AAngle);
    x = (x * ca) - (y * sa);
    y = (x * sa) + (y * ca);
  }

};

//----------------------------------------------------------------------

KODE_Vector2  KODE_Vector2_add(KODE_Vector2 v0, KODE_Vector2 v1)  { KODE_Vector2 r = { v0.x + v1.x, v0.y + v1.y }; return r; }
KODE_Vector2  KODE_Vector2_sub(KODE_Vector2 v0, KODE_Vector2 v1)  { KODE_Vector2 r = { v0.x - v1.x, v0.y - v1.y }; return r; }
KODE_Vector2  KODE_Vector2_mul(KODE_Vector2 v0, KODE_Vector2 v1)  { KODE_Vector2 r = { v0.x * v1.x, v0.y * v1.y }; return r; }
KODE_Vector2  KODE_Vector2_div(KODE_Vector2 v0, KODE_Vector2 v1)  { KODE_Vector2 r = { v0.x / v1.x, v0.y / v1.y }; return r; }

KODE_Vector2  KODE_Vector2_add(KODE_Vector2 v, float f)   { KODE_Vector2 r = { v.x + f, v.y + f }; return r; }
KODE_Vector2  KODE_Vector2_sub(KODE_Vector2 v, float f)   { KODE_Vector2 r = { v.x - f, v.y - f }; return r; }
KODE_Vector2  KODE_Vector2_mul(KODE_Vector2 v, float f)   { KODE_Vector2 r = { v.x * f, v.y * f }; return r; }
KODE_Vector2  KODE_Vector2_div(KODE_Vector2 v, float f)   { KODE_Vector2 r = { v.x / f, v.y / f }; return r; }

float KODE_Vector2_dot(KODE_Vector2 v0, KODE_Vector2 v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y);
}

KODE_Vector2 KODE_Vector2_cross(KODE_Vector2 v0, KODE_Vector2 v1) {
  return KODE_Vector2(
    0,
    0
  );
} // USUALLY FAIL

float KODE_Vector2_length(KODE_Vector2 v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) );
}

float KODE_Vector2_length_sq(KODE_Vector2 v) {
  return ( (v.x*v.x) + (v.y*v.y) );
}

KODE_Vector2 KODE_Vector2_normalize(KODE_Vector2 v) {
  float fi = 1.0f / KODE_Vector2_length(v);
  return KODE_Vector2(
    v.x * fi,
    v.y * fi
  );
}

//----------------------------------------------------------------------
// KODE_Vector3
//----------------------------------------------------------------------

struct KODE_Vector3 {

  union {
    float el[3];
    struct { float x,y,z; };
    struct { float r,g,b; };
    KODE_Vector2 xy;
  };

  //----------

  KODE_Vector3()                              { x=0.0f; y=0.0f; z=0.0f; }
  KODE_Vector3(float v)                       { x=v; y=v; z=v; }
  KODE_Vector3(float ax, float ay, float az)  { x=ax; y=ay; z=az; }
  //KODE_Vector3(KODE_Vector2 axy, float az, float aw)            { x=axy.x; y=axy.y; z=az; w=aw; }
  //KODE_Vector3(KODE_Vector3 axyz, float aw)                     { x=axyz.x; y=axyz.y; z=axyz.z; w=aw; }

  KODE_Vector3  operator +  (const KODE_Vector3& rhs) const { return KODE_Vector3(x+rhs.x,y+rhs.y,z+rhs.z); }
  KODE_Vector3  operator -  (const KODE_Vector3& rhs) const { return KODE_Vector3(x-rhs.x,y-rhs.y,z-rhs.z); }
  KODE_Vector3  operator *  (const KODE_Vector3& rhs) const { return KODE_Vector3(x*rhs.x,y*rhs.y,z*rhs.z); }
  KODE_Vector3  operator /  (const KODE_Vector3& rhs) const { return KODE_Vector3(x/rhs.x,y/rhs.y,z/rhs.z); }
  KODE_Vector3& operator += (const KODE_Vector3& v)         { x+=v.x; y+=v.y; z+=v.z; return *this; }
  KODE_Vector3& operator -= (const KODE_Vector3& v)         { x-=v.x; y-=v.y; z-=v.z; return *this; }
  KODE_Vector3& operator *= (const KODE_Vector3& v)         { x*=v.x; y*=v.y; z*=v.z; return *this; }
  KODE_Vector3& operator /= (const KODE_Vector3& v)         { x/=v.x; y/=v.y; z/=v.z; return *this; }
  KODE_Vector3  operator +  (float s)           const { return KODE_Vector3(x+s,y+s,z+s); }
  KODE_Vector3  operator -  (float s)           const { return KODE_Vector3(x-s,y-s,z-s); }
  KODE_Vector3  operator *  (float s)           const { return KODE_Vector3(x*s,y*s,z*s); }
  KODE_Vector3  operator /  (float s)           const { return KODE_Vector3(x/s,y/s,z/s); }
  KODE_Vector3& operator += (const float& s)          { x+=s; y+=s; z+=s; return *this; }
  KODE_Vector3& operator -= (const float& s)          { x-=s; y-=s; z-=s; return *this; }
  KODE_Vector3& operator *= (const float& s)          { x*=s; y*=s; z*=s; return *this; }
  KODE_Vector3& operator /= (const float& s)          { x/=s; y/=s; z/=s; return *this; }
  KODE_Vector3  operator -  ()                  const { return KODE_Vector3(-x,-y,-z); }

  float dot(KODE_Vector3 v1) {
    return (x * v1.x) + (y * v1.y) + (z * v1.z);
  }

  KODE_Vector3 cross(KODE_Vector3 v1) {
    return KODE_Vector3(
      (y * v1.z) - (z * v1.y),
      (z * v1.x) - (x * v1.z),
      (x * v1.y) - (y * v1.x)
    );
  }

  void cross(KODE_Vector3 v0, KODE_Vector3 v1) {
    x = (v0.y * v1.z) - (v0.z * v1.y);
    y = (v0.z * v1.x) - (v0.x * v1.z);
    z = (v0.x * v1.y) - (v0.y * v1.x);
  }

  float length(void) {
    return sqrtf(x*x + y*y + z*z);
  }

  float length_sq(void) {
    return (x*x + y*y + z*z);
  }

  void normalize(void) {
    float il = 1.0f / length();
    x *= il;
    y *= il;
    z *= il;
  }

};

//----------------------------------------------------------------------

KODE_Vector3  KODE_Vector3_add(KODE_Vector3 v0, KODE_Vector3 v1)  { KODE_Vector3 r = { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z }; return r; }
KODE_Vector3  KODE_Vector3_sub(KODE_Vector3 v0, KODE_Vector3 v1)  { KODE_Vector3 r = { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z }; return r; }
KODE_Vector3  KODE_Vector3_mul(KODE_Vector3 v0, KODE_Vector3 v1)  { KODE_Vector3 r = { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z }; return r; }
KODE_Vector3  KODE_Vector3_div(KODE_Vector3 v0, KODE_Vector3 v1)  { KODE_Vector3 r = { v0.x / v1.x, v0.y / v1.y, v0.z / v1.z }; return r; }

KODE_Vector3  KODE_Vector3_add(KODE_Vector3 v, float f)   { KODE_Vector3 r = { v.x + f, v.y + f, v.z + f }; return r; }
KODE_Vector3  KODE_Vector3_sub(KODE_Vector3 v, float f)   { KODE_Vector3 r = { v.x - f, v.y - f, v.z - f }; return r; }
KODE_Vector3  KODE_Vector3_mul(KODE_Vector3 v, float f)   { KODE_Vector3 r = { v.x * f, v.y * f, v.z * f }; return r; }
KODE_Vector3  KODE_Vector3_div(KODE_Vector3 v, float f)   { KODE_Vector3 r = { v.x / f, v.y / f, v.z / f }; return r; }

float KODE_Vector3_dot(KODE_Vector3 v0, KODE_Vector3 v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y)
       + (v0.z * v1.z);
}

KODE_Vector3 KODE_Vector3_cross(KODE_Vector3 v0, KODE_Vector3 v1) {
  return KODE_Vector3(
    (v0.y * v1.z) - (v0.z * v1.y),
    (v0.z * v1.x) - (v0.x * v1.z),
    (v0.x * v1.y) - (v0.y * v1.x)
  );
}

float KODE_Vector3_length(KODE_Vector3 v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

float KODE_Vector3_length_sq(KODE_Vector3 v) {
  return ( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

KODE_Vector3 KODE_Vector3_normalize(KODE_Vector3 v) {
  float fi = 1.0f / KODE_Vector3_length(v);
  return KODE_Vector3(
    v.x * fi,
    v.y * fi,
    v.z * fi
  );
}

//----------------------------------------------------------------------
#endif
