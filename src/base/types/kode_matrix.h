#ifndef kode_matrix_included
#define kode_matrix_included
//----------------------------------------------------------------------

/*
  see:
  https://github.com/Gargaj/livin_in_a_box-w32/tree/master/src
*/

//----------------------------------------------------------------------

#include "kode_vector.h"

// /home/skei/Desktop/DATA/code/_unsorted/demoscene/jfyeNG - lightwave scene player/sources/platform/renderer/software/matrix.c

/*
  m00  m01  m02  m03
  m10  m11  m12  m13
  m20  m21  m22  m23
  m30  m31  m32  m33

  m[ 0]     m[ 1]     m[ 2]     m[ 3]
  m[ 4]     m[ 5]     m[ 6]     m[ 7]
  m[ 8]     m[ 9]     m[10]     m[11]
  m[12]     m[13]     m[14]     m[15]

  mm[0][0]  mm[0][1]  mm[0][2]  mm[0][3]
  mm[1][0]  mm[1][1]  mm[1][2]  mm[1][3]
  mm[2][0]  mm[2][1]  mm[2][2]  mm[2][3]
  mm[3][0]  mm[3][1]  mm[3][2]  mm[3][3]

*/

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

struct KODE_Matrix4;

KODE_Matrix4 KODE_Matrix4_identity(void);
KODE_Matrix4 KODE_Matrix4_rotateX(float xa);
KODE_Matrix4 KODE_Matrix4_rotateY(float ya);
KODE_Matrix4 KODE_Matrix4_rotateZ(float za);
KODE_Matrix4 KODE_Matrix4_rotate(float xa, float ya, float za);
KODE_Matrix4 KODE_Matrix4_translate(float tx, float ty, float tz);
KODE_Matrix4 KODE_Matrix4_scale(float sx, float sy, float sz);
KODE_Matrix4 KODE_Matrix4_frustum(float left, float right, float bottom, float top, float znear, float zfar);
KODE_Matrix4 KODE_Matrix4_ortho(float left, float right, float bottom, float top, float znear, float zfar);
KODE_Matrix4 KODE_Matrix4_mul(KODE_Matrix4 m1, KODE_Matrix4 m2);
KODE_Vector3 KODE_Matrix4_transform(KODE_Vector3 v);
KODE_Matrix4 KODE_Matrix4_lookat(KODE_Vector3 from, KODE_Vector3 to, KODE_Vector3 tmp = KODE_Vector3(0,1,0));

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

struct KODE_Matrix4 {

  union {
    struct {
      float m00, m01, m02, m03;
      float m10, m11, m12, m13;
      float m20, m21, m22, m23;
      float m30, m31, m32, m33;
    };
    float m[16];
    float mm[4][4];
  };

  //----------

  KODE_Matrix4() {}

  //KODE_Matrix4(KODE_Matrix4 m2) { copy(m2); }

  KODE_Matrix4(float v) {
    m00=v; m01=v; m02=v; m03=v;
    m10=v; m11=v; m12=v; m13=v;
    m20=v; m21=v; m12=v; m23=v;
    m30=v; m31=v; m32=v; m33=v;
  }

  KODE_Matrix4( float m_00, float m_01, float m_02, float m_03,
        float m_10, float m_11, float m_12, float m_13,
        float m_20, float m_21, float m_22, float m_23,
        float m_30, float m_31, float m_32, float m_33) {
    m00=m_00; m01=m_01; m02=m_02; m03=m_03;
    m10=m_10; m11=m_11; m12=m_12; m13=m_13;
    m20=m_20; m21=m_21; m22=m_22; m23=m_23;
    m30=m_30; m31=m_31; m32=m_32; m33=m_33;
  }

  //----------

  KODE_Matrix4 operator * (const KODE_Matrix4& rhs)  const {
    KODE_Matrix4 temp = KODE_Matrix4(
      m00, m01, m02, m03,
      m10, m11, m12, m13,
      m20, m21, m12, m23,
      m30, m31, m12, m33
    );
    temp.mul(rhs);
    return temp;
  }

  KODE_Matrix4& operator *= (const KODE_Matrix4& v) {
    mul(v);
    return *this;
  }

  //----------

  void copy(KODE_Matrix4 m2) {
    for (uint32_t i=0; i<16; i++) m[i] = m2.m[i];
  }

  void identity(void) {
    m00 = 1.0f;  m01 = 0.0f;  m02 = 0.0f;  m03 = 0.0f;
    m10 = 0.0f;  m11 = 1.0f;  m12 = 0.0f;  m13 = 0.0f;
    m20 = 0.0f;  m21 = 0.0f;  m22 = 1.0f;  m23 = 0.0f;
    m30 = 0.0f;  m31 = 0.0f;  m32 = 0.0f;  m33 = 1.0f;
  }

  void rotateX(float xa) {
    identity();
    float sinAngle = sinf(xa);
    float cosAngle = cosf(xa);
    m11 = cosAngle;
    m12 = sinAngle;
    m21 = -sinAngle;
    m22 = cosAngle;
  }

