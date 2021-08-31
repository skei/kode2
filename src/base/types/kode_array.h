#ifndef kode_array_included
#define kode_array_included
//----------------------------------------------------------------------

#include "base/utils/kode_integer_math.h" // KODE_NextPowerOfTwo
#include "base/kode_stdlib.h"

//----------------------------------------------------------------------

template<class _T>
class KODE_Array {

//------------------------------
private:
//------------------------------

  const uint32_t	MTypeSize = sizeof(_T);
  _T*       MBuffer         = KODE_NULL;
  uint32_t	MBufferSize     = 0;
  uint32_t	MSize           = 0;

//------------------------------
public:
//------------------------------

  KODE_Array() {
    //MBuffer     = KODE_NULL;
    //MBufferSize = 0;
    //MSize       = 0;
  }

  //----------

  KODE_Array(const KODE_Array& AArray) {
    MBuffer = (_T*)malloc(MTypeSize*AArray.MBufferSize);
    MBufferSize = AArray.MBufferSize;
    MSize = AArray.MSize;
    memcpy(MBuffer, AArray.MBuffer, MTypeSize*AArray.MBufferSize);
  }

  //----------

  ~KODE_Array() {
    if (MBuffer) {
      free(MBuffer);
    }
  }

//------------------------------
public:
//------------------------------

  uint32_t  size(void)                    { return MSize; }
  void*     buffer(void)                  { return MBuffer; }
  _T&       item(const uint32_t AIndex)   { return MBuffer[AIndex]; }
  uint32_t  bufferSize(void)              { return MBufferSize; }

//------------------------------
public:
//------------------------------

  _T& operator [] (const uint32_t AIndex) {
    return MBuffer[AIndex];
  }

  //----------

  KODE_Array<_T>& operator = (const KODE_Array<_T> &AArray) {
    if (this == &AArray) return *this;
//      if (AArray.MSize == 0) clear();
//      setSize(AArray.MSize);
    memcpy(MBuffer, AArray.MBuffer, MTypeSize*AArray.MSize);
    return *this;
  }

//------------------------------
private:
//------------------------------

  void deleteItems(void) {
    for (uint32_t i=0; i<MSize; i++) delete MBuffer[i];
  }

  //----------

  void clearBuffer(bool AErase=false) {
    if (AErase) {
      if (MBuffer) free(MBuffer);
      MBuffer = KODE_NULL;
      MBufferSize = 0;
    }
    MSize = 0;
  }

  //----------

  void resizeBuffer(const uint32_t ABufferSize) {
    uint32_t size = KODE_NextPowerOfTwo(ABufferSize);
    if (size != MBufferSize) {
      MBuffer = (_T*)realloc(MBuffer, MTypeSize*size);
      MBufferSize = size;
      //MSize = size;
      //if (MSize > MBufferSize) MSize = MBufferSize;
    }
  }

  //----------

  void growBuffer(const uint32_t ANum) {
    uint32_t newsize = MSize + ANum;
    resizeBuffer(newsize);
  }

  //----------

  void shrinkBuffer(const uint32_t ANum) {
    uint32_t newsize = MSize - ANum;
    resizeBuffer(newsize);
  }

//------------------------------
public:
//------------------------------

  void clear(bool AErase=true) {
    clearBuffer(AErase);
  }

  //----------

  void insert(const _T& AItem, uint32_t APos) {
    growBuffer(1);
    memmove( &MBuffer[APos+1], &MBuffer[APos], (MSize-APos) * MTypeSize);
    MBuffer[APos] = AItem;
    MSize++;
  }

  //----------

  void insert(KODE_Array<_T>& AArray, uint32_t APos) {
    uint32_t num = AArray.size();
    growBuffer(num);
    memmove( &MBuffer[APos+num], &MBuffer[APos], (MSize-APos) * MTypeSize);
    memcpy( &MBuffer[APos], AArray.buffer(), num * MTypeSize );
    MSize += num;
  }

  //----------

  void insertHead(const _T& AItem) {
    insert(AItem,0);
  }

  //----------

  void insertTail(const _T& AItem) {
    growBuffer(1);
    MBuffer[MSize] = AItem;
    MSize++;
  }

  //------------------------------

  void remove(const uint32_t APos) {
    memmove( &MBuffer[APos], &MBuffer[APos+1], (MSize-APos-1) * MTypeSize);
    shrinkBuffer(1);
    MSize--;
  }

  //----------

  void remove(const uint32_t APos, uint32_t ANum) {
    //moveBufferItems(APos+ANum,APos,ANum);
    memmove( &MBuffer[APos], &MBuffer[APos+ANum], (MSize-APos-ANum) * MTypeSize);
    shrinkBuffer(ANum);
    MSize -= ANum;
  }

  //----------

  void removeHead(void) {
    remove(0);
  }

  //----------

  void removeTail(void) {
    MSize--;
    shrinkBuffer(1);
  }

//------------------------------
public:
//------------------------------

  int32_t find(const _T& AItem) {
    for (uint32_t i=0; i<MSize; i++) {
      if (MBuffer[i] == AItem) return i;
    }
    return -1;
  }

  //----------

  void append(_T& AItem) {
    insertTail(AItem);
  }

  //----------

  _T& head(void) {
    return MBuffer[0];
  }

  //----------

  _T& tail(void) {
    return MBuffer[MSize-1];
  }

  //----------

  void push(_T AItem) {
    insertTail(AItem);
  }

  //----------

  _T pop(void) {
    removeTail();
    return MBuffer[MSize];
  }


};

//----------------------------------------------------------------------

typedef KODE_Array<char>          KODE_CharArray;
typedef KODE_Array<char*>         KODE_CharPtrArray;
typedef KODE_Array<void*>         KODE_PtrArray;
typedef KODE_Array<void**>        KODE_PtrPtrArray;
typedef KODE_Array<float*>        KODE_FloatPtrArray;
typedef KODE_Array<float>         KODE_FloatArray;
typedef KODE_Array<bool>          KODE_BoolArray;
typedef KODE_Array<int32_t>       KODE_Int32Array;
typedef KODE_Array<uint32_t>      KODE_Uint32Array;

//typedef KODE_Array<KODE_String>   KODE_StringArray;
//typedef KODE_Array<KODE_Sample>   KODE_SampleArray;
//typedef KODE_Array<KODE_Sample*>  KODE_SamplePtrArray;

//----------------------------------------------------------------------
#endif














