#ifndef kode_base_painter_included
#define kode_base_painter_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "base/types/kode_color.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"
//
//----------------------------------------------------------------------

//class KODE_BaseImage;
//class KODE_BaseSurface;
//class KODE_BaseWindow;
//class KODE_BasePainter;

//----------------------------------------------------------------------

class KODE_BasePainter {
public:
  KODE_BasePainter(KODE_Drawable* ATarget) {}
  virtual ~KODE_BasePainter() {}
public:
  virtual void  resize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void  setClip(KODE_FRect ARect) {}
  virtual void  resetClip() {}
  virtual float getTextWidth(const char* AText) { return 0; }
  virtual float getTextHeight(const char* AText) { return 0; }
  virtual void  drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void  drawRect(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void  fillRect(KODE_FRect ARect, KODE_Color AColor) {}
  virtual void  drawPie(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void  fillPie(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor) {}
  virtual void  drawText(float AXpos, float AYpos, const char* AText, KODE_Color AColor) {}
  virtual void  drawText(KODE_FRect ARect, const char* AText, uint32_t AAlignment, KODE_Color AColor) {}
  virtual void  uploadBitmap(float AXpos, float AYpos, KODE_Bitmap* ABitmap) {}
  virtual void  drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource) {}
  virtual void  drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource, KODE_FRect ASrc) {}
  virtual void  drawBitmap(KODE_FRect ADst, KODE_Drawable* ASource, KODE_FRect ASrc) {}
};

//----------------------------------------------------------------------
#endif

