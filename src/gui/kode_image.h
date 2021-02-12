#ifndef kode_image_included
#define kode_image_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "gui/kode_gui_implementation.h"

#ifdef KODE_USE_XCB
  #include "gui/xcb/kode_xcb_image.h"
#endif

#ifdef KODE_GUI_XCB
  typedef KODE_XcbImage KODE_ImplementedImage;
#endif

//----------------------------------------------------------------------

class KODE_Image
: public KODE_ImplementedImage {

//------------------------------
public:
//------------------------------

  KODE_Image(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32)
  : KODE_ImplementedImage(AOwner,AWidth,AHeight,ADepth) {
  }

  //----------

  KODE_Image(KODE_Drawable* AOwner, KODE_Bitmap* ABitmap)
  : KODE_XcbImage(AOwner,ABitmap) {
  }

  //----------

  virtual ~KODE_Image() {
  }

};



//----------------------------------------------------------------------
#endif
