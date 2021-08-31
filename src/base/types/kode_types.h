#ifndef kode_types_included
#define kode_types_included
//----------------------------------------------------------------------

#include <stdint.h>

//#ifndef _STDINT_H
//  typedef unsigned char       uint8_t;
//  typedef unsigned short      uint16_t;
//  typedef unsigned long       uint32_t;
//  typedef unsigned long long  uint64_t;
//  typedef signed char         int8_t;
//  typedef signed short        int16_t;
//  typedef signed long         int32_t;
//  typedef signed long long    int64_t;
//  typedef intptr              intptr_t;
//#endif

//----------

#ifdef KODE_CPP11
  #define KODE_NULL nullptr
#else
  #define KODE_NULL 0
#endif

//----------------------------------------------------------------------

#include "base/types/kode_array.h"
#include "base/types/kode_assoc_array.h"
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
#endif
