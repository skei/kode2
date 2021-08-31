#ifndef kode_template_widget_includedbutton
#define kode_template_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

class KODE_CircularWaveformWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

    float*      MBuffer       = KODE_NULL;
    int32_t     MBufferSize   = 0;
    //float       MRadius       = 100.0f;
    float       MOuterRadius  = 1.0f;
    float       MInnerRadius  = 0.6f;
    bool        MBipolar      = false;
    KODE_Color  MLineColor    = KODE_COLOR_LIGHT_GRAY;

//------------------------------
public:
//------------------------------

  KODE_CircularWaveformWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_CircularWaveformWidget");
    setHint("circularwaveform");
  }

  //----------

  virtual ~KODE_CircularWaveformWidget() {
    //if (MBuffer) SFree(MBuffer);
  }

//------------------------------
public:
//------------------------------

  public:

    float*    getBuffer(void)                         { return MBuffer; }
    float     getBuffer(int32_t AIndex)               { return MBuffer[AIndex]; }
    int32_t   getBufferSize(void)                     { return MBufferSize; }

    void      setBuffer(int32_t AIndex, float AValue) { MBuffer[AIndex] = AValue; }
    void      setBuffer(float* ABuffer)               { MBuffer = ABuffer; }
    void      setBuffersize(int32_t ASize)            { MBufferSize = ASize; }
    //void      setRadius(float ARadius)              { MRadius = ARadius; }
    void      setOuterRadius(float ARadius)           { MOuterRadius = ARadius; }
    void      setInnerRadius(float ARadius)           { MInnerRadius = ARadius; }
    void      setBipolar(bool ABipolar=true)          { MBipolar = ABipolar; }

//------------------------------
public:
//------------------------------

#define NUMSTEPS 1024

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_FRect mrect =getRect();
    int32_t xcenter = mrect.x + (mrect.w / 2);
    int32_t ycenter = mrect.y + (mrect.h / 2);
    float radius = KODE_Min(mrect.w,mrect.h);
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
    //APainter->setDrawColor(MLineColor);
    if (MBipolar) {
      for (int32_t i=0; i<NUMSTEPS; i++) {
        vv = (MBuffer[i] + 1.0f) * 0.5f;      // 0..1
        v  = MInnerRadius + (vv * r_range);   // 0.5 + (vv*0.5) = 0.5+0..0.5+0.5 = 0.5..1
        xx = cosf(angle*KODE_PI2);
        yy = sinf(angle*KODE_PI2);
        float x = xx * v * radius * 0.5;
        float y = yy * v * radius * 0.5;
        APainter->drawLine(xcenter+prevx,ycenter+prevy,xcenter+x,ycenter+y,MLineColor);
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
        APainter->drawLine(xcenter+x1,ycenter+y1,xcenter+x2,ycenter+y2,MLineColor);
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
