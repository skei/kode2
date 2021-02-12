#ifndef kode_debug_included
#define kode_debug_included
//----------------------------------------------------------------------

#ifndef KODE_DEBUG

  #undef KODE_DEBUG_ASSERT
  #undef KODE_DEBUG_CALLSTACK
  #undef KODE_DEBUG_CRASH_HANDLER
  #undef KODE_DEBUG_MEMORY
  #undef KODE_DEBUG_PRINT_LOGFILE
  #undef KODE_DEBUG_PRINT_SOCKET
  #undef KODE_DEBUG_PRINT_THREAD
  #undef KODE_DEBUG_PRINT_TIME
  #undef KODE_DEBUG_PRINT_WINDOW

  #undef KODE_DEBUG_LADSPA
  #undef KODE_DEBUG_DSSI
  #undef KODE_DEBUG_LV2
  #undef KODE_DEBUG_VST2
  #undef KODE_DEBUG_VST3
  #undef KODE_DEBUG_EXE

  #undef KODE_DEBUG_CAIRO
  #undef KODE_DEBUG_CTX
  #undef KODE_DEBUG_XCB

#endif // KODE_DEBUG

//----------------------------------------------------------------------

#include "base/debug/kode_debug_print.h"
#include "base/debug/kode_debug_assert.h"
#include "base/debug/kode_debug_callstack.h"
#include "base/debug/kode_debug_crash_handler.h"
#include "base/debug/kode_debug_memory.h"

//----------------------------------------------------------------------

#ifdef KODE_DEBUG

  class KODE_Debug {
  public:
    KODE_Debug() {
      #ifdef KODE_DEBUG_PRINT_TIME
      _kode_debug_time_start();
      #endif
      #ifdef KODE_DEBUG_PRINT_SOCKET
      _kode_debug_socket_init();
      #endif
    }
    ~KODE_Debug() {
      //#ifdef KODE_DEBUG_PRINT_TIME
      //#endif
      #ifdef KODE_DEBUG_PRINT_SOCKET
      _kode_debug_socket_close();
      #endif
    }
  };

  //----------

  KODE_Debug _kode_debug_;

#endif // KODE_DEBUG

//----------------------------------------------------------------------
#endif
