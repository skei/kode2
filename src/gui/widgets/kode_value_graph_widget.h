#if 0

#ifndef kode_value_graph_widget_included
#define kode_value_graph_widget_included
//----------------------------------------------------------------------

#include "base/kode_random.h"
#include "gui/kode_widget.h"

#define KODE_NUM_WIDGETNODES 1024

//----------

struct KODE_ValueGraphNode {
  float     value;
  uint32_t  flags;
};

//----------

class KODE_ValueGraphWidget
: public KODE_Widget {

  private:
    KODE_Color      MBackColor;
    KODE_Color      MLineColor;
    KODE_Color      MNodeColor;
    int32_t         MNumNodes;
    KODE_ValueGraphNode MNodes[KODE_NUM_WIDGETNODES];
    int32_t         MSelected;

  public:

    KODE_ValueGraphWidget(KODE_Rect ARect, int32_t ANum=0)
    : KODE_Widget(ARect) {
      MName = "KODE_ValueGraphWidget";
      MHint = "nodes";
      MBackColor = KODE_DarkGrey;
      MLineColor = KODE_LightGrey;
      MNodeColor = KODE_BrightYellow;
      MNumNodes = ANum;
      memset(MNodes,0,sizeof(MNodes));
      MSelected = -1;

      setFlag(KODE_WIDGET_HOVER);

      //setFlag(kwf_clip);
      for (int32_t i=0; i<KODE_NUM_WIDGETNODES; i++) {
        MNodes[i].value = KODE_RandomSigned();
        MNodes[i].flags = 0;
      }
    }

    virtual ~KODE_ValueGraphWidget() {
    }

  public:

    void numNodes(int32_t ANum) {
      MNumNodes = ANum;
    }

    void setNode(int32_t AIndex, float AValue, uint32_t AFlags) {
      MNodes[AIndex].value = AValue;
      MNodes[AIndex].flags = AFlags;
    }

    void appendNode(float AValue, uint32_t AFlags) {
      MNodes[MNumNodes].value = AValue;
      MNodes[MNumNodes].flags = AFlags;
      MNumNodes++;
    }

    int32_t findNode(int32_t AXpos, int32_t AYpos) {
      float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      w *= (MNumNodes-1);
      int32_t index = roundf(w);// SMin( MNumNodes-1, roundf(w) );
      //STrace("w %f index %i\n",w,index);
      return index;//-1;
    }

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect/*, uint32 AMode=0*/) override {
      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect);
      //APainter->setFillColor(MNodeColor);
      if (MNumNodes>0) {
        //int32 halfw = (MRect.w/2);
        int32_t halfh = (MRect.h/2);
        if (MNumNodes==1) {
          //STrace("NumNodes: %i\n",MNumNodes);
          int32_t h = halfh - ( (MNodes[0].value * halfh ) );
          APainter->setDrawColor(MLineColor);
          APainter->drawLine(MRect.x,MRect.y+h,MRect.x2(),MRect.y+h);
        }
        else if (MNumNodes==2) {
          int32_t h1 = halfh - ( (MNodes[0].value * halfh ) );
          int32_t h2 = halfh - ( (MNodes[1].value * halfh ) );
          APainter->setDrawColor(MLineColor);
          APainter->drawLine(MRect.x,MRect.y+h1,MRect.x2(),MRect.y+h2);
        }
        else {
          float w = (float)MRect.w / (float)(MNumNodes-1);
          float x = MRect.x;
          for (int32_t i=0; i<MNumNodes; i++) {
            int32_t h1,h2;
            h1 = halfh - ( (MNodes[i].value * halfh ) );
            if (i<MNumNodes-1) {
              h2 = halfh - ( (MNodes[i+1].value * halfh ) );
              APainter->setDrawColor(MLineColor);
              APainter->drawLine(x,MRect.y+h1,x+w,MRect.y+h2);
            }

            if (i==MSelected) {
              //APainter->drawRectangle( x+w-2, MRect.y+h2-2, x+w+2, MRect.y+h2+2 );
              APainter->setFillColor(MNodeColor);
              APainter->fillRectangle( x-2, MRect.y+h1-2, x+2, MRect.y+h1+2 );
            }

            x += w;
          }
        }
      }
    }

    //----------

    void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);

      //float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      //int32 index = (w * (float)MNumNodes);

      int32_t index = findNode(AXpos,AYpos);
      if (index != MSelected) {
        MSelected = index;
        //STrace("MSelected: %i\n",MSelected);
        do_redraw(this,MRect);
      }
    }

    //----------

    //virtual
    void on_leave(float AXpos, float AYpos, KODE_Widget* AWidget=nullptr) override {
      //SWidget::on_leave(AWidget);
      if (MSelected>=0) {
        MSelected = -1;
        do_redraw(this,MRect);
      }
    }


    //virtual
    //void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
    //}

    //virtual
    //void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_keyDown(int32 AKey, int32 AState) {
    //}

    //virtual
    //void on_keyUp(int32 AKey, int32 AState) {
    //}

    //virtual
    //void on_enter(SWidget* AWidget) {
    //}

    //virtual
    //void on_leave(SWidget* AWidget) {
    //}

};

#undef KODE_NUM_WIDGETNODES

//----------------------------------------------------------------------
#endif

#endif // 0
