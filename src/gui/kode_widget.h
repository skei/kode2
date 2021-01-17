#ifndef kode_widget_included
#define kode_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_array.h"
#include "base/types/kode_point.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"

//----------------------------------------------------------------------

struct KODE_WidgetOptions {
  bool autoCursor = false;
  bool autoHint = false;
  bool autoClip = false;
};

//----------

struct KODE_WidgetLayout {
  uint32_t    alignment       = KODE_WIDGET_ALIGN_NONE;
  KODE_FPoint widgetSpacing   = KODE_FPoint(0,0);
  KODE_FRect  innerBorder     = KODE_FRect(0,0,0,0);
  //KODE_FRect  outerBorder     = KODE_FRect(0,0,0,0);
};

//----------

struct KODE_WidgetState {
  bool  isActive        = false;
  bool  isVisible       = true;
  bool  isHovering      = false;
  bool  isInteracting   = false;
};

//----------

class KODE_Widget;
typedef KODE_Array<KODE_Widget*> KODE_Widgets;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_Widget {

  friend class KODE_Window;

//------------------------------
protected:
//------------------------------

  KODE_Widgets        MChildWidgets;
  KODE_WidgetOptions  MOptions;
  KODE_WidgetLayout   MLayout;
  KODE_WidgetState    MState;

  int32_t             MIndex        = 0;
  KODE_Widget*        MParent       = KODE_NULL;
  KODE_FRect          MRect         = KODE_FRect(0);
  KODE_FRect          MInitialRect  = KODE_FRect(0);
  KODE_FRect          MContentRect  = KODE_FRect(0);
  int32_t             MMouseCursor  = KODE_CURSOR_DEFAULT;
  const char*         MHint         = "";

  float               MValue        = 0.0f;
  void*               MParameter    = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Widget(KODE_FRect ARect) {
    MRect         = ARect;
    MInitialRect  = ARect;
    MContentRect  = KODE_FRect(0);
  }

  //----------

  virtual ~KODE_Widget() {
    #ifndef KODE_NO_AUTODELETE
    deleteChildWidgets();
    #endif
  }

//------------------------------
public:
//------------------------------

  KODE_Widgets*       getWidgets()          { return &MChildWidgets; }
  KODE_Widget*        getWidget(uint32_t i) { return MChildWidgets[i]; }

  KODE_WidgetOptions* getWidgetOptions()    { return &MOptions; }
  KODE_WidgetLayout*  getLayout()           { return &MLayout; }
  KODE_WidgetState*   getState()            { return &MState; }

  int32_t             getIndex()            { return MIndex; }
  KODE_Widget*        getParent()           { return MParent; }
  KODE_FRect          getRect()             { return MRect; }
  KODE_FRect          getInitialRect()      { return MInitialRect; }
  KODE_FRect          getContentRect()      { return MContentRect; }
  int32_t             getMouseCursor()      { return MMouseCursor; }
  const char*         getHint()             { return MHint; }

  float               getValue()            { return MValue; }
  void*               getParameter()        { return MParameter; }

  //----------

//  void setIndex(int32_t AIndex)         { MIndex = AIndex; }
//  void setParent(KODE_Widget* AParent)  { MParent = AParent; }
  //void setRect(KODE_FRect ARect)        { MRect = ARect; }
  //void setMouseCursor(int32_t c)        { MMouseCursor = c; }
  //void setHint(const char* h)           { MHint = h; }

  void setValue(float AValue)         { MValue = AValue; }
  void setParameter(void* AParameter) { MParameter = AParameter; }

//------------------------------
public:
//------------------------------

  void appendChildWidget(KODE_Widget* AWidget) {
    int32_t index = MChildWidgets.size();
    //AWidget->setIndex(index);
    //AWidget->setParent(this);
    AWidget->MIndex = index;
    AWidget->MParent = this;
    MChildWidgets.append(AWidget);
  }

  //----------

  void deleteChildWidgets() {
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      KODE_Delete MChildWidgets[i];
      MChildWidgets[i] = KODE_NULL;
    }
  }

  //----------

  void setWidgetPos(float AXpos, float AYpos) {
    MRect.setPos(AXpos,AYpos);
  }

  //----------

  void setWidgetSize(float AWidth, float AHeight) {
    MRect.setSize(AWidth,AHeight);
  }

  //----------

  void alignChildWidgets(uint32_t AMode) {
    //KODE_FRect client_rect = MWidgetRect;
    //client_rect.shrink(MWidgetLayout.innerBorder);
  }

  //----------

  /*
    ARect = update rect
  */

  void paintChildWidgets(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) {
    //KODE_PRINT;
    for (uint32_t i=0; i<MChildWidgets.size(); i++) {
      KODE_Widget* widget = MChildWidgets[i];
      KODE_FRect rect = widget->MRect;
      rect.intersect(ARect);
      if (rect.isNotEmpty()) {
        widget->on_widget_paint(APainter,rect,AMode);
      }
    }
  }

  //----------

  KODE_Widget* findChildWidget(float AXpos, float AYpos, bool ASub=true) {
    uint32_t num = MChildWidgets.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_Widget* widget = MChildWidgets[i];
      if ( widget->MRect.contains(AXpos,AYpos) ) {
        KODE_Widget* child = widget->findChildWidget(AXpos,AYpos);
        if (child) return child;
        return widget;
      }
    }
    return KODE_NULL;
  }

  //----------

  //void redraw() {
  //  do_widget_redraw(this,MRect,0);
  //}

