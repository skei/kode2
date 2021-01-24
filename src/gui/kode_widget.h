#ifndef kode_widget_included
#define kode_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_array.h"
#include "base/types/kode_point.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

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

  KODE_Widget*  MParent         = KODE_NULL;
  KODE_Widgets  MChildWidgets;
  const char*   MName           = "Widget";
  int32_t       MIndex          = 0;
  uint32_t      MAlignment      = KODE_WIDGET_ALIGN_PARENT;

  KODE_FPoint   MWidgetSpacing  = KODE_FPoint(0,0);
  KODE_FRect    MOuterBorder    = KODE_FRect(0,0,0,0);
  KODE_FRect    MInnerBorder    = KODE_FRect(0,0,0,0);
//  uint32_t      MAnchors        = 0;

  bool          MAutoCursor     = false;
  bool          MAutoHint       = false;
  bool          MAutoClip       = false;
  bool          MIsActive       = true;
  bool          MIsVisible      = true;
  bool          MIsHovering     = false;
  bool          MIsClicked      = false;

  KODE_FRect    MRect           = KODE_FRect(0);
  KODE_FRect    MInitialRect    = KODE_FRect(0);
  KODE_FRect    MContentRect    = KODE_FRect(0);
  //KODE_FRect    MAlignedRect    = KODE_FRect(0);

  int32_t       MMouseCursor    = KODE_CURSOR_DEFAULT;
  const char*   MHint           = "";
  float         MValue          = 0.0f;
  void*         MParameter      = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Widget(KODE_FRect ARect) {
    MName         = "KODE_Widget";
    MRect         = ARect;
    MInitialRect  = ARect;
    MContentRect  = KODE_FRect(0);
    //MAlignedRect  = KODE_FRect(0);
  }

  virtual ~KODE_Widget() {
    #ifndef KODE_NO_AUTODELETE
    deleteChildWidgets();
    #endif
  }

//------------------------------
public:
//------------------------------

  void        setName(const char* AName)      { MName = AName; }
  void        setRect(KODE_FRect ARect)       { MRect = ARect; }
  void        setValue(float AValue)          { MValue = AValue; }
  void        setParameter(void* AParam)      { MParameter = AParam; }
  void        setAlignment(uint32_t AAlign)   { MAlignment = AAlign; }

  void        setOuterBorder(KODE_FRect R)    { MOuterBorder = R; }
  void        setInnerBorder(KODE_FRect R)    { MInnerBorder = R; }
  void        setWidgetSpacing(KODE_FPoint P) { MWidgetSpacing = P; }

  const char*         getName()               { return MName; }
  KODE_FRect          getRect()               { return MRect; }
  float               getValue()              { return MValue; }
  void*               getParameter()          { return MParameter; }
  uint32_t            getAlignment()          { return MAlignment; }

  KODE_Widgets*       getWidgets()            { return &MChildWidgets; }
  KODE_Widget*        getWidget(uint32_t i)   { return MChildWidgets[i]; }
  int32_t             getIndex()              { return MIndex; }
  KODE_Widget*        getParent()             { return MParent; }
  KODE_FRect          getInitialRect()        { return MInitialRect; }
  KODE_FRect          getContentRect()        { return MContentRect; }
  int32_t             getMouseCursor()        { return MMouseCursor; }
  const char*         getHint()               { return MHint; }

