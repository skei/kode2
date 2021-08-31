#ifndef kode_inifile_included
#define kode_inifile_included
//----------------------------------------------------------------------

#include "base/kode_file.h"
#include "base/kode_parser.h"
#include "base/kode_paths.h"

//----------------------------------------------------------------------

#define KODE_INI_KEY_NAME_LENGTH      127
#define KODE_INI_KEY_VALUE_LENGTH     127
#define KODE_INI_SECTION_NAME_LENGTH  127

//----------

// -> KODE_IniKeyValuePair

struct KODE_IniKey {
  char name[KODE_INI_KEY_NAME_LENGTH+1] = {0};
  char value[KODE_INI_KEY_VALUE_LENGTH+1] = {0};
};

typedef KODE_Array<KODE_IniKey*> KODE_IniKeys;

//----------

struct KODE_IniSection {
  char          name[KODE_INI_SECTION_NAME_LENGTH+1] = {0};
  KODE_IniKeys  keys;
};

typedef KODE_Array<KODE_IniSection*> KODE_IniSections;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_IniFile {

//------------------------------
private:
//------------------------------

  KODE_IniSections  MSections;

//------------------------------
public:
//------------------------------

  KODE_IniFile() {
  }

  //----------

  ~KODE_IniFile() {
    #ifndef KODE_NO_AUTODELETE
      deleteSections();
    #endif
  }

//------------------------------
public:
//------------------------------

  void appendSection(const char* ASection) {
    KODE_IniSection* section = KODE_New KODE_IniSection();
    KODE_Strncpy(section->name,ASection,KODE_INI_SECTION_NAME_LENGTH);
    MSections.append(section);
  }

  //----------

  void deleteSections() {
    for (uint32_t i=0; i<MSections.size(); i++) {
      KODE_IniSection* section = MSections[i];
      for (uint32_t j=0; j<section->keys.size(); j++) {
        KODE_Delete section->keys[j];
      }
      delete MSections[i];
    }
  }

  //----------

  KODE_IniSection* findSection(const char* ASection) {
    for (uint32_t i=0; i<MSections.size(); i++) {
      KODE_IniSection* section = MSections[i];
      if (KODE_Strcmp(section->name, ASection) == 0) {
        return section;
      }
    }
    return  KODE_NULL;
  }

  //----------

  void appendKey(const char* ASection, const char* AName, const char* AValue) {
    KODE_IniSection* section = findSection(ASection);
    KODE_IniKey* key = KODE_New KODE_IniKey();
    KODE_Strncpy(key->name,AName,KODE_INI_KEY_NAME_LENGTH);
    KODE_Strncpy(key->value,AValue,KODE_INI_KEY_VALUE_LENGTH);
    section->keys.append(key);
  }

  //----------

  KODE_IniKey* findKey(const char* ASection, const char* AKey) {
    KODE_IniSection* section = findSection(ASection);
    if (section) {
      for (uint32_t i=0; i<section->keys.size(); i++) {
        KODE_IniKey* key =section->keys[i];
        if (KODE_Strcmp(key->name, AKey) == 0) return key;
      }
    }
    return KODE_NULL;
  }

  //----------

  uint32_t getNumSections() {
    return MSections.size();
  }

  KODE_IniSection* getSection(uint32_t AIndex) {
    return MSections[AIndex];
  }

  uint32_t getNumKeys(KODE_IniSection* ASection) {
    return ASection->keys.size();
  }

  KODE_IniKey* getKey(KODE_IniSection* ASection, uint32_t AIndex) {
    return ASection->keys[AIndex];
  }

//------------------------------
public:
//------------------------------

  const char* getString(const char* ASection, const char* AKey) {
    KODE_IniKey* key = findKey(ASection,AKey);
    if (key) {
      return key->value;
    }
    return KODE_NULL;
  }

  //----------

  float getFloat(const char* ASection, const char* AKey) {
    KODE_IniKey* key = findKey(ASection,AKey);
    if (key) {
      float f = KODE_StringToFloat((char*)key->value);
      return f;
    }
    return 0.0f;
  }

  //----------

  int32_t getInt(const char* ASection, const char* AKey) {
    KODE_IniKey* key = findKey(ASection,AKey);
    if (key) {
      int32_t i = KODE_StringToInt((char*)key->value);
      return i;
    }
    return 0;
  }

//------------------------------
public:
//------------------------------

  bool load(void) {
    char filename[256];
    KODE_GetBaseFile(filename);
    KODE_StripFileExt(filename);
    KODE_Strcat(filename,".ini");
    return load(filename);
  }

  //----------

  bool load(const char* filename) {
    KODE_File file;
    if (file.exists(filename)) {
      file.open(filename,KODE_FILE_READ_BINARY);
      void* buffer = KODE_NULL;
      uint32_t buffersize = file.length();
      if (buffersize>0) {
        buffer = KODE_Malloc(buffersize);
        file.read(buffer,buffersize);
      }
      file.close();
      parse(buffer,buffersize);
      KODE_Free(buffer);
      return true;
    }
    return false;
  }

  //----------

  bool save(void) {
    char filename[256];
    KODE_GetBaseFile(filename);
    KODE_StripFileExt(filename);
    KODE_Strcat(filename,".ini");
    return save(filename);
  }

  //----------

  bool save(const char* filename) {
    KODE_File file;
    if (file.exists(filename)) {
      KODE_DTrace("%s already exists. overwriting\n",filename);
    }
    file.open(filename,KODE_FILE_WRITE_TEXT);
    for (uint32_t i=0; i<MSections.size(); i++) {
      KODE_IniSection* section = MSections[i];
      // [
      file.write((void*)"[",1);
      // section name
      const char* txt = section->name;
      uint32_t len = KODE_Strlen(txt);
      file.write((void*)txt,len);
      // ]
      file.write((void*)"]\n",2);
      for (uint32_t j=0; j<section->keys.size(); j++) {
        KODE_IniKey* key =section->keys[j];
        // key.name
        const char* txt = key->name;
        uint32_t len = KODE_Strlen(txt);
        bool has_spaces = false;
        for (uint32_t s=0; s<len; s++) {
          if (txt[s] == ' ') {
            has_spaces = true;
            break;
          }
        }
        if (has_spaces) {
          // "key"
          file.write((void*)"\"",1);
          file.write((void*)txt,len);
          file.write((void*)"\"",1);
        }
        else {
          // key
          file.write((void*)txt,len);
        }
        // =
        file.write((void*)" = ",3);
        txt = key->value;
        len = KODE_Strlen(txt);
        has_spaces = false;
        for (uint32_t s=0; s<len; s++) {
          if (txt[s] == ' ') {
            has_spaces = true;
            break;
          }
        }
        if (has_spaces) {
          // "value"
          file.write((void*)"\"",1);
          file.write((void*)txt,len);
          file.write((void*)"\"",1);
        }
        else {
          // value
          file.write((void*)txt,len);
        }
        file.write((void*)"\n",1);
      }
    }
    file.close();
    return false;
  }

  //----------

  //void unload(void) {
  //  if (MBuffer) KODE_Free(MBuffer);
  //  MBuffer = KODE_NULL;
  //}

  //----------

  // todo:
  // parser->getNameValuePair(..)

  void parse(void* ABuffer, uint32_t ABufferSize) {
    KODE_Parser* parser = KODE_New KODE_Parser();
    parser->tokenize((const char*)ABuffer,ABufferSize);

    const char* section = "";
    uint32_t t = 0;
    while (t < parser->getNumTokens()) {
      char* token = parser->getToken(t++);
      uint32_t tlen = KODE_Strlen(token);
      if ((token[0] == '[') && (token[tlen-1] == ']')) {
        token[tlen-1] = 0;
        section = token + 1;
        appendSection(section);
      }
      else {
        const char* equal = parser->getToken(t++);
        if ((equal[0] == '=') && (equal[1] == 0)) {
          const char* value = parser->getToken(t++);
          appendKey(section,token,value);
        }
      }
    }

  }

  //----------

};

//----------------------------------------------------------------------
#endif
