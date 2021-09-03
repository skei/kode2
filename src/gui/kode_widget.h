#ifndef kode_widget_included
#define kode_widget_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_painter.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

#define MAX_PARAMS 16

//

struct KODE_WidgetFlags {
  bool active           = true;
  bool visible          = true;
  bool opaque           = false;
  bool interacting      = false;
  bool sizePercent      = false;
  bool posPercent       = false;
  bool autoCursor       = true;
  bool autoHint         = false;
  bool autoClip         = true;
  bool autoMouseLock    = false;
  bool autoMouseHide    = false;
  bool canDrag          = false;
  bool canDrop          = false;
};

struct KODE_WidgetLayout {
  uint32_t    alignment     = KODE_WIDGET_ALIGN_PARENT;
  KODE_FRect  innerBorder   = KODE_FRect(0);              // space between widgets and parent edges
  KODE_FPoint spacing       = KODE_FPoint(0);             // space inbetween widgets
  KODE_FRect  extraBorder   = KODE_FRect(0);              // extra space/border for each widget
  bool        contentBorder = true;                       // true if content rect includes innerBorder of parent
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

  const char*       MName                   = "KODE_Widget";
  const char*       MHint                   = "widget";
  int32_t           MCursor                 = KODE_CURSOR_DEFAULT;
  int32_t           MIndex                  = -1;
  float             MValue                  = 0.0f;
  float             MDefaultValue           = 0.0f;
  KODE_FRect        MRect                   = KODE_FRect(0);
  KODE_FRect        MInitialRect            = KODE_FRect(0);
  KODE_FRect        MContentRect            = KODE_FRect(0);
  KODE_Widget*      MParent                 = KODE_NULL;
  KODE_Widgets      MChildren;
  KODE_Parameter*   MParameters[MAX_PARAMS] = {0};

//uint32_t          MSelectedParameter      = 0;

//------------------------------
protected:
//------------------------------

  KODE_Surface*     MWidgetSurface          = KODE_NULL;
  bool              MWidgetSurfaceAllocated = false;
  uint32_t          MTileCount              = 0;
  uint32_t          MTileXcount             = 0;
  uint32_t          MTileYcount             = 0;
  uint32_t          MTileWidth              = 0;
  uint32_t          MTileHeight             = 0;

  float             MChildrenXOffset        = 0.0f;
  float             MChildrenYOffset        = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_WidgetFlags  flags;
  KODE_WidgetLayout layout;

//------------------------------
public:
//------------------------------

  KODE_Widget(KODE_FRect ARect) {
    init(ARect);
  }

  KODE_Widget(float AWidth, float AHeight) {
    KODE_FRect arect = KODE_FRect(0,0,AWidth,AHeight);
    init(arect);
  }

  KODE_Widget(float ASize) {
    KODE_FRect arect = KODE_FRect(ASize);
    init(arect);
  }

  //----------

  virtual ~KODE_Widget() {
    #ifndef KODE_NO_AUTODELETE
    deleteChildren();
    #endif
    if (MWidgetSurface && MWidgetSurfaceAllocated) delete MWidgetSurface;
  }

//------------------------------
public: // set
//------------------------------

  void init(KODE_FRect ARect) {
    setRect(ARect);
    setInitialRect(ARect);
    setContentRect(KODE_FRect(0));
  }

//------------------------------
public: // set
//------------------------------

