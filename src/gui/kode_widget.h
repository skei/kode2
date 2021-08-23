#ifndef kode_widget_included
#define kode_widget_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_painter.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

#define KODE_WIDGET_MAX_PARAMETERS 16

//

struct KODE_WidgetOptions {
  bool autoMouseCapture = false;
  bool autoMouseHide    = false;
  bool autoMouseCursor  = true;
  bool autoClip         = false;
  bool autoHint         = false;
//bool canDrag          = false;
//bool canDrop          = false;
  bool  sizePercent     = false;
};

struct KODE_WidgetStates {
  bool active           = false;
  bool visible          = false;
  bool interactive      = false;
  bool hovering         = false;
  bool clicked          = false;
//bool dragging         = false;
};

struct KODE_WidgetLayout {
  uint32_t    alignment     = KODE_WIDGET_ALIGN_PARENT;
  KODE_FRect  innerBorder   = KODE_FRect(0);  // space between widgets and parent edges
  KODE_FPoint spacing       = KODE_FPoint(0); // space inbetween widgets
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
private:
//------------------------------

  KODE_WidgetOptions  MOptions;
  KODE_WidgetLayout   MLayout;
  KODE_WidgetStates   MStates;
  KODE_Widgets        MChildren;
  float               MValue              = 0.0f;
  float               MDefaultValue       = 0.0f;
  KODE_Widget*        MParent             = KODE_NULL;
  const char*         MName               = "KODE_Widget";
  KODE_FRect          MRect               = KODE_FRect(0);
  KODE_FRect          MInitialRect        = KODE_FRect(0);
  KODE_FRect          MContentRect        = KODE_FRect(0);
  int32_t             MCursor             = KODE_CURSOR_DEFAULT;  // ps: MouseCursor clashes with KODE_Window::setMouseCursor
  uint32_t            MSelectedParameter  = 0;
  //KODE_Parameter*     MParameterPtr       = KODE_NULL;
  KODE_Parameter*     MParameterPtrs[KODE_WIDGET_MAX_PARAMETERS] = {0};

//------------------------------
public:
//------------------------------

  KODE_Widget(KODE_FRect ARect) {
    setRect(ARect);
    setInitialRect(ARect);
    setContentRect(KODE_FRect(0));
  }

  virtual ~KODE_Widget() {
    #ifndef KODE_NO_AUTODELETE
    deleteChildren();
    #endif
  }

//------------------------------
public: // set
//------------------------------

  virtual void setName(const char* AName)               { MName = AName; }
  virtual void setValue(float AValue)                   { MValue = AValue; }
  virtual void setDefaultValue(float AValue)            { MDefaultValue = AValue; }
  virtual void setRect(KODE_FRect ARect)                { MRect = ARect; }
  virtual void setInitialRect(KODE_FRect ARect)         { MInitialRect = ARect; }
  virtual void setContentRect(KODE_FRect ARect)         { MContentRect = ARect; }
  virtual void setCursor(int32_t ACursor)               { MCursor = ACursor; }
  virtual void setparent(KODE_Widget* AParent) { MParent = AParent; }
  virtual void setSelectedParameter(uint32_t AIndex)    { MSelectedParameter = AIndex; }
  virtual void setParameterPtr(KODE_Parameter* AParameter, uint32_t AIndex=0) { MParameterPtrs[AIndex] = AParameter; }
  //virtual void setParameterPtr(KODE_Parameter* p)       { MParameterPtr = p; }


//------------------------------
public: // get
//------------------------------

  virtual float               getValue()                    { return MValue; }
  virtual float               getDefaultValue()             { return MDefaultValue; }
  virtual KODE_WidgetLayout*  getLayout()                   { return &MLayout; }
  virtual KODE_WidgetOptions* getOptions()                  { return &MOptions; }
  virtual KODE_WidgetStates*  getStates()                   { return &MStates; }
  virtual KODE_FRect          getRect()                     { return MRect; }
  virtual KODE_FRect          getInitialRect()              { return MInitialRect; }
  virtual KODE_FRect          getContainerRect()            { return MContentRect; }
  virtual int32_t             getCursor()                   { return MCursor; }
  virtual KODE_Parameter*     getParameterPtr(uint32_t i=0) { return MParameterPtrs[i]; }
  virtual uint32_t            getSelectedParameter()        { return MSelectedParameter; }
//virtual KODE_Parameter*     getParameter(uint32_t AIndex) { return MParameters[AIndex]; }

//------------------------------
public:
//------------------------------