//------------------------------
public: // widget
//------------------------------

  virtual void on_widget_setPos(float AXpos, float AYpos) {
    //setWidgetPos(AXpos,AYpos);
  }

  virtual void on_widget_setSize(float AWidth, float AHeight) {
    //setWidgetSize(AWidth,AHeight);
    //alignChildWidgets(0);
    //redrawWidgets();

  }

  virtual void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) {
    paintChildWidgets(APainter,ARect,AMode);
  }

  virtual void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
  }

  virtual void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
  }

  virtual void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState) {
  }

  virtual void on_widget_keyPress(uint32_t AKey, uint32_t AChar, uint32_t AState) {
  }

  virtual void on_widget_keyRelease(uint32_t AKey, uint32_t AChar, uint32_t AState) {
  }

  virtual void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom) {
  }

  virtual void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) {
  }

//------------------------------
public: // "widget_listener"
//------------------------------

  virtual void do_widget_move(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) {
    if (MParent) MParent->do_widget_move(AWidget,AXdelta,AYdelta);
  }

  virtual void do_widget_resize(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) {
    if (MParent) MParent->do_widget_resize(AWidget,AXdelta,AYdelta);
  }

  virtual void do_widget_update(KODE_Widget* AWidget) {
    if (MParent) MParent->do_widget_update(AWidget);
  }

  virtual void do_widget_redraw(KODE_Widget* AWidget, KODE_FRect ARect, uint32_t AMode=0) {
    if (MParent) MParent->do_widget_redraw(AWidget, ARect, AMode=0);
  }

  virtual void do_widget_setHint(KODE_Widget* AWidget, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(AWidget,AHint);
  }

  virtual void do_widget_setCursor(KODE_Widget* AWidget, uint32_t ACursor) {
    if (MParent) MParent->do_widget_setCursor(AWidget, ACursor);
  }

  virtual void do_widget_setModal(KODE_Widget* AWidget) {
    if (MParent) MParent->do_widget_setModal(AWidget);
  }

  virtual void do_widget_keyCapture(KODE_Widget* AWidget) {
    if (MParent) MParent->do_widget_keyCapture(AWidget);
  }

  virtual void do_widget_mouseCapture(KODE_Widget* AWidget) {
    if (MParent) MParent->do_widget_mouseCapture(AWidget);
  }

  virtual void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) {
    if (MParent) MParent->do_widget_notify(AWidget, AValue);
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