//------------------------------
public:
//------------------------------

  void appendChildWidget(KODE_Widget* AWidget) {
    int32_t index = MChildWidgets.size();
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
    int32_t num = MChildWidgets.size();
    if (num > 0) {
      for (int32_t i=(num-1); i>=0; i--) {
        //for (uint32_t i=0; i<num; i++) {
        KODE_Widget* widget = MChildWidgets[i];
        if ( widget->MRect.contains(AXpos,AYpos) ) {
          KODE_Widget* child = widget->findChildWidget(AXpos,AYpos);
          if (child) return child;
          return widget;
        }
      } // for
    }
    return KODE_NULL;
  }

  //----------

  void alignChildWidgets() {

    //KODE_FRect parent = MRect;

    KODE_FRect client = MRect;
    client.shrink(MInnerBorder);

    uint32_t num = MChildWidgets.size();
    for (uint32_t i=0; i<num; i++) {

      KODE_Widget* widget = MChildWidgets[i];

      /*
        MRect has already been aligned, and children has been aligned by
        _previous_ MRect..
        so we start rebuilding the coords from 'scratch' (initial rect)..
        (if we knew how much these have changed from, for example, the initial
        rect, we could just subtract it..)
      */

      KODE_FRect rect = widget->MInitialRect;

      switch (widget->MAlignment) {

        case KODE_WIDGET_ALIGN_NONE:
          break;

        case KODE_WIDGET_ALIGN_PARENT:
          //rect.x += parent.x;
          //rect.y += parent.y;
          //break;

        case KODE_WIDGET_ALIGN_CLIENT:
          rect.x += client.x;
          rect.y += client.y;
          break;

        case KODE_WIDGET_ALIGN_FILL_CLIENT:
          rect.x = client.x;
          rect.y = client.y;
          rect.w = client.w;
          rect.h = client.h;
          break;

        case KODE_WIDGET_ALIGN_FILL_LEFT:
          rect.x = client.x;
          rect.y = client.y;
          rect.h = client.h;
          client.x += (rect.w + MWidgetSpacing.x);
          client.w -= (rect.w + MWidgetSpacing.x);
          break;

        case KODE_WIDGET_ALIGN_FILL_RIGHT:
          rect.x = client.x2() - rect.w;
          rect.y = client.y;
          rect.h = client.h;
          //client.x += (rect.w + MWidgetSpacing.x);
          client.w -= (rect.w + MWidgetSpacing.x);
          break;

        case KODE_WIDGET_ALIGN_FILL_TOP:
          rect.x = client.x;
          rect.y = client.y;
          rect.w = client.w;
          client.y += (rect.h + MWidgetSpacing.y);
          client.h -= (rect.h + MWidgetSpacing.y);
          break;

        case KODE_WIDGET_ALIGN_FILL_BOTTOM:
          rect.x = client.x;
          rect.y = client.y2() - rect.h;
          rect.w = client.w;
          //client.y += (rect.h + MWidgetSpacing.y);
          client.h -= (rect.h + MWidgetSpacing.y);
          break;

        case KODE_WIDGET_ALIGN_LEFT:
          rect.x += client.x;
          break;

        case KODE_WIDGET_ALIGN_LEFT_TOP:
          rect.x += client.x;
          rect.y += client.y;
          break;

        case KODE_WIDGET_ALIGN_LEFT_BOTTOM:
          rect.x += client.x;
          rect.y += client.y2() - rect.h;
          break;

        case KODE_WIDGET_ALIGN_RIGHT:
          rect.x += client.x2() - rect.w;
          break;

        case KODE_WIDGET_ALIGN_RIGHT_TOP:
          rect.x += client.x2() - rect.w;
          rect.y += client.y;
          break;

        case KODE_WIDGET_ALIGN_RIGHT_BOTTOM:
          rect.x += client.x2() - rect.w;
          rect.y += client.y2() - rect.h;
          break;

        case KODE_WIDGET_ALIGN_TOP:
          rect.y += client.y;
          break;

        case KODE_WIDGET_ALIGN_TOP_LEFT:
          rect.x += client.x;
          rect.y += client.y;
          break;

        case KODE_WIDGET_ALIGN_TOP_RIGHT:
          rect.x += client.x2() - rect.w;
          rect.y += client.y;
          break;

        case KODE_WIDGET_ALIGN_BOTTOM:
          rect.y += client.y2() - rect.h;
          break;

        case KODE_WIDGET_ALIGN_BOTTOM_LEFT:
          rect.x += client.x;
          rect.y += client.y2() - rect.h;
          break;

        case KODE_WIDGET_ALIGN_BOTTOM_RIGHT:
          rect.x += client.x2() - rect.w;
          rect.y += client.y2() - rect.h;
          break;

        //case KODE_WIDGET_STACK_HORIZ:
        //  break;

        //case KODE_WIDGET_STACK_VERT:
        //  break;


      } // switch

      widget->MRect = rect;
      widget->alignChildWidgets();

    } // for

    //MAlignedRect = MRect;
  }

//------------------------------
public: // widget
//------------------------------

  // never called?

//  virtual void on_widget_setPos(float AXpos, float AYpos) {
//    //setWidgetPos(AXpos,AYpos);
//  }

  // never called?

//  virtual void on_widget_setSize(float AWidth, float AHeight) {
//    //setWidgetSize(AWidth,AHeight);
//    //alignChildWidgets(0);
//    //redrawWidgets();
//  }

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
    MIsHovering = true;
    //do_widget_redraw(this,MRect,0);
  }

  virtual void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) {
    MIsHovering = false;
    //do_widget_redraw(this,MRect,0);
  }

  //----------

  virtual void on_widget_connect(KODE_Parameter* AParameter) {
  }

  //----------

//  virtual bool on_widget_startDrag(uint32_t* AType, void** APtr) {
//    *AType = 0;
//    *APtr = KODE_NULL;
//    return false;
//  }
//
//  //----------
//
//  virtual void on_widget_endDrag(KODE_Widget* AFrom, uint32_t AType, void* APtr) {
//  }
//
//  //----------
//
//  virtual bool on_widget_canDrop(uint32_t AType, void* APtr, float AXpos, float AYpos) {
//    return false;
//  }

//------------------------------
public: // "widget_listener"
//------------------------------

  virtual void do_widget_moved(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) {
    if (MParent) MParent->do_widget_moved(AWidget,AXdelta,AYdelta);
  }

  virtual void do_widget_resized(KODE_Widget* AWidget, uint32_t AXdelta, uint32_t AYdelta) {
    if (MParent) MParent->do_widget_resized(AWidget,AXdelta,AYdelta);
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

  //virtual void do_widget_mouseCapture(KODE_Widget* AWidget) {
  //  if (MParent) MParent->do_widget_mouseCapture(AWidget);
  //}

  virtual void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) {
    if (MParent) MParent->do_widget_notify(AWidget, AValue);
  }

  //----------

  // ASender = sizer.MTarget

//  virtual void do_sizer(KODE_Widget* ASender, float ADeltaX, float ADeltaY, uint32_t AMode) {
//    //MResized.w += ADeltaX;
//    //MResized.h += ADeltaY;
//    float w = MInitialRect.w + MResized.w + ADeltaX;
//    float h = MInitialRect.h + MResized.h + ADeltaY;
//    //KODE_DTrace("MInitialRect.w %.0f MResized.w %.0f ADeltaX %.0f (MMinSize.w %.0f) -> w %.0f\n",MInitialRect.w,MResized.w,ADeltaX,MMinSize.w,w);
//    if (w < MMinSize.w) MResized.w = MMinSize.w - MInitialRect.w;
//    else if (w > MMaxSize.w) MResized.w = MMaxSize.w - MInitialRect.w;
//    else MResized.w += ADeltaX;
//    if (h < MMinSize.h) MResized.h = MMinSize.h - MInitialRect.h;
//    else if (h > MMaxSize.h) MResized.h = MMaxSize.h - MInitialRect.h;
//    else MResized.h += ADeltaY;
//    do_resized( this, MInitialRect.w + MResized.w ,MInitialRect.h + MResized.h );
//  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif
