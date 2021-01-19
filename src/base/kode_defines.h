#ifndef kode_defines_included
#define kode_defines_included
//----------------------------------------------------------------------

//------------------------------
// os
//------------------------------

#ifdef __gnu_linux__
  #define KODE_LINUX
#endif

//----------

//#ifdef _WIN32
//  #define  KODE_WIN32
//#endif

//------------------------------
// bits
//------------------------------

#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define KODE_32BIT
#endif

//----------

#if defined (__x86_64) || (__LP64__)
  #define KODE_64BIT
#endif

//------------------------------
// endianness
//------------------------------

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define KODE_BIG_ENDIAN
#endif

//----------

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define KODE_LITTLE_ENDIAN
#endif

//------------------------------
// c++11
//------------------------------

#if (__cplusplus >= 201103L)
  #define KODE_CPP11
#endif

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

#undef KODE_PLUGIN_DEFINED

//#ifdef KODE_NO_PLUGIN
//#endif

//----------

#ifdef KODE_PLUGIN_EXE
  #define KODE_PLUGIN_DEFINED
#endif

#ifdef KODE_PLUGIN_VST2
  #define KODE_PLUGIN_DEFINED
#endif

#ifdef KODE_PLUGIN_VST3
  #define KODE_PLUGIN_DEFINED
#endif

//----------

#ifndef KODE_PLUGIN_DEFINED
  #warning NO PLUGIN DEFINED
  #define KODE_NO_PLUGIN
#endif

//----------------------------------------------------------------------
// painter
//----------------------------------------------------------------------

#undef KODE_PAINTER_DEFINED

#ifdef KODE_NO_PAINTER
  #define KODE_PAINTER_DEFINED
#endif

//----------

#ifdef KODE_PAINTER_CAIRO
  #define KODE_PAINTER_DEFINED
  #define KODE_CAIRO
  #define KODE_XCB
#endif

//#ifdef KODE_PAINTER_CTX
//  #define KODE_PAINTER_DEFINED
//#endif

//#ifdef KODE_PAINTER_SDL
//  #define KODE_PAINTER_DEFINED
//#endif

#ifdef KODE_PAINTER_XCB
  #define KODE_PAINTER_DEFINED
  #define KODE_XCB
#endif

//----------

#ifndef KODE_PAINTER_DEFINED
  #warning NO PAINTER DEFINED
  //#define KODE_NO_GUI
#endif

//----------------------------------------------------------------------
// gui
//----------------------------------------------------------------------

#undef KODE_GUI_DEFINED

#ifdef KODE_NO_GUI
  #define KODE_GUI_DEFINED
#endif

//----------

//#ifdef KODE_GUI_CAIRO
//  #define KODE_GUI_DEFINED
//  #define KODE_CAIRO
//  #define KODE_XCB
//#endif

//#ifdef KODE_GUI_CTX
//  #define KODE_GUI_DEFINED
//#endif

//#ifdef KODE_GUI_SDL
//  #define KODE_GUI_DEFINED
//#endif

#ifdef KODE_GUI_XCB
  #define KODE_GUI_DEFINED
  #define KODE_XCB
#endif

//----------

#ifndef KODE_GUI_DEFINED
  #warning NO GUI DEFINED
  #define KODE_NO_GUI
#endif

//----------------------------------------------------------------------
#endif