  virtual void setName(const char* AName)               { MName = AName; }
  virtual void setValue(float AValue)                   { MValue = AValue; }
  virtual void setDefaultValue(float AValue)            { MDefaultValue = AValue; }
  virtual void setRect(KODE_FRect ARect)                { MRect = ARect; }
  virtual void setRect(float x, float y, float w, float h)  { MRect.x=x; MRect.y=y; MRect.w=w;MRect.h=h; }
  virtual void setPos(float AXpos, float AYpos)         { MRect.x = AXpos; MRect.y = AYpos; }
  virtual void setSize(float AWidth, float AHeight)     { MRect.w = AWidth; MRect.h = AHeight; }
  virtual void setWidth(float AWidth)                   { MRect.w = AWidth; }
  virtual void setHeight(float AHeight)                 { MRect.h = AHeight; }
  virtual void setContentRect(KODE_FRect ARect)         { MContentRect = ARect; }
  virtual void setInitialRect(KODE_FRect ARect)         { MInitialRect = ARect; }
  virtual void setInitialPos(float AX, float AY)        { MInitialRect.x = AX; MInitialRect.y = AY; }
  virtual void setInitialSize(float AW, float AH)       { MInitialRect.w = AW; MInitialRect.h = AH; }
  virtual void setInitialWidth(float AW)                { MInitialRect.w = AW; }
  virtual void setInitialHeight(float AH)               { MInitialRect.h = AH; }
  virtual void setCursor(int32_t ACursor)               { MCursor = ACursor; }
  virtual void setparent(KODE_Widget* AParent)          { MParent = AParent; }
  virtual void setIndex(int32_t AIndex)                 { MIndex = AIndex; }
  virtual void setHint(const char* AHint)               { MHint = AHint; }
  virtual void setChildrenXOffset(float AX)             { MChildrenXOffset = AX; }
  virtual void setChildrenYOffset(float AY)             { MChildrenYOffset = AY; }
  virtual void setChildrenOffset(float AX, float AY)    { MChildrenXOffset = AX; MChildrenYOffset = AY; }

  virtual void setParameter(KODE_Parameter* AParameter, uint32_t AIndex=0) { MParameters[AIndex] = AParameter; }

  //virtual void setSelectedParameter(uint32_t AIndex)    { MSelectedParameter = AIndex; }
  //virtual void setParameterPtr(KODE_Parameter* p)       { MParameterPtr = p; }

//------------------------------
public: // get
//------------------------------

  virtual float               getValue()                    { return MValue; }
  virtual float               getDefaultValue()             { return MDefaultValue; }
  virtual KODE_FRect          getRect()                     { return MRect; }
  //virtual float               getWidth()                    { return MRect.w; }
  //virtual float               getHeight()                   { return MRect.h; }
  virtual KODE_FRect          getInitialRect()              { return MInitialRect; }
  virtual KODE_FRect          getContentRect()              { return MContentRect; }
  virtual int32_t             getCursor()                   { return MCursor; }
  virtual KODE_Parameter*     getParameter(uint32_t i=0)    { return MParameters[i]; }
  virtual KODE_Widget*        getParent()                   { return MParent; }
  virtual const char*         getHint()                     { return MHint; }
  virtual uint32_t            getNumChildren()              { return MChildren.size(); }
  virtual KODE_Widget*        getChild(uint32_t AIndex)     { return MChildren[AIndex]; }
  virtual int32_t             getIndex()                    { return MIndex; }

  //virtual KODE_WidgetLayout*  getLayout()                   { return &MLayout; }
  //virtual KODE_WidgetOptions* getOptions()                  { return &MOptions; }
  //virtual KODE_WidgetStates*  getStates()                   { return &MStates; }
  //virtual uint32_t            getSelectedParameter()        { return MSelectedParameter; }
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

  //----------

  virtual void setImage(KODE_Drawable* ATarget, KODE_Surface* ASurface) {
    MWidgetSurface = ASurface;
    MWidgetSurfaceAllocated = false;
  }

  virtual void setImage(KODE_Drawable* ATarget, KODE_Bitmap* ABitmap) {
    MWidgetSurface = new KODE_Surface(ATarget,ABitmap->getWidth(),ABitmap->getHeight());
    MWidgetSurfaceAllocated = true;
    KODE_Painter* painter = new KODE_Painter(MWidgetSurface);
    painter->uploadBitmap(0,0,ABitmap);
    //painter->flush();
    delete painter;
  }

  virtual void setImage(KODE_Drawable* ATarget, uint8_t* ABuffer, uint32_t ASize, KODE_Color ABackground) {
    KODE_Bitmap* bitmap = new KODE_Bitmap(ABuffer,ASize);
    bitmap->premultAlpha( (uint32_t)ABackground );
    setImage(ATarget,bitmap);
    delete bitmap;
  }

  virtual void setImage(KODE_Drawable* ATarget, const char* AFilename, KODE_Color ABackground) {
    KODE_Bitmap* bitmap = new KODE_Bitmap(AFilename);
    bitmap->premultAlpha( (uint32_t)ABackground );
    setImage(ATarget,bitmap);
    delete bitmap;
  }

  //----------

  virtual void setupTiles(uint32_t AXcount, uint32_t AYcount) {
    MTileXcount = AXcount;
    MTileYcount = AYcount;
    MTileWidth  = MWidgetSurface->getWidth() / AXcount;
    MTileHeight = MWidgetSurface->getHeight() / AYcount;
  }

