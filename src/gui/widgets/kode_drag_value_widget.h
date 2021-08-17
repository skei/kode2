#ifndef kode_drag_value_widget_included
#define kode_drag_value_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_value_widget.h"

#define KODE_VALUE_WIDGET_LABEL_SPACE 4.0f

//----------------------------------------------------------------------

class KODE_DragValueWidget
: public KODE_ValueWidget {

//------------------------------
private:
//------------------------------

  bool        MIsDragging         = false;
  float       MClickedXpos        = 0.0f;
  float       MClickedYpos        = 0.0f;
  float       MClickedValue       = 0.0f;
  float       MPrevXpos           = 0.0f;
  float       MPrevYpos           = 0.0f;
  uint32_t    MPrevClickTime      = 0;

  bool        MSnap           = false;
  float       MSnapPos        = 0.5;
  float       MSnapDist       = 0.1;
  uint32_t    MSnapMode       = 1; // 0: always snap, 1: shift disables snapping

  bool        MQuantize       = false;
  uint32_t    MQuantizeSteps  = 0;
  uint32_t    MQuantizeMode   = 1;

//------------------------------
protected:
//------------------------------

  // drag
  bool        MCanDragValue       = true;
  float       MDragSensitivity    = 0.004f;
  float       MDragSensitivity2   = 0.05;
  uint32_t    MDragDirection      = KODE_UP;

  // double-click
  bool        MDblClickReset      = true;


//------------------------------
public:
//------------------------------

  KODE_DragValueWidget(KODE_FRect ARect)
  : KODE_ValueWidget(ARect) {
    MName = "KODE_DragValueWidget";
  }

  virtual ~KODE_DragValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void      setCanDragValue(bool ADrag=true)      { MCanDragValue = ADrag; }
  virtual void      setDragSensitivity(float AValue)      { MDragSensitivity = AValue; }
  virtual void      setDragSensitivity2(float AValue)     { MDragSensitivity2 = AValue; }
  virtual void      setDragDirection(uint32_t ADirection) { MDragDirection = ADirection; }

  virtual void      setSnap(bool ASnap=true)              { MSnap = ASnap; }
  virtual void      setSnapPos(float APos)                { MSnapPos = APos; }
  virtual void      setSnapDist(float ADist)              { MSnapDist = ADist; }
  virtual void      setSnapMode(uint32_t AMode)           { MSnapMode = AMode; }

  virtual void      setQuantize(bool AQuantize=true)      { MQuantize = AQuantize; }
  virtual void      setQuantizeSteps(uint32_t ASteps)     { MQuantizeSteps = ASteps; }
  virtual void      setQuantizeMode(uint32_t AMode)       { MQuantizeMode = AMode; }

  //----------

  virtual bool      getSnap()                             { return MSnap; }
  virtual float     getSnapPos()                          { return MSnapPos; }
  virtual float     getSnapDist()                         { return MSnapDist; }
  virtual uint32_t  getSnapMode()                         { return MSnapMode; }

  virtual bool      getQuantize()                         { return MQuantize; }
  virtual uint32_t  getQuantizeSteps()                    { return MQuantizeSteps; }
  virtual uint32_t  getQuantizeMode()                     { return MQuantizeMode; }


//------------------------------
protected:
//------------------------------

  virtual float snapValue(float AValue) {
    float v = AValue;
    float s = 1.0f;
    float dist = fabs( AValue - MSnapPos );
    if (dist < MSnapDist) {
      v = MSnapPos;
    }
    else {
      // scale left
      if (AValue < MSnapPos) {
        float sp_sd = MSnapPos - MSnapDist;
        if (sp_sd > 0) s = MSnapPos / sp_sd;
        v = AValue * s;
      }
      // scale right
      else if (AValue > MSnapPos) {
        float iv = 1.0f - AValue;
        float isp = (1.0f - MSnapPos);
        float isp_sd = isp - MSnapDist;
        if (isp_sd > 0) s = isp / isp_sd;
        v = iv * s;
        v = 1.0f - v;
      }
    }
    v = KODE_Clamp(v,0.0f,1.0f);
    return v;
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {

      if (MDblClickReset) {
        if ((ATimeStamp - MPrevClickTime) < KODE_GUI_DBLCLICK_MS) {
          if (MParameter) setValue( MParameter->getDefValue() );
          else setValue( getDefaultValue() );
          update();
          redraw();
        }
      }
      MPrevClickTime = ATimeStamp;

      if (MCanDragValue) {
        do_widget_setMouseCursor(this,KODE_CURSOR_GRAB);
        do_widget_setMouseCursor(this,KODE_CURSOR_HIDE);
        MPrevXpos = AXpos;
        MPrevYpos = AYpos;
        MIsDragging = true;
      }

    }
  }

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (MCanDragValue) {
      if (AButton == KODE_BUTTON_LEFT) {
        do_widget_setMouseCursor(this,KODE_CURSOR_SHOW);
        do_widget_setMouseCursor(this,KODE_CURSOR_RELEASE);
        MIsDragging = false;
      }
    }
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (MCanDragValue) {
      if (MIsDragging) {
        float deltax = AXpos - MPrevXpos; // right is increasing
        float deltay = MPrevYpos - AYpos; // up is increasing
        float v = MDragSensitivity;
        if (AState & KODE_KEY_SHIFT) v *= MDragSensitivity2;
        switch (MDragDirection) {
          case KODE_UP:
            setValue(getValue() + (deltay * v));
            break;
          case KODE_DOWN:
            setValue(getValue() - (deltay * v));
            break;
          case KODE_LEFT:
            setValue(getValue() - (deltax * v));
            break;
          case KODE_RIGHT:
            setValue(getValue() + (deltax * v));
            break;
        }
        setValue(KODE_Clamp(getValue(),0.0f, 1.0f));
        update();
        redraw();
      }
      MPrevXpos = AXpos;
      MPrevYpos = AYpos;
    }
  }

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) final {
    if (MCanDragValue) {
      switch (MDragDirection) {
        case KODE_LEFT:
        case KODE_RIGHT:
          do_widget_setMouseCursor(this,KODE_CURSOR_ARROWLEFTRIGHT);
          break;
        case KODE_UP:
        case KODE_DOWN:
          do_widget_setMouseCursor(this,KODE_CURSOR_ARROWUPDOWN);
          break;
      }
    }
  }

  //void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
  //  do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
  //}

//------------------------------

};

//----------------------------------------------------------------------
#endif
