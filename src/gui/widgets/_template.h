#ifndef kode_template_included
#define kode_temlpate_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_Template
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  //KODE_Color  MColor        = KODE_Color( 0.0f, 0.0f, 0.0f );
  //KODE_Color  MHoverColor   = KODE_Color( 0.0f, 0.0f, 0.0f );
  //KODE_Color  MClickColor   = KODE_Color( 0.0f, 0.0f, 0.0f );
  //KODE_Color  MOnColor      = KODE_Color( 0.0f, 0.0f, 0.0f );

//------------------------------
public:
//------------------------------

  KODE_Template(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    //KODE_Print("x %.1f y %.1f w %.1f h %.1f\n",ARect.x,ARect.y,ARect.w,ARect.h);
  }

  virtual ~KODE_Template() {
    //KODE_Print("\n");
  };

//------------------------------
public: // widget
//------------------------------

  //void on_widget_setPos(float AXpos, float AYpos) final {
  //  //KODE_Print("x %.1f y %.1f\n",AXpos,AYpos);
  //}

  //void on_widget_setSize(float AWidth, float AHeight) final {
  //  //KODE_Print("w %.1f h %.1f\n",AWidth,AHeight);
  //}

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {
    //KODE_Print("x %.1f y %.1f w %.1f h %.1f (m %i)\n",ARect.x,ARect.y,ARect.w,ARect.h,AMode);
    /*
    KODE_Color color = MColor;
    if (MState.isInteracting) color = MClickColor;
    else if (MState.isHovering) color = MHoverColor;
    else if (MValue >= 0.5f) color = MOnColor;
    APainter->fillRect(MRect.x,MRect.y,MRect.x2(),MRect.y2(),color);
    */
  }

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
    //KODE_Print("x %.1f y %.1f b %i s %i\n",AXpos,AYpos,AButton,AState);
    /*
    MValue = 1.f;
    do_widget_update(this);
    MState.isInteracting = true;
    do_widget_redraw(this,MRect,0);
    */
  }

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
    //KODE_Print("x %.1f y %.1f b %i s %i\n",AXpos,AYpos,AButton,AState);
    /*
    MValue = 0.f;
    do_widget_update(this);
    MState.isInteracting = false;
    do_widget_redraw(this,MRect,0);
    */
  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState) final {
    //KODE_Print("x %.1f y %.1f s %i\n",AXpos,AYpos,AState);
  }

  void on_widget_keyPress(uint32_t AKey, uint32_t AChar, uint32_t AState) final {
    //KODE_Print("k %.1f c %.1f s %i\n",AKey,AChar,AState);
  }

  void on_widget_keyRelease(uint32_t AKey, uint32_t AChar, uint32_t AState) final {
    //KODE_Print("k %.1f c %.1f s %i\n",AKey,AChar,AState);
  }

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom) final {
    //KODE_Print("x %.1f y %.1f from %p\n",AXpos,AYpos,AFrom);
    /*
    MState.isHovering = true;
    do_widget_redraw(this,MRect,0);
    */
  }

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) final {
    //KODE_Print("x %.1f y %.1f to %p\n",AXpos,AYpos,ATo);
    /*
    MState.isHovering = false;
    do_widget_redraw(this,MRect,0);
    */
  }

  void on_widget_connect(KODE_Parameter* AParameter) final {
  }


//------------------------------
public: // "widget listener"
//------------------------------

  //void do_widget_update(KODE_Widget* ASender) final {
  //  //KODE_Print("w %p\n",ASender);
  //  KODE_Widget::do_widget_update(ASender);
  //}

  //void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode=0) final {
  //  //KODE_Print("w %p x %.1f y %.1f w %.1f h %.1f (m %i)\n",ASender,ARect.x,ARect.y,ARect.w,ARect.h,AMode);
  //  KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
  //}

};

//----------------------------------------------------------------------
#endif
