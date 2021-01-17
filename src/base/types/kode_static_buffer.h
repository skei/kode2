#ifndef kode_static_buffer_included
#define kode_static_buffer_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/utils/kode_integer.h"

//----------------------------------------------------------------------
//
// static buffer
//
//----------------------------------------------------------------------

template <class TYPE, int SIZE>
class KODE_StaticBuffer {

//------------------------------
protected:
//------------------------------

  TYPE  MBuffer[SIZE] = {0};

//------------------------------
public:
//------------------------------

  KODE_StaticBuffer() {
  }

  KODE_StaticBuffer(const KODE_StaticBuffer& ABuffer) {
    //KODE_StaticBuffer* buf = (KODE_StaticBuffer*)&ABuffer;
    //KODE_Memcpy(MBuffer,buf->ptr());
  }

  KODE_StaticBuffer(void* ABuffer, uint32_t ASize) {
    //KODE_Strcpy(MBuffer,ABuffer,ASize);
  }

  virtual ~KODE_StaticBuffer() {
  }

//------------------------------
public:
//------------------------------

  TYPE& operator [] (const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  operator TYPE* () {
    return MBuffer;
  }

  operator const TYPE* ()	const {
    return MBuffer;
  }

  KODE_StaticBuffer& operator = (TYPE* ABuffer) {
    KODE_Memcpy(MBuffer,ABuffer,SIZE * sizeof(TYPE));
    return *this;
  }

  KODE_StaticBuffer& operator = (/*const*/ KODE_StaticBuffer& ABuffer) {
    KODE_Memcpy(MBuffer,ABuffer,SIZE * sizeof(TYPE));
    return *this;
  }

  //KODE_StaticBuffer& operator += (TYPE* ABuffer) {
  //  //KODE_Strcat(MBuffer,str);
  //  return *this;
  //}

  //bool operator == (const TYPE* str) {
  //  //return (KODE_Strcmp(MBuffer,str) == 0) ? true : false;
  //  return false;
  //}

//------------------------------
public:
//------------------------------

  TYPE* ptr() {
    return MBuffer;
  }

  uint32_t size() {
    return SIZE;
  }

  void clearBuffer() {
    KODE_Memset(MBuffer,0,SIZE*sizeof(TYPE));
  }

  void fillBuffer(TYPE AItem) {
    for (uint32_t i=0; i<SIZE; i++) {
      MBuffer[i] = AItem;
    }
  }

  void fillBuffer() {
    for (uint32_t i=0; i<SIZE; i++) {
      MBuffer[i] = i;
    }
  }

  /*

  todo: cap src/dst to biffer size

  void insertSpace(uint32_t APos, uint32_t ACount) {
    uint32_t num = (MBufferSize - APos);
    resizeBuffer(MBufferSize + ACount);
    TYPE* dst = &MBuffer[APos + ACount];
    TYPE* src = &MBuffer[APos];
    KODE_Memmove(dst,src,num * sizeof(TYPE));
  }

  void removeSpace(uint32_t APos, uint32_t ACount) {
    TYPE* dst = &MBuffer[APos];
    TYPE* src = &MBuffer[APos + ACount];
    uint32_t num = (MBufferSize - APos);
    KODE_Memmove(dst,src,num * sizeof(TYPE));
    resizeBuffer(MBufferSize - ACount);
  }

  */

//------------------------------
public:
//------------------------------

  int32_t find(TYPE AItem) {
    for (uint32_t i=0; i<SIZE; i++) {
      if (MBuffer[i] == AItem) return i;
    }
    return -1;
  }

};

//----------------------------------------------------------------------

//template <int SIZE, class T>
//KODE_StaticString<SIZE> operator + (T var, const KODE_StaticString<SIZE>& str)  {
//  KODE_StaticString<SIZE> svar = var;
//  return svar += str;
//}


//----------------------------------------------------------------------
#endif

