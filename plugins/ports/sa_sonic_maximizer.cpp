
#define KODE_GUI_XCB
#define KODE_PLUGIN_LV2_DUMPTTL
#define KODE_DEBUG_PRINT_SOCKET

//----------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"
#include "gui/kode_widgets.h"

#include "../data/img/sa_logo_40_trans_black.h"

#ifdef KODE_PLUGIN_LV2_DUMPTTL
  #include "plugin/lv2/kode_lv2_utils.h"
#endif

//----------------------------------------------------------------------

/*
  based on:
    BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
    http://forum.cockos.com/showthread.php?t=91439
    license: ?
    from the forum:
    skei (me) "i converted both the sonic maximizer (just the first version,
               yet..) and the sonic enhancer to vst, available here [...] i
               haven't studied the licensing agreements, so if you don't want
               me to port your plugins, let me know, and i'll remove it, no
               problem.."
    domino49: "great!!"
    so i think it should be ok?
*/

//----------------------------------------------------------------------

//#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 -KODE_PI2 * freqHP
#define freqLP_p2 -KODE_PI2 * freqLP

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : KODE_Descriptor() {
    #ifdef KODE_DEBUG
      setName("sa_sonic_maximizer_debug");
    #else
      setName("sa_sonic_maximizer");
    #endif
    setAuthor("skei.audio");
    setVersion(0x00000001);
    appendInput( KODE_New KODE_PluginPort("input1"));
    appendInput( KODE_New KODE_PluginPort("input2"));
    appendOutput( KODE_New KODE_PluginPort("output1"));
    appendOutput( KODE_New KODE_PluginPort("output2"));
    appendParameter( KODE_New KODE_FloatParameter("Low Cont",  1,  0,  10, 0.1 ) );
    appendParameter( KODE_New KODE_FloatParameter("Process",   1,  0,  10, 0.1 ) );
    appendParameter( KODE_New KODE_FloatParameter("Output",   -3, -30, 0,  0.1 ) );
    setHasEditor();
    setEditorSize(320,155);

    #ifdef KODE_PLUGIN_LV2_DUMPTTL
      KODE_WriteLv2Manifest(this);
    #endif

  }

  //----------

  //virtual ~myDescriptor() {
  //}

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myEditor : public KODE_Editor {

//------------------------------
private:
//------------------------------

  KODE_Bitmap*  MBitmap = KODE_NULL;

//------------------------------
public:
//------------------------------

  myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  : KODE_Editor(AInstance,AParent) {
    setFillBackground();
    setBackgroundColor(KODE_Color(0.6,0.6,0.6));

    MBitmap = KODE_New KODE_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);
    MBitmap->premultAlpha(0x999999);
    KODE_ImageWidget* imagewidget = KODE_New KODE_ImageWidget( KODE_FRect(10,10,64,64) );
    imagewidget->setImage(MBitmap);
    appendWidget(imagewidget);

    KODE_TextWidget* text1 = KODE_New KODE_TextWidget( KODE_FRect(64,10,200,20) );
    KODE_TextWidget* text2 = KODE_New KODE_TextWidget( KODE_FRect(64,30,200,20) );
    text1->setText("sa_sonic_maximizer");
    text2->setText("v0.0.2");
    appendWidget(text1);
    appendWidget(text2);
    KODE_SliderWidget* slider1 = KODE_New KODE_SliderWidget( KODE_FRect( 10,64 + 10,300,20) );
    KODE_SliderWidget* slider2 = KODE_New KODE_SliderWidget( KODE_FRect( 10,64 + 35,300,20) );
    KODE_SliderWidget* slider3 = KODE_New KODE_SliderWidget( KODE_FRect( 10,64 + 60,300,20) );

    //appendWidget(header);
    appendWidget(slider1);
    appendWidget(slider2);
    appendWidget(slider3);

    connectParameter(slider1,0);
    connectParameter(slider2,1);
    connectParameter(slider3,2);
  }

  //----------

  virtual ~myEditor() {
    KODE_Delete MBitmap;
  }

//------------------------------
private:
//------------------------------

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

//------------------------------
private:
//------------------------------

  myEditor* MEditor     = KODE_NULL;
  bool      need_recalc = true;

  float     param1      = 0.0f;
  float     param2      = 0.0f;
  float     param3      = 0.0f;

  float     band1       = 0.0f;
//float     band2       = 1.0f;
  float     band3       = 0.0f;
  float     tmplLP      = 0.0f;
  float     tmprLP      = 0.0f;
  float     tmplHP      = 0.0f;
  float     tmprHP      = 0.0f;
  float     amp         = 0.0f;
  float     xLP         = 0.0f;
  float     xHP         = 0.0f;
  float     a0LP        = 0.0f;
  float     a0HP        = 0.0f;
  float     b1LP        = 0.0f;
  float     b1HP        = 0.0f;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  //----------

  //virtual ~myInstance() {
  //}

//------------------------------
private:
//------------------------------

  void recalc(float srate) {
    xHP  = expf(freqHP_p2/srate);
    a0HP = 1.0 - xHP;
    b1HP = -xHP;
    xLP  = expf(freqLP_p2/srate);
    a0LP = 1.0 - xLP;
    b1LP = -xLP;
  }

//------------------------------
public:
//------------------------------

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch (AIndex) {
      case 0:
        param1 = AValue;
        band1 = expf( (param1+3) / c_ampdB );
        break;
      case 1:
        param2 = AValue;
        band3 = expf( (param2+3) / c_ampdB );
        break;
      case 2:
        param3 = AValue;
        amp = expf( param3 / c_ampdB );
        break;
    }
    //band2 := 1; // exp(0/c_ampdB);
  }

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    if (need_recalc) {
      recalc(AContext->samplerate);
      need_recalc = false;
    }
    uint32_t len    = AContext->numsamples;
    float* input0   = AContext->inputs[0];
    float* input1   = AContext->inputs[1];
    float* output0  = AContext->outputs[0];
    float* output1  = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;
      float s0  = *input0++;
      float s1  = *input1++;
      tmplLP    = a0LP*s0 - b1LP*tmplLP;// + cDenorm;
      tmprLP    = a0LP*s1 - b1LP*tmprLP;// + cDenorm;
      float sp0 = tmplLP;
      float sp1 = tmprLP;
      tmplHP    = a0HP*s0 - b1HP*tmplHP;// + cDenorm;
      tmprHP    = a0HP*s1 - b1HP*tmprHP;// + cDenorm;
      float sp4 = s0 - tmplHP;
      float sp5 = s1 - tmprHP;
      float sp2 = s0 - sp0 - sp4;
      float sp3 = s1 - sp1 - sp5;
      *output0++ = (sp0 * band1 + sp2 /* * band2 */ + sp4 * band3) * amp;
      *output1++ = (sp1 * band1 + sp3 /* * band2 */ + sp5 * band3) * amp;
    }
  }

//------------------------------
public:
//------------------------------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    MEditor = KODE_New myEditor(this,AParent);
    return MEditor;
  }

  //----------

  void on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    //KODE_Delete (myEditor*)AEditor;
    KODE_Assert(AEditor == MEditor);
    if (MEditor) {
      KODE_Delete MEditor;
      MEditor = KODE_NULL;
    }
  }

  //----------

  //void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
  //  KODE_Assert(AEditor == MEditor);
  //}

  #endif

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
