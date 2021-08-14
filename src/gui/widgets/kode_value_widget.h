#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_text_widget.h"

#define KODE_VALUE_WIDGET_LABEL_SPACE 4.0f

//----------------------------------------------------------------------

class KODE_ValueWidget
: public KODE_TextWidget {

//------------------------------
private:
//------------------------------

  char        MValueText[32]      = {0};

  bool        MIsDragging         = false;
  float       MClickedXpos        = 0.0f;
  float       MClickedYpos        = 0.0f;
  float       MClickedValue       = 0.0f;
  float       MPrevXpos           = 0.0f;
  float       MPrevYpos           = 0.0f;
  uint32_t    MPrevClickTime      = 0;

//------------------------------
protected:
//------------------------------

  // value text
  bool        MDrawValueText      = true;
  KODE_Color  MValueTextColor     = KODE_Color(1.0f);
  uint32_t    MValueTextAlignment = KODE_TEXT_ALIGN_RIGHT;
  KODE_FRect  MValueTextOffset    = KODE_FRect(2);

  // label
  bool        MDrawLabel          = false;
  const char* MLabel              = "";
  KODE_Color  MLabelColor         = KODE_Color(0.8f);

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

  KODE_ValueWidget(KODE_FRect ARect)
  : KODE_TextWidget(ARect) {
  }

  virtual ~KODE_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValueText(bool ADraw=true) {
    MDrawValueText = ADraw;
  }

  virtual void setValueTextAlignment(uint32_t AAlignment) {
    MValueTextAlignment = AAlignment;
  }

  virtual void setValueTextColor(KODE_Color AColor) {
    MValueTextColor = AColor;
  }

  virtual void setDrawLabel(bool ADraw=true) {
    MDrawLabel = ADraw;
  }

  virtual void setLabel(const char* ALabel) {
    MLabel = ALabel;
  }

  virtual void setLabelColor(KODE_Color AColor) {
    MLabelColor = AColor;
  }

  //

  virtual void setCanDragValue(bool ADrag=true) {
    MCanDragValue = ADrag;
  }

  virtual void setDragSensitivity(float AValue) {
    MDragSensitivity = AValue;
  }

  virtual void setDragSensitivity2(float AValue) {
    MDragSensitivity2 = AValue;
  }

  virtual void setDragDirection(uint32_t ADirection) {
    MDragDirection = ADirection;
  }

  //----------

  virtual void drawValueText(KODE_BasePainter* APainter) {
    if (MDrawValueText) {
      KODE_FRect  value_rect  = MRect;
      KODE_FRect  label_rect  = MRect;
      float       value       = MValue;
      const char* label       = MLabel;
      if (MParameter) {
        value = MParameter->from01(value);
        label = MParameter->getLabel();
      }
      value_rect.shrink(MValueTextOffset);
      label_rect.shrink(MValueTextOffset);
      if (MDrawLabel) {
        float width = APainter->getTextWidth(label);
        label_rect.x = value_rect.x2() - width;
        label_rect.w = width;
        value_rect.w -= (width + KODE_VALUE_WIDGET_LABEL_SPACE);
      }
      KODE_FloatToString(MValueText,value);
      if (MDrawValueText) {
        APainter->drawText(value_rect,MValueText,MValueTextAlignment,MValueTextColor);
      }
      if (MDrawLabel) {
        APainter->drawText(label_rect,label,MValueTextAlignment,MLabelColor);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter);
    paintChildren(APainter,ARect,AMode);
    drawText(APainter);
    drawValueText(APainter);
    drawBorder(APainter);
  }

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    if (AButton == KODE_BUTTON_LEFT) {

      if (MDblClickReset) {
        if ((ATimeStamp - MPrevClickTime) < KODE_GUI_DBLCLICK_MS) {
          if (MParameter) MValue = MParameter->getDefValue();
          else MValue = 0.0f;
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
            MValue += (deltay * v);
            break;
          case KODE_DOWN:
            MValue -= (deltay * v);
            break;
          case KODE_LEFT:
            MValue -= (deltax * v);
            break;
          case KODE_RIGHT:
            MValue += (deltax * v);
            break;
        }
        MValue = KODE_Clamp(MValue,0.0f, 1.0f);
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

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
    do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
