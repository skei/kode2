#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

class KODE_SliderWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  KODE_FRect  MInnerBorder          = KODE_FRect(5,0,5,0);

  bool        MFillBackground       = true;
  bool        MDrawNameText         = true;
  bool        MDrawValueText        = true;
  bool        MDrawLabelText        = true;
  bool        MDrawValueBar         = true;
  bool        MDrawBorder           = true;

  uint32_t    MNameTextColor        = 0xffa0a0a0;
  uint32_t    MValueTextColor       = 0xffffffff;
  uint32_t    MLabelTextColor       = 0xffa0a0a0;

  uint32_t    MBackgroundColor      = 0xff505050;
  uint32_t    MValueBarColor        = 0xff404040;
  uint32_t    MBorderColor          = 0xff909090;

  uint32_t    MHoverBackgroundColor = 0xff484848;
  uint32_t    MHoverValueBarColor   = 0xff383838;
  uint32_t    MHoverBorderColor     = 0xffb0b0b0;

  uint32_t    MInterBackgroundColor = 0xff483838;
  uint32_t    MInterValueBarColor   = 0xff382828;
  uint32_t    MInterBorderColor     = 0xffb0a0a0;

  float MDragSensitivity            = 0.005f;
  float MPrevMouseX                 = 0.0f;
  float MPrevMouseY                 = 0.0f;

  //float MClickedX                   = 0.0f;
  //float MClickedY                   = 0.0f;
  //float MClickedValue               = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_SliderWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    //KODE_Print("x %.1f y %.1f w %.1f h %.1f\n",ARect.x,ARect.y,ARect.w,ARect.h);
  }

  //----------

  virtual ~KODE_SliderWidget() {
    //KODE_Print("\n");
  };

//------------------------------
public:
//------------------------------

  //----------

//------------------------------
public: // widget
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {

    KODE_FRect rect = MRect;
    rect.shrink(MInnerBorder);

    const char* name  = getName();
    float       value = getValue();
    const char* label = "";

    KODE_Parameter* parameter = (KODE_Parameter*)getParameter();
    if (parameter) {
      name  = parameter->getName();
      //value = parameter->from01( value );
      label = parameter->getLabel();
    }

    if (MFillBackground) {
      uint32_t color = MBackgroundColor;
      if (MState.isHovering)  color = MHoverBackgroundColor;
      else if (MState.isInteracting) color = MInterBackgroundColor;
      APainter->fillRect(MRect,color);
    }

    if (MDrawValueBar) {
      uint32_t color = MValueBarColor;
      if (MState.isHovering)  color = MHoverValueBarColor;
      else if (MState.isInteracting) color = MInterValueBarColor;
      KODE_FRect r = MRect;
      r.w *= value;
      APainter->fillRect(r,color);

    }

    if (MDrawNameText) {
      APainter->drawText(rect,name,MNameTextColor,KODE_TEXT_ALIGN_LEFT);
    }

    if (MDrawLabelText & (label[0] != 0)) {
      float labelwidth = APainter->getTextWidth("_");
      labelwidth += APainter->getTextWidth(label);
      APainter->drawText(rect,label,MLabelTextColor,KODE_TEXT_ALIGN_RIGHT);
      rect.w -= labelwidth;
    }

    if (MDrawValueText) {
      char buffer[64];
      if (parameter) {
        parameter->getDisplayText(value,buffer);
      }
      else {
        KODE_FloatToString(buffer,value);
      }
      APainter->drawText(rect,buffer,MValueTextColor,KODE_TEXT_ALIGN_RIGHT);
    }

    if (MDrawBorder) {
      uint32_t color = MBorderColor;
      if (MState.isHovering)  color = MHoverBorderColor;
      else if (MState.isInteracting) color = MInterBorderColor;
      //APainter->setLineWidth(0.5f);
      APainter->drawRect(MRect,color);
    }

  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
    MState.isHovering = false;
    MState.isInteracting = true;
    MPrevMouseY = AYpos;
    MPrevMouseX = AXpos;
    MPrevMouseY = AYpos;
    do_widget_redraw(this,MRect,0);
    //MClickedX = AXpos;
    //MClickedY = AYpos;
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
    MState.isInteracting = false;
    if (MRect.contains(AXpos,AYpos)) MState.isHovering = true;
    do_widget_redraw(this,MRect,0);
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState) final {
    float diff = MPrevMouseY - AYpos;
    diff *= MDragSensitivity;
    //KODE_Print("%.3f\n",diff);
    float value = KODE_Clamp((MValue + diff),0.0f,1.0f);
    //if (KODE_AlmostEqual(value,MValue)
    if (value != MValue) {
      MValue = value;
      do_widget_update(this);
      do_widget_redraw(this,MRect,0);
    }
    MPrevMouseX = AXpos;
    MPrevMouseY = AYpos;
  }

  //----------

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom) final {
    MState.isHovering = true;
    do_widget_redraw(this,MRect,0);
  }

  //----------

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) final {
    MState.isHovering = false;
    do_widget_redraw(this,MRect,0);
  }

//------------------------------
public: // "widget listener"
//------------------------------

  //void do_widget_update(KODE_Widget* ASender) final {
  //  //KODE_Print("w %p\n",ASender);
  //  KODE_Widget::do_widget_update(ASender);
  //}

  //----------

  //void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode=0) final {
  //  //KODE_Print("w %p x %.1f y %.1f w %.1f h %.1f (m %i)\n",ASender,ARect.x,ARect.y,ARect.w,ARect.h,AMode);
  //  KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
  //}

};

//----------------------------------------------------------------------
#endif
