
class KODE_right_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_right_panel( KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {

    layout.alignment = KODE_WIDGET_ALIGN_FILL_RIGHT;
    layout.innerBorder = 10;
    layout.spacing = 10;

  }

  //----------

  virtual ~KODE_right_panel() {
  }

};
