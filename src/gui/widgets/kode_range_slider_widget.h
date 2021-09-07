#ifndef kode_range_slider_widget_included
#define kode_range_slider_widget_included
//----------------------------------------------------------------------

/*
  todo:

  * drag middle section to modify both values..

  * use drag_value instead of mouse x pos ?

  * base on slider
    - drawing
    - grab/hide cursor
    - shift-drag
*/

/*
*/


#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_RangeSliderWidget
: public KODE_Widget {
//: public KODE_DragValueWidget {

//------------------------------
protected:
//------------------------------

  KODE_Color  MBackgroundColor  = KODE_COLOR_DARK_GRAY;
  KODE_Color  MBarColor         = KODE_COLOR_LIGHT_GRAY;
  KODE_Color  MEdgeColor        = KODE_COLOR_WHITE;
  uint32_t    MEdgeWidth        = 3;
  float       MWidgetValue2     = 0.0f;
  int32_t     MHoverEdge        = -1;
  bool        MIsDragging1      = false;
  bool        MIsDragging2      = false;
  int32_t     MEdgeDistance     = 3;

//------------------------------
public:
//------------------------------

  KODE_RangeSliderWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_RangeSliderWidget");
    setHint("rangeslider");
  }

//------------------------------
public:
//------------------------------

  virtual void setBackgroundColor(KODE_Color AColor)  { MBackgroundColor = AColor; }
  virtual void setBarColor(KODE_Color AColor)         { MBarColor = AColor; }
  virtual void setEdgeColor(KODE_Color AColor)        { MEdgeColor = AColor; }

  virtual float getValue2(void)         { return MWidgetValue2; }
  virtual void  setValue2(float AValue) { MWidgetValue2 = AValue; }

//------------------------------
public:
//------------------------------

  virtual void findHoverEdge(float AXpos) {
    KODE_FRect mrect = getRect();
    float v = (AXpos - mrect.x) / mrect.w;
    float d1 = v - getValue();
    float d2 = v - getValue2();
    int32_t d = 0;
    int32_t prev = MHoverEdge;
    MHoverEdge = -1;
    //const char* hint = "";
    if (fabs(d1) <= fabs(d2)) {
      d = d1 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "PgDn";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "left thumb";
      }
      else {
        MHoverEdge = 0;
        //hint = "left";
      }
    }
    else {
      d = d2 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "right thumb";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "PgUp";
      }
      else {
        MHoverEdge = 1;
        //hint = "right";
      }
    }

    if (MHoverEdge != prev) {
      //do_widget_setHint(this,hint);
      do_widget_redraw(this,mrect,0);
      if (MHoverEdge >= 0) {
        do_widget_setMouseCursor(this,KODE_CURSOR_ARROWLEFTRIGHT);
      }
      else {
        do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    KODE_FRect mrect = getRect();
    APainter->fillRectangle( mrect, MBackgroundColor );
    float x1 = mrect.x + (mrect.w * getValue());
    float x2 = mrect.x + (mrect.w * getValue2()) - 1;
    float w  = (x2 - x1 + 1);
    if (w > 0) {
      APainter->fillRectangle( KODE_FRect(x1,mrect.y,w,mrect.h), MBarColor );
    }
    if (MHoverEdge == 0) {
      APainter->fillRectangle( KODE_FRect(x1,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
    }
    else if (MHoverEdge == 1) {
      APainter->fillRectangle( KODE_FRect(x2-MEdgeWidth,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
    }
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
      if (MHoverEdge==0) MIsDragging1 = true;
      else if (MHoverEdge==1) MIsDragging2 = true;
      //MIsInteractive = true;
      //if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_HIDE);
      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {
      MIsDragging1 = false;
      MIsDragging2 = false;
      if (getRect().contains(AXpos,AYpos)) findHoverEdge(AXpos);
      else {
        MHoverEdge = -1;
        do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
      }
      //MIsInteractive = false;
      //if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_SHOW);
      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_FRect mrect = getRect();
    float v = (AXpos - mrect.x) / mrect.w;
    v = KODE_Clamp(v,0,1);
    float value1 = getValue();
    float value2 = getValue2();
    if (MIsDragging1) {
      value1 = v;
      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //KODE_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 1;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_redraw(this,mrect,0);
    }
    else if (MIsDragging2) {
      value2 = v;
      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //KODE_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 0;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_redraw(this,mrect,0);
    }
    else {
      findHoverEdge(AXpos);
    }
  }

  //----------

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) final {
    findHoverEdge(AXpos);
    if (flags.autoHint) do_widget_setHint(this,getHint());
  }

  //----------

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
    if (MHoverEdge >= 0) {
      MHoverEdge = -1;
      do_widget_redraw(this,getRect(),0);
      do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
    }
    if (flags.autoHint) do_widget_setHint(this,"");
  }

};

//----------------------------------------------------------------------
#endif