  void rotateY(float ya) {
    identity();
    float sinAngle = sinf(ya);
    float cosAngle = cosf(ya);
    m00 = cosAngle;
    m02 = -sinAngle;
    m20 = sinAngle;
    m22 = cosAngle;
  }

  void rotateZ(float za) {
    identity();
    float sinAngle = sinf(za);
    float cosAngle = cosf(za);
    m00 = cosAngle;
    m01 = sinAngle;
    m10 = -sinAngle;
    m11 = cosAngle;
  }

  void rotate(float xa, float ya, float za) {
    KODE_Matrix4 temp;
    temp.rotateX(xa);
    mul(temp);
    temp.rotateY(ya);
    mul(temp);
    temp.rotateZ(za);
    mul(temp);
    copy(temp);
  }



  void translate(float tx, float ty, float tz) {
    identity();
    m03 = tx;
    m13 = ty;
    m23 = tz;
}

  void scale(float sx, float sy, float sz) {
    identity();
    m00 = sx;
    m11 = sy;
    m22 = sz;
  }

  void frustum(float left, float right, float bottom, float top, float znear, float zfar) {
    identity();
	  m00 = (2.0f*znear)/(right-left);
    m02 = (right+left)/(right-left);
    m11 = (2.0f*znear)/(top-bottom);
    m12 = (top+bottom)/(top-bottom);
    m22 = -(zfar+znear)/(zfar-znear);
    m23 = -(2.0f*zfar*znear)/(zfar-znear);
    m32 = -1.0f;
  }

  void ortho(float left, float right, float bottom, float top, float znear, float zfar) {
    identity();
    m00 = 2.0f/(right-left);
    m03 = (right+left)/(right-left);
    m11 = 2.0f/(top-bottom);
    m13 = (top+bottom)/(top-bottom);
    m22 = -2.0f/(zfar-znear);
    m23 = (zfar+znear)/(zfar-znear);
    m33 = 1.0f;
  }

  //----------

  // premult? postmult?

  void mul(KODE_Matrix4 m2) {
	  KODE_Matrix4 temp;
    for (uint32_t c=0; c<4; c++) {
      for (uint32_t l=0; l<4; l++) {
        temp.mm[c][l] =
          mm[0][l] * m2.mm[c][0]
        + mm[1][l] * m2.mm[c][1]
        + mm[2][l] * m2.mm[c][2]
        + mm[3][l] * m2.mm[c][3];
      }
    }
    copy(temp);
  }

  KODE_Vector3 transform(KODE_Vector3 v) {
    float x = v.x*m00 + v.y*m01 + v.z*m02 + m03;
    float y = v.x*m10 + v.y*m11 + v.z*m12 + m13;
    float z = v.x*m20 + v.y*m21 + v.z*m22 + m23;
    return KODE_Vector3(x,y,z);
  }

  void lookat(KODE_Vector3 eye, KODE_Vector3 target, KODE_Vector3 up = KODE_Vector3(0,1,0)) {
    KODE_Vector3 forward  = KODE_Vector3_normalize(target - eye);
    KODE_Vector3 right    = KODE_Vector3_cross(up, forward);
    KODE_Vector3 up_      = KODE_Vector3_cross(forward, right);
    m00 = right.x;  m10 = up_.x;  m20 = forward.x;  m30 = eye.x;
    m01 = right.y;  m11 = up_.y;  m21 = forward.y;  m31 = eye.y;
    m02 = right.z;  m12 = up_.z;  m22 = forward.z;  m32 = eye.z;
    m03 = 0.0f;     m13 = 0.0f;  m23 = 0.0f;        m33 = 1.0f;
  }


};

//----------------------------------------------------------------------

KODE_Matrix4 KODE_Matrix4_identity(void) {
  return KODE_Matrix4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  );
}

KODE_Matrix4 KODE_Matrix4_rotateX(float xa) {
  KODE_Matrix4 temp = KODE_Matrix4_identity();
  float sinAngle = sinf(xa);
  float cosAngle = cosf(xa);
  temp.m11 = cosAngle;
  temp.m12 = sinAngle;
  temp.m21 = -sinAngle;
  temp.m22 = cosAngle;
  return temp;
}

KODE_Matrix4 KODE_Matrix4_rotateY(float ya) {
  KODE_Matrix4 temp = KODE_Matrix4_identity();
  float sinAngle = sinf(ya);
  float cosAngle = cosf(ya);
  temp.m00 = cosAngle;
  temp.m02 = -sinAngle;
  temp.m20 = sinAngle;
  temp.m22 = cosAngle;
  return temp;
}

KODE_Matrix4 KODE_Matrix4_rotateZ(float za) {
  KODE_Matrix4 temp = KODE_Matrix4_identity();
  float sinAngle = sinf(za);
  float cosAngle = cosf(za);
  temp.m00 = cosAngle;
  temp.m01 = sinAngle;
  temp.m10 = -sinAngle;
  temp.m11 = cosAngle;
  return temp;
}

