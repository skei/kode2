#ifndef kode_dynamic_buffer_included
#define kode_dynamic_buffer_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/utils/kode_integer.h"

//----------------------------------------------------------------------

template <class TYPE>
class KODE_DynamicBuffer {

//------------------------------
protected:
//------------------------------

  TYPE*     MBuffer     = KODE_NULL;
  uint32_t  MSize       = 0;
  uint32_t  MAllocated  = 0;

//------------------------------
public:
//------------------------------

  //KODE_DynamicBuffer() {
  //}

  //----------

  virtual ~KODE_DynamicBuffer() {
    deleteBuffer();
  }

//------------------------------
public:
//------------------------------

  TYPE& operator [] (const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  //----------

  operator TYPE* () {
    return MBuffer;
  }

  //----------

  operator const TYPE* ()	const {
    return MBuffer;
  }

//------------------------------
public:
//------------------------------

  TYPE* ptr() {
    return MBuffer;
  }

  //----------

  uint32_t size() {
    return MSize;
  }

  //----------

  void clearBuffer() {
    KODE_Memset(MBuffer,0,MSize * sizeof(TYPE));
  }

  //----------

  void fillBuffer(TYPE AItem) {
    for (uint32_t i=0; i<MSize; i++) {
      MBuffer[i] = AItem;
    }
  }

  //----------

  void fillBuffer() {
    for (uint32_t i=0; i<MSize; i++) {
      MBuffer[i] = i;
    }
  }

//------------------------------
public:
//------------------------------

  void createBuffer(uint32_t ASize) {
    uint32_t size = KODE_NextPowerOfTwo(ASize);
    //if (size != MAllocated) {
      MBuffer = (TYPE*)KODE_Malloc(size * sizeof(TYPE));
      MAllocated = size;
      MSize = ASize;
    //}
  }

  //----------

  void deleteBuffer() {
    if (MBuffer) KODE_Free(MBuffer);
    MBuffer = KODE_NULL;
    MAllocated = 0;
    MSize = 0;
  }

  //----------

  void resizeBuffer(uint32_t ASize) {
    //if (MAllocated == 0) createBuffer(ASize);
    uint32_t size2 = KODE_NextPowerOfTwo(ASize);
    if (size2 != MAllocated) {
      MBuffer = (TYPE*)KODE_Realloc(MBuffer,size2 * sizeof(TYPE));
      MAllocated = size2;
    }
    MSize = ASize;
  }

  //----------

  void insertSpace(uint32_t APos, uint32_t ACount) {
    uint32_t num = (MSize - APos);
    resizeBuffer(MSize + ACount);
    TYPE* dst = &MBuffer[APos + ACount];
    TYPE* src = &MBuffer[APos];
    KODE_Memmove(dst,src,num * sizeof(TYPE));
  }

  //----------

  //TODO
  void insertHead(uint32_t ACount) {
    //insertSpace(0,ACount);
  }

  //----------

  //TODO
  void insertTail(uint32_t ACount) {
    //resizeBuffer(MSize+ACount);
  }

  //----------

  void removeSpace(uint32_t APos, uint32_t ACount) {
    TYPE* dst = &MBuffer[APos];
    TYPE* src = &MBuffer[APos + ACount];
    uint32_t num = (MSize - APos);
    KODE_Memmove(dst,src,num * sizeof(TYPE));
    resizeBuffer(MSize - ACount);
  }

  //----------

  //TODO
  void removeHead(uint32_t ACount) {
    //removeSpace(0,ACount);
  }

  //----------

  //TODO
  void removeTail(uint32_t ACount) {
    //resizeBuffer(MSize-ACount);
  }

};

//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif

