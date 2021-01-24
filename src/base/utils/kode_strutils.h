#ifndef kode_strutils_included
#define kode_strutils_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/utils/kode_random.h"

//----------------------------------------------------------------------
// private:
//----------------------------------------------------------------------

bool _kode_str_match(char* wildcards, char* str);
bool _kode_str_scan(char*& wildcards, char*& str);

//----------

bool _kode_str_match(char* wildcards, char* str) {
  bool Yes = 1;
  //iterate and delete '?' and '*' one by one
  while(*wildcards != '\0' && Yes && *str != '\0') {
    if (*wildcards == '?') str ++;
    else if (*wildcards == '*') {
      Yes = _kode_str_scan(wildcards, str);
      wildcards --;
    }
    else {
      Yes = (*wildcards == *str);
      str ++;
    }
    wildcards ++;
  }
  while (*wildcards == '*' && Yes)  wildcards ++;
  return Yes && *str == '\0' && *wildcards == '\0';
}

//----------

bool _kode_str_scan(char*& wildcards, char*& str) {
  // remove the '?' and '*'
  for (wildcards ++; *str != '\0' && (*wildcards == '?' || *wildcards == '*'); wildcards ++)
    if (*wildcards == '?') str ++;
  while ( *wildcards == '*') wildcards ++;
  // if str is empty and wildcards has more characters or,
  // wildcards is empty, return
  if (*str == '\0' && *wildcards != '\0') return false;
  if (*str == '\0' && *wildcards == '\0')	return true;
  // else search substring
  else {
    char* wds_copy = wildcards;
    char* str_copy = str;
    bool  Yes     = 1;
    do {
      if (!_kode_str_match(wildcards,str))	str_copy++;
      wildcards = wds_copy;
      str = str_copy;
      while ((*wildcards != *str) && (*str != '\0')) str ++;
      wds_copy = wildcards;
      str_copy = str;
    } while ((*str != '\0') ? !_kode_str_match(wildcards, str) : (Yes = false) != false);
    if (*str == '\0' && *wildcards == '\0')	return true;
    return Yes;
  }
}

//----------------------------------------------------------------------
// public:
//----------------------------------------------------------------------

// modifies length

/*
void KODE_AppendString(char* buffer, const char* str) {
  KODE_Strcat(buffer,str);
}
*/

//----------------------------------------------------------------------
// public:
//----------------------------------------------------------------------

// ptr to (reversed) hex
// prefix + ptr2hex (inverted) + random number
// (for creating unique strings (based on pointers)
// TODO: hash?

void KODE_CreateUniqueString(char* buffer, const char* APrefix, void* APtr) {
  const char hex_table[17] = "0123456789ABCDEF";  // +'\0' = 17
  const uint32_t hex_size  = sizeof(void*) * 2;
  const uint32_t rand_size = sizeof(int32_t) * 2;
  //_resize_buffer( KODE_Strlen(APrefix) + hex_size + 1 + rand_size );
  char* buf = buffer;//temp;
  while (*APrefix != '\0') *buf++ = *APrefix++;   // memset + update ptrs
  *buf++ = ':';
  intptr_t iptr = (intptr_t)APtr;
  for (uint32_t i=0; i<hex_size; i++) {
    int j = iptr & 0x0f;
    iptr >>= 4;
    *buf++ = hex_table[j];
  }
  *buf++ = ':';
  int32_t rnd = KODE_RandomInt();
  for (uint32_t i=0; i<rand_size; i++) {
    int j = rnd & 0x0f;
    rnd >>= 4;
    *buf++ = hex_table[j];
  }
  *buf++ = '\0';
}

//----------

// returns first occurence of 'str'
// or NULL if not found

char* KODE_FindString(char* buffer, char* str) {
  return KODE_Strstr(buffer,str);
}

//----------

// https://stackoverflow.com/questions/7666509/hash-function-for-string

uint32_t KODE_HashString(char* buffer) {
  char* ptr = buffer;
  unsigned long h = 5381;
  int c;
  while ((c = *ptr++)) {
    h = ((h << 5) + h) + c; // h * 33 + c
  }
  return h;
}

//----------

void KODE_LowerCase(char* buffer) {
  char* ptr = buffer;
  int c;
  while ((c = *ptr)) {
    if ((c>='A') && (c<='Z')) *ptr = c + 0x20;
    ptr++;
  }
}

//----------

void KODE_MakeValidSymbol(char* buffer) {
  for (uint32_t i=0; i<KODE_Strlen(buffer); i++) {
    unsigned char c = buffer[i];
    if ((c<32) || (c>127)) buffer[i] = '_';
    else buffer[i] = KODE_CSYMBOLS[c];
  }
}

//----------

/*
void KODE_ReplaceFileExt(const char* ext) {
  KODE_StripFileExt();
  append(".");
  append(ext);
}
*/

//----------

// replace all occurences of str1 with str2
// assumes final size will be < 1024
// be careful about recursion! replace r with r2 for example.. :-/
// todo: wipe out src after replacing, so we don't re-find it?

void KODE_ReplaceString(char* buffer, const char* str1, const char* str2) {
  //_grow_buffer(1024);
  char temp[1024];
  uint32_t length = KODE_Strlen(buffer);
  KODE_Memcpy(temp,buffer,length+1);
  char *pos;
  int clen1 = KODE_Strlen(str1);
  int clen2 = KODE_Strlen(str2);
  while ((pos = KODE_Strstr(temp,str1))) {
    //char* bufpos = pos - temp + MBuffer;
    KODE_Memmove(pos + clen2, pos + clen1, KODE_Strlen(pos) - clen1 + 1);
    KODE_Memcpy(pos, str2, clen2);
    //KODE_Memset(pos , 1, clen1);
  }
  //_resize_buffer( KODE_Strlen(MBuffer) );
  KODE_Strcpy(buffer,temp);
}

//----------

// replace all instances of c1 with c2

void KODE_ReplaceChars(char* buffer, const char c1, const char c2) {
  char *pos;
  while ((pos = KODE_Strchr(buffer,c1))) {
    *pos = c2;
  }
}

//----------

void KODE_ReverseString(char* buffer) {
  int i;
  int len = KODE_Strlen(buffer) - 1;
  int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
  for (i=0; i<=mid; ++i) {
    char buf = buffer[i];
    buffer[i] = buffer[len - i];
    buffer[len - i] = buf;
  }
}

//----------

// search the match string
// wildcards can be '?' and '*' combination
// return value : true (pattern matchs string), false (no match)

bool KODE_SearchWildcards(char* buffer, const char* wildcards) {
  return _kode_str_match((char*)wildcards, buffer);
}

//----------

// problem if there is a dot in the path
// todo: find last dot..

/*
void KODE_stripFileExt(void) {
  char *dot;
  dot = KODE_Strrchr(MBuffer,'.');
  if (dot) {
    int32_t len = dot - MBuffer;
    _resize_buffer(len);
  }
}
*/

//----------

// /home/skei/test -> test

/*
void KODE_StripPath(char* buffer) {
  const char *slash, *backslash;
  slash     = KODE_Strrchr(buffer,'/');
  backslash = KODE_Strrchr(buffer,'\\');//+1;
  if (slash) {
    _copy_buffer(slash + 1);
  }
  else if (backslash) {
    _copy_buffer(backslash + 1); // see above
  }
}
*/

//----------

void KODE_UpperCase(char* buffer) {
  char* ptr = buffer;
  int c;
  while ((c = *ptr)) {
    if ((c>='a') && (c<='z')) *ptr = c - 0x20;
    ptr++;
  }
}


//----------------------------------------------------------------------
#endif

