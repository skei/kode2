#ifndef kode_global_included
#define kode_global_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/types/kode_array.h"

//class KODE_Module;
typedef void* KODE_Module;
typedef KODE_Array<KODE_Module*>  KODE_ModuleArray;

//----------------------------------------------------------------------


//----------------------------------------------------------------------

class KODE_Global {

//------------------------------
private:
//------------------------------

  KODE_ModuleArray  MModules;

//------------------------------
public:
//------------------------------

  KODE_Global() {
  }

  //----------

  ~KODE_Global() {
    #ifndef KODE_NO_AUTODELETE
    deleteModules();
    #endif
  }

//------------------------------
public:
//------------------------------

  uint32_t appendModule(KODE_Module* AModule) {
    uint32_t index = MModules.size();
    MModules.append(AModule);
    return index;
  }

  //----------

  void deleteModules() {
    for (uint32_t i=0; i<MModules.size(); i++) {
      delete MModules[i];
      MModules[i] = KODE_NULL;
    }
  }

  //----------

    KODE_Module* getModule(uint32_t AIndex) {
      return MModules[AIndex];
    }

};

//----------------------------------------------------------------------

KODE_Global KODE_GLOBAL;

//----------------------------------------------------------------------
#endif
