#ifndef demo_editor_included
#define demo_editor_included
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_editor.h"
#include "gui/kode_widgets.h"

#include "../../data/img/sa_logo_40_trans_white.h"

#include "pages/system_page.h"
#include "pages/plugin_page.h"
#include "pages/gui_page.h"
#include "pages/audio_page.h"
#include "pages/gfx_page.h"

class demo_editor
: public KODE_Editor {

//------------------------------
public:
//------------------------------

  KODE_SAHeaderWidget*  MHeaderWidget = KODE_NULL;
  KODE_MenuWidget2*     MMenuWidget   = KODE_NULL;
  KODE_TextWidget*      MFooterWidget = KODE_NULL;
  KODE_Widget*          MMainWidget   = KODE_NULL;

  system_page*          MSystemPage   = KODE_NULL;
  plugin_page*          MPluginPage   = KODE_NULL;
  gui_page*             MGUIPage      = KODE_NULL;
  audio_page*           MAudioPage    = KODE_NULL;
  gfx_page*             MGfxPage      = KODE_NULL;

//------------------------------
public:
//------------------------------

  demo_editor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {

    setFillBackground(true);
    //setBackgroundColor(KODE_Color(0.6));
    setBackgroundColor(0.5f);
    layout.innerBorder = 10;
    layout.spacing = 5;

    //--------------------
    // header
    //--------------------

    MHeaderWidget = new KODE_SAHeaderWidget( KODE_FRect(60) );
    MHeaderWidget->setImage (this,(uint8_t*)sa_logo_40_trans_white,sa_logo_40_trans_white_size,KODE_Color(0.6));
    MHeaderWidget->setPluginName("KODE demo");
    MHeaderWidget->setPluginVersion("0.0.0");
    MHeaderWidget->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    appendWidget(MHeaderWidget);

    //--------------------
    // footer
    //--------------------

    MFooterWidget = new KODE_TextWidget( KODE_FRect(20) );
    MFooterWidget->layout.alignment = KODE_WIDGET_ALIGN_FILL_BOTTOM;
    MFooterWidget->setFillBackground(false);
    MFooterWidget->setText("footer");
    appendWidget(MFooterWidget);

    //--------------------
    // pages
    //--------------------

    MSystemPage = new system_page();
    MPluginPage = new plugin_page();
    MGUIPage    = new gui_page();
    MAudioPage  = new audio_page();
    MGfxPage    = new gfx_page();

    //--------------------
    // tabs
    //--------------------

    KODE_TabsWidget* tabs = new KODE_TabsWidget(KODE_FRect());//, 5);
    tabs->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
    tabs->layout.spacing = 5;
    appendWidget(tabs);

    tabs->appendPage("System",MSystemPage);
    tabs->appendPage("Plugin",MPluginPage);
    tabs->appendPage("GUI",MGUIPage);
    tabs->appendPage("Audio",MAudioPage);
    tabs->appendPage("Gfx",MGfxPage);
    tabs->selectPage(0);

    //--------------------
    //
    //--------------------

//    KODE_ValueWidget* widget;
//    widget = (KODE_ValueWidget*)appendWidget(new KODE_ValueWidget( KODE_FRect(10,10,150,20) ));
//    connectParameter(widget,0);
//
//    widget = (KODE_ValueWidget*)appendWidget(new KODE_ValueWidget( KODE_FRect(10,35,150,20) ));
//    widget->setDrawLabel(true);
//    connectParameter(widget,1);
//
//    KODE_SliderWidget* slider;
//    slider = (KODE_SliderWidget*)appendWidget(new KODE_SliderWidget( KODE_FRect(10,60,150,20) ));
//    slider->setValueBarDirection(KODE_RIGHT);
//    slider->setDrawLabel(true);
//    connectParameter(slider,2);

  }

  //----------

  //virtual ~myEditor() {
  //}

};

#endif // KODE_NO_GUI

//----------------------------------------------------------------------
#endif
