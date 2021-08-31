
//#define KODE_DEBUG
//#define KODE_DEBUG_VST2
//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

//----------------------------------------------------------------------

/*
  based on:
    ladspa plugins 'matched' and 'unmatched' (c) tim goetze
    http://quitte.de/dsp/caps.html
    (gpl3)
    see bottom of this file for info
*/

#define KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

const char* txt_mode[2] = { "unmatched", "matched" };

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName = "sap_cabinet";
    MAuthor = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    appendParameter( new KODE_Parameter("gain", 1) );
    appendParameter( new KODE_TextParameter("mode", 0, 2, txt_mode ) );

  }

  //----------

  virtual ~myDescriptor() {
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  //float MLeft   = 0.0f;
  //float MRight  = 0.0f;

  float   gain = 1.0f;
  int32_t mode = 0;
  float   z0 = 0.0f;
  float   z1 = 0.0f;
  float   x0[16] = {0};
  float   y0[16] = {0};
  float   x1[16] = {0};
  float   y1[16] = {0};

  //reset();

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    reset();
  }

  virtual ~myInstance() {
    //gain = 1;
    //mode = 0;
  }

private:

  void reset(void) {
    for (int i=0; i<16; i++) {
      x0[i] = 0;
      y0[i] = 0;
      x1[i] = 0;
      y1[i] = 0;
    }
    z0 = 0;
    z1 = 0;
    //gain = 1;
    //mode = 0;
  }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch (AIndex) {
      case 0:
        gain = AValue;
        break;
      case 1:
        mode = AValue;
        break;
    }
  }

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {

      // left

      int32_t z = z0;
      float a = *in0++;
      x0[z] = a;
      switch(mode) {
        case 0:
          a *= -0.0000186978325089078301339973881001199629;
          a += -0.0019343756431780716794482932741061631532 * x0[(z-1 ) & 15];
          a += +0.4882799441735049539303759047470521181822 * y0[(z-1 ) & 15];
          a += -0.0068581720709373307151568432971089350758 * x0[(z-2 ) & 15];
          a += +0.3391784486546852162014431542047532275319 * y0[(z-2 ) & 15];
          a += -0.0080712033745105205700953732161906373221 * x0[(z-3 ) & 15];
          a += +0.0041013313624351710115489844099556648871 * y0[(z-3 ) & 15];
          a += +0.0062425515113487175383744087753257190343 * x0[(z-4 ) & 15];
          a += -0.2937578593108242097642346379871014505625 * y0[(z-4 ) & 15];
          a += +0.0300330856260519092615357550357657601126 * x0[(z-5 ) & 15];
          a += -0.3135107132656380080071301108546322211623 * y0[(z-5 ) & 15];
          a += +0.0420998422314063230897929202001250814646 * x0[(z-6 ) & 15];
          a += -0.0941769772306991492794026044066413305700 * y0[(z-6 ) & 15];
          a += +0.0209068926357999021470313039117172593251 * x0[(z-7 ) & 15];
          a += +0.1218530135894568267040938280842965468764 * y0[(z-7 ) & 15];
          a += -0.0293365015269708810652371511196179199032 * x0[(z-8 ) & 15];
          a += +0.1605362805340295739853218037751503288746 * y0[(z-8 ) & 15];
          a += -0.0748650971084933170018871351203415542841 * x0[(z-9 ) & 15];
          a += +0.0105280663428685625215841525914584053680 * y0[(z-9 ) & 15];
          a += -0.0883889691308883534981433172106335405260 * x0[(z-10) & 15];
          a += -0.1946868621427640388965585316327633336186 * y0[(z-10) & 15];
          a += -0.0719149556841363057912985823350027203560 * x0[(z-11) & 15];
          a += -0.2560883692448795989626830760244047269225 * y0[(z-11) & 15];
          a += -0.0389244603189328741699171132495393976569 * x0[(z-12) & 15];
          a += -0.0719592227789066923859806479413236957043 * y0[(z-12) & 15];
          a += -0.0054345766014978463126494645507591485512 * x0[(z-13) & 15];
          a += +0.2253269103535170958529931795055745169520 * y0[(z-13) & 15];
          a += +0.0081937491360965508668812162795802578330 * x0[(z-14) & 15];
          a += +0.2589115724912437466187498102954123169184 * y0[(z-14) & 15];
          a += +0.0020664782446009415627086180933247305802 * x0[(z-15) & 15];
          a += -0.4263943664352933815564483666094020009041 * y0[(z-15) & 15];
          break;
        case 1:
          a *= +1.09170598489534098618; \
          a += +0.09079972214735353153 * x0 [(z-1) & 15]; \
          a += +0.25594415074966925250 * y0 [(z-1) & 15]; \
          a += +0.06275851860674004634 * x0 [(z-2) & 15]; \
          a += +0.20269069460094527035 * y0 [(z-2) & 15]; \
          a += +0.01060723399364926073 * x0 [(z-3) & 15]; \
          a += +0.09969966728889961827 * y0 [(z-3) & 15]; \
          a += -0.04003298804448884862 * x0 [(z-4) & 15]; \
          a += -0.01698308894227101157 * y0 [(z-4) & 15]; \
          a += -0.05098733387148121632 * x0 [(z-5) & 15]; \
          a += -0.10025666121442594614 * y0 [(z-5) & 15]; \
          a += -0.00212027924805496424 * x0 [(z-6) & 15]; \
          a += -0.12376947415521612206 * y0 [(z-6) & 15]; \
          a += +0.08216861428657665223 * x0 [(z-7) & 15]; \
          a += -0.09837065464554287098 * y0 [(z-7) & 15]; \
          a += +0.14558022807771681117 * x0 [(z-8) & 15]; \
          a += -0.05792397408788292623 * y0 [(z-8) & 15]; \
          a += +0.14107979919296464999 * x0 [(z-9) & 15]; \
          a += -0.02742170520254189059 * y0 [(z-9) & 15]; \
          a += +0.06863603090258478534 * x0 [(z-10) & 15]; \
          a += -0.00754151013889273192 * y0 [(z-10) & 15]; \
          a += -0.02221952179046714215 * x0 [(z-11) & 15]; \
          a += +0.01411725095447000089 * y0 [(z-11) & 15]; \
          a += -0.06830834967903634536 * x0 [(z-12) & 15]; \
          a += +0.03438707838281616913 * y0 [(z-12) & 15]; \
          a += -0.04122301442705932767 * x0 [(z-13) & 15]; \
          a += +0.02016038635935246681 * y0 [(z-13) & 15]; \
          a += +0.03204378656092733701 * x0 [(z-14) & 15]; \
          a += -0.07309957271207177809 * y0 [(z-14) & 15]; \
          a += +0.09023862342328989317 * x0 [(z-15) & 15]; \
          a += -0.25732881619369524229 * y0 [(z-15) & 15];
          break;
      }
      y0[z] = a;
      *out0++ = a * gain;
      z = (z + 1) & 15;
      z0 = z;

      // right

      z = z1;
      a = *in1++;
      x1[z] = a;
      switch(mode) {
        case 0:
            a *= -0.0000186978325089078301339973881001199629;
            a += -0.0019343756431780716794482932741061631532 * x1[(z-1 ) & 15];
            a += +0.4882799441735049539303759047470521181822 * y1[(z-1 ) & 15];
            a += -0.0068581720709373307151568432971089350758 * x1[(z-2 ) & 15];
            a += +0.3391784486546852162014431542047532275319 * y1[(z-2 ) & 15];
            a += -0.0080712033745105205700953732161906373221 * x1[(z-3 ) & 15];
            a += +0.0041013313624351710115489844099556648871 * y1[(z-3 ) & 15];
            a += +0.0062425515113487175383744087753257190343 * x1[(z-4 ) & 15];
            a += -0.2937578593108242097642346379871014505625 * y1[(z-4 ) & 15];
            a += +0.0300330856260519092615357550357657601126 * x1[(z-5 ) & 15];
            a += -0.3135107132656380080071301108546322211623 * y1[(z-5 ) & 15];
            a += +0.0420998422314063230897929202001250814646 * x1[(z-6 ) & 15];
            a += -0.0941769772306991492794026044066413305700 * y1[(z-6 ) & 15];
            a += +0.0209068926357999021470313039117172593251 * x1[(z-7 ) & 15];
            a += +0.1218530135894568267040938280842965468764 * y1[(z-7 ) & 15];
            a += -0.0293365015269708810652371511196179199032 * x1[(z-8 ) & 15];
            a += +0.1605362805340295739853218037751503288746 * y1[(z-8 ) & 15];
            a += -0.0748650971084933170018871351203415542841 * x1[(z-9 ) & 15];
            a += +0.0105280663428685625215841525914584053680 * y1[(z-9 ) & 15];
            a += -0.0883889691308883534981433172106335405260 * x1[(z-10) & 15];
            a += -0.1946868621427640388965585316327633336186 * y1[(z-10) & 15];
            a += -0.0719149556841363057912985823350027203560 * x1[(z-11) & 15];
            a += -0.2560883692448795989626830760244047269225 * y1[(z-11) & 15];
            a += -0.0389244603189328741699171132495393976569 * x1[(z-12) & 15];
            a += -0.0719592227789066923859806479413236957043 * y1[(z-12) & 15];
            a += -0.0054345766014978463126494645507591485512 * x1[(z-13) & 15];
            a += +0.2253269103535170958529931795055745169520 * y1[(z-13) & 15];
            a += +0.0081937491360965508668812162795802578330 * x1[(z-14) & 15];
            a += +0.2589115724912437466187498102954123169184 * y1[(z-14) & 15];
            a += +0.0020664782446009415627086180933247305802 * x1[(z-15) & 15];
            a += -0.4263943664352933815564483666094020009041 * y1[(z-15) & 15];
          break;
        case 1:
          a *= +1.09170598489534098618; \
          a += +0.09079972214735353153 * x1 [(z-1) & 15];
          a += +0.25594415074966925250 * y1 [(z-1) & 15];
          a += +0.06275851860674004634 * x1 [(z-2) & 15];
          a += +0.20269069460094527035 * y1 [(z-2) & 15];
          a += +0.01060723399364926073 * x1 [(z-3) & 15];
          a += +0.09969966728889961827 * y1 [(z-3) & 15];
          a += -0.04003298804448884862 * x1 [(z-4) & 15];
          a += -0.01698308894227101157 * y1 [(z-4) & 15];
          a += -0.05098733387148121632 * x1 [(z-5) & 15];
          a += -0.10025666121442594614 * y1 [(z-5) & 15];
          a += -0.00212027924805496424 * x1 [(z-6) & 15];
          a += -0.12376947415521612206 * y1 [(z-6) & 15];
          a += +0.08216861428657665223 * x1 [(z-7) & 15];
          a += -0.09837065464554287098 * y1 [(z-7) & 15];
          a += +0.14558022807771681117 * x1 [(z-8) & 15];
          a += -0.05792397408788292623 * y1 [(z-8) & 15];
          a += +0.14107979919296464999 * x1 [(z-9) & 15];
          a += -0.02742170520254189059 * y1 [(z-9) & 15];
          a += +0.06863603090258478534 * x1 [(z-10) & 15];
          a += -0.00754151013889273192 * y1 [(z-10) & 15];
          a += -0.02221952179046714215 * x1 [(z-11) & 15];
          a += +0.01411725095447000089 * y1 [(z-11) & 15];
          a += -0.06830834967903634536 * x1 [(z-12) & 15];
          a += +0.03438707838281616913 * y1 [(z-12) & 15];
          a += -0.04122301442705932767 * x1 [(z-13) & 15];
          a += +0.02016038635935246681 * y1 [(z-13) & 15];
          a += +0.03204378656092733701 * x1 [(z-14) & 15];
          a += -0.07309957271207177809 * y1 [(z-14) & 15];
          a += +0.09023862342328989317 * x1 [(z-15) & 15];
          a += -0.25732881619369524229 * y1 [(z-15) & 15];
          break;
      }
      y1[z] = a;
      *out1++ = a * gain;
      z = (z + 1) & 15;
      z1 = z;
    }

  }

  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}

};



//----------------------------------------------------------------------

//KODE_EXE_MAIN(KODE_Descriptor,KODE_Instance);
//KODE_VST3_MAIN(KODE_Descriptor,KODE_Instance);

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)





#if 0

/*
	unmatched.c

	Copyright (C) 2002 Tim Goetze <tim@quitte.de>

	http://quitte.de/dsp/

	16-pole IIR Filter modeled after an impulse response from
	a Matchless Chieftain, which comes as part of Steve Harris'
	impulse convolver (http://plugin.org.uk).
*/

/*
	matched.c

	Copyright (C) 2003 Tim Goetze <tim@quitte.de>

	http://quitte.de/dsp/

	16th order IIR filter modeled after an impulse response from
	a matchless chieftain guitar amplifier, corrected for the
	use in conjunction with the 'preamp' (see url above) plugin.
*/

/*
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
	02111-1307, USA or point your web browser to http://www.gnu.org.
*/


#endif // 0
