
class KODE_bottom_panel
: public KODE_PanelWidget {

//------------------------------
public:
//------------------------------

  KODE_bottom_panel( KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {

    layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    layout.innerBorder = 10;
    layout.spacing = 10;


    // colors

    add_square(KODE_COLOR_DARK_RED);
    add_square(KODE_COLOR_DARK_ORANGE);
    add_square(KODE_COLOR_DARK_YELLOW);
    add_square(KODE_COLOR_DARK_LIME);
    add_square(KODE_COLOR_DARK_GREEN);
    add_square(KODE_COLOR_DARK_GREEN2);
    add_square(KODE_COLOR_DARK_CYAN);
    add_square(KODE_COLOR_DARK_CYAN2);
    add_square(KODE_COLOR_DARK_BLUE1);
    add_square(KODE_COLOR_DARK_BLUE);
    add_square(KODE_COLOR_DARK_BLUE2);
    add_square(KODE_COLOR_DARK_MAGENTA);
    add_square(KODE_COLOR_DARK_RED2);

    add_square(KODE_COLOR_RED);
    add_square(KODE_COLOR_ORANGE);
    add_square(KODE_COLOR_YELLOW);
    add_square(KODE_COLOR_LIME);
    add_square(KODE_COLOR_GREEN);
    add_square(KODE_COLOR_GREEN2);
    add_square(KODE_COLOR_CYAN);
    add_square(KODE_COLOR_CYAN2);
    add_square(KODE_COLOR_BLUE1);
    add_square(KODE_COLOR_BLUE);
    add_square(KODE_COLOR_BLUE2);
    add_square(KODE_COLOR_MAGENTA);
    add_square(KODE_COLOR_RED2);

    add_square(KODE_COLOR_LIGHT_RED);
    add_square(KODE_COLOR_LIGHT_ORANGE);
    add_square(KODE_COLOR_LIGHT_YELLOW);
    add_square(KODE_COLOR_LIGHT_LIME);
    add_square(KODE_COLOR_LIGHT_GREEN);
    add_square(KODE_COLOR_LIGHT_GREEN2);
    add_square(KODE_COLOR_LIGHT_CYAN);
    add_square(KODE_COLOR_LIGHT_CYAN2);
    add_square(KODE_COLOR_LIGHT_BLUE1);
    add_square(KODE_COLOR_LIGHT_BLUE);
    add_square(KODE_COLOR_LIGHT_BLUE2);
    add_square(KODE_COLOR_LIGHT_MAGENTA);
    add_square(KODE_COLOR_LIGHT_RED2);

    add_square(KODE_COLOR_BRIGHT_RED);
    add_square(KODE_COLOR_BRIGHT_ORANGE);
    add_square(KODE_COLOR_BRIGHT_YELLOW);
    add_square(KODE_COLOR_BRIGHT_LIME);
    add_square(KODE_COLOR_BRIGHT_GREEN);
    add_square(KODE_COLOR_BRIGHT_GREEN2);
    add_square(KODE_COLOR_BRIGHT_CYAN);
    add_square(KODE_COLOR_BRIGHT_CYAN2);
    add_square(KODE_COLOR_BRIGHT_BLUE1);
    add_square(KODE_COLOR_BRIGHT_BLUE);
    add_square(KODE_COLOR_BRIGHT_BLUE2);
    add_square(KODE_COLOR_BRIGHT_MAGENTA);
    add_square(KODE_COLOR_BRIGHT_RED2);

    add_square(KODE_COLOR_BLACK);
    add_square(KODE_COLOR_DARK_GRAY);
    add_square(KODE_COLOR_GRAY);
    add_square(KODE_COLOR_LIGHT_GRAY);
    add_square(KODE_COLOR_WHITE);

  }

  //----------

  virtual ~KODE_bottom_panel() {
  }

//------------------------------
public:
//------------------------------

  void add_square(KODE_Color AColor) {
    KODE_PanelWidget* panel = new KODE_PanelWidget(KODE_FRect(15));
    panel->layout.alignment = KODE_WIDGET_STACK_HORIZ;
    panel->setFillBackground();
    panel->setBackgroundColor(AColor);
    appendWidget(panel);
  }

//  void add_squares(float x, float y, KODE_Color c1, KODE_Color c2, KODE_Color c3, KODE_Color c4) {
//    KODE_PanelWidget* panel1 = new KODE_PanelWidget(KODE_FRect(  x,    y,  15, 15 ));
//    KODE_PanelWidget* panel2 = new KODE_PanelWidget(KODE_FRect(  x+20, y,  15, 15 ));
//    KODE_PanelWidget* panel3 = new KODE_PanelWidget(KODE_FRect(  x+40, y,  15, 15 ));
//    KODE_PanelWidget* panel4 = new KODE_PanelWidget(KODE_FRect(  x+60, y,  15, 15 ));
//    panel1->setBackgroundColor(c1);
//    panel2->setBackgroundColor(c2);
//    panel3->setBackgroundColor(c3);
//    panel4->setBackgroundColor(c4);
//    panel1->setCursor(KODE_CURSOR_FINGER);
//    panel2->setCursor(KODE_CURSOR_MOVE);
//    panel3->setCursor(KODE_CURSOR_PENCIL);
//    panel4->setCursor(KODE_CURSOR_WAIT);
//    appendWidget(panel1);
//    appendWidget(panel2);
//    appendWidget(panel3);
//    appendWidget(panel4);
//  }

};
