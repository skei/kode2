#ifndef kode_painter_included
#define kode_painter_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "gui/kode_gui_implementation.h"

#ifdef KODE_XCB
  #include "gui/xcb/kode_xcb_painter.h"
#endif

#ifdef KODE_GUI_XCB
  typedef KODE_XcbPainter KODE_ImplementedPainter;
#endif


//----------------------------------------------------------------------

class KODE_Painter
: public KODE_ImplementedPainter {

//------------------------------
public:
//------------------------------

  KODE_Painter(KODE_Drawable* ATarget)
  : KODE_ImplementedPainter(ATarget) {
  }

  //----------

  virtual ~KODE_Painter() {
  }

//------------------------------
public:
//------------------------------

//  void drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, KODE_Color AColor) override {}
//  void drawLines(float* ACoords, uint32_t ACount, KODE_Color AColor) override {}
//  void drawRect(KODE_Rect ARect, KODE_Color AColor) override {}
//  void fillRect(KODE_Rect ARect, KODE_Color AColor) override {}
//  void drawPie(KODE_Rect ARect, float AAngle1, float AAngle2, KODE_Color AColor) override {}
//  void fillPie(KODE_Rect ARect, float AAngle1, float AAngle2, KODE_Color AColor) override {}
//  void drawText(float AXpos, float AYpos, const char* AText, KODE_Color AColor) override {}
//  void drawText(KODE_Rect ARect, const char* AText, uint32_t AAlignment, KODE_Color AColor) override {}
//  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource) override {}
//  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource, KODE_Rect ASrc) override {}
//  void drawBitmap(KODE_Rect ADst, KODE_Drawable* ASource, KODE_Rect ASrc) override {}
//  void setClip(KODE_Rect ARect) override {}
//  void resetClip() override {}
////void resize(uint32_t AWidth, uint32_t AHeight) override {}

};



//----------------------------------------------------------------------
#endif
