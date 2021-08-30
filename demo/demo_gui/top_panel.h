


class KODE_top_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_top_panel( KODE_FRect ARect, KODE_Drawable* ATarget)
  : KODE_PanelWidget(ARect) {

    layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = 10;
    layout.spacing = 10;

  }

  //----------

  virtual ~KODE_top_panel() {
  }

//------------------------------
public:
//------------------------------

};

