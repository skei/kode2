#ifndef kode_stdlib_included
#define kode_stdlib_included
//----------------------------------------------------------------------

//#include "kode.h"

//----------------------------------------------------------------------

#include <inttypes.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

//----------

//void _kode_handle_new(const char* file, const char* func, int line) {
//}
//
//void _kode_handle_delete(const char* file, const char* func, int line) {
//}
//
//#define KODE_New(x) { _kode_new_prefix( __FILE__, __FUNCTION__, __LINE__ ); new x; }
//
//#define KODE_Delete(x) { _kode_delete_prefix(__FILE__,__FUNCTION__,__LINE__); delete x; }

#define KODE_New      new
#define KODE_Delete   delete

#define KODE_Malloc   malloc
#define KODE_Calloc   calloc
#define KODE_Realloc  realloc
#define KODE_Free     free

#define KODE_Memset   memset
#define KODE_Memcpy   memcpy
#define KODE_Memmove  memmove

//----------

#define KODE_Strcat   strcat
#define KODE_Strchr   strchr
#define KODE_Strcmp   strcmp
#define KODE_Strcpy   strcpy
#define KODE_Strrchr  strrchr
#define KODE_Strlen   strlen
#define KODE_Strncpy  strncpy
#define KODE_Strstr   strstr

//----------------------------------------------------------------------
#endif
