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
  virtual void            flush() {}
  virtual void            dirty() {}
  virtual void            dirty(KODE_FRect ARect) {}
  virtual void            finish() {}
  virtual KODE_Drawable*  getTarget() { return KODE_NULL; }
  virtual void            resize(uint32_t AWidth, uint32_t AHeight) {}

  virtual void            setClip(KODE_FRect ARect) {}
  virtual void            resetClip() {}

  virtual void            pushClip(KODE_FRect ARect) {}
  virtual KODE_FRect      popClip() { return KODE_FRect(0,0); }
  virtual void            resetClipStack() {}
  virtual void            setClipRect(KODE_FRect ARect) {}
  virtual KODE_FRect      getClipRect() { return KODE_FRect(0,0); }

  virtual float           getTextWidth(const char* AText) { return 0; }
  virtual float           getTextHeight(const char* AText) { return 0; }

  virtual void            setColor(KODE_Color AColor) {}
  virtual void            setLineWidth(float ASize) {}
  virtual void            setLineDash(double* ADashes, uint32_t ANumDashes, float AOffset) {}
  virtual void            setLineCap(uint32_t ALineCap) {}
  virtual void            setLineJoin(uint32_t ALineJoin) {}
  virtual void            setFontSize(float ASize) {}
  virtual void            setFontFace(const char* AName, uint32_t ASlant, uint32_t AWeight) {}

  virtual void            strokePath(bool APreserve=false) {}
  virtual void            fillPath(bool APreserve=false) {}
  virtual void            fillPathGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical, bool APreserve=false) {}

  virtual void            moveTo(float AX, float AY) {}
  virtual void            lineTo(float AX, float AY) {}
  virtual void            curveTo(float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) {}
  virtual void            horizLine(float AX1, float AY1, float AX2) {}
  virtual void            vertLine(float AX1, float AY1, float AY2) {}
  virtual void            rectangle(KODE_FRect ARect) {}
  virtual void            roundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners) {}
  virtual void            ellipse(KODE_FRect ARect) {}
  virtual void            arc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) {}
  virtual void            triangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) {}

  virtual void            drawPoint(float AX, float AY, KODE_Color AColor) {}
  virtual void            drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, KODE_Color AColor, uint32_t AWidth=1) {}
//virtual void            drawLines(int32 ANum, float* ACoords) override {}
  virtual void            drawRectangle(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void            drawRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void            drawArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void            drawEllipse(KODE_FRect ARect, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void            drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, KODE_Color AColor, uint32_t AWidth=1) {}
  virtual void            drawCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4, KODE_Color AColor, uint32_t AWidth=1) {}

  virtual void            fillRectangle(KODE_FRect ARect, KODE_Color AColor) {}
  virtual void            fillRoundedRectangle(KODE_FRect ARect, float ARadius, uint32_t ACorners, KODE_Color AColor) {}
  virtual void            fillArc(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor) {}
  virtual void            fillEllipse(KODE_FRect ARect, KODE_Color AColor) {}
  virtual void            fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, KODE_Color AColor) {}
//virtual void            fillPolygon(int32 ANum, float* ACoords, KODE_Color AColor) {}

  virtual void            drawText(float AXpos, float AYpos, const char* AText, KODE_Color AColor) {}
  virtual void            drawText(KODE_FRect ARect, const char* AText, uint32_t AAlignment, KODE_Color AColor) {}

  virtual void            uploadBitmap(float AXpos, float AYpos, KODE_Bitmap* ABitmap) {}
  virtual void            drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource) {}
  virtual void            drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource, KODE_FRect ASrc) {}
  virtual void            drawBitmap(KODE_FRect ADst, KODE_Drawable* ASource, KODE_FRect ASrc) {}

};

//----------------------------------------------------------------------
#endif

