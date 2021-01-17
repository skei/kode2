#ifndef kode_exe_included
#define kode_exe_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------

#ifdef KODE_DEBUG_EXE
  #define KODE_ExePrint KODE_Print
  #define KODE_ExeDPrint KODE_DPrint
  #define KODE_EXEPRINT KODE_PRINT
#else
  #define KODE_ExePrint KODE_NoPrint
  #define KODE_ExeDPrint KODE_NoPrint
  #define KODE_EXEPRINT {}
#endif

//----------------------------------------------------------------------
#endif
