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
