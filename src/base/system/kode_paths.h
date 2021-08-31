#ifndef kode_linux_paths_included
#define kode_linux_paths_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // readlink
#include <dlfcn.h>

/*
  KODE_GetHomePath    '/home/<user>/'
  KODE_GetExeFilename '/home/<user>/Desktop/skeilib/bin/skeilib_debug'
  KODE_GetExePath     '/home/<user>/Desktop/skeilib/bin/'
*/

//----------------------------------------------------------------------

char* KODE_GetHomePath(char* ABuffer) {
  ABuffer[0] = '\0';
  char* env = getenv("HOME");
  if (env) KODE_Strcat(ABuffer,env);
  //KStrcat(ABuffer, (char*)"\\");
  KODE_Strcat(ABuffer, (char*)"/");
  return ABuffer;
}

//----------

const char* KODE_GetExeFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  int32_t len = readlink("/proc/self/exe",ABuffer,KODE_MAX_PATH_LENGTH-1);
  if (len != -1) {
    ABuffer[len] = '\0';
  }
  return ABuffer;
}

const char* KODE_GetExePath(char* ABuffer) {
  char filepath[KODE_MAX_PATH_LENGTH] = "";
  long rd_res = readlink("/proc/self/exe", filepath, KODE_MAX_PATH_LENGTH);
  filepath[KODE_MAX_PATH_LENGTH-1] = '\0';
  if (rd_res) {
    const char* slash = KODE_Strrchr(filepath, '/');
    if (slash) {
      int len = (slash + 1) - (char*)filepath;
      KODE_Strncpy(ABuffer, filepath, len);
      ABuffer[len] = '\0';
    }
    else {
      //SStrcat(ABuffer, (char*)"./");
      //SStrcat(ABuffer, (char*)"/"); // -> "/" -> root :-/
    }
  }
  return ABuffer;
}

//----------

const char* KODE_GetLibFilename(char* ABuffer) {
  ABuffer[0] = '\0';
  Dl_info dli;
  dladdr(__func__, &dli);
  //int32 len = KODE_Strlen(dli.dli_fname);
  KODE_Strcpy(ABuffer,dli.dli_fname);
  return ABuffer;//KODE_NULL;
}

//----------

const char* KODE_GetLibPath(char* ABuffer) {
  Dl_info dli;
  dladdr(__func__, &dli);
  const char* slash = KODE_Strrchr(dli.dli_fname, '/');
  if (slash) {
    int len = (slash + 1) - (char*)dli.dli_fname;
    KODE_Strncpy(ABuffer, dli.dli_fname, len);
    ABuffer[len] = '\0';
  }
  else {
    KODE_Strcat(ABuffer, (char*)"./");
  }
  return ABuffer;
}

//----------------------------------------------------------------------

#ifdef KODE_LIB
  #define KODE_GetBaseFilename  KODE_GetLibFilename
  #define KODE_GetBasePath      KODE_GetLibPath
#else
  #define KODE_GetBaseFilename  KODE_GetExeFilename
  #define KODE_GetBasePath      KODE_GetExePath
#endif


//----------------------------------------------------------------------
#endif











#if 0

/*
  linux exe
  KODE_DTrace("KGetHomePath %s\n",KGetHomePath(buffer)); // '/home/skei/'
  KODE_DTrace("KGetBaseFile %s\n",KGetBaseFile(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug'
  KODE_DTrace("KGetBasePath %s\n",KGetBasePath(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug./' !!
*/

//----------

#include <stdio.h>
#include <stdlib.h>

#ifdef KODE_LINUX
  #include <unistd.h> // readlink
  #include <dlfcn.h>
#endif

#ifdef KODE_WIN32
  #include <Shlobj.h>
#endif

#include "base/kode.h"
#include "base/kode_const.h"

//----------------------------------------------------------------------

