#ifndef kode_vector_included
#define kode_vector_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/utils/kode_math.h"

#include <math.h>

//----------------------------------------------------------------------
// vec2_t
//----------------------------------------------------------------------

struct vec2_t {

  union {
    float el[2];
    struct { float x,y; };
  };

  //----------

  vec2_t()                    {}
  vec2_t(float v)             { x=v; y=v; }
  vec2_t(float ax, float ay)  { x=ax; y=ay; }

  vec2_t  operator +  (const vec2_t& rhs)  const { return vec2_t(x+rhs.x,y+rhs.y); }
  vec2_t  operator -  (const vec2_t& rhs)  const { return vec2_t(x-rhs.x,y-rhs.y); }
  vec2_t  operator *  (const vec2_t& rhs)  const { return vec2_t(x*rhs.x,y*rhs.y); }
  vec2_t  operator /  (const vec2_t& rhs)  const { return vec2_t(x/rhs.x,y/rhs.y); }
  vec2_t& operator += (const vec2_t& v)          { x+=v.x; y+=v.y; return *this; }
  vec2_t& operator -= (const vec2_t& v)          { x-=v.x; y-=v.y; return *this; }
  vec2_t& operator *= (const vec2_t& v)          { x*=v.x; y*=v.y; return *this; }
  vec2_t& operator /= (const vec2_t& v)          { x/=v.x; y/=v.y; return *this; }
  vec2_t  operator +  (float s)          const { return vec2_t(x+s,y+s); }
  vec2_t  operator -  (float s)          const { return vec2_t(x-s,y-s); }
  vec2_t  operator *  (float s)          const { return vec2_t(x*s,y*s); }
  vec2_t  operator /  (float s)          const { return vec2_t(x/s,y/s); }
  vec2_t& operator += (const float& s)         { x+=s; y+=s; return *this; }
  vec2_t& operator -= (const float& s)         { x-=s; y-=s; return *this; }
  vec2_t& operator *= (const float& s)         { x*=s; y*=s; return *this; }
  vec2_t& operator /= (const float& s)         { x/=s; y/=s; return *this; }
  vec2_t  operator -  ()                 const { return vec2_t(-x,-y); }

  float dot(vec2_t v1) {
    return x * v1.x + y * v1.y;
  }

  vec2_t cross(vec2_t v1) {
    return vec2_t(
      0,
      0
    );
  }

  void cross(vec2_t v0, vec2_t v1) {
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

};

//----------------------------------------------------------------------

vec2_t  KODE_Vector2_add(vec2_t v0, vec2_t v1)  { vec2_t r = { v0.x + v1.x, v0.y + v1.y }; return r; }
vec2_t  KODE_Vector2_sub(vec2_t v0, vec2_t v1)  { vec2_t r = { v0.x - v1.x, v0.y - v1.y }; return r; }
vec2_t  KODE_Vector2_mul(vec2_t v0, vec2_t v1)  { vec2_t r = { v0.x * v1.x, v0.y * v1.y }; return r; }
vec2_t  KODE_Vector2_div(vec2_t v0, vec2_t v1)  { vec2_t r = { v0.x / v1.x, v0.y / v1.y }; return r; }

vec2_t  KODE_Vector2_add(vec2_t v, float f)   { vec2_t r = { v.x + f, v.y + f }; return r; }
vec2_t  KODE_Vector2_sub(vec2_t v, float f)   { vec2_t r = { v.x - f, v.y - f }; return r; }
vec2_t  KODE_Vector2_mul(vec2_t v, float f)   { vec2_t r = { v.x * f, v.y * f }; return r; }
vec2_t  KODE_Vector2_div(vec2_t v, float f)   { vec2_t r = { v.x / f, v.y / f }; return r; }

float KODE_Vector2_dot(vec2_t v0, vec2_t v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y);
}

vec2_t KODE_Vector2_cross(vec2_t v0, vec2_t v1) {
  return vec2_t(
    0,
    0
  );
} // USUALLY FAIL

float KODE_Vector2_length(vec2_t v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) );
}

float KODE_Vector2_length_sq(vec2_t v) {
  return ( (v.x*v.x) + (v.y*v.y) );
}

vec2_t KODE_Vector2_normalize(vec2_t v) {
  float fi = 1.0f / KODE_Vector2_length(v);
  return vec2_t(
    v.x * fi,
    v.y * fi
  );
}

//----------------------------------------------------------------------
// vec3_t
//----------------------------------------------------------------------

struct vec3_t {

  union {
    float el[3];
    struct { float x,y,z; };
    struct { float r,g,b; };
    vec2_t xy;
  };

  //----------

