#ifndef kode_template_widget_includedbutton
#define kode_template_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_CircularWaveformWidget
: public KODE_Widget {

//------------------------------
public:
//------------------------------

  KODE_CircularWaveformWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_CircularWaveformWidget");
    setHint("circularwaveform");
  }

  virtual ~KODE_CircularWaveformWidget() {
  }

//------------------------------
public:
//------------------------------

//  void on_widget_setPos(float AXpos, float AYpos) override {
//  }

//  void on_widget_setSize(float AWidth, float AHeight) override {
//  }

//  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
//  }

//  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
//    //if (flags.autoHint) do_widget_setHint(this,MHint);
//  }

//  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
//    //if (flags.autoHint) do_widget_setHint(this,"");
//  }

//  void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) override {
//  }

//------------------------------
public:
//------------------------------

//  void do_widget_update(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_update(ASender);
//  }

//  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) final {
//    KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
//  }

//  void do_widget_moved(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) final {
//    KODE_Widget::do_widget_moved(ASender,ADeltaX,ADeltaY);
//  }

//  void do_widget_resized(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_resized(ASender);
//  }

//  void do_widget_grabMouseCursor(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabMouseCursor(ASender);
//  }

//  void do_widget_grabKeyboard(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabKeyboard(ASender);
//  }

//  void do_widget_grabModal(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabModal(ASender);
//  }

//  void do_widget_setMouseCursor(KODE_Widget* ASender, int32_t ACursor) final {
//    KODE_Widget::do_widget_setMouseCursor(ASender,ACursor);
//  }

//  void do_widget_setMouseCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) final {
//    KODE_Widget::do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
//  }

//  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) final {
//    KODE_Widget::do_widget_setHint(ASender,AHint);
//  }

//  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) final {
//    KODE_Widget::do_widget_notify(AWidget,AValue);
//  }

};

//----------------------------------------------------------------------
#endif

#if 0

#ifndef kode_widget_circular_waveform_widget_included
#define kode_widget_circular_waveform_widget_included
//----------------------------------------------------------------------

//#include "base/kode_integer.h"
#include "gui/kode_widget.h"

#define NUMSTEPS 1024


//----------

class KODE_CircularWaveformWidget
: public KODE_Widget {

  private:

    float*      MBuffer;
    int32_t     MBufferSize;
    //float       MRadius;
    float       MOuterRadius;
    float       MInnerRadius;
    bool        MBipolar;
    KODE_Color  MLineColor;

  public:

    KODE_CircularWaveformWidget(KODE_Rect ARect)
    : KODE_Widget(ARect) {
      MName         = "KODE_CircularWaveformWidget";
      MHint         = "circular aveform";
      MBuffer       = nullptr;//(float*)SMalloc(NUMSTEPS*sizeof(float));
      MBufferSize   = 0;//1024;
      //MRadius       = 100.0f;
      MOuterRadius  = 1.0f;
      MInnerRadius  = 0.6f;
      MLineColor    = KODE_LightGrey;
      MBipolar      = false;//true;
    }

    virtual ~KODE_CircularWaveformWidget() {
      //if (MBuffer) SFree(MBuffer);
    }

  public:

    float*  buffer(void)                        { return MBuffer; }
    float   buffer(int32_t AIndex)                { return MBuffer[AIndex]; }
    void    buffer(int32_t AIndex, float AValue)  { MBuffer[AIndex] = AValue; }
    void    buffer(float* ABuffer)              { MBuffer = ABuffer; }
    int32_t   buffersize(void)                    { return MBufferSize; }
    void    buffersize(int32_t ASize)             { MBufferSize = ASize; }
    //void    radius(float ARadius)               { MRadius = ARadius; }
    void    outerRadius(float ARadius)          { MOuterRadius = ARadius; }
    void    innerRadius(float ARadius)          { MInnerRadius = ARadius; }
    void    bipolar(bool ABipolar=true)         { MBipolar = ABipolar; }

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      int32_t xcenter = MRect.x + (MRect.w / 2);
      int32_t ycenter = MRect.y + (MRect.h / 2);

      float radius = KODE_Min(MRect.w,MRect.h);
      float r_range  = ( MOuterRadius - MInnerRadius );
      //float r_center = MInnerRadius + ( r_range / 2.0f );

      float angle = 0;
      float angleadd = 1.0f / NUMSTEPS;

      //float prevx = cosf(angle*KODE_PI2) * r_center;
      //float prevy = sinf(angle*KODE_PI2) * r_center;

      float vv;//  = 0.5f;
      float xx;//  = 1.0f;//cosf(0.0f);
      float yy;//  = 0.0f;//sinf(0.0f);
      float v   = MInnerRadius + (0.5f * r_range);
      float prevx = v * radius * 0.5f;
      float prevy = 0;

      APainter->setDrawColor(MLineColor);

      if (MBipolar) {
        for (int32_t i=0; i<NUMSTEPS; i++) {
          vv = (MBuffer[i] + 1.0f) * 0.5f;      // 0..1
          v  = MInnerRadius + (vv * r_range);   // 0.5 + (vv*0.5) = 0.5+0..0.5+0.5 = 0.5..1
          xx = cosf(angle*KODE_PI2);
          yy = sinf(angle*KODE_PI2);
          float x = xx * v * radius * 0.5;
          float y = yy * v * radius * 0.5;
          APainter->drawLine(xcenter+prevx,ycenter+prevy,xcenter+x,ycenter+y);
          prevx = x;
          prevy = y;
          angle += angleadd;
        }
      }
      else {
        for (int32_t i=0; i<NUMSTEPS; i++) {
          v  = MInnerRadius;
          xx = cosf(angle*KODE_PI2);
          yy = sinf(angle*KODE_PI2);
          float x1 = xx * v * radius * 0.5;
          float y1 = yy * v * radius * 0.5;
          vv = MBuffer[i];      // 0..1
          v  = MInnerRadius + (vv * r_range);   // 0.5 + (vv*0.5) = 0.5+0..0.5+0.5 = 0.5..1
          xx = cosf(angle*KODE_PI2);
          yy = sinf(angle*KODE_PI2);
          float x2 = xx * v * radius * 0.5;
          float y2 = yy * v * radius * 0.5;

          APainter->drawLine(xcenter+x1,ycenter+y1,xcenter+x2,ycenter+y2);

          //APainter->drawPoint(xcenter+x1,ycenter+y1);


          //prevx = x;
          //prevy = y;
          angle += angleadd;
        }
      }
    }

};


#undef NUMSTEPS

//----------------------------------------------------------------------
#endif

#endif // 0
