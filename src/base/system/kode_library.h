#ifndef kode_linux_library_included
#define kode_linux_library_included
//----------------------------------------------------------------------

#include <dlfcn.h>

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class KODE_Library {
  protected:
    bool  MLibLoaded;
    void* MLibHandle;

  public:

    KODE_Library() {
      MLibLoaded = false;
      MLibHandle = KODE_NULL;
    }

    ~KODE_Library() {
      MLibLoaded = false;
      MLibHandle = KODE_NULL;
    }

  //public:
  //  bool  loaded(void) { return MLibLoaded; }

  public:

    virtual
    bool load(const char* AFilename) {
      MLibHandle = dlopen(AFilename,RTLD_LAZY | RTLD_LOCAL ); // RTLD_NOW, RTLD_LAZY
      if (!MLibHandle) return false;
      MLibLoaded = true;
      return true;
    }

    virtual
    void unload(void) {
      if (MLibLoaded && MLibHandle) {
        dlclose(MLibHandle);
        MLibLoaded = false;
        MLibHandle = KODE_NULL;
      }
    }

    virtual
    void* getSymbol(const char* ASymbol) {
      void* result = dlsym(MLibHandle,ASymbol);
      return result;
    }

};

//----------------------------------------------------------------------
#endif
