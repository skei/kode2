#ifndef kode_surface_included
#define kode_surface_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

#ifdef KODE_XCB
  #include "gui/xcb/kode_xcb_surface.h"
  #ifdef KODE_GUI_XCB
    typedef KODE_XcbSurface KODE_ImplementedSurface;
  #endif
#endif

//----------------------------------------------------------------------

class KODE_Surface
: public KODE_ImplementedSurface {

public:

  KODE_Surface(KODE_Drawable* AOwner)
  : KODE_ImplementedSurface(AOwner) {
  }

  KODE_Surface(KODE_Drawable* AOwner,uint32_t AWidth, uint32_t AHeight,uint32_t ADepth=0)
  : KODE_ImplementedSurface(AOwner,AWidth,AHeight,ADepth) {
  }

  virtual ~KODE_Surface() {
  }

};

//----------------------------------------------------------------------
#endif
