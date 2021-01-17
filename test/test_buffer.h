
#define KODE_DEBUG
//#define KODE_DEBUG_PRINT_TIME
//#define KODE_DEBUG_PRINT_THREAD


#include "base/kode.h"
#include "base/types/kode_buffer.h"

//----------

#define SIZE 16

KODE_DynamicBuffer<int> dbuffer;
KODE_StaticBuffer<int,SIZE> sbuffer;

//----------

void dump_buffer(int* buffer) {
  //KODE_DPrint("\n");
  for (uint32_t i=0; i<SIZE; i++) {
    KODE_DPrint("%i ",buffer[i]);
  }
  KODE_DPrint("\n");
}

//----------

int main(void) {

//------------------------------
// static buffer
//------------------------------

  sbuffer.fillBuffer(1);
  sbuffer[1] = 10;
  //sbuffer.resizeBuffer(10);
  //sbuffer.insertSpace(3,3);
  KODE_Print("size     %i\n",sbuffer.size());
  dump_buffer(sbuffer);

//------------------------------
// dynamic buffer
//------------------------------

  dbuffer.setBufferSize(16);
  dbuffer.fillBuffer();
  dbuffer[1] = 10;
  dbuffer.resizeBuffer(10);
  dbuffer.insertSpace(3,3);
  KODE_Print("size     %i\n",dbuffer.size());
  dump_buffer(dbuffer);

//------------------------------
//
//------------------------------

// array
// stack
// queue
// dynamicString, staticString, shortString, charString

//------------------------------
//
//------------------------------



  return 0;
}
