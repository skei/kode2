#ifndef vst3_utils_included
#define vst3_utils_included
//----------------------------------------------------------------------

#include "plugin/vst3/kode_vst3.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

void KODE_CharToUtf16(const void* C, void* U) {
  // TODO
}

//----------

void KODE_Utf16ToChar(const void* U, void* C) {
  // TODO
}

//----------

bool KODE_iidEqual(const void* iid1, const void* iid2) {
  uint32_t* ptr1 = (uint32_t*)iid1;
  uint32_t* ptr2 = (uint32_t*)iid2;
  if (ptr1[0] != ptr2[0]) return false;
  if (ptr1[1] != ptr2[1]) return false;
  if (ptr1[2] != ptr2[2]) return false;
  if (ptr1[3] != ptr2[3]) return false;
  return true;
}

//----------

//#define strncpy8 strncpy

//char* strncpy8(char* destination, const char* source, size_t num) {
//  memset(destination,0,num);
//  strncpy(destination,source,num-1);
//  return destination;
//}


//----------------------------------------------------------------------
#endif
