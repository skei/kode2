#ifndef kode_base_window_included
#define kode_base_window_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "base/types/kode_color.h"
//#include "gfx/kode_bitmap.h"
//#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

class KODE_BaseWindow {
public:
  KODE_BaseWindow(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL) {}
  virtual ~KODE_BaseWindow() {}
public:
  virtual KODE_Painter* getPainter() { return KODE_NULL; }
public:

  virtual void on_window_open(uint32_t AWidth, uint32_t AHeight) { KODE_Print("%i,%i\n",AWidth,AHeight); }
  virtual void on_window_close() { KODE_Print("\n"); }

  virtual void on_window_move(uint32_t AXpos, uint32_t AYpos) { /*KODE_PRINT;*/ }
  virtual void on_window_resize(uint32_t AWidth, uint32_t AHeight) { /*KODE_PRINT;*/ }
  virtual void on_window_paint(uint32_t AXpos, uint32_t AYpos, uint32_t AWidth, uint32_t AHeight) { /*KODE_PRINT;*/ }
  virtual void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) {}
  virtual void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) {}
  virtual void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) {}
  virtual void on_window_timer() {}
  virtual void on_window_idle() {}
  virtual void on_window_clientMessage(uint32_t AMsg, void* Data) {}
public:
  virtual void setPos(uint32_t AXpos, uint32_t AYpos) {}
  virtual void setSize(uint32_t AWidth, uint32_t AHeight) {}
  virtual void setTitle(const char* ATitle) {}
  virtual void open() { /*KODE_PRINT;*/ }
  virtual void close() { /*KODE_PRINT;*/ }
  virtual void eventLoop() { /*KODE_PRINT;*/ }
  virtual void reparent(void* AParent) {}
  virtual void startTimer(uint32_t ms) {}
  virtual void stopTimer(void) {}
  virtual void setMouseCursor(uint32_t ACursor) {}
  virtual void setMouseCursorPos(int32_t AXpos, int32_t AYpos) {}
  virtual void hideMouseCursor(void) {}
  virtual void showMouseCursor(void) {}
  virtual void grabMouseCursor(void) {}
  virtual void releaseMouseCursor(void) {}
  virtual void beginPaint() { /*KODE_PRINT;*/ }
  virtual void paint(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) { /*KODE_PRINT;*/ }
  virtual void endPaint() { /*KODE_PRINT;*/ }
  virtual void invalidate(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight) {}
  virtual void sendEvent(uint32_t AData, uint32_t AType) {}
  virtual void flush(void) {}
  virtual void sync(void) {}
//public:
//  virtual void fill(KODE_Color AColor) {}
//  virtual void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, KODE_Color AColor) {}
//  virtual void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) {}
//  virtual void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}
};

//----------------------------------------------------------------------
#endif

