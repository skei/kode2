
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

    add_squares( 0,   0,   KODE_COLOR_DARK_RED,     KODE_COLOR_RED,     KODE_COLOR_LIGHT_RED,     KODE_COLOR_BRIGHT_RED     );
    add_squares( 80,  0,   KODE_COLOR_DARK_ORANGE,  KODE_COLOR_ORANGE,  KODE_COLOR_LIGHT_ORANGE,  KODE_COLOR_BRIGHT_ORANGE  );
    add_squares( 160, 0,   KODE_COLOR_DARK_YELLOW,  KODE_COLOR_YELLOW,  KODE_COLOR_LIGHT_YELLOW,  KODE_COLOR_BRIGHT_YELLOW  );
    add_squares( 240, 0,   KODE_COLOR_DARK_LIME,    KODE_COLOR_LIME,    KODE_COLOR_LIGHT_LIME,    KODE_COLOR_BRIGHT_LIME    );

    add_squares( 0,   20,  KODE_COLOR_DARK_GREEN,   KODE_COLOR_GREEN,   KODE_COLOR_LIGHT_GREEN,   KODE_COLOR_BRIGHT_GREEN   );
    add_squares( 80,  20,  KODE_COLOR_DARK_GREEN2,  KODE_COLOR_GREEN2,  KODE_COLOR_LIGHT_GREEN2,  KODE_COLOR_BRIGHT_GREEN2  );
    add_squares( 160, 20,  KODE_COLOR_DARK_CYAN,    KODE_COLOR_CYAN,    KODE_COLOR_LIGHT_CYAN,    KODE_COLOR_BRIGHT_CYAN    );
    add_squares( 240, 20,  KODE_COLOR_DARK_CYAN2,   KODE_COLOR_CYAN2,   KODE_COLOR_LIGHT_CYAN2,   KODE_COLOR_BRIGHT_CYAN2   );

    add_squares( 0,   40,  KODE_COLOR_DARK_BLUE1,   KODE_COLOR_BLUE1,   KODE_COLOR_LIGHT_BLUE1,   KODE_COLOR_BRIGHT_BLUE1   );
    add_squares( 80,  40,  KODE_COLOR_DARK_BLUE,    KODE_COLOR_BLUE,    KODE_COLOR_LIGHT_BLUE,    KODE_COLOR_BRIGHT_BLUE    );
    add_squares( 160, 40,  KODE_COLOR_DARK_BLUE2,   KODE_COLOR_BLUE2,   KODE_COLOR_LIGHT_BLUE2,   KODE_COLOR_BRIGHT_BLUE2   );
    add_squares( 240, 40,  KODE_COLOR_DARK_MAGENTA, KODE_COLOR_MAGENTA, KODE_COLOR_LIGHT_MAGENTA, KODE_COLOR_BRIGHT_MAGENTA );

    add_squares( 0,   60, KODE_COLOR_DARK_RED2,    KODE_COLOR_RED2,    KODE_COLOR_LIGHT_RED2,    KODE_COLOR_BRIGHT_RED2    );
    add_squares( 80,  60, KODE_COLOR_DARK_GRAY,    KODE_COLOR_GRAY,    KODE_COLOR_LIGHT_GRAY,    KODE_COLOR_WHITE          );

    //black
    KODE_PanelWidget* black_panel = KODE_New KODE_PanelWidget(KODE_FRect( 160, 60, 15,15 ));
    black_panel->setBackgroundColor(KODE_COLOR_BLACK);
    black_panel->setCursor(KODE_CURSOR_FINGER);
    appendWidget(black_panel);

  }

  //----------

  virtual ~KODE_bottom_panel() {
  }

//------------------------------
public:
//------------------------------

  void add_squares(float x, float y, KODE_Color c1, KODE_Color c2, KODE_Color c3, KODE_Color c4) {
    KODE_PanelWidget* panel1 = KODE_New KODE_PanelWidget(KODE_FRect(  x,    y,  15, 15 ));
    KODE_PanelWidget* panel2 = KODE_New KODE_PanelWidget(KODE_FRect(  x+20, y,  15, 15 ));
    KODE_PanelWidget* panel3 = KODE_New KODE_PanelWidget(KODE_FRect(  x+40, y,  15, 15 ));
    KODE_PanelWidget* panel4 = KODE_New KODE_PanelWidget(KODE_FRect(  x+60, y,  15, 15 ));
    panel1->setBackgroundColor(c1);
    panel2->setBackgroundColor(c2);
    panel3->setBackgroundColor(c3);
    panel4->setBackgroundColor(c4);
    panel1->setCursor(KODE_CURSOR_FINGER);
    panel2->setCursor(KODE_CURSOR_MOVE);
    panel3->setCursor(KODE_CURSOR_PENCIL);
    panel4->setCursor(KODE_CURSOR_WAIT);
    appendWidget(panel1);
    appendWidget(panel2);
    appendWidget(panel3);
    appendWidget(panel4);
  }

};
