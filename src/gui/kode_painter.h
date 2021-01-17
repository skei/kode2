#ifndef kode_painter_included
#define kode_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

#if defined KODE_CTX
  #include "gui/ctx/kode_ctx_painter.h"
  #ifdef KODE_GUI_CTX
    typedef KODE_CtxPainter KODE_ImplementedPainter;
  #endif
#endif

#if defined KODE_XCB
  #include "gui/xcb/kode_xcb_painter.h"
  #ifdef KODE_GUI_XCB
    typedef KODE_XcbPainter KODE_ImplementedPainter;
  #endif
#endif

//----------------------------------------------------------------------

class KODE_Painter
: public KODE_ImplementedPainter {

public:

  KODE_Painter(KODE_Drawable* ATarget)
  : KODE_ImplementedPainter(ATarget) {
  }

  virtual ~KODE_Painter() {
  }

};

//----------------------------------------------------------------------
#endif

