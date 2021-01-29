#ifndef kode_types_included
#define kode_types_included
//----------------------------------------------------------------------

#include <stdint.h>

//#ifdef _STDINT_H
//  typedef uint8_t             uint8;
//  typedef uint16_t            uint16;
//  typedef uint32_t            uint32;
//  typedef uint64_t            uint64;
//  typedef int8_t              int8;
//  typedef int16_t             int16;
//  typedef int32_t             int32;
//  typedef int64_t             int64;
//  typedef intptr_t            intptr;
//#else
//  typedef unsigned char       uint8;
//  typedef unsigned short      uint16;
//  typedef unsigned long       uint32;
//  typedef unsigned long long  uint64;
//  typedef signed char         int8;
//  typedef signed short        int16;
//  typedef signed long         int32;
//  typedef signed long long    int64;
//  typedef intptr_t            intptr;
//#endif

//----------

#ifdef KODE_CPP11
  #define KODE_NULL nullptr
#else
  #define KODE_NULL 0
#endif

//----------------------------------------------------------------------

#include "base/types/kode_array.h"
#include "base/types/kode_buffer.h"
#include "base/types/kode_complex.h"
#include "base/types/kode_list.h"
#include "base/types/kode_queue.h"
#include "base/types/kode_stack.h"
#include "base/types/kode_tree.h"

#include "base/types/kode_color.h"
#include "base/types/kode_matrix.h"
#include "base/types/kode_point.h"
#include "base/types/kode_quaternion.h"
#include "base/types/kode_rect.h"
#include "base/types/kode_string.h"
#include "base/types/kode_vector.h"

//----------------------------------------------------------------------

//template <typename T>
//class KODE_Array {
//};
//
//struct KODE_Rect {
//  float x,y,w,h;
//  KODE_Rect(float v) { x=v; y=v; w=v; h=v; }
//  KODE_Rect(float aw, float ah) { x=0; y=0; w=aw; h=ah; }
//  KODE_Rect(float ax, float ay, float aw, float ah) { x=ax; y=ay; w=aw; h=ah; }
//};
//
//struct KODE_Color {
//  float r,g,b,a;
//};

//----------------------------------------------------------------------
#endif
