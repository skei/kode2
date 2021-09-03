#ifndef kode_painter_included
#define kode_painter_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/types/kode_stack.h"
//#include "gui/kode_gui_implementation.h"

//----------------------------------------------------------------------

#ifdef KODE_USE_CAIRO
  #include "gui/cairo/kode_cairo_painter.h"
#endif

#ifdef KODE_USE_XCB
  #include "gui/xcb/kode_xcb_painter.h"
#endif

//----------

#ifdef KODE_GUI_CAIRO
  typedef KODE_CairoPainter KODE_ImplementedPainter;
#endif

#ifdef KODE_GUI_XCB
  typedef KODE_XcbPainter KODE_ImplementedPainter;
#endif

//----------------------------------------------------------------------

typedef KODE_Stack<KODE_FRect,16> KODE_RectStack;

//----------------------------------------------------------------------

class KODE_Painter
: public KODE_ImplementedPainter {

//------------------------------
public:
//------------------------------

  KODE_FRect      MClipRect;
  KODE_RectStack  MClipStack;

//------------------------------
public:
//------------------------------

  KODE_Painter(KODE_Drawable* ATarget)
  : KODE_ImplementedPainter(ATarget) {
    MClipRect = KODE_FRect( ATarget->getWidth()-1, ATarget->getHeight()-1 );
    //MClipStack.push(MClipRect);
  }

  //----------

  virtual ~KODE_Painter() {
  }

//------------------------------
public:
//------------------------------

  /*
    - push current clip rect
    - set new clip rect
  */

  void pushClip(KODE_FRect ARect) override {
    //KODE_Trace("pushing x %.0f y %.0f w %.0f h %.0f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    MClipStack.push(MClipRect);
    MClipRect = ARect;
//    resetClip();
    setClip(MClipRect);
  }

  //----------

  /*
    - pop rect
    - set clip rect to popped rect
  */

  KODE_FRect popClip() override {
    //KODE_FRect prev_rect = MClipRect;
    MClipRect = MClipStack.pop();
//    resetClip();
    setClip(MClipRect);
    return MClipRect;
  }

  //----------

  void resetClipStack() override {
    MClipStack.reset();
  }

  //----------

  void setClipRect(KODE_FRect ARect) override {
    MClipRect = ARect;
  }

  //----------

  KODE_FRect getClipRect() override {
    return MClipRect;
  }

};



//----------------------------------------------------------------------
#endif
