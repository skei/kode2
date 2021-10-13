#ifndef kode_debug_print_included
#define kode_debug_print_included
//----------------------------------------------------------------------

// #define KODE_DEBUG_PRINT_TIME
// #define KODE_DEBUG_PRINT_THREAD
//
// #define KODE_DEBUG_PRINT_SOCKET
// nc -U -l -k /tmp/kode.socket
//
// KODE_PRINT;
// KODE_Print("test %i\n",i);
// KODE_DPrint("test %i\n",i);
// KODE_NoPrint(...);
//
// [time:thread:file:func:line] ..

//----------------------------------------------------------------------

/*
see socket output by entering this in a console:
nc -U -l -k /tmp/kode.socket
*/

#include <stdarg.h>   // va_
//#include "kode.h"
#include "base/kode_stdlib.h"

void KODE_NoPrint(const char*,...) {}

//----------------------------------------------------------------------

#ifdef KODE_DEBUG

  #ifdef KODE_DEBUG_PRINT_TIME
    //#include <time.h>     // timer
    #include <sys/time.h> // gettimeofday
    double _kode_debug_start_time = 0.0;
  #endif

  #ifdef KODE_DEBUG_PRINT_THREAD
    #include <sys/syscall.h>
    #include <sys/unistd.h>
    #define gettid() syscall(SYS_gettid)
  #endif

  #ifdef KODE_DEBUG_PRINT_SOCKET
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/un.h>
    int _kode_debug_socket_handle = 0;
  #endif

  char _kode_debug_print_buffer[256]  = {0};
  char _kode_debug_prefix_buffer[256] = {0};

  //----------------------------------------------------------------------
  //
  //----------------------------------------------------------------------

  #ifdef KODE_DEBUG_PRINT_TIME

  void _kode_debug_time_start() {
    //kode_debug_start_time = clock();
    struct timeval time;
    gettimeofday(&time,NULL);
    _kode_debug_start_time = (double)time.tv_sec + (double)time.tv_usec * .000001;
  }

  //----------

  double _kode_debug_time_elapsed() {
    //clock_t currenttime = clock();
    //double elapsed = currenttime - kode_debug_start_time;
    //return elapsed / CLOCKS_PER_SEC;
    struct timeval time;
    gettimeofday(&time,NULL);
    double currenttime = (double)time.tv_sec + (double)time.tv_usec * .000001;
    double elapsed = currenttime - _kode_debug_start_time;
    return elapsed;
  }

  #endif

  //----------------------------------------------------------------------
  // thread
  //----------------------------------------------------------------------

  #ifdef KODE_DEBUG_PRINT_THREAD

  pid_t _kode_debug_get_process_id() {
    return getpid();
  }

  //----------

  pid_t _kode_debug_get_thread_id() {
    return gettid();
  }

  #endif

  //----------------------------------------------------------------------
  // thread
  //----------------------------------------------------------------------

  #ifdef KODE_DEBUG_PRINT_SOCKET

  void _kode_debug_socket_init() {
    //open_socket();
    _kode_debug_socket_handle = socket(PF_UNIX,SOCK_STREAM,0);
    sockaddr_un address;
    memset(&address,0,sizeof(sockaddr_un));
    address.sun_family = AF_UNIX;
    snprintf(address.sun_path,108,"/tmp/kode.socket"); // max 108?
    connect(_kode_debug_socket_handle,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
  }

  void _kode_debug_socket_close() {
    close(_kode_debug_socket_handle);
  }

  void _kode_debug_socket_print(char* buffer) {
    dprintf(_kode_debug_socket_handle,"%s",buffer);
  }

  #endif

  //----------------------------------------------------------------------
  // path
  //----------------------------------------------------------------------

  // /home/skei/test -> test
  // returns everything after the last /

  const char* _kode_debug_strip_path(const char* buffer) {
    const char *slash;
    slash = strrchr(buffer,'/');
    if (slash) {
      return slash + 1;
    }
    return buffer;
  }

  //----------------------------------------------------------------------
  //
  //----------------------------------------------------------------------

  void _kode_print_prefix(const char* file, const char* func, int line) {
    char buffer[256];
    const char* filename  = _kode_debug_strip_path(file);
    strcat(_kode_debug_prefix_buffer,"[");
    #ifdef KODE_DEBUG_PRINT_TIME
      double time = _kode_debug_time_elapsed();
      sprintf(buffer,"%.3f:",time);
      strcat(_kode_debug_prefix_buffer,buffer);
    #endif
    #ifdef KODE_DEBUG_PRINT_THREAD
      uint32_t thread_id = _kode_debug_get_thread_id();
      sprintf(buffer,"%i:",thread_id);
      strcat(_kode_debug_prefix_buffer,buffer);
    #endif
    sprintf(buffer,"%s:%s:%i",filename,func,line);
    strcat(_kode_debug_prefix_buffer,buffer);
    strcat(_kode_debug_prefix_buffer,"] ");
  }

  //----------

  void _kode_print(const char* format, ...) {
    va_list args;
    va_start(args,format);
    vsprintf(_kode_debug_print_buffer,format,args);
    strcat(_kode_debug_prefix_buffer,_kode_debug_print_buffer);

    #if defined KODE_DEBUG_PRINT_SOCKET
      _kode_debug_socket_print(_kode_debug_prefix_buffer);
    #else
      printf("%s",_kode_debug_prefix_buffer);
    #endif

    _kode_debug_prefix_buffer[0] = 0;
    va_end(args);
  }

  //----------

  void _kode_dprint(const char* format, ...) {
    va_list args;
    va_start(args,format);
    vsprintf(_kode_debug_print_buffer,format,args);

    #if defined KODE_DEBUG_PRINT_SOCKET
      _kode_debug_socket_print(_kode_debug_print_buffer);
    #else
      printf("%s",_kode_debug_print_buffer);
    #endif

    va_end(args);
  }

  //----------------------------------------------------------------------
  //
  //----------------------------------------------------------------------

  #define KODE_Print \
    _kode_print_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    _kode_print

  #define KODE_DPrint \
    _kode_dprint

  #define KODE_PRINT \
    _kode_print_prefix( __FILE__, __FUNCTION__, __LINE__ ); \
    _kode_print("\n");

#else // KODE_DEBUG

  #define KODE_Print  KODE_NoPrint
  #define KODE_DPrint KODE_NoPrint
  #define KODE_PRINT  {}

#endif // KODE_DEBUG

//----------------------------------------------------------------------
#endif











//#include <iostream>
//#include <fstream>
//using namespace std;
//
//#include "src/h_BasePath.h"
//
//#ifdef H_DEBUG_LOG_UNIQUE
//  #include "src/h_Timestamp.h"
//#endif
//
////----------------------------------------------------------------------
//
//#ifndef H_DEBUG_LOG_APPEND
//  #define H_DEBUG_LOG_APPEND
//#else
//  #undef H_DEBUG_LOG_APPEND
//  #define H_DEBUG_LOG_APPEND | std::fstream::app
//#endif
//
////----------------------------------------------------------------------
//
//class KODE_LogFile {
//
//private:
//
//  char          m_LogFileName[H_MAX_PATHSIZE];
//  fstream       m_LogFile; // static?
//  #ifdef H_DEBUG_LOG_UNIQUE
//  h_Timestamp m_TS;
//  #endif
//
//public:
//
//  inline fstream& _trace(void) { return m_LogFile; }
//
//public:
//
//  KODE_LogFile(const char* a_FileName)
//    {
//      #ifndef H_DEBUG_LOG_HOME
//        // try writing to base path
//
//        m_LogFileName[0] = '\0';
//        h_GetBasePath(m_LogFileName);
//        #ifdef H_DEBUG_LOG_UNIQUE
//          h_Strcat(m_LogFileName, m_TS.str());
//          h_Strcat(m_LogFileName, (char*)"_");
//        #endif
//
//        h_Strcat(m_LogFileName, a_FileName);
//        m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
//
//        if (m_LogFile.fail())
//        {
//          printf( "ERROR: cannot write log to base path: %s\n",
//                m_LogFileName);
//      #endif
//          // write to home path instead
//
//          m_LogFileName[0] = '\0';
//          h_GetHomePath(m_LogFileName);
//          #ifdef H_DEBUG_LOG_UNIQUE
//            h_Strcat(m_LogFileName, m_TS.str());
//            h_Strcat(m_LogFileName, (char*)"_");
//          #endif
//
//          h_Strcat(m_LogFileName, a_FileName);
//          m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);
//
//          if (m_LogFile.fail())
//          {
//            printf( "ERROR: cannot write log to home path: %s\n",
//                  m_LogFileName);
//            m_LogFile.clear();
//          }
//      #ifndef H_DEBUG_LOG_HOME
//        }
//      #endif
//    }
//
//  ~KODE_LogFile()
//    {
//      m_LogFile.close();
//    }
//
//  //void print(const char* x)
//  //  {
//  //    m_LogFile << x << endl;
//  //  }
//
//};