KODE_Matrix4 KODE_Matrix4_rotate(float xa, float ya, float za) {
  KODE_Matrix4 rx = KODE_Matrix4_rotateX(xa);
  KODE_Matrix4 ry = KODE_Matrix4_rotateY(ya);
  KODE_Matrix4 rz = KODE_Matrix4_rotateZ(za);
  KODE_Matrix4 temp = rx;
  temp = KODE_Matrix4_mul(temp,ry);
  temp = KODE_Matrix4_mul(temp,rz);
  return temp;
}



KODE_Matrix4 KODE_Matrix4_translate(float tx, float ty, float tz) {
  KODE_Matrix4 temp = KODE_Matrix4_identity();
  temp.m03 = tx;
  temp.m13 = ty;
  temp.m23 = tz;
  return temp;
}

KODE_Matrix4 KODE_Matrix4_scale(float sx, float sy, float sz) {
  KODE_Matrix4 temp = KODE_Matrix4_identity();
  temp.m00 = sx;
  temp.m11 = sy;
  temp.m22 = sz;
  return temp;
}

KODE_Matrix4 KODE_Matrix4_frustum(float left, float right, float bottom, float top, float znear, float zfar) {
  KODE_Matrix4 temp = KODE_Matrix4_identity();
  temp.m00 = (2.0f*znear)/(right-left);
  temp.m02 = (right+left)/(right-left);
  temp.m11 = (2.0f*znear)/(top-bottom);
  temp.m12 = (top+bottom)/(top-bottom);
  temp.m22 = -(zfar+znear)/(zfar-znear);
  temp.m23 = -(2.0f*zfar*znear)/(zfar-znear);
  temp.m32 = -1.0f;
  return temp;
}

KODE_Matrix4 KODE_Matrix4_ortho(float left, float right, float bottom, float top, float znear, float zfar) {
  KODE_Matrix4 temp = KODE_Matrix4_identity();
  temp.m00 = 2.0f/(right-left);
  temp.m03 = (right+left)/(right-left);
  temp.m11 = 2.0f/(top-bottom);
  temp.m13 = (top+bottom)/(top-bottom);
  temp.m22 = -2.0f/(zfar-znear);
  temp.m23 = (zfar+znear)/(zfar-znear);
  temp.m33 = 1.0f;
  return temp;
}

//----------

// premult? postmult?

KODE_Matrix4 KODE_Matrix4_mul(KODE_Matrix4 m1, KODE_Matrix4 m2) {
  KODE_Matrix4 temp;
  for (uint32_t c=0; c<4; c++) {
    for (uint32_t l=0; l<4; l++) {
      temp.mm[c][l] =
        m1.mm[0][l] * m2.mm[c][0]
      + m1.mm[1][l] * m2.mm[c][1]
      + m1.mm[2][l] * m2.mm[c][2]
      + m1.mm[3][l] * m2.mm[c][3];
    }
  }
  return temp;
}

KODE_Vector3 KODE_Matrix4_transform(KODE_Vector3 v, KODE_Matrix4 m) {
  float x = v.x*m.m00 + v.y*m.m01 + v.z*m.m02 + m.m03;
  float y = v.x*m.m10 + v.y*m.m11 + v.z*m.m12 + m.m13;
  float z = v.x*m.m20 + v.y*m.m21 + v.z*m.m22 + m.m23;
  return KODE_Vector3(x,y,z);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

KODE_Matrix4 KODE_Matrix4_lookat(KODE_Vector3 eye, KODE_Vector3 target, KODE_Vector3 up) {
  //KODE_Vector3 forward  = KODE_Vector3_normalize(eye - target);
  KODE_Vector3 forward  = KODE_Vector3_normalize(target-eye);
  KODE_Vector3 right    = KODE_Vector3_cross(forward, KODE_Vector3_normalize(up) );
  KODE_Vector3 up_      = KODE_Vector3_cross(right, forward);
  KODE_Matrix4 result = KODE_Matrix4(
    right.x,  up_.x,  forward.x,  eye.x,
    right.y,  up_.y,  forward.y,  eye.y,
    right.z,  up_.z,  forward.z,  eye.z,
    0.0f,     0.0f,   0.0f,       1.0f
  );
  return result;
  //return KODE_Matrix4_mul(result, KODE_Matrix4_translate(KODE_Vector3_mul(eye, -1.0f)));
}

//----------

/*
  x = (x*p)/z
  y = (y*p)/z

  1)

  x = x*p
  y = y*p

  p/z 0   0   0
  0   p/z 0   0


*/

/*
KODE_Matrix4 KODE_Matrix4_perspective(float d) {
  if (d>0) {
    return KODE_Matrix4(
      1.0f/d, 0.0f,   0.0f,   0.0f,
      0.0f,   1.0f/d, 0.0f,   0.0f,
      0.0f,   0.0f,   1.0f,   0.0f,
      0.0f,   0.0f,   0.0f,   0.0f
    );
  }
  else return KODE_Matrix4_identity();
}
*/

//----------------------------------------------------------------------
#endif
