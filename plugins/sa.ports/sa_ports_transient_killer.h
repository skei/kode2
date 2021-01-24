//----------------------------------------------------------------------

// based on the 'transientkiller' jesusonic effect by 'loser'

/*

(C) 2006, Michael Gruhn.

NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
ENTRUST SOMEBODY ELSE WITH DOING SO.

*/
//----------------------------------------------------------------------


//#define KODE_DEBUG
//#define KODE_DEBUG_VST2

//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

#define KODE_NO_GUI

//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/utils/kode_math.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

// cat /proc/sys/kernel/random/uuid
// bd9a8de8-fa93-481f-a782-6fa982c1366e
uint8_t myPluginId[16]  = {0xbd,0x9a,0x8d,0xe8,0xfa,0x93,0x48,0x1f,0xa7,0x82,0x6f,0xa9,0x82,0xc1,0x36,0x6e};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName = "sa.ports.transient_killer";
    MAuthor = "skei.audio";
    MLongId = myPluginId;

    /*
    slider1:0<-12,1,.1>Threshold (dB)
    slider2:1<1,50,.1>Ratio
    */

    appendParameter( KODE_New KODE_FloatParameter("threshold",  0.0f, -12.0f,  1.0f) );
    appendParameter( KODE_New KODE_FloatParameter("ratio",      1.0f,   1.0f, 50.0f) );
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  /*
  @init
  cAmpDB_ = 8.65617025;
  */

  float cAmpDB_ = 8.65617025;

  float treshDB = 0.0f;
  float ratio   = 0.0f;
  float vol     = 0.0f;

  float db0 = 0.0f;
  float db1 = 0.0f;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

public:

  /*
  @slider
  treshDB = slider1;
  ratio = slider2;
  vol = 1/exp( (treshDB -treshDB/ratio) / cAmpDB_);
  */

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    //KODE_Trace("%i %.3f\n",AIndex,AValue);
    switch(AIndex) {
      case 0: treshDB = AValue; break;
      case 1: ratio   = AValue; break;
    }
    vol = 1/exp( (treshDB -treshDB/ratio) / cAmpDB_);
  }

  /*
  @sample
  (db0 = log(abs(spl0))*cAmpDB_) > treshDB ? spl0 = exp( (treshDB + (db0 - treshDB)/ratio) / cAmpDB_)* sign(spl0);
  (db1 = log(abs(spl1))*cAmpDB_) > treshDB ? spl1 = exp( (treshDB + (db1 - treshDB)/ratio) / cAmpDB_)* sign(spl1);
  spl0 *= vol;
  spl1 *= vol;
  */

  void on_processBlock(KODE_ProcessContext* AContext) final {
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      db0 = log(abs(spl0)) * cAmpDB_;
      if (db0 > treshDB) spl0 = exp( (treshDB + (db0 - treshDB)/ratio) / cAmpDB_) * KODE_Sign(spl0);
      db1 = log(abs(spl1)) * cAmpDB_;
      if (db1 > treshDB) spl1 = exp( (treshDB + (db1 - treshDB)/ratio) / cAmpDB_) * KODE_Sign(spl1);
      *out0++ = spl0 * vol;
      *out1++ = spl1 * vol;
    }
  }

};



//----------------------------------------------------------------------

//KODE_EXE_MAIN(KODE_Descriptor,KODE_Instance);
//KODE_VST3_MAIN(KODE_Descriptor,KODE_Instance);

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
