#if 0

#ifndef kode_symbol_widget_included
#define kode_symbol_widget_included
//----------------------------------------------------------------------

//#include "gui/widgets/kode_widget_button.h"
//#include "gui/widgets/kode_widget_menu.h"
//#include "gui/widgets/kode_widget_menuitem.h"

//----------

class KODE_SymbolWidget
: public KODE_Widget {

  private:

    int32_t     MSymbol = KODE_SYMBOL_FILL_TRI_DOWN;
    KODE_Color  MColor  = KODE_LightGrey;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_SymbolWidget(KODE_Rect ARect, uint32_t ASymbol=KODE_SYMBOL_FILL_TRI_DOWN, KODE_Color AColor=KODE_LightGrey)
    : KODE_Widget(ARect) {
      MName   = "KODE_Widget_Symbol";
      MHint   = "symbol";
      MSymbol = ASymbol;
      MColor  = AColor;
    }

    //----------

    virtual ~KODE_SymbolWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void setSymbol(uint32_t ASymbol) { MSymbol = ASymbol; }
    void setColor(KODE_Color AColor) { MColor = AColor; }

  //------------------------------
  //
  //------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      switch(MSymbol) {
        case KODE_SYMBOL_NONE:
          break;
        case KODE_SYMBOL_DRAW_RECT:
          APainter->setDrawColor(MColor);
          APainter->drawRectangle(MRect);
          break;
        case KODE_SYMBOL_FILL_RECT:
          APainter->setFillColor(MColor);
          APainter->fillRectangle(MRect);
          break;
        case KODE_SYMBOL_DRAW_TRI_DOWN:
          APainter->setDrawColor(MColor);
          APainter->drawTriangle( MRect.x, MRect.y, MRect.x2(), MRect.y, MRect.x + (MRect.w/2), MRect.y2() );
          break;
        case KODE_SYMBOL_FILL_TRI_DOWN:
          APainter->setFillColor(MColor);
          APainter->fillTriangle( MRect.x, MRect.y, MRect.x2(), MRect.y, MRect.x + (MRect.w/2), MRect.y2() );
          break;
        case KODE_SYMBOL_DRAW_ELLIPSE:
          APainter->setDrawColor(MColor);
          APainter->drawEllipse(MRect);
          break;
        case KODE_SYMBOL_FILL_ELLIPSE:
          APainter->setFillColor(MColor);
          APainter->fillEllipse(MRect);
          break;
      }
    }

    //----------

};

//----------------------------------------------------------------------
#endif

#endif // 0
