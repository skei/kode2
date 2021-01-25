
#define KODE_NO_GUI

#include "base/utils/kode_math.h"
#include "base/utils/kode_random.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

// cat /proc/sys/kernel/random/uuid
// 0a305663-5dd2-4d96-80a7-6aa5fade3604
uint8_t myPluginId[16]  = {0x0a,0x30,0x56,0x63,0x5d,0xd2,0x4d,0x96,0x80,0xa7,0x6a,0xa5,0xfa,0xde,0x36,0x04};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName       = "sa.ports.tube_harmonics_amp";
    MAuthor     = "skei.audio";
    MLongId = myPluginId;

    appendParameter( KODE_New KODE_FloatParameter("Even Harmonics",   0.3f,   0.0f,   1.0f) );
    appendParameter( KODE_New KODE_FloatParameter("Odd Harmonics",    0.3f,   0.0f,   1.0f) );
    appendParameter( KODE_New KODE_FloatParameter("Fluctuation",      0.3f,   0.0f,   1.0f) );
    appendParameter( KODE_New KODE_FloatParameter("TS Input (dB)",    0.0f, -24.0f,  24.0f) );
    appendParameter( KODE_New KODE_FloatParameter("TS Output (dB)",   0.0f, -24.0f,  24.0f) );
    appendParameter( KODE_New KODE_FloatParameter("Output Gain (dB)", 0.0f, -12.0f,  12.0f) );

  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  float seed0     = 0.0f;
  float seed1     = 0.0f;
  float sc_y0     = 1.0f;
  float sc_y1     = 1.0f;
  float ka        = 0.99f;
  float kb        = 1.0f - ka;
  float lim       = 0.4f;

  //@slider
  float tgt_drve  = 0.0f;
  float drvo      = 0.0f;
  float kr        = 0.0f;
  float kabs      = 0.0f;
  float ingain    = 0.0f;
  float hgain     = 0.0f;
  float trim      = 0.0f;

  //@block
  float d_drve    = 0.0f;
  float drve      = 0.0f;
  float src_drve  = 0.0f;
  float tgt_y0    = 0.0f;
  float d_y0      = 0.0f;
  float y0        = 0.0f;
  float src_y0    = 0.0f;
  float tgt_y1    = 0.0f;
  float d_y1      = 0.0f;
  float y1        = 0.0f;
  float src_y1    = 0.0f;
  float tgt_abs0  = 0.0f;
  float d_abs0    = 0.0f;
  float abs0      = 0.0f;
  float src_abs0  = 0.0f;
  float tgt_abs1  = 0.0f;
  float d_abs1    = 0.0f;
  float abs1      = 0.0f;
  float src_abs1  = 0.0f;
  float ch0       = 0.0f;
  float ch1       = 0.0f;
  float drve_rnd0 = 0.0f;
  float drve_rnd1 = 0.0f;
  float drvo_rnd0 = 0.0f;
  float drvo_rnd1 = 0.0f;
  float h0        = 0.0f;
  float h1        = 0.0f;
  float dcf00     = 0.0f;
  float dcf01     = 0.0f;
  float dc00      = 0.0f;
  float dc01      = 0.0f;
  float lim0      = 0.0f;
  float lim1      = 0.0f;
  float m01       = 0.0f;
  float m03       = 0.0f;
  float m00       = 0.0f;
  float m02       = 0.0f;
  float fir0      = 0.0f;
  float m11       = 0.0f;
  float m13       = 0.0f;
  float m10       = 0.0f;
  float m12       = 0.0f;
  float fir1      = 0.0f;
  float dcf10     = 0.0f;
  float dcf11     = 0.0f;
  float dc10      = 0.0f;
  float dc11      = 0.0f;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {

    seed0 = KODE_RandomRange(0,999);
    seed1 = KODE_RandomRange(0,999);
    //sc_y0 = 1;
    //sc_y1 = 1;
    //ka    = 0.99;
    //b     = 1 - ka;
    //im    = 0.4;

  }

