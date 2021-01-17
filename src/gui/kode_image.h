#ifndef kode_image_included
#define kode_image_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

#ifdef KODE_XCB
  #include "gui/xcb/kode_xcb_image.h"
  #ifdef KODE_GUI_XCB
    typedef KODE_XcbImage KODE_ImplementedImage;
  #endif
#endif

//----------------------------------------------------------------------

class KODE_Image
: public KODE_ImplementedImage {

public:

  KODE_Image(KODE_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32)
  : KODE_ImplementedImage(ATarget,AWidth,AHeight) {
  }

  virtual ~KODE_Image() {
  }

};

//----------------------------------------------------------------------
#endif
