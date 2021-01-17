#ifndef kode_base_image_included
#define kode_base_image_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_drawable.h"

class KODE_BaseImage
/*: public KODE_Drawable*/ {

public:

  KODE_BaseImage(KODE_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32) {}
  virtual ~KODE_BaseImage() {}

};

//----------------------------------------------------------------------
#endif
