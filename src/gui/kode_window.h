#ifndef kode_window_included
#define kode_window_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "gui/kode_gui_implementation.h"
#include "gui/kode_painter.h"
#include "gui/kode_surface.h"
#include "gui/kode_widget.h"

#ifdef KODE_USE_XCB
  #include "gui/xcb/kode_xcb_window.h"
#endif

#ifdef KODE_GUI_XCB
  typedef KODE_XcbWindow KODE_ImplementedWindow;
#endif

//----------------------------------------------------------------------

class KODE_Window
: public KODE_ImplementedWindow
, public KODE_Widget {

//------------------------------
protected:
//------------------------------

  // painted

  uint32_t      MWindowWidth            = 0;  // overrides xcbWindow
  uint32_t      MWindowHeight           = 0;
  KODE_Painter* MWindowPainter          = KODE_NULL;

  // buffered

  #ifndef KODE_NO_WINDOW_BUFFERING
  uint32_t      MBufferWidth            = 0;
  uint32_t      MBufferHeight           = 0;
  KODE_Painter* MBufferPainter          = KODE_NULL;
  KODE_Surface* MBufferSurface          = KODE_NULL;
  #endif

  KODE_Widget*  MHoverWidget            = KODE_NULL;
  KODE_Widget*  MModalWidget            = KODE_NULL;
  KODE_Widget*  MMouseClickedWidget     = KODE_NULL;
  KODE_Widget*  MKeyCaptureWidget       = KODE_NULL;

  bool          MFillBackground         = false;
  KODE_Color    MBackgroundColor        = KODE_Color(0.5f);

//------------------------------
protected:
//------------------------------

//------------------------------
public:
//------------------------------

  KODE_Window(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_ImplementedWindow(AWidth,AHeight,ATitle,AParent)
  , KODE_Widget(KODE_FRect(AWidth,AHeight)) {
    MName = "KODE_Window";
    MRect = KODE_FRect(AWidth,AHeight);
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    MWindowPainter = KODE_New KODE_Painter(this);
    #ifndef KODE_NO_WINDOW_BUFFERING
      createBuffer(AWidth,AHeight);
    #endif
  }

  //----------

  virtual ~KODE_Window() {
    if (MWindowPainter) KODE_Delete MWindowPainter;
    #ifndef KODE_NO_WINDOW_BUFFERING
      destroyBuffer();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t getWindowWidth() { return MWindowWidth; }
  virtual uint32_t getWindowHeight() { return MWindowHeight; }

//------------------------------
public: // painted
//------------------------------

  void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
  }

  //----------

  void setBackgroundColor(KODE_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  void fillBackground(KODE_FRect ARect) {
    #ifdef KODE_NO_WINDOW_BUFFERING
      MWindowPainter->fillRect(ARect,MBackgroundColor);
    #else
      MBufferPainter->fillRect(ARect,MBackgroundColor);
    #endif
  }

//------------------------------
public: // window
//------------------------------

  void open() override {
    //KODE_PRINT;
    alignChildren();
    KODE_ImplementedWindow::open();
    //#ifndef KODE_PLUGIN_EXE
    //  //on_window_paint(0,0,MWindowWidth,MWindowHeight);
    //  paintWidget(this,MRect,0);
    //#endif
  }

//------------------------------
private: // buffer
//------------------------------

  #ifndef KODE_NO_WINDOW_BUFFERING

  void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferSurface = KODE_New KODE_Surface(this,AWidth,AHeight);
    MBufferPainter = KODE_New KODE_Painter(MBufferSurface);
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
  }

  //----------

  void destroyBuffer() {
    if (MBufferPainter) KODE_Delete MBufferPainter;
    if (MBufferSurface) KODE_Delete MBufferSurface;
    MBufferPainter = KODE_NULL;
    MBufferSurface = KODE_NULL;
  }

  //----------

  void resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    uint32_t w = KODE_NextPowerOfTwo(AWidth);
    uint32_t h = KODE_NextPowerOfTwo(AHeight);
    if ((w != MBufferWidth) || (h != MBufferHeight)) {
      destroyBuffer();
      createBuffer(w,h);
    }
  }

  #endif // KODE_NO_WINDOW_BUFFERING

//------------------------------
public:
//------------------------------

  void paintWidget(KODE_Widget* AWidget, KODE_FRect ARect, uint32_t AMode=0) {
    #ifdef KODE_NO_WINDOW_BUFFERING
      AWidget->on_widget_paint(MWindowPainter,ARect,AMode);
    #else
      AWidget->on_widget_paint(MBufferPainter,ARect,AMode);
      MWindowPainter->drawBitmap(ARect.x,ARect.y,MBufferSurface,ARect);
    #endif
  }

  //----------

  void resizeWindow(uint32_t AWidth, uint32_t AHeight) {
    #ifndef KODE_NO_WINDOW_BUFFERING
      resizeBuffer(AWidth,AHeight);
    #endif
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    MRect.w = AWidth;
    MRect.h = AHeight;
    alignChildren();
    //if (MWindowPainter) MWindowPainter->resize(AWidth,AHeight);
  }

  //----------

  void updateHoverWidget(uint32_t AXpos, uint32_t AYpos, bool ALeave=false) {
    KODE_Widget* hover = KODE_NULL;
    if (!ALeave) hover = findChild(AXpos,AYpos);
    if (hover != MHoverWidget) {
      if (MHoverWidget) {
        MHoverWidget->MStates.hovering = false;
        MHoverWidget->on_widget_leave(AXpos,AYpos,hover);
      }
      if (hover) {
        hover->MStates.hovering = true;
        hover->on_widget_enter(AXpos,AYpos,MHoverWidget);
      }
      MHoverWidget = hover;
    }
  }

//------------------------------
public: // base window
//------------------------------

  void on_window_move(uint32_t AXpos, uint32_t AYpos) override {
  }

  //----------

  void on_window_resize(uint32_t AWidth, uint32_t AHeight) override {
    resizeWindow(AWidth,AHeight);
  }

  //----------

  void on_window_paint(uint32_t AXpos, uint32_t AYpos, uint32_t AWidth, uint32_t AHeight) override {
    KODE_FRect rect = KODE_FRect(AXpos,AYpos,AWidth,AHeight);
    if (MFillBackground) fillBackground(rect);
    paintWidget(this,rect,0);
  }

  //----------

  void on_window_keyPress(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    if (MKeyCaptureWidget) MKeyCaptureWidget->on_widget_keyPress(AKey,0,AState);
  }

  //----------

  void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    if (MKeyCaptureWidget) MKeyCaptureWidget->on_widget_keyRelease(AKey,0,AState);
  }

  //----------

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    if (MHoverWidget) {
      MMouseClickedWidget = MHoverWidget;
      MHoverWidget->MStates.clicked = true;
      MHoverWidget->on_widget_mouseClick(AXpos,AYpos,AButton,AState);
      //grabMouseCursor();
    }
  }

  //----------

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    if (MMouseClickedWidget) {
      MMouseClickedWidget->MStates.clicked = false;
      MMouseClickedWidget->on_widget_mouseRelease(AXpos,AYpos,AButton,AState);
      MMouseClickedWidget = KODE_NULL;
      //releaseMouseCursor();
    }
    //else {
    //  on_widget_mouseRelease(AXpos,AYpos,AButton,AState);
    //}
  }

  //----------

  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    if (MMouseClickedWidget) {
      MMouseClickedWidget->on_widget_mouseMove(AXpos,AYpos,AState);
    }
    else {
      updateHoverWidget(AXpos,AYpos);
    }
  }

  //----------

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    updateHoverWidget(AXpos,AYpos);
    //on_widget_enter(AXpos,AYpos,KODE_NULL);
  }

  //----------

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    updateHoverWidget(AXpos,AYpos,true);
    //on_widget_leave(AXpos,AYpos,KODE_NULL);
  }

  //----------

  void on_window_timer() override {
  }

  //----------

  void on_window_idle() override {
  }

  //----------

  void on_window_clientMessage(uint32_t AMsg, void* AData) override {
  }