char* KODE_GetHomePath(char* ABuffer) {
  //SStrcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';

  #ifdef KODE_LINUX
  char* env = getenv("HOME");
  if (env) KODE_Strcat(ABuffer,env);
  //KStrcat(ABuffer, (char*)"\\");
  KODE_Strcat(ABuffer, (char*)"/");
  #endif // KODE_LINUX

  #ifdef KODE_WIN32
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
  WCHAR wch[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, wch))) {
    /*int l =*/ wcstombs(ABuffer,wch,KODE_MAX_STRING_LENGTH-1);
    KODE_Strcat(ABuffer, (char*)"\\");
  }
  #endif // KODE_WIN32

  return ABuffer;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const char* KODE_GetBaseFile(char* ABuffer) {
  ABuffer[0] = '\0';

  #ifdef KODE_LINUX
    //#include <dlfcn.h>
    #if defined KODE_LIBRARY
      Dl_info dli;
      dladdr(__func__, &dli);
      //DTrace("SGetBaseFile: '" << dli.dli_fname << "'");
      //int32 len = SStrlen(dli.dli_fname);
      KODE_Strcpy(ABuffer,dli.dli_fname);
//      return ABuffer;//dli.dli_fname;
    #else //KODE_LIBRARY
      int32_t len = readlink("/proc/self/exe",ABuffer,KODE_MAX_PATH_LENGTH-1);
      if (len != -1) {
        ABuffer[len] = '\0';
//        return ABuffer;
      }
    #endif //KODE_LIBRARY
  #endif // KODE_LINUX

  #ifdef KODE_WIN32
    #if defined KODE_LIBRARY || defined KODE_PLUGIN_EXE
      //char filepath[KODE_MAX_PATH_LENGTH] = "";
      //GetModuleFileName(kode_global_WinInstance, filepath, KODE_MAX_PATH_LENGTH-1);
      //filepath[KODE_MAX_PATH_LENGTH-1] = '\0';
      GetModuleFileName(kode_global_WinInstance, ABuffer, KODE_MAX_PATH_LENGTH-1);
      //ABuffer[KODE_MAX_PATH_LENGTH-1] = '\0';
      //const char* slash = KStrrchr(filepath,'\\');
      //if (slash) {
      //  int len = (slash+1) - (char*)filepath;
      //  KStrncpy(ABuffer, filepath, len);
      //  ABuffer[len] = '\0';
      //}
      //else {
      //  KStrcat(ABuffer,(char*)".\\");
      //}
    #else
      KStrcat(ABuffer,(char*)".\\");
    #endif
  #endif // KODE_WIN32

  return ABuffer;//KODE_NULL;
}


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef KODE_LINUX

  #include <dlfcn.h>
  #include <unistd.h>

  const char* KODE_GetBasePath(char* ABuffer) {
    #if defined KODE_LIBRARY
      Dl_info dli;
      dladdr(__func__, &dli);
      const char* slash = KODE_Strrchr(dli.dli_fname, '/');
      if (slash) {
        int len = (slash + 1) - (char*)dli.dli_fname;
        KODE_Strncpy(ABuffer, dli.dli_fname, len);
        ABuffer[len] = '\0';
      }
      else {
        KODE_Strcat(ABuffer, (char*)"./");
      }
    #elif defined KODE_PLUGIN_EXE
      char filepath[KODE_MAX_PATH_LENGTH] = "";
      long rd_res = readlink("/proc/self/exe", filepath, KODE_MAX_PATH_LENGTH);
      filepath[KODE_MAX_PATH_LENGTH-1] = '\0';
      if (rd_res) {
        const char* slash = KODE_Strrchr(filepath, '/');
        if (slash) {
          int len = (slash + 1) - (char*)filepath;
          KODE_Strncpy(ABuffer, filepath, len);
          ABuffer[len] = '\0';
        }
        else {
          //SStrcat(ABuffer, (char*)"./");
          //SStrcat(ABuffer, (char*)"/"); // -> "/" -> root :-/
        }
      }
    #else
      KODE_Strcat(ABuffer, (char*)"./");
    #endif
    return ABuffer;
  }

#endif // KODE_LINUX

//------------------------------

#ifdef KODE_WIN32

  //#include <windows.h>

  char* KODE_GetBasePath(char* ABuffer) {
    #if defined KODE_LIBRARY || defined KODE_PLUGIN_EXE
      char filepath[KODE_MAX_PATH_LENGTH] = "";
      GetModuleFileName(kode_global_WinInstance, filepath, KODE_MAX_PATH_LENGTH-1);
      filepath[KODE_MAX_PATH_LENGTH-1] = '\0';
      const char* slash = KStrrchr(filepath,'\\');
      if (slash) {
        int len = (slash+1) - (char*)filepath;
        KODE_Strncpy(ABuffer, filepath, len);
        ABuffer[len] = '\0';
      }
      else {
        KODE_Strcat(ABuffer,(char*)".\\");
      }
    #else
      KODE_Strcat(ABuffer,(char*)".\\");
    #endif
    return ABuffer;
  }

#endif // KODE_WIN32

#endif // 0
