#ifndef kode_defines_included
#define kode_defines_included
//----------------------------------------------------------------------

//#include "kode.h"

//----------------------------------------------------------------------
//
// os
//
//----------------------------------------------------------------------

#ifdef __gnu_linux__
  #define KODE_LINUX
#endif

//----------

//#ifdef _WIN32
//  #define  KODE_WIN32
//#endif

//----------------------------------------------------------------------
//
// bits
//
//----------------------------------------------------------------------

#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define KODE_32BIT
#endif

//----------

#if defined (__x86_64) || (__LP64__)
  #define KODE_64BIT
#endif

//----------------------------------------------------------------------
//
// endianness
//
//----------------------------------------------------------------------

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define KODE_BIG_ENDIAN
#endif

//----------

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define KODE_LITTLE_ENDIAN
#endif

//----------------------------------------------------------------------
//
// c++11
//
//----------------------------------------------------------------------

#if (__cplusplus >= 201103L)
  #define KODE_CPP11
#endif

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

#undef KODE_PLUGIN_DEFINED

  #ifdef KODE_NO_PLUGIN
    #define KODE_PLUGINGUI_DEFINED
  #endif

  #ifdef KODE_PLUGIN_DSSI
    #define KODE_PLUGIN_DEFINED
    #define KODE_DSSI
  #endif

  #ifdef KODE_PLUGIN_EXE
    #define KODE_PLUGIN_DEFINED
    #define KODE_EXE
  #endif

  #ifdef KODE_PLUGIN_LADSPA
    #define KODE_PLUGIN_DEFINED
    #define KODE_LADSPA
  #endif

  #ifdef KODE_PLUGIN_LV2
    #define KODE_PLUGIN_DEFINED
    #define KODE_LV2
  #endif

  #ifdef KODE_PLUGIN_VST2
    #define KODE_PLUGIN_DEFINED
    #define KODE_VST2
  #endif

  #ifdef KODE_PLUGIN_VST3
    #define KODE_PLUGIN_DEFINED
    #define KODE_VST3
  #endif

#ifndef KODE_PLUGIN_DEFINED
  #warning no plugin format defined!
  #define KODE_NO_PLUGIN
  //#define KODE_EXE
  //#define KODE_PLUGIN_EXE
#endif

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

#undef KODE_GUI_DEFINED

  #ifdef KODE_NO_GUI
    #define KODE_GUI_DEFINED
  #endif

  #ifdef KODE_GUI_XCB
    #define KODE_GUI_DEFINED
    #define KODE_USE_XCB
  #endif

  #ifdef KODE_GUI_CAIRO
    #define KODE_GUI_DEFINED
    #undef  KODE_GUI_XCB
    #define KODE_USE_XCB
    #define KODE_USE_CAIRO
  #endif

#ifndef KODE_GUI_DEFINED
  #warning no gui type defined!
  #undef KODE_USE_XCB
  //#undef KODE_USE_CAIRO
  #define KODE_NO_GUI
  //#define KODE_USE_XCB
  //#define KODE_GUI_XCB
#endif

//----------------------------------------------------------------------
#endif
