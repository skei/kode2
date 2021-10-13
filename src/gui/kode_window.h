#ifndef kode_window_included
#define kode_window_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "gui/kode_gui_implementation.h"
#include "gui/kode_painter.h"
#include "gui/kode_surface.h"
#include "gui/kode_widget.h"

//----------------------------------------------------------------------

#ifdef KODE_USE_XCB
  #include "gui/xcb/kode_xcb_window.h"
#endif

//----------

#ifdef KODE_GUI_CAIRO
  typedef KODE_XcbWindow KODE_ImplementedWindow;
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

  bool          MFillBackground         = false;
  KODE_Color    MBackgroundColor        = KODE_Color(0.5f);
  // painted
  KODE_Painter* MWindowPainter          = KODE_NULL;
  // buffered
  #ifndef KODE_NO_WINDOW_BUFFERING
  uint32_t      MBufferWidth            = 0;
  uint32_t      MBufferHeight           = 0;
  KODE_Painter* MBufferPainter          = KODE_NULL;
  KODE_Surface* MBufferSurface          = KODE_NULL;
  #endif
  // status
  KODE_Widget*  MMouseHoverWidget       = KODE_NULL;
  KODE_Widget*  MMouseModalWidget       = KODE_NULL;
  KODE_Widget*  MMouseClickedWidget     = KODE_NULL;
  KODE_Widget*  MMouseLockedWidget      = KODE_NULL;
  KODE_Widget*  MKeyInputWidget         = KODE_NULL;
  int32_t       MMouseX                 = 0;
  int32_t       MMouseY                 = 0;
  int32_t       MMousePrevX             = 0;
  int32_t       MMousePrevY             = 0;
  int32_t       MMouseClickedX          = 0;
  int32_t       MMouseClickedY          = 0;
  uint32_t      MMouseClickedB          = 0;
  uint32_t      MMouseClickedS          = 0;
  int32_t       MMouseDragX             = 0;
  int32_t       MMouseDragY             = 0;
  uint32_t      MPrevClickTime          = 0;

//------------------------------
public:
//------------------------------

  KODE_Window(uint32_t AWidth, uint32_t AHeight, const char* ATitle="", void* AParent=KODE_NULL)
  : KODE_ImplementedWindow(AWidth,AHeight,ATitle,AParent)
  , KODE_Widget(KODE_FRect(AWidth,AHeight)) {
    //MOwner = this;
    setName("KODE_Window");
    setRect(KODE_FRect(AWidth,AHeight));
    MWindowPainter = new KODE_Painter(this);
    #ifndef KODE_NO_WINDOW_BUFFERING
      createBuffer(AWidth,AHeight);
    #endif
    //flags.autoClip = true;
  }

  //----------

  virtual ~KODE_Window() {
    if (MWindowPainter) delete MWindowPainter;
    #ifndef KODE_NO_WINDOW_BUFFERING
      destroyBuffer();
    #endif
  }

//------------------------------
public:
//------------------------------

  //KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
  //  if (AWidget) {
  //    //AWidget->MOwner = this;//setOwner(this);
  //    KODE_Widget::appendWidget(AWidget);
  //  }
  //  return AWidget;
  //}

//------------------------------
public:
//------------------------------

  //virtual bool isBuffered() {
  //  #ifdef KODE_NO_WINDOW_BUFFERING
  //  return false;
  //  #else
  //  return true;
  //  #endif
  //}

  //virtual KODE_Painter* getPainter() {
  KODE_Painter* getPainter() override {
    #ifdef KODE_NO_WINDOW_BUFFERING
    return MWindowPainter;
    #else
    return MBufferPainter;
    #endif
  }

  KODE_Widget* getMouseHoverWidget()    { return MMouseHoverWidget; }
  KODE_Widget* getMouseModalWidget()    { return MMouseModalWidget; }
  KODE_Widget* getMouseClickedWidget()  { return MMouseClickedWidget; }
  KODE_Widget* getMouseLockedWidget()  { return MMouseLockedWidget; }
  KODE_Widget* getKeyInputWidget()      { return MKeyInputWidget; }

//------------------------------
public: // painted
//------------------------------

  virtual void setFillBackground(bool AFill=true) {
    MFillBackground = AFill;
  }

  //----------

  void setBackgroundColor(KODE_Color AColor) {
    MBackgroundColor = AColor;
  }

  //----------

  void fillBackground(KODE_FRect ARect) {
    #ifdef KODE_NO_WINDOW_BUFFERING
      MWindowPainter->fillRectangle(ARect,MBackgroundColor);
    #else
      MBufferPainter->fillRectangle(ARect,MBackgroundColor);
    #endif
  }

//------------------------------
public: // window
//------------------------------

  void open() override {
    attachWindow(this);
    alignChildren();
    KODE_ImplementedWindow::open();
    //#ifndef KODE_PLUGIN_EXE
    //  //on_window_paint(0,0,MRect.w,MRect.h);
    //  paintWidget(this,MRect,0);
    //#endif
  }

  //void close() override {
  //  attachWindow(KODE_NULL);
  //  KODE_ImplementedWindow::close();
  //}

//------------------------------
private: // buffer
//------------------------------

  #ifndef KODE_NO_WINDOW_BUFFERING

  void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferSurface = new KODE_Surface(this,AWidth,AHeight);
    MBufferPainter = new KODE_Painter(MBufferSurface);
    MBufferWidth = AWidth;
    MBufferHeight = AHeight;
  }

  //----------

  void destroyBuffer() {
    if (MBufferPainter) delete MBufferPainter;
    if (MBufferSurface) delete MBufferSurface;
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
      //if (flags.autoClip) MWindowPainter->pushClip(ARect);
      AWidget->on_widget_paint(MWindowPainter,ARect,AMode);
      //if (flags.autoClip) MWindowPainter->popClip();
    #else
      //if (flags.autoClip) MBufferPainter->pushClip(ARect);
      AWidget->on_widget_paint(MBufferPainter,ARect,AMode);
      //if (flags.autoClip) MBufferPainter->popClip();
      MWindowPainter->drawBitmap(ARect.x,ARect.y,MBufferSurface,ARect);
    #endif
  }

  //----------

  void resizeWindow(uint32_t AWidth, uint32_t AHeight) {
    #ifndef KODE_NO_WINDOW_BUFFERING
      resizeBuffer(AWidth,AHeight);
    #endif
    MRect.w = AWidth;
    MRect.h = AHeight;
    alignChildren(/*this*/);
    if (MWindowPainter) MWindowPainter->resize(AWidth,AHeight);
  }

  //----------

  void updateHoverWidget(uint32_t AXpos, uint32_t AYpos, uint32_t ATimeStamp=0) {
    KODE_Widget* hover = KODE_NULL;
    if (MMouseModalWidget) {
      hover = MMouseModalWidget->findChild(AXpos,AYpos);
    }
    else {
      hover = findChild(AXpos,AYpos);
    }
    if (hover) {
      if (hover != MMouseHoverWidget) {
        if (MMouseHoverWidget) {
          MMouseHoverWidget->on_widget_leave(AXpos,AYpos,hover,ATimeStamp);
        }
        if (hover->flags.active) {
          MMouseHoverWidget = hover;
          MMouseHoverWidget->on_widget_enter(AXpos,AYpos,MMouseHoverWidget,ATimeStamp);
        }
      }
    }
    else {
      if (MMouseHoverWidget) {
        MMouseHoverWidget->on_widget_leave(AXpos,AYpos,hover,ATimeStamp);
        MMouseHoverWidget = KODE_NULL;
      }
    }
  }

  //----------

  // assume no prev widget, only prev clicked
  // called after mouse release

  void releaseHoverWidget(KODE_Widget* AClicked, uint32_t AXpos, uint32_t AYpos, uint32_t ATimeStamp=0) {
    KODE_Widget* hover = KODE_NULL;
    if (MMouseModalWidget) {
      hover = MMouseModalWidget->findChild(AXpos,AYpos);
    }
    else {
      hover = findChild(AXpos,AYpos);
    }
    if (AClicked && (hover != AClicked)) {
      AClicked->on_widget_leave(AXpos,AYpos,hover,ATimeStamp);
    }
    if (hover) {
      if (hover->flags.active) {
        hover->on_widget_enter(AXpos,AYpos,MMouseHoverWidget,ATimeStamp);
        MMouseHoverWidget = hover;
      }
    }
    else {
      MMouseHoverWidget = KODE_NULL;
    }
  }

//------------------------------
public: // base window
//------------------------------

  // ATimestamp = ms

  //----------

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
    if (MKeyInputWidget) MKeyInputWidget->on_widget_keyPress(AKey,0,AState,ATimeStamp);
  }

  //----------

  void on_window_keyRelease(uint32_t AKey, uint32_t AState, uint32_t ATimeStamp) override {
    if (MKeyInputWidget) MKeyInputWidget->on_widget_keyRelease(AKey,0,AState,ATimeStamp);
  }

  //----------

  /*
    click/release doesn't take different buttins onto account..
  */

  //----------

  void on_window_mouseClick(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    //bool double_click = false;
    //if ((ATimeStamp - MPrevClickTime) < KODE_GUI_DBLCLICK_MS) {
    //  double_click = true;
    //}
    //MPrevClickTime = ATimeStamp;
    MMouseClickedX  = AXpos;
    MMouseClickedY  = AYpos;
    MMousePrevX     = AXpos;
    MMousePrevY     = AYpos;
    MMouseDragX     = AXpos;
    MMouseDragY     = AYpos;
    if (MMouseHoverWidget) {
      if (MMouseHoverWidget->flags.active) {
        grabMouseCursor();
        MMouseClickedWidget = MMouseHoverWidget;
        MMouseClickedWidget->on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
      }
    }
  }

  //----------

  void on_window_mouseRelease(int32_t AXpos, int32_t AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
    if (MMouseClickedWidget) {
      MMouseClickedWidget->on_widget_mouseRelease(AXpos,AYpos,AButton,AState,ATimeStamp);
      releaseMouseCursor();
      releaseHoverWidget(MMouseClickedWidget,AXpos,AYpos,ATimeStamp);
      MMouseClickedWidget = KODE_NULL; // must be after releaseHoverWidget
    }
    //else {
    //  on_widget_mouseRelease(AXpos,AYpos,AButton,AState);
    //}
  }

  //----------

  void on_window_mouseMove(int32_t AXpos, int32_t AYpos, uint32_t AState, uint32_t ATimeStamp) override {
    MMouseX = AXpos;
    MMouseY = AYpos;
    if (MMouseClickedWidget) {
      if (MMouseLockedWidget) {
        if ((AXpos == MMouseClickedX) && (AYpos == MMouseClickedY)) {
          MMousePrevX = AXpos;
          MMousePrevY = AYpos;
          return;
        }
        int32_t deltax = AXpos - MMouseClickedX;
        int32_t deltay = AYpos - MMouseClickedY;
        MMouseDragX += deltax;
        MMouseDragY += deltay;
        MMouseClickedWidget->on_widget_mouseMove(MMouseDragX,MMouseDragY,AState,ATimeStamp);
        setMouseCursorPos(MMouseClickedX,MMouseClickedY);
      }
      else { // no captured widgets
        MMouseClickedWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
      }
    }
    else { // no widget clicked
      updateHoverWidget(AXpos,AYpos,ATimeStamp);
      if (MMouseHoverWidget) MMouseHoverWidget->on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);
    }
    MMousePrevX = AXpos;
    MMousePrevY = AYpos;
  }

  //----------

  void on_window_enter(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    if (!MMouseClickedWidget) {
      MMouseHoverWidget = KODE_NULL;
      updateHoverWidget(AXpos,AYpos,ATimeStamp);
      //on_widget_enter(AXpos,AYpos,KODE_NULL);
    }
  }

  //----------

  //TODO: if not dragging?

  void on_window_leave(int32_t AXpos, int32_t AYpos, uint32_t ATimeStamp) override {
    if (!MMouseClickedWidget) {
      //MMouseHoverWidget = KODE_NULL;
      updateHoverWidget(AXpos,AYpos,ATimeStamp);
      //on_widget_leave(AXpos,AYpos,KODE_NULL);
    }
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

  //virtual KODE_Widget* do_widget_get_owner(KODE_Widget* AWidget) override {
  //  return this;
  //}

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

  void do_widget_realign(KODE_Widget* ASender, bool ARecursive=true) override {
  }

  //----------

  //void do_widget_moved(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) override {
  //}

  //----------

  /*
    widget has been resized
    notify parent of widget, and realign/redraw
  */

  void do_widget_resized(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) override {
    //KODE_Widget* parent = ASender->getParent();
    //if (parent) {
    //  parent->alignChildren();
    //  do_widget_redraw(parent,parent->getRect(),0);
    //}
    alignChildren();
    redraw();
  }

  //----------

  void do_widget_grabMouseCursor(KODE_Widget* ASender) override {
    //if (ASender) grabMouseCursor();
    //else releaseMouseCursor();
    MMouseLockedWidget = ASender;
  }

  //----------

  void do_widget_grabKeyboard(KODE_Widget* AWidget) override {
    //KODE_Print("%p\n",AWidget);
    MKeyInputWidget = AWidget;
  }

  //----------

  void do_widget_grabModal(KODE_Widget* AWidget) override {
    //KODE_Print("%p\n",AWidget);
    MMouseModalWidget = AWidget;
    //if (AWidget)
      updateHoverWidget(MMouseX,MMouseY);
    //else
    //  releaseHoverWidget(MMouseClickedWidget,MMouseX,MMouseY,0);
  }

  //----------

  void do_widget_setMouseCursor(KODE_Widget* AWidget, int32_t ACursor) override {
    switch (ACursor) {
      case KODE_CURSOR_GRAB:
        grabMouseCursor();
        MMouseLockedWidget = AWidget;
        break;
      case KODE_CURSOR_RELEASE:
        releaseMouseCursor();
        MMouseLockedWidget = KODE_NULL;
        break;
      case KODE_CURSOR_SHOW:
        showMouseCursor();
        break;
      case KODE_CURSOR_HIDE:
        hideMouseCursor();
        break;
      default:
        //if (ACursor != MCurrentCursor) {
        //  MCurrentCursor = ACursor;
        //  setCursor(ACursor);
        //}
        setMouseCursor(ACursor);
        break;
    }
  }

  //----------

  void do_widget_setMouseCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) override {
    setMouseCursorPos(AXpos,AYpos);
  }

  //----------

  void do_widget_setHint(KODE_Widget* AWidget, const char* AHint) override {
    //MHintString = AHint;
  }

  //----------

  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) override {
  }


};

//----------------------------------------------------------------------
#endif
