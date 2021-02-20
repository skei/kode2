#ifndef kode_widget_included
#define kode_widget_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_gui_base.h"
#include "gui/kode_painter.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

#define KODE_WIDGET_MAX_PARAMETERS 16

struct KODE_WidgetOptions {
  bool autoMouseCapture = true;
  bool autoClip         = true;
  bool autoHint         = true;
};

struct KODE_WidgetStates {
  bool active       = true;
  bool visible      = true;
  bool interactive  = true;
  bool hovering     = true;
  bool clicked      = true;
};

struct KODE_WidgetLayout {
  uint32_t    alignment     = KODE_WIDGET_ALIGN_PARENT;
  KODE_FRect  innerBorder   = KODE_FRect(0);
  KODE_FPoint widgetSpacing = KODE_FPoint(0);
};

class KODE_Widget;
typedef KODE_Array<KODE_Widget*> KODE_Widgets;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_Widget {

  friend class KODE_Window;
  friend class KODE_Editor;

//------------------------------
protected:
//------------------------------

  const char*         MName         = "KODE_Widget";
  float               MValue        = 0.0f;

  KODE_WidgetLayout   MLayout;
  KODE_WidgetOptions  MOptions;
  KODE_WidgetStates   MStates;

  KODE_Widget*        MParent       = KODE_NULL;
  KODE_Widgets        MChildren;

  KODE_FRect          MRect         = KODE_FRect(0);
  KODE_FRect          MInitialRect  = KODE_FRect(0);
  KODE_FRect          MContentRect  = KODE_FRect(0);

  KODE_Parameter*     MParameter                              = KODE_NULL;

  uint32_t            MSelectedParameter                      = 0;
  KODE_Parameter*     MParameters[KODE_WIDGET_MAX_PARAMETERS] = {0};

//------------------------------
public:
//------------------------------

  KODE_Widget(KODE_FRect ARect) {
    MRect        = ARect;
    MInitialRect = ARect;
    MContentRect = KODE_FRect(0);
  }

  virtual ~KODE_Widget() {
    #ifndef KODE_NO_AUTODELETE
    deleteChildren();
    #endif
  }

//------------------------------
public:
//------------------------------

  //void setValue(float AValue) {
  //  MValue = AValue;
  //}

  //----------

  //void setParameter(KODE_Parameter* p) {
  //  MParameter = p;
  //}

//------------------------------
public:
//------------------------------

  float getValue() {
    return MValue;
  }

  //----------

  KODE_Parameter* getParameter() {
    return MParameter;
  }

  //----------

  //void setParameter(KODE_Parameter* AParameter, uint32_t AIndex=0) {
  //  MParameters[AIndex] = AParameter;
  //  MParameter = AParameter;
  //}

  //----------

  void selectParameter(uint32_t AIndex) {
    MParameter = MParameters[AIndex];
  }

//------------------------------
public:
//------------------------------

  void update() {
    do_widget_update(this);
  }

  //----------

  void redraw() {
    do_widget_redraw(this,MRect,0);
  }

//------------------------------
public:
//------------------------------

  KODE_Widget* appendWidget(KODE_Widget* AWidget) {
    AWidget->MParent = this;
    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  void deleteChildren() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      KODE_Delete MChildren[i];
      MChildren[i] = KODE_NULL;
    }
  }

  //----------

  KODE_Widget* findChild(float AXpos, float AYpos) {
    int32_t num = MChildren.size();
    if (num > 0) {
      for (int32_t i=(num-1); i>=0; i--) {
        //for (uint32_t i=0; i<num; i++) {
        KODE_Widget* widget = MChildren[i];
        if ( widget->MRect.contains(AXpos,AYpos) ) {
          KODE_Widget* child = widget->findChild(AXpos,AYpos);
          if (child) return child;
          return widget;
        }
      } // for
    }
    return KODE_NULL;
  }

  //----------

  void paintChildren(KODE_IPainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    //KODE_PRINT;
    for (uint32_t i=0; i<MChildren.size(); i++) {
      KODE_Widget* widget = MChildren[i];
      KODE_FRect rect = widget->MRect;
      rect.intersect(ARect);
      if (rect.isNotEmpty()) {
        widget->on_widget_paint(APainter,/*rect*/ARect,AMode);
      }
    }
  }

  //----------

  /*

    * the widgets are aligned from their MInitialRect, set up when the
      widget is created.. so, if we resize the widget, it will be lost next
      time the widgets are being 'realigned.. :-/

      todo: have a MInitialDelta rect? difference from MInitialRect? in case
      of runtime moving, resizing, etc.. ???

      or modify the MInitialRect directly when needed (resizing the widget,
      etc)..

  */

  void alignChildren() {
    //KODE_FRect parent = MRect;
    KODE_FRect client = MRect;
    client.shrink(MLayout.innerBorder);
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_Widget* widget = MChildren[i];

      /*
        MRect has already been aligned, and children has been aligned by
        _previous_ MRect.. so we start rebuilding the coords from 'scratch'
        (initial rect).. (if we knew how much these have changed from, for
        example, the initial rect, we could just subtract it)..
      */

      KODE_FRect rect = widget->MInitialRect;
      switch (widget->MLayout.alignment) {

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
          client.x += (rect.w + MLayout.widgetSpacing.x);
          client.w -= (rect.w + MLayout.widgetSpacing.x);
          break;

        case KODE_WIDGET_ALIGN_FILL_RIGHT:
          rect.x = client.x2() - rect.w;
          rect.y = client.y;
          rect.h = client.h;
          //client.x += (rect.w + MWidgetSpacing.x);
          client.w -= (rect.w + MLayout.widgetSpacing.x);
          break;

        case KODE_WIDGET_ALIGN_FILL_TOP:
          rect.x = client.x;
          rect.y = client.y;
          rect.w = client.w;
          client.y += (rect.h + MLayout.widgetSpacing.y);
          client.h -= (rect.h + MLayout.widgetSpacing.y);
          break;

        case KODE_WIDGET_ALIGN_FILL_BOTTOM:
          rect.x = client.x;
          rect.y = client.y2() - rect.h;
          rect.w = client.w;
          //client.y += (rect.h + MLayout.widgetSpacing.y);
          client.h -= (rect.h + MLayout.widgetSpacing.y);
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
      //widget->on_widget_setPos();
      //widget->on_widget_setSize();

      //widget->MParentRect = MRect;
      widget->alignChildren();
    } // for
    //MAlignedRect = MRect;
  }

