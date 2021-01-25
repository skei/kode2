#ifndef kode_painter_included
#define kode_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_rect.h"
#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

#if defined KODE_CAIRO
  #include "gui/cairo/kode_cairo_painter.h"
  #ifdef KODE_PAINTER_CAIRO
    typedef KODE_CairoPainter KODE_ImplementedPainter;
  #endif
#endif

//----------

//#if defined KODE_CTX
//  #include "gui/ctx/kode_ctx_painter.h"
//  #ifdef KODE_PAINTER_CTX
//    typedef KODE_CtxPainter KODE_ImplementedPainter;
//  #endif
//#endif

//----------

#if defined KODE_XCB
  #include "gui/xcb/kode_xcb_painter.h"
  #ifdef KODE_PAINTER_XCB
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

public:

  //void  blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) {}
  //void  blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}
  //void  stretch(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) {}

public:

//  void drawLine(float x1, float y1, float x2, float y2, KODE_Color color) final {
//    moveTo(x1 + 0.5f,y1 + 0.5f);
//    lineTo(x2 + 0.5f,y2 + 0.5f);
//    setColor(color);
//    stroke();
//  }
//
//  //----------
//
//  void drawRect(KODE_FRect rect, KODE_Color color) final {
//    rectangle(rect.x + 0.5f,rect.y + 0.5f,rect.w,rect.h);
//    setColor(color);
//    stroke();
//  }
//
//  //----------
//
//  void drawPie(KODE_FRect rect, float a1, float a2, KODE_Color color) final {
//    float cx = rect.x + (rect.w * 0.5f);
//    float cy = rect.y + (rect.h * 0.5f);
//    moveTo(cx,cy);
//    //moveTo(cx,rect.y);
//    arc(rect.x,rect.y,rect.w,rect.h,a1,a2,0);
//    moveTo(cx,cy);
//    //moveTo(cx,rect.y);
//    setColor(color);
//    stroke();
//
//  }
//
//  //----------
//
//  void drawEllipse(KODE_FRect rect, KODE_Color color) final {
//    float cx = rect.x + (rect.w * 0.5f);
//    //float cy = rect.y + (rect.h * 0.5f);
//    //moveTo(cx,cy);
//    moveTo(cx,rect.y);
//    arc(rect.x,rect.y,rect.w,rect.h,0,1,0);
//    //moveTo(cx,cy);
//    //moveTo(cx,rect.y);
//    setColor(color);
//    stroke();
//
//  }
//
//  //----------
//
//  void drawText(float x, float y, const char* text, KODE_Color color) final {
//    setColor(color);
//    moveTo(x,y);
//    fillText(text);
//  }
//
//  //----------
//
//  void drawText(KODE_FRect rect, const char* text, KODE_Color color, uint32_t alignment) final {
//    float x,y;
//    alignText(rect.x,rect.y,rect.x2(),rect.y2(),text,alignment,&x,&y);
//    setColor(color);
//    moveTo(x,y);
//    fillText(text);
//  }
//
//  //----------
//
//  void fillRect(KODE_FRect rect, KODE_Color color) final {
//    rectangle(rect.x + 0.5f,rect.y + 0.5f,rect.w,rect.h);
//    setColor(color);
//    fill();
//  }
//
//  //----------
//
//  void fillPie(KODE_FRect rect, float a1, float a2, KODE_Color color) final {
//    float cx = rect.x + (rect.w * 0.5f);
//    float cy = rect.y + (rect.h * 0.5f);
//    moveTo(cx,cy);
//    arc(rect.x,rect.y,rect.w,rect.h,a1,a2,0);
//    moveTo(cx,cy);
//    setColor(color);
//    fill();
//  }
//
//  //----------
//
//  /*
//  - After cairo_clip(), the current path will be cleared from the cairo
//    context.
//  - Calling cairo_clip() can only make the clip region smaller,
//    never larger.
//  */
//
//  void setClip(KODE_FRect rect) final {
//    resetClip();
//    rectangle(rect.x,rect.y,rect.w,rect.h);
//    clip();
//  }

};

//----------------------------------------------------------------------
#endif