  vec3_t()                              { x=0.0f; y=0.0f; z=0.0f; }
  vec3_t(float v)                       { x=v; y=v; z=v; }
  vec3_t(float ax, float ay, float az)  { x=ax; y=ay; z=az; }
  //vec3_t(vec2_t axy, float az, float aw)            { x=axy.x; y=axy.y; z=az; w=aw; }
  //vec3_t(vec3_t axyz, float aw)                     { x=axyz.x; y=axyz.y; z=axyz.z; w=aw; }

  vec3_t  operator +  (const vec3_t& rhs) const { return vec3_t(x+rhs.x,y+rhs.y,z+rhs.z); }
  vec3_t  operator -  (const vec3_t& rhs) const { return vec3_t(x-rhs.x,y-rhs.y,z-rhs.z); }
  vec3_t  operator *  (const vec3_t& rhs) const { return vec3_t(x*rhs.x,y*rhs.y,z*rhs.z); }
  vec3_t  operator /  (const vec3_t& rhs) const { return vec3_t(x/rhs.x,y/rhs.y,z/rhs.z); }
  vec3_t& operator += (const vec3_t& v)         { x+=v.x; y+=v.y; z+=v.z; return *this; }
  vec3_t& operator -= (const vec3_t& v)         { x-=v.x; y-=v.y; z-=v.z; return *this; }
  vec3_t& operator *= (const vec3_t& v)         { x*=v.x; y*=v.y; z*=v.z; return *this; }
  vec3_t& operator /= (const vec3_t& v)         { x/=v.x; y/=v.y; z/=v.z; return *this; }
  vec3_t  operator +  (float s)           const { return vec3_t(x+s,y+s,z+s); }
  vec3_t  operator -  (float s)           const { return vec3_t(x-s,y-s,z-s); }
  vec3_t  operator *  (float s)           const { return vec3_t(x*s,y*s,z*s); }
  vec3_t  operator /  (float s)           const { return vec3_t(x/s,y/s,z/s); }
  vec3_t& operator += (const float& s)          { x+=s; y+=s; z+=s; return *this; }
  vec3_t& operator -= (const float& s)          { x-=s; y-=s; z-=s; return *this; }
  vec3_t& operator *= (const float& s)          { x*=s; y*=s; z*=s; return *this; }
  vec3_t& operator /= (const float& s)          { x/=s; y/=s; z/=s; return *this; }
  vec3_t  operator -  ()                  const { return vec3_t(-x,-y,-z); }

  float dot(vec3_t v1) {
    return (x * v1.x) + (y * v1.y) + (z * v1.z);
  }

  vec3_t cross(vec3_t v1) {
    return vec3_t(
      (y * v1.z) - (z * v1.y),
      (z * v1.x) - (x * v1.z),
      (x * v1.y) - (y * v1.x)
    );
  }

  void cross(vec3_t v0, vec3_t v1) {
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

vec3_t  KODE_Vector3_add(vec3_t v0, vec3_t v1)  { vec3_t r = { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z }; return r; }
vec3_t  KODE_Vector3_sub(vec3_t v0, vec3_t v1)  { vec3_t r = { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z }; return r; }
vec3_t  KODE_Vector3_mul(vec3_t v0, vec3_t v1)  { vec3_t r = { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z }; return r; }
vec3_t  KODE_Vector3_div(vec3_t v0, vec3_t v1)  { vec3_t r = { v0.x / v1.x, v0.y / v1.y, v0.z / v1.z }; return r; }

vec3_t  KODE_Vector3_add(vec3_t v, float f)   { vec3_t r = { v.x + f, v.y + f, v.z + f }; return r; }
vec3_t  KODE_Vector3_sub(vec3_t v, float f)   { vec3_t r = { v.x - f, v.y - f, v.z - f }; return r; }
vec3_t  KODE_Vector3_mul(vec3_t v, float f)   { vec3_t r = { v.x * f, v.y * f, v.z * f }; return r; }
vec3_t  KODE_Vector3_div(vec3_t v, float f)   { vec3_t r = { v.x / f, v.y / f, v.z / f }; return r; }

float KODE_Vector3_dot(vec3_t v0, vec3_t v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y)
       + (v0.z * v1.z);
}

vec3_t KODE_Vector3_cross(vec3_t v0, vec3_t v1) {
  return vec3_t(
    (v0.y * v1.z) - (v0.z * v1.y),
    (v0.z * v1.x) - (v0.x * v1.z),
    (v0.x * v1.y) - (v0.y * v1.x)
  );
}

float KODE_Vector3_length(vec3_t v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

float KODE_Vector3_length_sq(vec3_t v) {
  return ( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

vec3_t KODE_Vector3_normalize(vec3_t v) {
  float fi = 1.0f / KODE_Vector3_length(v);
  return vec3_t(
    v.x * fi,
    v.y * fi,
    v.z * fi
  );
}

//----------------------------------------------------------------------
#endif
