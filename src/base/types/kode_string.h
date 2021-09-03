#ifndef kode_string_included
#define kode_string_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/types/kode_buffer.h"
#include "base/utils/kode_strutils.h"

#include "kode.h"
#include "base/types/kode_buffer.h"
#include "base/utils/kode_convert.h"
#include "base/utils/kode_strutils.h"

#define KODE_String KODE_DynamicString
//#define KODE_String KODE_StaticString


//----------------------------------------------------------------------
//
// static string
//
//----------------------------------------------------------------------

template <int SIZE>
class KODE_StaticString
: public KODE_StaticBuffer<char,SIZE> {
};

//

//----------------------------------------------------------------------
//
// dynamic string
//
//----------------------------------------------------------------------

class KODE_DynamicString
: public KODE_DynamicBuffer<char> {

//------------------------------
public:
//------------------------------

  KODE_DynamicString() {
  }

  //----------

  KODE_DynamicString(const KODE_DynamicString& str) {
    uint32_t size = strlen(str);
    resizeBuffer(size+1);
    strcpy(MBuffer,str);
  }

  //----------

  KODE_DynamicString(const char* str) {
    uint32_t size = strlen(str);
    resizeBuffer(size+1);
    strcpy(MBuffer,str);
  }

  //----------

  virtual ~KODE_DynamicString() {
    //KODE_Print;
  }


//------------------------------
public:
//------------------------------

  uint32_t length() {
    return strlen(MBuffer);
  }

  //----------

  void setString(const char* str) {
    uint32_t size = strlen(str);
    resizeBuffer(size+1);
    strcpy(MBuffer,str);
  }

  //----------

  void appendString(const char* str) {
    uint32_t size = length() + strlen(str);
    resizeBuffer(size+1);
    strcat(MBuffer,str);
  }

//------------------------------
public:
//------------------------------

  KODE_DynamicString& operator = (char* str) {
    setString(str);
    return *this;
  }

  //----------

  KODE_DynamicString& operator = (const char* str) {
    setString(str);
    return *this;
  }

  //----------

  KODE_DynamicString& operator = (KODE_DynamicString& str) {
    setString(str.ptr());
    return *this;
  }

  //----------

  KODE_DynamicString& operator = (int AValue) {
    char temp[16];
    char* str = KODE_IntToString(temp,AValue);
    setString(str);
    return *this;
  }

  //----------

  KODE_DynamicString& operator = (float AValue) {
    char temp[16];
    char* str = KODE_FloatToString(temp,AValue);
    setString(str);
    return *this;
  }

  //------------------------------

  KODE_DynamicString& operator += (char* str) {
    appendString(str);
    return *this;
  }

  //----------

  KODE_DynamicString& operator += (const char* str) {
    appendString(str);
    return *this;
  }

  //----------

  KODE_DynamicString& operator += (KODE_DynamicString& str) {
    appendString(str.ptr());
    return *this;
  }

  //----------

  KODE_DynamicString& operator += (const int i) {
    char temp[16];
    char* str = KODE_IntToString(temp,i);
    appendString(str);
    return *this;
  }

  //----------

  KODE_DynamicString& operator += (const float f) {
    char temp[16];
    char* str = KODE_FloatToString(temp,f);
    appendString(str);
    return *this;
  }

  //------------------------------

  bool operator == (const char* str) {
    return (strcmp(MBuffer,str) == 0) ? true : false;
  }

  //----------

  bool operator == (KODE_DynamicString& str) {
    return (strcmp(MBuffer,str.ptr()) == 0) ? true : false;
  }

  //----------

  bool operator != (const char* str) {
    return (strcmp(MBuffer,str) != 0) ? true : false;
  }

  //----------

  bool operator != (KODE_DynamicString& str) {
    return (strcmp(MBuffer,str.ptr()) != 0) ? true : false;
  }

//------------------------------
public: // strutils
//------------------------------

  char* createUnique(const char* APrefix, void* APtr) {
    uint32_t len = strlen(APrefix);
    const uint32_t hex_size  = sizeof(void*) * 2;   // 8*2 = 16
    const uint32_t rand_size = sizeof(int32_t) * 2; // 4*2 = 8
    // prefix + ':' + inv_ptr + ':' + random + '\0'
    resizeBuffer( len + 1 + hex_size + 1 + rand_size + 1);
    KODE_CreateUniqueString(MBuffer,APrefix,APtr);
    return MBuffer;
  }

  //----------

  char* find(char* str) {
    return KODE_FindString(MBuffer,str);
  }

  //----------

  uint32_t hash() {
    return KODE_HashString(MBuffer);
  }

  //----------

  char* lowerCase() {
    KODE_LowerCase(MBuffer);
    return MBuffer;
  }

  //----------

  void makeValidSymbol() {
    KODE_MakeValidSymbol(MBuffer);
  }

  //----------

  //void replaceFileExt(const char* ext) {
  //  KODE_ReplaceFileExt(const char* ext);
  //}

  //----------

  char* replace(const char* str1, const char* str2) {
    KODE_ReplaceString(MBuffer,str1,str2);
    return MBuffer;
  }

  //----------

  char* replace(const char c1, const char c2) {
    KODE_ReplaceChars(MBuffer,c1,c2);
    return MBuffer;
  }

  //----------

  char* reverse() {
    KODE_ReverseString(MBuffer);
    return MBuffer;
  }

  //----------

  bool searchWildcards(const char* wildcards) {
    return KODE_SearchWildcards(MBuffer,wildcards);
  }

  //----------

  //void stripFileExt(void) {
  //  KODE_stripFileExt(void);
  //}

  //----------

  //void stripPath() {
  //  KODE_StripPath(MBuffer);
  //}

  //----------

  char* upperCase() {
    KODE_UpperCase(MBuffer);
    return MBuffer;
  }

  //----------

};

