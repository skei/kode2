#if 0

#ifndef kode_plot_widget_included
#define kode_plot_widget_included
//----------------------------------------------------------------------

#define MAX_WIDTH 4096

class KODE_PlotWidget
: public KODE_Widget {

  private:

    KODE_Color  MBackColor;
    KODE_Color  MLineColor;
    bool        MDrawBackground;
    float       MValues[MAX_WIDTH];
    bool        MBipolar;

  public:

    KODE_PlotWidget(KODE_Rect ARect)
    : KODE_Widget(ARect) {
      MName           = "KODE_PlotWidget";
      MHint           = "plot";
      MBackColor      = KODE_DarkGrey;
      MLineColor      = KODE_LightGrey;
      MDrawBackground = true;
      MBipolar        = true;
      memset(MValues,0,sizeof(MValues));
    }

    //----------

    virtual ~KODE_PlotWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    int32_t width(void)   { return MRect.w; }
    int32_t height(void)  { return MRect.h; }

    void value(int32_t AIndex, float AValue) {
      MValues[AIndex] = AValue;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {

      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRectangle(MRect);
      }

      float h2 = (float)MRect.h * 0.5f;
      APainter->setDrawColor( MLineColor );
      int32_t yprev = MRect.y2() - 1;
      if (MBipolar) yprev -= h2;
      int32_t x = MRect.x;
      for (int32_t i=0; i<MRect.w; i++) {
        float v = MValues[i];
        if (MBipolar) v = (v+1) * 0.5;        // -1..1 -> 0..1
        //int32 y = MRect.y2() - (v*h2) - 1;
        int32_t y = MRect.y2() - (v*MRect.h) - 1;
        APainter->drawLine(x,yprev,x+1,y);
        x++;
        yprev = y;
      }

      KODE_Widget::on_paint(APainter,ARect);
    }

    //----------

};

#undef MAX_WIDTH

//----------------------------------------------------------------------
#endif

#endif // 0