  virtual void update() {
    do_widget_update(this);
  }

  //----------

  virtual void redraw() {
    do_widget_redraw(this,getRect(),0);
  }

//------------------------------
public:
//------------------------------

  virtual KODE_Widget* appendWidget(KODE_Widget* AWidget) {
    AWidget->MParent = this;
    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  virtual void deleteChildren() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      KODE_Delete MChildren[i];
      MChildren[i] = KODE_NULL;
    }
  }

  //----------

  virtual KODE_Widget* findChild(float AXpos, float AYpos) {
    int32_t num = MChildren.size();
    if (num > 0) {
      for (int32_t i=(num-1); i>=0; i--) {
        KODE_Widget* widget = MChildren[i];
        if ( widget->getRect().contains(AXpos,AYpos) ) {
          KODE_Widget* child = widget->findChild(AXpos,AYpos);
          if (child) return child;
          return widget;
        }
      }
    }
    return KODE_NULL;
  }

  //----------

  virtual KODE_Widget* getOwner() {
    if (MParent) return MParent->getOwner();
    else return this;
  }

  //----------

  virtual void paintChildren(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      KODE_Widget* widget = MChildren[i];
      KODE_FRect rect = widget->getRect();
      rect.intersect(ARect);
      if (rect.isNotEmpty()) {
        widget->on_widget_paint(APainter,/*rect*/ARect,AMode);
      }
    }
  }

  //----------

  /*
    the widgets are aligned from their MInitialRect, set up when the
    widget is created.. so, if we resize the widget, it will be lost next
    time the widgets are being 'realigned
    maybe:
      a) have a MInitialDelta rect? difference from MInitialRect? in case
         of runtime moving, resizing,.
      b) modify the MInitialRect directly when needed (resize widget, etc)
      c) pre/post for scrollboxes, to find size of child widgets, etc..
      d) take scroll/offset position into account while realigning
      e) like prev lib, un_scroll, realign, re_scroll

    todo:
    - relative positions
    - percentages
  */

  virtual void alignChildren(float AXOffset=0.0f, float AYOffset=0.0f) {
    KODE_FRect client = getRect();
    client.shrink(getLayout()->innerBorder);
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_Widget* widget = MChildren[i];

      /*
        MRect has already been aligned, and children has been aligned by
        _previous_ MRect.. so we start rebuilding the coords from 'scratch'
        (initial rect).. (if we knew how much these have changed from, for
        example, the initial rect, we could just subtract it)..
      */

      KODE_FRect rect = widget->getInitialRect();

      if (widget->getOptions()->sizePercent) {
        //rect.x = client.x * (rect.x * 0.01f);
        //rect.y = client.y * (rect.y * 0.01f);
        rect.w = client.w * (rect.w * 0.01f);
        rect.h = client.h * (rect.h * 0.01f);
      }

      switch (widget->getLayout()->alignment) {

        case KODE_WIDGET_ALIGN_NONE:
          break;

        case KODE_WIDGET_ALIGN_PARENT:
          //rect.x += parent.x;
          //rect.y += parent.y;
          break;

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
          client.x += (rect.w + getLayout()->spacing.x);
          client.w -= (rect.w + getLayout()->spacing.x);
          break;

        case KODE_WIDGET_ALIGN_FILL_RIGHT:
          rect.x = client.x2() - rect.w;
          rect.y = client.y;
          rect.h = client.h;
          //client.x += (rect.w + getWidgetSpacing.x);
          client.w -= (rect.w + getLayout()->spacing.x);
          break;

        case KODE_WIDGET_ALIGN_FILL_TOP:
          rect.x = client.x;
          rect.y = client.y;
          rect.w = client.w;
          client.y += (rect.h + getLayout()->spacing.y);
          client.h -= (rect.h + getLayout()->spacing.y);
          break;

        case KODE_WIDGET_ALIGN_FILL_BOTTOM:
          rect.x = client.x;
          rect.y = client.y2() - rect.h;
          rect.w = client.w;
          //client.y += (rect.h + getLayout()->spacing.y);
          client.h -= (rect.h + getLayout()->spacing.y);
          break;

        case KODE_WIDGET_ALIGN_CENTER:
          rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
          rect.y = client.y + (client.h * 0.5f) - (rect.h * 0.5f);
          break;

        case KODE_WIDGET_ALIGN_CENTER_LEFT:
          rect.x += client.x;
          rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
          break;

        case KODE_WIDGET_ALIGN_CENTER_RIGHT:
          rect.x += client.x2() - rect.w;
          rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
          break;

        case KODE_WIDGET_ALIGN_CENTER_TOP:
          rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
          rect.y += client.y;
          break;

        case KODE_WIDGET_ALIGN_CENTER_BOTTOM:
          rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
          rect.y += client.y2() - rect.h;
          break;

        case KODE_WIDGET_ALIGN_LEFT:
          rect.x += client.x;
          break;

        case KODE_WIDGET_ALIGN_LEFT_TOP:
          rect.x += client.x;
          rect.y += client.y;
          break;

        //case KODE_WIDGET_ALIGN_LEFT_CENTER:
        //  break;

        case KODE_WIDGET_ALIGN_LEFT_BOTTOM:
          rect.x += client.x;
          rect.y += client.y2() - rect.h;
          break;

        case KODE_WIDGET_ALIGN_RIGHT:
          rect.x += client.x2() - rect.w;
          break;

        case KODE_WIDGET_ALIGN_RIGHT_TOP:
        //  break;


          rect.x += client.x2() - rect.w;
          rect.y += client.y;
          break;

        //case KODE_WIDGET_ALIGN_RIGHT_CENTER:
        //  break;

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

        //case KODE_WIDGET_ALIGN_TOP_CENTER:
        //  break;
        //case KODE_WIDGET_ALIGN_LEFT_CENTER:
        //  break;


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

        //case KODE_WIDGET_ALIGN_BOTTOM_CENTER:
        //  break;

        case KODE_WIDGET_ALIGN_BOTTOM_RIGHT:
          rect.x += client.x2() - rect.w;
          rect.y += client.y2() - rect.h;
          break;

        //case KODE_WIDGET_STACK_HORIZ:
        //  break;

        //case KODE_WIDGET_STACK_VERT:
        //  break;


      } // switch

      widget->setRect(rect);
      //widget->on_widget_setPos();
      //widget->on_widget_setSize();

      //widget->MParentRect = MRect;
      widget->alignChildren(/*0,0*/);
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

  virtual void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) {
    paintChildren(APainter,ARect,AMode);
  }

  virtual void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) {
    //if (MOptions.autoMouseHide) do_widget_setMouseCursor(this,KODE_CURSOR_HIDE);
    //if (MOptions.autoMouseCapture) do_widget_grabMouseCursor(this);
  }

  virtual void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) {
    //if (MOptions.autoMouseHide) do_widget_setMouseCursor(this,KODE_CURSOR_SHOW);
    //if (MOptions.autoMouseCapture) do_widget_grabMouseCursor(KODE_NULL);
  }

  virtual void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) {
  }

  virtual void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) {
  }

  virtual void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) {
  }

  virtual void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) {
    //if (MOptions.autoMouseCursor) do_widget_setMouseCursor(this,MMouseCursor);
  }

  virtual void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) {
    //if (MOptions.autoMouseCursor) do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
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

  virtual void do_widget_grabMouseCursor(KODE_Widget* ASender) {
    if (MParent) MParent->do_widget_grabMouseCursor(ASender);
  }

  virtual void do_widget_grabKeyboard(KODE_Widget* ASender) {
    if (MParent) MParent->do_widget_grabKeyboard(ASender);
  }

  virtual void do_widget_grabModal(KODE_Widget* ASender) {
    if (MParent) MParent->do_widget_grabModal(ASender);
  }

  virtual void do_widget_setMouseCursor(KODE_Widget* ASender, int32_t ACursor) {
    if (MParent) MParent->do_widget_setMouseCursor(ASender,ACursor);
  }

  virtual void do_widget_setMouseCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) {
    if (MParent) MParent->do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
  }

  virtual void do_widget_setHint(KODE_Widget* ASender, const char* AHint) {
    if (MParent) MParent->do_widget_setHint(ASender,AHint);
  }

  virtual void do_widget_notify(KODE_Widget* ASender, uint32_t AValue=0) {
    if (MParent) MParent->do_widget_notify(ASender,AValue);
  }

};

//----------------------------------------------------------------------
#endif
