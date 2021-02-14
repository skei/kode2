#ifndef kode_debug_crash_handler_included
#define kode_debug_crash_handler_included
//----------------------------------------------------------------------

/*
  compile with: -g -rdynamic
*/

//----------------------------------------------------------------------

#ifdef KODE_DEBUG_CRASH_HANDLER


  #include <signal.h>

  //#include <stdlib.h>
  // struct sigaction act = {0};

  /*
    https://lasr.cs.ucla.edu/vahab/resources/signals.html
    The only two signals for which a handler cannot be defined are SIGKILL and
    SIGSTOP.
  */

  const char* KODE_SignalNames[] = {
    "(kill(pid,0))",
    "SIGHUP (Hangup)",
    "SIGINT (Interactive attention signal)",
    "SIGQUIT (Quit)",
    "SIGILL (Illegal instruction)",
    "SIGTRAP (Trace/breakpoint trap)",
    "SIGABRT (Abnormal termination)",
    "(-)",
    "SIGFPE (Erroneous arithmetic operation)",
    "SIGKILL (Killed)",
    "SIGBUS (Bus error)",
    "SIGSEGV (Invalid access to storage)",
    "SIGSYS (Bad system call)",
    "SIGPIPE (Broken pipe)",
    "SIGALRM (Alarm clock)",
    "SIGTERM (Termination request)",
    "SIGURG (Urgent data is available at a socket)",
    "SIGSTOP (Stop, unblockable)",
    "SIGTSTP (Keyboard stop)",
    "SIGCONT (Continue)",
    "SIGCHLD (Child terminated or stopped)",
    "SIGTTIN (Background read from control terminal)",
    "SIGTTOU (Background write to control terminal)",
    "SIGPOLL (Pollable event occurred)",
    "SIGXCPU (CPU time limit exceeded)",
    "SIGXFSZ (File size limit exceeded)",
    "SIGVTALRM (Virtual timer expired)",
    "SIGPROF (Profiling timer expired)",
    "SIGWINCH (Window size change)",
    "(-)",
    "SIGUSR1 (User-defined signal 1)",
    "SIGUSR2 (User-defined signal 2)"
  };

  //----------

  void kode_crash_handler_callback(int sig) {
    KODE_DPrint("\nkode_crash_handler_callback %i %s\n",sig,KODE_SignalNames[sig]);
    KODE_DumpCallStackSkip(0); // 2
    exit(1); //_exit(1);
  }

  //----------

  void KODE_InitCrashHandler(int sig) {
    signal(SIGSEGV,kode_crash_handler_callback);
  }

  bool KODE_initCrashHandlers() {
    //for (int i=0; i<32; i++) KODE_InitSignalHandler(i);
    KODE_InitCrashHandler(SIGILL);
    KODE_InitCrashHandler(SIGABRT);
    KODE_InitCrashHandler(SIGFPE);
    KODE_InitCrashHandler(SIGSEGV);
    return true;
  }

  //----------

  // uninstall signal handler?
  // done automatically when program exits?

  //------------------------------

  bool kode_is_crash_handler_initialized = KODE_initCrashHandlers();

#endif // KODE_DEBUG_CRASH_HANDLER

//----------------------------------------------------------------------
#endif
