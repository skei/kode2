#ifndef kode_time_included
#define kode_time_included
//----------------------------------------------------------------------

// -lrt

#include <time.h>     // timer
#include <signal.h>   // sigval
#include <errno.h>    // errno

#include "base/kode_const.h"
#include "base/utils/kode_math.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  The C library function clock_t clock(void) returns the number of clock ticks
  elapsed since the program was launched. To get the number of seconds used by
  the CPU, you will need to divide by CLOCKS_PER_SEC.
  On a 32 bit system where CLOCKS_PER_SEC equals 1000000 this function will
  return the same value approximately every 72 minutes.
*/

// http://man7.org/linux/man-pages/man2/timer_create.2.html

double KODE_GetTimeMS(void) {
  clock_t curtime = clock();
  return (double)(curtime * 1000.0) / CLOCKS_PER_SEC;
  //return (double)curtime;
}

//----------

struct KODE_CurrentTime {
  int32_t year;
  int32_t month;
  int32_t day;
  int32_t hour;
  int32_t minutes;
  int32_t seconds;
};

//----------

void KODE_GetLocalTime(KODE_CurrentTime* ATime) {
  time_t t       = time(KODE_NULL);
  struct tm tm   = *localtime(&t); // cppcheck recommends localtime_r
  ATime->year    = tm.tm_year + 1900;
  ATime->month   = tm.tm_mon + 1;
  ATime->day     = tm.tm_mday;
  ATime->hour    = tm.tm_hour;
  ATime->minutes = tm.tm_min;
  ATime->seconds = tm.tm_sec;
  //KODE_Print("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  //return &CODE_CURRENT_TIME;
}

//----------

// usleep is deprecated

//void KODE_Sleep(KODE_ui32 ms) {
//  usleep(ms*1000); // ms*1000;
//}

int KODE_Sleep(long ms) {
  struct timespec req, rem;
  if (ms > 999) {
    req.tv_sec = (int)(ms / 1000);                            // Must be Non-Negative
    req.tv_nsec = (ms - ((long)req.tv_sec * 1000)) * 1000000; // Must be in range of 0 to 999999999
  }
  else {
    req.tv_sec = 0;               // Must be Non-Negative
    req.tv_nsec = ms * 1000000;   // Must be in range of 0 to 999999999
  }
  return nanosleep(&req , &rem);
}

//----------

//----------------------------------------------------------------------
// timer
//----------------------------------------------------------------------

class KODE_TimerListener {
  public:
    virtual void on_timerCallback(void) {}
};

//----------

void kode_timer_callback(sigval val) {
  KODE_TimerListener* listener = (KODE_TimerListener*)val.sival_ptr;
  if (listener) listener->on_timerCallback();
}

//----------

class KODE_Timer {

private:

  sigevent    MSigEvent;
  timer_t     MTimer;
  itimerspec  MTimerSpec;

public:

  KODE_Timer(KODE_TimerListener* AListener) {
    MSigEvent.sigev_notify            = SIGEV_THREAD;
    MSigEvent.sigev_notify_function   = kode_timer_callback;
    MSigEvent.sigev_notify_attributes = KODE_NULL;
    MSigEvent.sigev_value.sival_int   = 0; // timer index?
    MSigEvent.sigev_value.sival_ptr   = AListener;
    //int res =
    timer_create(CLOCK_REALTIME, &MSigEvent, &MTimer);
    //KODE_Trace("MTimer %i\n",MTimer);
    /*if (res == -1) {
      switch(errno) {
        case EAGAIN:
          KODE_Trace("timer_create error: Temporary error during kernel allocation of timer structures.\n");
          break;
        case EINVAL:
          KODE_Trace("timer_create error: Clock ID, sigev_notify, sigev_signo, or sigev_notify_thread_id is invalid.\n");
          break;
        case ENOMEM:
          KODE_Trace("timer_create error: Could not allocate memory.\n");
          break;
      }
    }*/
  }

  //----------

  // timer_delete() deletes the timer whose ID is given in timerid.  If
  // the timer was armed at the time of this call, it is disarmed before
  // being deleted.  The treatment of any pending signal generated by the
  // deleted timer is unspecified.

  ~KODE_Timer() {
    timer_delete(MTimer);
  }

  //----------

  void start(float ms, bool oneshot=false) {
    float s = ms * 0.001f;
    float sec = KODE_Trunc(s);
    float nsec = (s-sec) * 1000000000; // 1000000.0f * (sec - KODE_Fract(s));
    time_t isec = sec;
    long insec = nsec;
    //KODE_Trace("s %.4f sec %.4f nsec %.4f isec %i insec %i\n",s,sec,nsec,isec,insec);
    if (oneshot) {
      MTimerSpec.it_interval.tv_sec   = 0;
      MTimerSpec.it_interval.tv_nsec  = 0;
    }
    else {
      MTimerSpec.it_interval.tv_sec   = isec;
      MTimerSpec.it_interval.tv_nsec  = insec;
    }
    MTimerSpec.it_value.tv_sec      = isec;
    MTimerSpec.it_value.tv_nsec     = insec;
    //int res =
    timer_settime(MTimer, 0, &MTimerSpec, 0);
    /*if (res == -1) {
      switch(errno) {
        case EFAULT:
          KODE_Trace("timer_settime error: new_value, old_value, or curr_value is not a valid pointer.\n");
          break;
        //case EINVAL:
        //  KODE_Trace("timer_settime error: timerid is invalid.\n");
        //  break;
        case EINVAL:
          KODE_Trace("timer_settime: new_value.it_value is negative; or new_value.it_value.tv_nsec is negative or greater than 999,999,999.\n");
          break;
      }
    }*/
  }

  //----------

  // If new_value->it_value specifies a nonzero value (i.e., either sub‐
  // field is nonzero), then timer_settime() arms (starts) the timer, set‐
  // ting it to initially expire at the given time.  (If the timer was
  // already armed, then the previous settings are overwritten.)  If
  // new_value->it_value specifies a zero value (i.e., both subfields are
  // zero), then the timer is disarmed.

  void stop(void) {
    MTimerSpec.it_interval.tv_sec   = 0;
    MTimerSpec.it_interval.tv_nsec  = 0;
    MTimerSpec.it_value.tv_sec      = 0;
    MTimerSpec.it_value.tv_nsec     = 0;
    timer_settime(&MTimer, 0, &MTimerSpec, 0);
  }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//class KODE_Time {
//
//  public:
//
//    KODE_Time(bool AStart=true) {
//      if (AStart) start();
//    }
//
//    ~KODE_Time() {}
//
//  public:
//
//    double start(void) {
//      MStartTime = clock();
//      return MStartTime;
//    }
//
//    /*
//      wraparound? negative?
//      if clock has wrapped around since start() was called, we would
//      clock_t -> long int
//    */
//
//    double sec(void) {
//      clock_t currenttime = clock();
//      double elapsed = currenttime - MStartTime;
//      return elapsed / CLOCKS_PER_SEC; // 1000000;
//    }
//
//    double ms(void) {
//      return sec() * 1000.0;
//    }
//
//    double us(void) {
//      return sec() * 1000000.0;
//    }
//
//  private:
//      clock_t MStartTime = 0; // cppcheck
//};

//----------------------------------------------------------------------
#endif
