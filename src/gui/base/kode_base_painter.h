#ifndef kode_base_painter_included
#define kode_base_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
//#include "base/types/kode_rect.h"
#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

class KODE_BasePainter {

public:

  KODE_BasePainter(KODE_Drawable* ATarget) {}
  virtual ~KODE_BasePainter() {}

public:
//------------------------------


//------------------------------
public:
//------------------------------


  virtual void  clear() {}
  virtual void  clear(uint32_t color) {}
  virtual void  dirty() {}
  virtual void  dirty(float x1, float y1, float x2, float y2) {}
  virtual void  flush() {}
  virtual void  finish() {}
  virtual void  save() {}
  virtual void  restore() {}
  virtual void  resize(uint32_t w, uint32_t h) {}

  virtual void  rotate(float a) {}
  virtual void  scale(float x, float y) {}
  virtual void  translate(float x, float y) {}

  virtual void  setFillRule(uint32_t rule) {}
  virtual void  setGlobalAlpha(float alpha) {}
  virtual void  setColor(uint32_t color) {}
  virtual void  setLineWidth(float w) {}
  virtual void  setLineDash(float* dashes, uint32_t numdashes, float offset) {}
  virtual void  setLineCap(uint32_t cap) {}
  virtual void  setLineJoin(uint32_t join) {}

  virtual void  setFont(const char* name) {}
  virtual void  setFontSize(float size) {}
  virtual void  setFontSlant(uint32_t slant) {}
  virtual void  setFontWeight(uint32_t weight) {}
  virtual void  loadFont(const char* name, const char* filepath) {}

  virtual void  gradientAddStop(float pos, uint32_t color) {}
  virtual void  gradientClearStops() {}

  virtual float getCurrentXPos() { return 0.0f; }
  virtual float getCurrentYPos() { return 0.0f; }
  virtual float getTextWidth(const char* str) { return 0.0f; }
  virtual float getTextHeight(const char* str) { return 0.0f; }

  virtual void  newPath() {}
  virtual void  closePath() {}
  virtual void  fill(bool APreserve=false) {}
  virtual void  stroke(bool APreserve=false) {}
  virtual void  paint() {}
  virtual void  clip(bool APreserve=false) {}
  virtual void  resetClip() {}

  virtual void  linearGradient(float x0, float y0, float x1, float y1) {}
  virtual void  radialGradient(float x0, float y0, float r0, float x1, float y1, float r1) {}

  virtual void  moveTo(float x, float y) {}
  virtual void  lineTo(float x, float y) {}
  virtual void  curveTo(float cx0, float cy0, float cx1, float cy1, float x, float y) {}
  virtual void  quadTo(float cx, float cy, float x, float y) {}
  virtual void  arcTo(float x1, float y1, float x2, float y2, float radius) {}

  virtual void  relMoveTo(float x, float y) {}
  virtual void  relLineTo(float x, float y) {}
  virtual void  relCurveTo(float cx0, float cy0, float cx1, float cy1, float x, float y) {}
  virtual void  relQuadTo(float cx, float cy, float x, float y) {}
  virtual void  relArcTo(float x1, float y1, float x2, float y2, float radius) {}

  virtual void  setPixel(uint32_t x, uint32_t y, uint32_t color) {}
  virtual void  rectangle(float x, float y, float w, float h) {}
  //virtual void  arc(float x, float y, float radius, float angle1, float angle2, int   direction) {}
  virtual void  arc(float x, float y, float w, float h, float angle1, float angle2, int   direction) {}
  virtual void  fillText(const char* txt) {}
  virtual void  strokeText(const char* txt) {}

  virtual void alignText(float x1, float y1, float x2, float y2, const char* text, uint32_t alignment, float* outx, float* outy) {}
  //virtual float getTextWidth(const char* AText) { return 0.0f; }
  //virtual float getTextHeight(const char* AText){ return 0.0f; }

//public:
//
//  virtual void  blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) {}
//  virtual void  blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}
//  virtual void  stretch(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) {}

public:

  virtual void drawLine(float x1, float y1, float x2, float y2, uint32_t color) {}
  virtual void drawRect(KODE_FRect rect, uint32_t color) {}
  virtual void drawEllipse(KODE_FRect rect, uint32_t color) {}
  virtual void drawPie(KODE_FRect rect, float a1, float a2, uint32_t color) {}
  virtual void drawText(float x, float y, const char* text, uint32_t color) {}
  virtual void drawText(KODE_FRect rect, const char* text, uint32_t color, uint32_t alignment) {}
  virtual void fillRect(KODE_FRect rect, uint32_t color) {}
  virtual void fillEllipse(KODE_FRect rect, uint32_t color) {}
  virtual void fillPie(KODE_FRect rect, float a1, float a2, uint32_t color) {}
  virtual void setClip(KODE_FRect rect) {}

};

//----------------------------------------------------------------------
#endif