//----------------------------------------------------------------------

template <class T>
KODE_DynamicString operator + (T var, const KODE_DynamicString& str)  {
  //KODE_Print;
  KODE_DynamicString svar = var;
  return svar += str;
}

//----------------------------------------------------------------------
#endif





//
//  KODE_StaticString(const KODE_StaticString& str) {
//    KODE_StaticString* sstr = (KODE_StaticString*)&str;
//    strcpy(MBuffer,sstr->ptr());
//  }
//
//  KODE_StaticString(const char* str) {
//    strcpy(MBuffer,str);
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char* ptr() {
//    return MBuffer;
//  }
//
//  //----------
//
//  uint32_t length() {
//    return strlen(MBuffer);
//  }
//
//  //----------
//
//  bool empty(void) {
//    if (MBuffer[0] == 0) return true;
//    return false;
//  }
//
//  //----------
//
//  bool notEmpty(void) {
//    if (MBuffer[0] == 0) return false;
//    return true;
//  }
//
//  //----------
//
//  void clear(bool AErase=false) {
//    if (AErase) memset(MBuffer,0,sizeof(MBuffer));
//    else MBuffer[0] = 0;
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char& operator [] (uint32_t AIndex) const {
//    return MBuffer[AIndex];
//  }
//
//  //----------
//
//  operator char* () {
//    return MBuffer;
//  }
//
//  //----------
//
//  operator const char* ()	const {
//    return MBuffer;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (const char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (/*const*/ KODE_StaticString& str) {
//    strcpy(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (int AValue) {
//    char temp[16];
//    char* str = KODE_IntToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (float AValue) {
//    char temp[16];
//    char* str = KODE_FloatToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (const char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (/*const*/ KODE_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (const int i) {
//    char temp[16];
//    char* str = KODE_IntToString(temp,i);
//    //KODE_Trace("%s\n",str);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (const float f) {
//    char temp[16];
//    char* str = KODE_FloatToString(temp,f);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  bool operator == (const char* str) {
//    return (strcmp(MBuffer,str) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator == (KODE_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (const char* str) {
//    return (strcmp(MBuffer,str) != 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (KODE_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) != 0) ? true : false;
//  }
//
////------------------------------
//private:
////------------------------------
//
//  bool _match(char* wildcards, char* str) {
//    bool Yes = 1;
//    //iterate and delete '?' and '*' one by one
//    while(*wildcards != '\0' && Yes && *str != '\0') {
//      if (*wildcards == '?') str ++;
//      else if (*wildcards == '*') {
//        Yes = _scan(wildcards, str);
//        wildcards --;
//      }
//      else {
//        Yes = (*wildcards == *str);
//        str ++;
//      }
//      wildcards ++;
//    }
//    while (*wildcards == '*' && Yes)  wildcards ++;
//    return Yes && *str == '\0' && *wildcards == '\0';
//  }
//
//  //----------
//
//  bool _scan(char*& wildcards, char*& str) {
//    // remove the '?' and '*'
//    for (wildcards ++; *str != '\0' && (*wildcards == '?' || *wildcards == '*'); wildcards ++)
//      if (*wildcards == '?') str ++;
//    while ( *wildcards == '*') wildcards ++;
//    // if str is empty and wildcards has more characters or,
//    // wildcards is empty, return
//    if (*str == '\0' && *wildcards != '\0') return false;
//    if (*str == '\0' && *wildcards == '\0')	return true;
//    // else search substring
//    else {
//      char* wds_copy = wildcards;
//      char* str_copy = str;
//      bool  Yes     = 1;
//      do {
//        if (!_match(wildcards,str))	str_copy++;
//        wildcards = wds_copy;
//        str = str_copy;
//        while ((*wildcards != *str) && (*str != '\0')) str ++;
//        wds_copy = wildcards;
//        str_copy = str;
//      } while ((*str != '\0') ? !_match(wildcards, str) : (Yes = false) != false);
//      if (*str == '\0' && *wildcards == '\0')	return true;
//      return Yes;
//    }
//  }
//
////------------------------------
//public:
////------------------------------
//
//  // search the match string
//  // wildcards can be '?' and '*' combination
//  // return value : true (pattern matchs string), false (no match)
//
//  bool search(const char* wildcards) {
//    return _match((char*)wildcards, MBuffer);
//  }
//
//  //----------
//
//  // returns first occurence of 'str'
//  // or NULL if not found
//
//  char* find(char* str) {
//    return strstr(MBuffer,str);
//  }
//
//  //----------
//
//  void append(const char* str) {
//    strcat(MBuffer,str);
//  }
//
//  //----------
//
//  void append(/*const*/KODE_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//  }
//
//  //----------
//
//  // replace all instances of c1 with c2
//
//  void replaceChars(const char c1, const char c2) {
//    char *pos;
//    while ((pos = strchr(MBuffer,c1))) {
//      *pos = c2;
//    }
//  }
//
//  //----------
//
//  // replace all occurences of str1 with str2
//  // assumes final size will be < 1024
//  // be careful about recursion! replace r with r2 for example.. :-/
//  // todo: wipe out src after replacing, so we don't re-find it?
//
//  void replace(const char* str1, const char* str2) {
//    //_grow_buffer(1024);
//    char temp[1024];
//    uint32_t length = strlen(MBuffer);
//    memcpy(temp,MBuffer,length+1);
//    char *pos;
//    int clen1 = strlen(str1);
//    int clen2 = strlen(str2);
//    while ((pos = strstr(temp,str1))) {
//      //char* bufpos = pos - temp + MBuffer;
//      memmove(pos + clen2, pos + clen1, strlen(pos) - clen1 + 1);
//      memcpy(pos, str2, clen2);
//      //memset(pos , 1, clen1);
//    }
//    //_resize_buffer( strlen(MBuffer) );
//    strcpy(MBuffer,temp);
//  }
//
//  //----------
//
//  void lowerCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='A') && (c<='Z')) *ptr = c + 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void upperCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='a') && (c<='z')) *ptr = c - 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void reverse(void) {
//    int i;
//    int len = strlen(MBuffer) - 1;
//    int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
//    for (i=0; i<=mid; ++i) {
//      char buf = MBuffer[i];
//      MBuffer[i] = MBuffer[len - i];
//      MBuffer[len - i] = buf;
//    }
//  }
//
//  //----------
//
//  void makeValidSymbol(void) {
//    for (uint32_t i=0; i<strlen(MBuffer); i++) {
//      unsigned char c = MBuffer[i];
//      if ((c<32) || (c>127)) MBuffer[i] = '_';
//      else MBuffer[i] = KODE_VALID_CSYMBOLS[c];
//    }
//  }
//
//  //----------
//
//  // ptr to (reversed) hex
//  // prefix + ptr2hex (inverted) + random number
//  // (for creating unique strings (based on pointers)
//  // TODO: hash?
//
//  void createUnique(const char* APrefix, void* APtr) {
//    const char hex_table[17] = "0123456789ABCDEF";  // +'\0' = 17
//    const uint32_t hex_size  = sizeof(void*) * 2;
//    const uint32_t rand_size = sizeof(int32_t) * 2;
//    //_resize_buffer( strlen(APrefix) + hex_size + 1 + rand_size );
//    char* buf = MBuffer;//temp;
//    while (*APrefix != '\0') *buf++ = *APrefix++;   // memset + update ptrs
//    intptr_t iptr = (intptr_t)APtr;
//    for (uint32_t i=0; i<hex_size; i++) {
//      int j = iptr & 0x0f;
//      iptr >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = ':';
//    int32_t rnd = KODE_RandomInt();
//    for (uint32_t i=0; i<rand_size; i++) {
//      int j = rnd & 0x0f;
//      rnd >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = '\0';
//  }
//
//  //----------
//
//  // https://stackoverflow.com/questions/7666509/hash-function-for-string
//
//  uint32_t hash(void) {
//    char* ptr = MBuffer;
//    unsigned long h = 5381;
//    int c;
//    while ((c = *ptr++)) {
//      h = ((h << 5) + h) + c; // h * 33 + c
//    }
//    return h;
//  }
//
//  //----------
//
//  // problem if there is a dot in the path
//  // todo: find last dot..
//
//  /*
//  void stripFileExt(void) {
//    char *dot;
//    dot = strrchr(MBuffer,'.');
//    if (dot) {
//      int32_t len = dot - MBuffer;
//      _resize_buffer(len);
//    }
//  }
//  */
//
//  //----------
//
//  /*
//  void replaceFileExt(const char* ext) {
//    stripFileExt();
//    append(".");
//    append(ext);
//  }
//  */
//
//  //----------
//
//  // /home/skei/test -> test
//
//  /*
//  void stripPath(void) {
//    const char *slash, *backslash;
//    slash     = strrchr(MBuffer,'/');
//    backslash = strrchr(MBuffer,'\\');//+1;
//    if (slash) {
//      _copy_buffer(slash + 1);
//    }
//    else if (backslash) {
//      _copy_buffer(backslash + 1); // see above
//    }
//  }
//  */
//
//
//};
//
//
////----------
//
////template <class T>
////KODE_StaticString operator + (T var, const KODE_StaticString& str)  {
////  KODE_StaticString svar = var;
////  return svar += str;
////}
//
//template <int SIZE, class T>
//KODE_StaticString<SIZE> operator + (T var, const KODE_StaticString<SIZE>& str)  {
//  KODE_StaticString<SIZE> svar = var;
//  return svar += str;
//}
//
//#endif // 0





//----------------------------------------------------------------------
//
//----------------------------------------------------------------------





//#if 0
//
//// todo
//// inherit from KODE_DynamicBuffer
//
//class KODE_DynamicString {
//
//private:
//
//  uint32_t  MLength;
//  uint32_t  MBufferSize;
//  char*     MBuffer;
//
//private:
//
//  void resize(uint32_t ASize) {
//  }
//
//};
//
////----------
//
//typedef KODE_StaticString<KODE_MAX_STRING_LENGTH> KODE_String;
//
//#endif // 0







//#include "base/kode_convert.h"
//#include "base/kode_random.h"
//
//template <int SIZE>
//class KODE_StaticString {
//
//private:
//
//  //char MBuffer[KODE_MAX_STRING_LENGTH];
//  char MBuffer[SIZE];
//
////------------------------------
////
////------------------------------
//
//public:
//
//  KODE_StaticString() {
//    MBuffer[0] = 0;
//  }
//
//  KODE_StaticString(const KODE_StaticString& str) {
//    KODE_StaticString* sstr = (KODE_StaticString*)&str;
//    strcpy(MBuffer,sstr->ptr());
//  }
//
//  KODE_StaticString(const char* str) {
//    strcpy(MBuffer,str);
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char* ptr() {
//    return MBuffer;
//  }
//
//  //----------
//
//  uint32_t length() {
//    return strlen(MBuffer);
//  }
//
//  //----------
//
//  bool empty(void) {
//    if (MBuffer[0] == 0) return true;
//    return false;
//  }
//
//  //----------
//
//  bool notEmpty(void) {
//    if (MBuffer[0] == 0) return false;
//    return true;
//  }
//
//  //----------
//
//  void clear(bool AErase=false) {
//    if (AErase) memset(MBuffer,0,sizeof(MBuffer));
//    else MBuffer[0] = 0;
//  }
//
////------------------------------
//public:
////------------------------------
//
//  const char& operator [] (uint32_t AIndex) const {
//    return MBuffer[AIndex];
//  }
//
//  //----------
//
//  operator char* () {
//    return MBuffer;
//  }
//
//  //----------
//
//  operator const char* ()	const {
//    return MBuffer;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (const char* str) {
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (/*const*/ KODE_StaticString& str) {
//    strcpy(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (int AValue) {
//    char temp[16];
//    char* str = KODE_IntToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator = (float AValue) {
//    char temp[16];
//    char* str = KODE_FloatToString(temp,AValue);
//    strcpy(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (const char* str) {
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (/*const*/ KODE_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (const int i) {
//    char temp[16];
//    char* str = KODE_IntToString(temp,i);
//    //KODE_Trace("%s\n",str);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  KODE_StaticString& operator += (const float f) {
//    char temp[16];
//    char* str = KODE_FloatToString(temp,f);
//    strcat(MBuffer,str);
//    return *this;
//  }
//
//  //----------
//
//  bool operator == (const char* str) {
//    return (strcmp(MBuffer,str) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator == (KODE_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) == 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (const char* str) {
//    return (strcmp(MBuffer,str) != 0) ? true : false;
//  }
//
//  //----------
//
//  bool operator != (KODE_StaticString& str) {
//    return (strcmp(MBuffer,str.ptr()) != 0) ? true : false;
//  }
//
////------------------------------
//private:
////------------------------------
//
//  bool _match(char* wildcards, char* str) {
//    bool Yes = 1;
//    //iterate and delete '?' and '*' one by one
//    while(*wildcards != '\0' && Yes && *str != '\0') {
//      if (*wildcards == '?') str ++;
//      else if (*wildcards == '*') {
//        Yes = _scan(wildcards, str);
//        wildcards --;
//      }
//      else {
//        Yes = (*wildcards == *str);
//        str ++;
//      }
//      wildcards ++;
//    }
//    while (*wildcards == '*' && Yes)  wildcards ++;
//    return Yes && *str == '\0' && *wildcards == '\0';
//  }
//
//  //----------
//
//  bool _scan(char*& wildcards, char*& str) {
//    // remove the '?' and '*'
//    for (wildcards ++; *str != '\0' && (*wildcards == '?' || *wildcards == '*'); wildcards ++)
//      if (*wildcards == '?') str ++;
//    while ( *wildcards == '*') wildcards ++;
//    // if str is empty and wildcards has more characters or,
//    // wildcards is empty, return
//    if (*str == '\0' && *wildcards != '\0') return false;
//    if (*str == '\0' && *wildcards == '\0')	return true;
//    // else search substring
//    else {
//      char* wds_copy = wildcards;
//      char* str_copy = str;
//      bool  Yes     = 1;
//      do {
//        if (!_match(wildcards,str))	str_copy++;
//        wildcards = wds_copy;
//        str = str_copy;
//        while ((*wildcards != *str) && (*str != '\0')) str ++;
//        wds_copy = wildcards;
//        str_copy = str;
//      } while ((*str != '\0') ? !_match(wildcards, str) : (Yes = false) != false);
//      if (*str == '\0' && *wildcards == '\0')	return true;
//      return Yes;
//    }
//  }
//
////------------------------------
//public:
////------------------------------
//
//  // search the match string
//  // wildcards can be '?' and '*' combination
//  // return value : true (pattern matchs string), false (no match)
//
//  bool search(const char* wildcards) {
//    return _match((char*)wildcards, MBuffer);
//  }
//
//  //----------
//
//  // returns first occurence of 'str'
//  // or NULL if not found
//
//  char* find(char* str) {
//    return strstr(MBuffer,str);
//  }
//
//  //----------
//
//  void append(const char* str) {
//    strcat(MBuffer,str);
//  }
//
//  //----------
//
//  void append(/*const*/KODE_StaticString& str) {
//    strcat(MBuffer,str.ptr());
//  }
//
//  //----------
//
//  // replace all instances of c1 with c2
//
//  void replaceChars(const char c1, const char c2) {
//    char *pos;
//    while ((pos = strchr(MBuffer,c1))) {
//      *pos = c2;
//    }
//  }
//
//  //----------
//
//  // replace all occurences of str1 with str2
//  // assumes final size will be < 1024
//  // be careful about recursion! replace r with r2 for example.. :-/
//  // todo: wipe out src after replacing, so we don't re-find it?
//
//  void replace(const char* str1, const char* str2) {
//    //_grow_buffer(1024);
//    char temp[1024];
//    uint32_t length = strlen(MBuffer);
//    memcpy(temp,MBuffer,length+1);
//    char *pos;
//    int clen1 = strlen(str1);
//    int clen2 = strlen(str2);
//    while ((pos = strstr(temp,str1))) {
//      //char* bufpos = pos - temp + MBuffer;
//      memmove(pos + clen2, pos + clen1, strlen(pos) - clen1 + 1);
//      memcpy(pos, str2, clen2);
//      //memset(pos , 1, clen1);
//    }
//    //_resize_buffer( strlen(MBuffer) );
//    strcpy(MBuffer,temp);
//  }
//
//  //----------
//
//  void lowerCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='A') && (c<='Z')) *ptr = c + 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void upperCase(void) {
//    char* ptr = MBuffer;
//    int c;
//    while ((c = *ptr)) {
//      if ((c>='a') && (c<='z')) *ptr = c - 0x20;
//      ptr++;
//    }
//  }
//
//  //----------
//
//  void reverse(void) {
//    int i;
//    int len = strlen(MBuffer) - 1;
//    int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
//    for (i=0; i<=mid; ++i) {
//      char buf = MBuffer[i];
//      MBuffer[i] = MBuffer[len - i];
//      MBuffer[len - i] = buf;
//    }
//  }
//
//  //----------
//
//  void makeValidSymbol(void) {
//    for (uint32_t i=0; i<strlen(MBuffer); i++) {
//      unsigned char c = MBuffer[i];
//      if ((c<32) || (c>127)) MBuffer[i] = '_';
//      else MBuffer[i] = KODE_VALID_CSYMBOLS[c];
//    }
//  }
//
//  //----------
//
//  // ptr to (reversed) hex
//  // prefix + ptr2hex (inverted) + random number
//  // (for creating unique strings (based on pointers)
//  // TODO: hash?
//
//  void createUnique(const char* APrefix, void* APtr) {
//    const char hex_table[17] = "0123456789ABCDEF";  // +'\0' = 17
//    const uint32_t hex_size  = sizeof(void*) * 2;
//    const uint32_t rand_size = sizeof(int32_t) * 2;
//    //_resize_buffer( strlen(APrefix) + hex_size + 1 + rand_size );
//    char* buf = MBuffer;//temp;
//    while (*APrefix != '\0') *buf++ = *APrefix++;   // memset + update ptrs
//    intptr_t iptr = (intptr_t)APtr;
//    for (uint32_t i=0; i<hex_size; i++) {
//      int j = iptr & 0x0f;
//      iptr >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = ':';
//    int32_t rnd = KODE_RandomInt();
//    for (uint32_t i=0; i<rand_size; i++) {
//      int j = rnd & 0x0f;
//      rnd >>= 4;
//      *buf++ = hex_table[j];
//    }
//    *buf++ = '\0';
//  }
//
//  //----------
//
//  // https://stackoverflow.com/questions/7666509/hash-function-for-string
//
//  uint32_t hash(void) {
//    char* ptr = MBuffer;
//    unsigned long h = 5381;
//    int c;
//    while ((c = *ptr++)) {
//      h = ((h << 5) + h) + c; // h * 33 + c
//    }
//    return h;
//  }
//
//  //----------
//
//  // problem if there is a dot in the path
//  // todo: find last dot..
//
//  /*
//  void stripFileExt(void) {
//    char *dot;
//    dot = strrchr(MBuffer,'.');
//    if (dot) {
//      int32_t len = dot - MBuffer;
//      _resize_buffer(len);
//    }
//  }
//  */
//
//  //----------
//
//  /*
//  void replaceFileExt(const char* ext) {
//    stripFileExt();
//    append(".");
//    append(ext);
//  }
//  */
//
//  //----------
//
//  // /home/skei/test -> test
//
//  /*
//  void stripPath(void) {
//    const char *slash, *backslash;
//    slash     = strrchr(MBuffer,'/');
//    backslash = strrchr(MBuffer,'\\');//+1;
//    if (slash) {
//      _copy_buffer(slash + 1);
//    }
//    else if (backslash) {
//      _copy_buffer(backslash + 1); // see above
//    }
//  }
//  */
//
//
//};
//
//
////----------
//
////template <class T>
////KODE_StaticString operator + (T var, const KODE_StaticString& str)  {
////  KODE_StaticString svar = var;
////  return svar += str;
////}
//
//template <int SIZE, class T>
//KODE_StaticString<SIZE> operator + (T var, const KODE_StaticString<SIZE>& str)  {
//  KODE_StaticString<SIZE> svar = var;
//  return svar += str;
//}





//----------------------------------------------------------------------
//
//----------------------------------------------------------------------





//// todo
//// inherit from KODE_DynamicBuffer
//
//class KODE_DynamicString {
//
//private:
//
//  uint32_t  MLength;
//  uint32_t  MBufferSize;
//  char*     MBuffer;
//
//private:
//
//  void resize(uint32_t ASize) {
//  }
//
//};
//
////----------
//
//typedef KODE_StaticString<KODE_MAX_STRING_LENGTH> KODE_String;
