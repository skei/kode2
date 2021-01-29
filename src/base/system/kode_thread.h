#ifndef kode_thread_included
#define kode_thread_included
//----------------------------------------------------------------------

#include <pthread.h>
//#include <unistd.h> // sleep

#include "base/system/kode_time.h"

class KODE_Thread {

private:

  bool        MThreadRunning;
  int32_t     MThreadSleep;
  void*       MUsr;
  pthread_t   MThreadHandle;

//------------------------------
public:
//------------------------------

  KODE_Thread() {
    MUsr = KODE_NULL;
    MThreadRunning = false;
    MThreadSleep = -1;
    MThreadHandle = 0;
  }

  //----------

  virtual ~KODE_Thread() {
    if (MThreadRunning) stop();
  }

//------------------------------
public:
//------------------------------

  virtual void on_threadFunc(void* usr) { /*KODE_Trace("on_threadFunc\n");^*/ };

//------------------------------
private:
//------------------------------

  static
  void* threadProc(void* data) {
    KODE_Print("threadProc\n");
    KODE_Thread* thr = (KODE_Thread*)data;
    if (thr) {
      if (thr->MThreadSleep >= 0) {
        while (thr->MThreadRunning) {
          thr->on_threadFunc(thr->MUsr);
          //usleep(thr->MThreadSleep*1000); //ms*1000;
          KODE_Sleep(thr->MThreadSleep);
        }
      }
      else {
        thr->on_threadFunc(thr->MUsr);
      }
    }
    return KODE_NULL;
  }

//------------------------------
public:
//------------------------------

  void start(void* usr, int32_t ms=-1) { // -1 = no timer
    KODE_Print("start\n");
    MUsr = usr;
    MThreadSleep = ms;
    MThreadRunning = true;
    pthread_create(&MThreadHandle,KODE_NULL,&threadProc,this);
  }

  //----------

  void stop(void) {
    KODE_Print("stop\n");
    MThreadRunning = false;
    void* ret;
    pthread_join(MThreadHandle,&ret);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