public:

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    //KODE_Trace("%i %.3f\n",AIndex,AValue);
    switch(AIndex) {
      case 0: tgt_drve  = AValue * 2.0f; break;
      case 1: drvo      = AValue * 2.0f; break;
      case 2: kr        = AValue * 0.3f;
              kabs      = AValue * 0.3f; break;
      case 3: ingain    = powf(2.0f,(AValue/6.0f)); break;
      case 4: hgain     = powf(2.0f,(AValue/6.0f)); break;
      case 5: trim      = powf(2.0f,(AValue/6.0f)); break;

    }
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {

    //*** interpolate parameters
    d_drve    = (tgt_drve - src_drve) / AContext->numsamples;
    drve      = src_drve;
    src_drve  = tgt_drve;
    sc_y0     = sinf(seed0 * sc_y0);
    seed0    += 1.0f;
    tgt_y0    = sc_y0 * kr;
    d_y0      = (tgt_y0 - src_y0) / AContext->numsamples;
    y0        = src_y0;
    src_y0    = tgt_y0;
    sc_y1     = sinf(seed1 * sc_y1);
    seed1    += 1.0f;
    tgt_y1    = sc_y1 * kr;
    d_y1      = (tgt_y1 - src_y1) / AContext->numsamples;
    y1        = src_y1;
    src_y1    = tgt_y1;
    tgt_abs0  = fabs(ch0) * kabs;
    d_abs0    = (tgt_abs0 - src_abs0) / AContext->numsamples;
    abs0      = src_abs0;
    src_abs0  = tgt_abs0;
    tgt_abs1  = fabs(ch1) * kabs;
    d_abs1    = (tgt_abs1 - src_abs1) / AContext->numsamples;
    abs1      = src_abs1;
    src_abs1  = tgt_abs1;

    //

    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;

      float spl0 = *in0++;
      float spl1 = *in1++;

      // set in gain
      ch0 = spl0 * ingain;
      ch1 = spl1 * ingain;
      //interpolate
      y0 += d_y0;
      y1 += d_y1;
      abs0 += d_abs0;
      abs1 += d_abs1;
      drve += d_drve;
      //set drive values
      drve_rnd0 = drve - abs0;
      drve_rnd1 = drve - abs1;
      drvo_rnd0 = drvo - abs0 - y0;
      drvo_rnd1 = drvo - abs1 - y1;
      //apply harmonics
      h0 = sinf(ch0 * drve_rnd0) / sinf(ch0) * drve + (tanf(ch0) * drvo_rnd0) * drvo * 0.3f;
      h1 = sinf(ch1 * drve_rnd1) / sinf(ch1) * drve + (tanf(ch1) * drvo_rnd1) * drvo * 0.3f;
      //dc filter i
      dcf00 = h0 * kb + dcf00 * ka;
      dcf01 = h1 * kb + dcf01 * ka;
      dc00 = h0 - dcf00;
      dc01 = h1 - dcf01;
      //limiter
      lim0 = KODE_Min(KODE_Max(dc00 * hgain,-lim),lim);
      lim1 = KODE_Min(KODE_Max(dc01 * hgain,-lim),lim);
      //fir filter
      m01  = m00;
      m03  = m02;
      m00  = lim0;
      m02  = 0.5f * (m01+m00);
      fir0 = 0.5f * (m03+m02);
      m11  = m10;
      m13  = m12;
      m10  = lim1;
      m12  = 0.5f * (m11+m10);
      fir1 = 0.5f * (m13+m12);

      //dc filter ii
      dcf10 = fir0 * kb + dcf10 * ka;
      dcf11 = fir1 * kb + dcf11 * ka;
      dc10 = fir0 - dcf10;
      dc11 = fir1 - dcf11;

      //sum
      spl0 = (spl0 + dc10) * trim;
      spl1 = (spl1 + dc11) * trim;

      *out0++ = spl0;
      *out1++ = spl1;

    }
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)










#if 0

// (C) 2009, Lubomir I. Ivanov
//
// NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
// WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
// ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
// OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
// MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
// TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
// USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
// PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
// ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
// PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
// ENTRUST SOMEBODY ELSE WITH DOING SO.
//
// Released under GPL:
// <http://www.gnu.org/licenses/>.
//
//**********************************************************
// Tube Harmonics:
// Simulates the behaviour of a vacuum tube circuit stage.
// Adds odd and even harmonics to the signal.
// Has flux and program dependent saturation.
//**********************************************************

desc: Tube Harmonics ( Amp Version )

slider1:0.3<0,1,0.001>Even Harmonics
slider2:0.3<0,1,0.001>Odd Harmonics
slider3:0.3<0,1,0.001>Fluctuation
slider4:0<-24,24,0.001>TS Input (dB)
slider5:0<-24,24,0.001>TS Output (dB)
slider6:0<-12,12,0.001>Output Gain (dB)

@init
ext_noinit=1;
seed0=rand(999)|0;
while(seed1=rand(999)|0;seed1==seed0;);
sc_y0=sc_y1=1;
ka=0.99;
kb=1-ka;
lim=0.4;

@slider
tgt_drve=slider1*2;
drvo=slider2*2;
kr=slider3*0.3;
kabs=slider3*0.3;
ingain=2^(slider4/6);
hgain=2^(slider5/6);
trim=2^(slider6/6);

@sample
// set in gain
ch0=spl0*ingain;
ch1=spl1*ingain;

//interpolate
y0+=d_y0;
y1+=d_y1;
abs0+=d_abs0;
abs1+=d_abs1;
drve+=d_drve;

//set drive values
drve_rnd0=drve-abs0;
drve_rnd1=drve-abs1;
drvo_rnd0=drvo-abs0-y0;
drvo_rnd1=drvo-abs1-y1;

//apply harmonics
h0=sin(ch0*drve_rnd0)/sin(ch0)*drve+(tan(ch0)*drvo_rnd0)*drvo*0.3;
h1=sin(ch1*drve_rnd1)/sin(ch1)*drve+(tan(ch1)*drvo_rnd1)*drvo*0.3;

