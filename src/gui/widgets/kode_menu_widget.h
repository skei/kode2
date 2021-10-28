#ifndef kode_menu_widget_included
#define kode_menu_widget_included
//----------------------------------------------------------------------

/*
  TODO:
  - take menuitem's text x/y offset into account when calulating
    menu size?
*/

#include "gui/widgets/kode_menu_item_widget.h"
#include "gui/widgets/kode_panel_widget.h"

//typedef vector<char*> KODE_MenuItems;
typedef KODE_Array<char*> KODE_MenuItems;

//----------------------------------------------------------------------

class KODE_MenuListener {
  public:
    virtual void on_menuEvent(int32_t AIndex) {}
};

//----------------------------------------------------------------------

class KODE_MenuWidget
: public KODE_PanelWidget
, public KODE_MenuListener {

//------------------------------
protected:
//------------------------------

  KODE_MenuListener*  MListener     = KODE_NULL;
  KODE_MenuItems      MItems;
  uint32_t            MBorderSize   = 5; //0; //1;
  float               MMenuWidth    = 96;
  float               MMenuHeight   = 16;
  uint32_t            MItemWidth    = 96;
  uint32_t            MItemHeight   = 16;
  int32_t             MSelectedItem = -1;
  int32_t             MPrevSelected = -1;
  uint32_t            MItemsX       = 1;
  uint32_t            MItemsY       = 1;
  bool                MMirrorX      = false;
  bool                MMirrorY      = false;

//------------------------------
public:
//------------------------------

  KODE_MenuWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {

    setName("KODE_MenuWidget");
    setHint("menu");
//    MMenuWidth  = ARect.w;
//    MMenuHeight = ARect.h;
    flags.active = false;
    flags.visible = false;
    setFillBackground();
    setBackgroundColor(KODE_COLOR_LIGHT_GRAY);
    setDrawBorder();
  }

  virtual ~KODE_MenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t getSelectedItem() {
    return MSelectedItem;
  }

  virtual uint32_t getNumItems() {
    return MItems.size();
  }

  virtual const char* getItem(uint32_t AIndex) {
    return MItems[AIndex];
  }

  //----------

  virtual void setItemWidth(int32_t AWidth) {
    //if (AWidth < 0) MItemWidth = (MRect.w * 100 / (-AWidth);
    //else
    MItemWidth = AWidth;
  }

  virtual void setItemHeight(int32_t AHeight) {
    //if (AHeight < 0) MItemHeight = (MRect.h * 100 / (-AHeight);
    //else
    MItemHeight = AHeight;
  }

  virtual void setItemSize(int32_t AWidth, int32_t AHeight) {
    //if (AHeight < 0) MItemHeight = (MRect.h * 100 / (-AHeight);
    //else
    MItemWidth = AWidth;
    MItemHeight = AHeight;
  }

  virtual void setItemLayout(int32_t x, int32_t y) {
    MItemsX = x;
    MItemsY = y;
  }

  virtual void setMenuMirror(bool x, bool y) {
    MMirrorX = x;
    MMirrorY = y;
  }

//------------------------------
public:
//------------------------------

  virtual void appendMenuItem(const char* AItem) {
    //MItems.append((char*)AItem);
    char* item = (char*)AItem;
    MItems.append(item);
  }

  //----------

  virtual void appendMenuItems(const char** AItems, uint32_t ANum) {
    for (uint32_t i=0; i<ANum; i++) {
      appendMenuItem(AItems[i]);
    }
  }

  //----------

//  virtual void clearMenu(void) {
//    MItems.clear();
//    deleteChildren();
//  }

  //----------

  virtual void open(KODE_MenuListener* AListener, int32_t AXpos, int32_t AYpos) {
    //KODE_FRect mrect = getRect();
    //KODE_Print("%i,%i / %i,%i\n",MItemWidth,MItemHeight,MItemsX,MItemsY);
    MListener = AListener;
    MPrevSelected = MSelectedItem;
    //int32_t w = (MItemWidth  * MItemsX) + (MBorderSize * (MItemsX+1));
    //int32_t h = (MItemHeight * MItemsY) + (MBorderSize * 2 /*(MItemsY+1)*/);
    MMenuWidth = (MItemWidth  * MItemsX) + (MBorderSize * (MItemsX+1));
    MMenuHeight = (MItemHeight * MItemsY) + (MBorderSize * 2 /*(MItemsY+1)*/);
    // h = (MItems.size() * MItemHeight)  + (MBorderSize * 2);
    if (MMirrorX) AXpos -= MMenuWidth;
    if (MMirrorY) AYpos -= MMenuHeight;

    //setRect(KODE_FRect(AXpos,AYpos,w,h));

    //KODE_Print("%i,%i - %i,%i\n",AXpos,AYpos,w,h);
    KODE_FRect R = KODE_FRect(AXpos+MBorderSize, AYpos+MBorderSize, MItemWidth, MItemHeight);
    uint32_t num = MItems.size();
    uint32_t i = 0;
    deleteChildren();
    for (uint32_t x=0; x<MItemsX; x++) {
      for (uint32_t y=0; y<MItemsY; y++) {
        if (i < num) {
          const char* txt = MItems[i];
          //KODE_Print("%i %i %s : %i %i %i %i\n",x,y,txt,(int)R.x,(int)R.y,(int)R.w,(int)R.h);
          KODE_MenuItemWidget* mi = (KODE_MenuItemWidget*)appendWidget( new KODE_MenuItemWidget( R ));
          mi->setText(txt);
          R.y += MItemHeight;
        }
        i++;
      }
      R.y = AYpos + MBorderSize;
      R.x += MItemWidth + MBorderSize;
    }
    //MIsActive = true;
    //MIsVisible = true;
    flags.active = true;
    flags.visible = true;
    //setWidth(MMenuWidth);
    //setHeight(MMenuHeight);

    KODE_FRect menu_rect = KODE_FRect(AXpos,AYpos,MMenuWidth,MMenuHeight);
    //KODE_Print("%.0f,%.0f - %.0f,%.0f\n",menu_rect.x,menu_rect.y,menu_rect.w,menu_rect.h);
    //setRect(AXpos,AYpos,MMenuWidth,MMenuHeight);
    setRect(menu_rect);
    do_widget_redraw(this,menu_rect,0); // redraw parent?
    do_widget_grabModal(this);
  }

  //----------

  virtual void close(void) {
    MListener = KODE_NULL;
    //MIsActive = false;
    //MIsVisible = false;
    flags.active = false;
    flags.visible = false;
    do_widget_redraw(this,getRect(),0);
    do_widget_grabModal(KODE_NULL);
    setWidth(0);
    setHeight(0);
  }

//------------------------------
public:
//------------------------------

  virtual void appendMenuItem2(const char* ALabel) {
    KODE_MenuItemWidget* text;
    text = new KODE_MenuItemWidget( KODE_FRect(50,20) );
    text->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    text->setText(ALabel);
    appendWidget(text);
  }

  //----------

  virtual void open2(int32_t AXpos, int32_t AYpos) {
  }

//------------------------------
public:
//------------------------------

  void alignChildren(bool ARecursive=true) override {
    KODE_PanelWidget::alignChildren(ARecursive);
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_PanelWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
    if (AButton == KODE_BUTTON_LEFT) {
      if (!getRect().contains(AXpos,AYpos)) {
        // left-clicked outside of widget
        MSelectedItem = MPrevSelected;
        if (MListener) MListener->on_menuEvent(MSelectedItem);
        close();
      }
    }
    if (AButton == KODE_BUTTON_RIGHT) {
      // right-clicked
      MSelectedItem = MPrevSelected;
      if (MListener) MListener->on_menuEvent(MSelectedItem);
      close();
    }
  }

//------------------------------
public:
//------------------------------

  // item selected

  void do_widget_update(KODE_Widget* ASender) final {
    int32_t index = ASender->getIndex();
    MSelectedItem = index;
    if (MListener) MListener->on_menuEvent(MSelectedItem);
    close();
    KODE_PanelWidget::do_widget_update(this);
    //KODE_Widget::do_widget_update(ASender);
  }

  //----------

  // (right button clicked)

  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) final {
    if (AValue == KODE_MENU_NOTIFY_CLOSE) {
      // right clicked on menuitem
      MSelectedItem = MPrevSelected;
      if (MListener) MListener->on_menuEvent(MSelectedItem);
      close();
    }
    //KODE_Widget::do_widget_notify(AWidget,AValue);
  }

};

//----------------------------------------------------------------------
#endif















#if 0





  public:

    void do_update(KODE_Widget* ASender) override {
    }

    void do_notify(KODE_Widget* ASender, int32_t AMsg) override {
    }

};

#endif // 0