  KODE_FRect getTileRect(uint32_t AIndex) {
    float x = floorf(AIndex % MTileXcount) * MTileWidth;
    float y = floorf(AIndex / MTileXcount) * MTileHeight;
    float w = MTileWidth - 1;
    float h = MTileHeight - 1;
    return KODE_FRect(x,y,w,h);
  }

//------------------------------
public:
//------------------------------

  virtual KODE_Widget* appendWidget(KODE_Widget* AWidget) {
    AWidget->setIndex(MChildren.size());
    AWidget->MParent = this;
    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  virtual void deleteChildren() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      delete MChildren[i];
      MChildren[i] = KODE_NULL;
    }
    MChildren.clear();
  }

  //----------

  virtual KODE_Widget* findChild(float AXpos, float AYpos) {
    int32_t num = MChildren.size();
    if (num > 0) {
      for (int32_t i=(num-1); i>=0; i--) {
        KODE_Widget* widget = MChildren[i];
        //if (widget->flags.visible) {
        if (widget->flags.active) {
          if ( widget->getRect().contains(AXpos,AYpos) ) {
            KODE_Widget* child = widget->findChild(AXpos,AYpos);
            if (child) return child;
            return widget;
          }
        }
      }
    }
    return KODE_NULL;
  }

  //----------

  /*
    todo:
    - cull/clip children using ARect (clipping rect)
  */

  virtual void paintChildren(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) {

    KODE_FRect mrect = getRect();
    KODE_FRect clip_rect = mrect;

    mrect.overlap(ARect);

    clip_rect.shrink(layout.innerBorder);
    clip_rect.overlap(ARect);

    if (clip_rect.isEmpty()) return;

    if (flags.autoClip) APainter->pushClip(clip_rect/*mrect*/);

    for (uint32_t i=0; i<MChildren.size(); i++) {
      KODE_Widget* child = MChildren[i];
      if (child->flags.visible) {
        KODE_FRect child_rect = child->getRect();
        if (child_rect.isNotEmpty()) {

//          if (child_rect.touches(mrect)) {

            KODE_FRect overlap_rect = mrect;
            overlap_rect.overlap(child_rect);
            if (overlap_rect.isNotEmpty()) { // shouldn't be empty, because they overlap

//              if (child->flags.autoClip) {
//                APainter->pushClip(overlap_rect);
//              } // clip

              //child->on_widget_paint(APainter,ARect,AMode);   // incoming rect
              //child->on_widget_paint(APainter,mrect,AMode);   // this/parent
              child->on_widget_paint(APainter,overlap_rect,AMode);  // clip rect

//              if (child->flags.autoClip) {
//                APainter->popClip();
//              } // clip

            } // !empty

//          } // touches

        } // !empty
      } // visible
    } // for all children

    if (flags.autoClip) APainter->popClip();

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

  // content includes innerBorder?

  virtual void alignChildren(bool ARecursive=true) {
    KODE_FRect client   = getRect();
    KODE_FRect parent   = client;
    KODE_FRect content  = client;
    client.shrink(layout.innerBorder);
    parent.shrink(layout.innerBorder);
    if (!layout.contentBorder) content.shrink(layout.innerBorder);
    content.setSize(0,0);
    float stackx = 0;
    float stacky = 0;
    float stack_highest = 0;
    float stack_widest = 0;
    //bool prev_was_stacking = false;
    uint32_t prev_alignment = KODE_WIDGET_ALIGN_NONE;
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_Widget* child = MChildren[i];
      if (child->flags.visible) {

        KODE_FRect  rect      = child->getInitialRect();
        uint32_t    alignment = child->layout.alignment;

        if (child->flags.sizePercent) {
          rect.w = client.w * (rect.w * 0.01f);
          rect.h = client.h * (rect.h * 0.01f);
        }
        if (child->flags.posPercent) {
          rect.x = client.w * (rect.x * 0.01f);
          rect.y = client.w * (rect.y * 0.01f);
        }

//----------

        //  if we were stacking, but isn't now..

        if (prev_alignment == KODE_WIDGET_STACK_HORIZ) {
          if (alignment != KODE_WIDGET_STACK_HORIZ) {
            client.y += (stacky + stack_highest + layout.spacing.y);
          }
        }

        if (prev_alignment == KODE_WIDGET_STACK_VERT) {
          if (alignment != KODE_WIDGET_STACK_VERT) {
            client.x += (stackx + stack_widest + layout.spacing.x);
          }
        }

        prev_alignment = alignment;

        //KODE_FRect new_client_rect = client;

//----------

        switch (alignment) {
          case KODE_WIDGET_ALIGN_NONE:
            break;
          case KODE_WIDGET_ALIGN_PARENT:
            rect.x += parent.x;
            rect.y += parent.y;
            break;
          case KODE_WIDGET_ALIGN_CLIENT:
            rect.x += client.x;
            rect.y += client.y;
            break;
          //-----
          case KODE_WIDGET_ALIGN_CENTER:
            rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y = client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            break;
          case KODE_WIDGET_ALIGN_CENTER_HORIZ:
            rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            break;
          case KODE_WIDGET_ALIGN_CENTER_VERT:
            rect.y = client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            break;
          //-----
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
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          case KODE_WIDGET_ALIGN_FILL_RIGHT:
            rect.x = client.x2() - rect.w;
            rect.y = client.y;
            rect.h = client.h;
            //client.x += (rect.w + getWidgetSpacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          case KODE_WIDGET_ALIGN_FILL_TOP:
            rect.x = client.x;
            rect.y = client.y;
            rect.w = client.w;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;
          case KODE_WIDGET_ALIGN_FILL_BOTTOM:
            rect.x = client.x;
            rect.y = client.y2() - rect.h;
            rect.w = client.w;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;
          //-----
          //case KODE_WIDGET_ALIGN_LEFT:
          //  rect.x += client.x;
          //  client.x += (rect.w + layout.spacing.x);
          //  client.w -= (rect.w + layout.spacing.x);
          //  break;
          case KODE_WIDGET_ALIGN_LEFT_TOP:
            rect.x += client.x;
            rect.y += client.y;
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          case KODE_WIDGET_ALIGN_LEFT_CENTER:
            rect.x += client.x;
            rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          case KODE_WIDGET_ALIGN_LEFT_BOTTOM:
            rect.x += client.x;
            rect.y += client.y2() - rect.h;
            client.x += (rect.w + layout.spacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          //-----
          //case KODE_WIDGET_ALIGN_RIGHT:
          //  rect.x += client.x2() - rect.w;
          //  //client.x += (rect.w + getWidgetSpacing.x);
          // client.w -= (rect.w + layout.spacing.x);
          //  break;
          case KODE_WIDGET_ALIGN_RIGHT_TOP:
            rect.x += client.x2() - rect.w;
            rect.y += client.y;
            //client.x += (rect.w + getWidgetSpacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          case KODE_WIDGET_ALIGN_RIGHT_CENTER:
            rect.x += client.x2() - rect.w;
            rect.y += client.y + (client.h * 0.5f) - (rect.h * 0.5f);
            //client.x += (rect.w + getWidgetSpacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          case KODE_WIDGET_ALIGN_RIGHT_BOTTOM:
            rect.x += client.x2() - rect.w;
            rect.y += client.y2() - rect.h;
            //client.x += (rect.w + getWidgetSpacing.x);
            client.w -= (rect.w + layout.spacing.x);
            break;
          //-----
          //case KODE_WIDGET_ALIGN_TOP:
          //  rect.y += client.y;
          // client.y += (rect.h + layout.spacing.y);
          //  client.h -= (rect.h + layout.spacing.y);
          //  break;
          case KODE_WIDGET_ALIGN_TOP_LEFT:
            rect.x += client.x;
            rect.y += client.y;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;
          case KODE_WIDGET_ALIGN_TOP_CENTER:
            rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y += client.y;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;
          case KODE_WIDGET_ALIGN_TOP_RIGHT:
            rect.x += client.x2() - rect.w;
            rect.y += client.y;
            client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;
          //-----
          //case KODE_WIDGET_ALIGN_BOTTOM:
          //  rect.y += client.y2() - rect.h;
          // //client.y += (rect.h + layout.spacing.y);
          // client.h -= (rect.h + layout.spacing.y);
          //  break;
          case KODE_WIDGET_ALIGN_BOTTOM_LEFT:
            rect.x += client.x;
            rect.y += client.y2() - rect.h;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;
          case KODE_WIDGET_ALIGN_BOTTOM_CENTER:
            rect.x = client.x + (client.w * 0.5f) - (rect.w * 0.5f);
            rect.y += client.y2() - rect.h;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;
          case KODE_WIDGET_ALIGN_BOTTOM_RIGHT:
            rect.x += client.x2() - rect.w;
            rect.y += client.y2() - rect.h;
            //client.y += (rect.h + layout.spacing.y);
            client.h -= (rect.h + layout.spacing.y);
            break;

          //-----

          /*
            - if first widget doesn't fit, it will jump to next line
          */

          case KODE_WIDGET_STACK_HORIZ:
            if ((stackx + rect.w + layout.innerBorder.w - layout.spacing.x) >= client.w) {
              if (stackx != 0) {  // first widget..
                stackx = 0;
                stacky += stack_highest + layout.spacing.y;
                stack_highest = 0;
              }
            }
            rect.x = (client.x + stackx);
            rect.y = (client.y + stacky);
            stackx += rect.w + layout.spacing.x;
            if (rect.h > stack_highest) stack_highest = rect.h;
            break;
          case KODE_WIDGET_STACK_VERT:
            if ((stacky + rect.h) >= client.h) {
              if (stacky != 0) {  // first widget..
                stacky = 0;
                stackx += stack_widest + layout.spacing.x;
                stack_widest = 0;
              }
            }
            rect.x += client.x + stackx;
            rect.y += client.y + stacky;
            stacky  += rect.h + layout.spacing.y;
            if (rect.w > stack_widest) stack_widest = rect.w;
            break;

        } // switch alignment

        //if (rect.w < child->getMinWidth()) rect.w = child->getMinWidth();
        //if (rect.h < child->getMinHeight()) rect.h = child->getMinHeight();
        //rect = new_client_rect;

        rect.shrink(child->layout.extraBorder);
        content.combine(rect);

        child->MRect.x = rect.x + MChildrenXOffset;
        child->MRect.y = rect.y + MChildrenYOffset;
        child->MRect.w = rect.w;
        child->MRect.h = rect.h;

        if (ARecursive) child->alignChildren(ARecursive);

      } // child visible
    } // for all children
    if (layout.contentBorder) {
      content.w += layout.innerBorder.w;
      content.h += layout.innerBorder.h;
    }
    MContentRect = content;
  }

  //----------

  virtual void scrollChildren(float AOffsetX, float AOffsetY) {
    uint32_t num = MChildren.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_Widget* child = MChildren[i];
      if (child->flags.visible) {
        //child->setChildrenOffset(AOffsetX,AOffsetY);
        child->MRect.x += AOffsetX;
        child->MRect.y += AOffsetY;
        child->scrollChildren(AOffsetX,AOffsetY);
      }
    }
  }

  virtual void resize(float ADeltaX=0.0f, float ADeltaY=0.0f) {
    MRect.w += ADeltaX;
    MRect.h += ADeltaY;
    MInitialRect.w += ADeltaX;
    MInitialRect.h += ADeltaY;
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

  //virtual void on_widget_realign(KODE_Painter* APainter, uint32_t AMode) {
  //}

  virtual void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) {
    paintChildren(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
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
    if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
    if (flags.autoHint) do_widget_setHint(this,MHint);
  }

  virtual void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) {
    if (flags.autoCursor) do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
    if (flags.autoHint) do_widget_setHint(this,"");
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

  /*
    - if this.opaque and this.mrect covers ARect, it's not visible,
      so we can ignore the update
    - if sender.opaque, nothing below it is visible, so we can start drawing
      from that widget (and downwards), sp pass on ASender, else we need to
      draw ourselves first, so send this as ASender..
    - (when it reaches window, ASender is the first widget to start painting
      at (and downwards), and ARect is the neede update window)
  */

  virtual void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) {
    if (MParent) MParent->do_widget_redraw(ASender,ARect,AMode);
  }

  virtual void do_widget_moved(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) {
    if (MParent) MParent->do_widget_moved(ASender,ADeltaX,ADeltaY);
  }

  // called by sizer

  virtual void do_widget_resized(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) {
    //if (MParent) MParent->do_widget_resized(ASender,ADeltaX,ADeltaY);
    resize(ADeltaX,ADeltaY);
    if (MParent) {
      MParent->alignChildren();
      MParent->redraw();
    }
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

#undef MAX_PARAMS

//----------------------------------------------------------------------
#endif