//dc filter i
dc00=h0-(dcf00=h0*kb+dcf00*ka);
dc01=h1-(dcf01=h1*kb+dcf01*ka);

//limiter
lim0=min(max(dc00*hgain,-lim),lim);
lim1=min(max(dc01*hgain,-lim),lim);

//fir filter
m01=m00;m03=m02;
fir0=0.5*(m03+m02=0.5*(m01+m00=lim0));
m11=m10;m13=m12;
fir1=0.5*(m13+m12=0.5*(m11+m10=lim1));

//dc filter ii
dc10=fir0-(dcf10=fir0*kb+dcf10*ka);
dc11=fir1-(dcf11=fir1*kb+dcf11*ka);

//sum
spl0=(spl0+dc10)*trim;
spl1=(spl1+dc11)*trim;

@block
//*** interpolate parameters

d_drve=(tgt_drve-src_drve)/samplesblock;
drve=src_drve;
src_drve=tgt_drve;

sc_y0=sin((seed0+=1)*sc_y0);
tgt_y0=sc_y0*kr;
d_y0=(tgt_y0-src_y0)/samplesblock;
y0=src_y0;
src_y0=tgt_y0;

sc_y1=sin((seed1+=1)*sc_y1);
tgt_y1=sc_y1*kr;
d_y1=(tgt_y1-src_y1)/samplesblock;
y1=src_y1;
src_y1=tgt_y1;

tgt_abs0=abs(ch0)*kabs;
d_abs0=(tgt_abs0-src_abs0)/samplesblock;
abs0=src_abs0;
src_abs0=tgt_abs0;

tgt_abs1=abs(ch1)*kabs;
d_abs1=(tgt_abs1-src_abs1)/samplesblock;
abs1=src_abs1;
src_abs1=tgt_abs1;

@gfx 200 42

drva=(drve+drvo)*0.75;

//draw tube i
gfx_a=1;
gfx_r=gfx_g=gfx_b=1;
ox=10;
gfx_x=ox;
gfx_y=10;
gfx_rectto(ox+20,50);
gfx_r=gfx_g=gfx_b=0;
gfx_x=ox+1;
gfx_y=10+1;
gfx_rectto(ox+20-1,50-1);
gfx_r=0.8;
gfx_g=0;
gfx_b=0;
gfx_x=ox;
gfx_y=42;
gfx_rectto(ox+20,gfx_y+5);
gfx_r=gfx_g=gfx_b=1;
gfx_x=ox+7;
gfx_y=8;
gfx_rectto(ox+13,gfx_y+2);
gfx_r=gfx_g=gfx_b=0.7;
gfx_x=ox+4;
gfx_y=50;
gfx_rectto(ox+6,gfx_y+4);
gfx_x=ox+9;
gfx_y=50;
gfx_rectto(ox+11,gfx_y+4);
gfx_x=ox+14;
gfx_y=50;
gfx_rectto(ox+16,gfx_y+4);
gfx_r=gfx_g=1;
gfx_b=0.4;
gfx_a=max(min(drva,1)-abs0,0.8);
gfx_x=ox+4;
gfx_y=14;
gfx_rectto(ox+16,gfx_y+26);

//draw tube ii
gfx_a=1;
gfx_r=gfx_g=gfx_b=1;
ox=gfx_w-30;
gfx_x=ox;
gfx_y=10;
gfx_rectto(ox+20,50);
gfx_r=gfx_g=gfx_b=0;
gfx_x=ox+1;
gfx_y=10+1;
gfx_rectto(ox+20-1,50-1);
gfx_r=0.8;
gfx_g=0;
gfx_b=0;
gfx_x=ox;
gfx_y=42;
gfx_rectto(ox+20,gfx_y+5);
gfx_r=gfx_g=gfx_b=1;
gfx_x=ox+7;
gfx_y=8;
gfx_rectto(ox+13,gfx_y+2);
gfx_r=gfx_g=gfx_b=0.7;
gfx_x=ox+4;
gfx_y=50;
gfx_rectto(ox+6,gfx_y+4);
gfx_x=ox+9;
gfx_y=50;
gfx_rectto(ox+11,gfx_y+4);
gfx_x=ox+14;
gfx_y=50;
gfx_rectto(ox+16,gfx_y+4);
gfx_r=gfx_g=1;
gfx_b=0.4;
gfx_a=max(min(drva,1)-abs1,0.80);
gfx_x=ox+4;
gfx_y=14;
gfx_rectto(ox+16,gfx_y+26);

//draw bkg
gfx_a=0.6;
gfx_r=0.1;
gfx_g=0.5;
gfx_b=0.75;
gfx_x=40;
gfx_y=10;
gfx_rectto(gfx_w-40,gfx_y+45);
gfx_a=0.15;
gfx_r=gfx_g=gfx_b=0;
gfx_x=42;
gfx_y=32;
gfx_rectto(gfx_w-43,gfx_y+23);


#endif // 0