//------------------------------
public:
//------------------------------

  virtual void on_widget_setPos(float AXpos, float AYpos) {
    //setPos(AXpos,AYpos);
  }

  virtual void on_widget_setSize(float AWidth, float AHeight) {
    //setSize(AWidth,AHeight);
  }

  virtual void on_widget_paint(KODE_IPainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    paintChildren(APainter,ARect,AMode);
  }

  virtual void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
  }

  virtual void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
  }

  virtual void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState) {
  }

  virtual void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState) {
  }

  virtual void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState) {
  }

  virtual void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom) {
  }

  virtual void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) {
  }

  virtual void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) {
    //setParameter(AParameter);
  }

//------------------------------
public:
//------------------------------

  virtual void do_widget_update(KODE_Widget* ASender) {
    if (MParent) MParent->do_widget_update(ASender);
  }

  virtual void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) {
    if (MParent) MParent->do_widget_redraw(ASender,ARect,AMode);
  }

  virtual void do_widget_grabMouse(KODE_Widget* ASender) {
    if (MParent) MParent->do_widget_grabMouse(ASender);
  }

  virtual void do_widget_grabKeyboard(KODE_Widget* ASender) {
    if (MParent) MParent->do_widget_grabKeyboard(ASender);
  }

  virtual void do_widget_grabModal(KODE_Widget* ASender) {
    if (MParent) MParent->do_widget_grabModal(ASender);
  }

  virtual void do_widget_setMouseCursor(KODE_Widget* ASender, uint32_t ACursor) {
    if (MParent) MParent->do_widget_setMouseCursor(ASender,ACursor);
  }

  virtual void do_widget_setMousePos(KODE_Widget* ASender, float AXpos, float AYpos) {
    if (MParent) MParent->do_widget_setMousePos(ASender,AXpos,AYpos);
  }

  virtual void do_widget_setHint(KODE_Widget* ASender, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(ASender,AHint);
  }

  virtual void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) {
  }

};



//----------------------------------------------------------------------
#endif
