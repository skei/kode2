#ifndef kode_parser_included
#define kode_parser_included
//----------------------------------------------------------------------

// TODO: tokenizer..

#include "base/kode_array.h"

//----------

class KODE_Parser {

  private:
    uint32_t          MBufLen;
    char*             MBuffer;
    KODE_CharPtrArray MTokens;

  public:

    KODE_Parser() {
      MBufLen = 0;
      MBuffer = KODE_NULL;
    }

    //----------

    ~KODE_Parser() {
      if (MBuffer) KODE_Free(MBuffer);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    uint32_t  getNumTokens(void) { return MTokens.size(); }
    char*     getToken(uint32_t i) { return MTokens[i]; }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      space is a separator! cr,lf,tab is too ( <= 32 )
      change <=32 to <32 to turn space into a character, not a separator

      copies buffer..
    */
    
    // todo: tokenize()

    void tokenize(const char* ASource, uint32_t ALength) {
      bool quotes = false;
      if (MBuffer) KODE_Free(MBuffer);
      //MBufLen = 0;
      MTokens.clear(/*true*/);

      MBufLen = ALength;
      MBuffer = (char*)KODE_Malloc(MBufLen+1);
      KODE_Memcpy(MBuffer,(char*)ASource,MBufLen);
      MBuffer[MBufLen] = 0;


      for (uint32_t i=0; i<MBufLen; i++) {
        // replace any non-visual (<32) chars with '\0'
        if (MBuffer[i]<32) {
          MBuffer[i] = 0;
        }
        // keep spaces inside "" -> one token
        if (MBuffer[i]==32) { // space
          if (!quotes) MBuffer[i] = 0;
        }
        if (MBuffer[i] == 34 ) { // "
          if (quotes) quotes = false;
          else quotes = true;
          MBuffer[i] = 0;
        }
      }

      // new token character after 0

      char prev_c = 0;
      for (uint32_t i=0; i<MBufLen; i++) {
        char c = MBuffer[i];
        if ((prev_c == 0) && (c != 0)) {
          char* ptr = &MBuffer[i];
          MTokens.append( ptr );
        }
        prev_c = c;
      }
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void dumpTokens(void) {
      int32_t num = MTokens.size();
      for (int32_t i=0; i<num; i++) {
        KODE_DTrace("%i '%s'\n",i,MTokens[i]);
      }
    }

};

//----------------------------------------------------------------------
#endif
