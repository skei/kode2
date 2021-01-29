#ifndef kode_surface_included
#define kode_surface_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_gui_implementation.h"

//----------------------------------------------------------------------

class KODE_Surface
: public KODE_ImplementedSurface {

//------------------------------
public:
//------------------------------

  KODE_Surface(KODE_Drawable* AOwner)
  : KODE_ImplementedSurface(AOwner) {
  }

  KODE_Surface(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : KODE_ImplementedSurface(AOwner,AWidth,AHeight,ADepth) {
  }

  //----------

  //KODE_Surface(KODE_Drawable* AOwner, KODE_BaseImage* AImage)
  //: KODE_ImplementedSurface(AOwner,AImage) {
  //}

  //----------

  virtual ~KODE_Surface() {
  }

};


//----------------------------------------------------------------------
#endif