//------------------------------
public: // "widget listener"
//------------------------------

//  void do_widget_moved(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) override {
//  }

  //----------

  /*
    widget has been resized
    notify parent of widget, and realign/redraw
  */

//  void do_resized(KODE_Widget* ASender, float AWidth, float AHeight) override {
//    KODE_Widget* parent = ASender->getParent();
//    if (parent) {
//      parent->on_realign(); // realignChildren();
//      do_redraw(parent,parent->getRect());
//    }
//  }

//  void do_widget_resized(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) override {
//  }

  //----------

  void do_widget_update(KODE_Widget* AWidget) override {
  }

  //----------

  /*
    when we invalidate, we lose all information about which widget triggered
    the redraw, so we need to paint from the top
  */

  void do_widget_redraw(KODE_Widget* AWidget, KODE_FRect ARect, uint32_t AMode=0) override {
    //KODE_PRINT;
    invalidate(ARect.x,ARect.y,ARect.w + 1,ARect.h + 1);
  }

  //----------

  void do_widget_setHint(KODE_Widget* AWidget, const char* AHint) override {
  }

  //----------

//  void do_setCursor(KODE_Widget* ASender, int32_t ACursor) override {
//    //KODE_Trace("%i\n",ACursor);
//    if (ACursor == KODE_CURSOR_HIDE) hideCursor();
//    else if (ACursor == KODE_CURSOR_SHOW) showCursor();
//    else {
//      if (ACursor != MCurrentCursor) {
//        MCurrentCursor = ACursor;
//        setCursor(ACursor);
//      }
//    }
//  }

  void do_widget_setMouseCursor(KODE_Widget* AWidget, uint32_t ACursor) override {
    setMouseCursor(ACursor);
  }

//  void do_setCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) override {
//    setCursorPos(AXpos,AYpos);
//  }

  //----------

  void do_widget_grabModal(KODE_Widget* AWidget) override {
    MModalWidget = AWidget;
  }

  //----------

  void do_widget_grabKeyboard(KODE_Widget* AWidget) override {
    MKeyCaptureWidget = AWidget;
  }

  //----------

  //void do_widget_mouseCapture(KODE_Widget* AWidget) override {
  //  MMouseClickedWidget = AWidget;
  //}

  //----------

  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) override {
  }

  //----------

  //void do_sizer(KODE_Widget* ASender, float ADeltaX, float ADeltaY, uint32_t AMode) override {
  //}

//------------------------------


};

//----------------------------------------------------------------------
#endif
