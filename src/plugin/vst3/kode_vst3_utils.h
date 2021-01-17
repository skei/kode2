#ifndef kode_vst3_utils_included
#define kode_vst3_utils_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/vst3/kode_vst3.h"

//----------------------------------------------------------------------

void VST3_PrintIID(const void* iid) {
  uint8_t* id = (uint8_t*)iid;
  char buffer[64];
  char* bufptr = buffer;
  const char hextable[] = "0123456789abcdef";
  for (uint32_t i=0; i<4; i++) {
    for (uint32_t j=0; j<4; j++) {
      //uint32_t k = (i * 4) + j;
      uint8_t byte = *id++;//id[k];
      *bufptr++ = hextable[byte >> 4];
      *bufptr++ = hextable[byte % 15];
    }
    if (i<3) *bufptr++ = '-';
  }
  *bufptr = 0;
  VST3_DPrint("%s\n",buffer);
}

//----------------------------------------------------------------------
#endif

/*
const char* type_txt[]      = { "kAudio", "kEvent" };
const char* dir_txt[]       = { "kInput", "kOutput" };
const char* iomode_txt[]    = { "kSimple", "kAdvanced", "kOfflineProcessing" };
const char* procmode_txt[]  = { "kRealtime", "kPrefetch", "kOffline" };
const char* sample_txt[]    = { "kSample32", "kSample64" };
*/

//#define KODE_CharToUtf16(C,U) UString128(C).copyTo(U,128)
//#define KODE_Utf16ToChar(U,C) UString128(U).toAscii(C,128)

//void KODE_CharToUtf16(const void* C, void* U) {
//}
//
//void KODE_Utf16ToChar(const void* U, void* C) {
//}
//
//void KODE_IidEqual(const void* iid1, const void* iid2) {
//}


//----------

//char* KODE_Vst3TuidToAscii(const vst3iid_t _iid, char* ABuffer) {
//  //char buf[64];
//  const char hextable[] = "0123456789abcdef";
//  char* ptr = ABuffer;
//  for (uint32_t i=0; i<4; i++) {
//    for (uint32_t j=0; j<4; j++) {
//      uint32_t k = i * 4 + j;
//      uint8_t byte = _iid[k];
//      *ptr++ = hextable[byte >> 4];
//      *ptr++ = hextable[byte % 15];
//    }
//    if (i<3) *ptr++ = '-';
//  }
//  *ptr = 0;
//  return ABuffer;
//}

//----------


//----------------------------------------------------------------------

// FUnknownPrivate::iidEqual(iid1,iid2)

//amespace FUnknownPrivate {
//SMTG_ALWAYS_INLINE bool iidEqual (const void* iid1, const void* iid2)
//bool KODE_Vst3IidEqual(const void* iid1, const void* iid2) {
//  const uint64_t* p1 = reinterpret_cast<const uint64_t*> (iid1);
//  const uint64_t* p2 = reinterpret_cast<const uint64_t*> (iid2);
//  return p1[0] == p2[0] && p1[1] == p2[1];
//}

