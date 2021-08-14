

#if 0

#define KODE_PLUGIN_LV2_DUMP_TTL
//#define KODE_MAX_PATH_SIZE 512 // -> KODE_MAX_PATH_LENGTH

//----------

#include "base/kode.h"
#include "plugin/lv2/kode_lv2.h"

//----------------------------------------------------------------------
// manifest
//----------------------------------------------------------------------

#ifdef KODE_PLUGIN_LV2_DUMP_TTL

  #define KODE_LV2_DUMP_TTL_SYMBOL asm ("dump_ttl");
  void kode_lv2_dump_ttl(void) KODE_LV2_DUMP_TTL_SYMBOL;

  //----------

  __KODE_DLLEXPORT
  void kode_lv2_dump_ttl() {

    KODE_Descriptor* descriptor = KODE_GetDescriptor();
    FILE* fp = nullptr;
    fp = fopen("manifest.ttl","w");
    if (fp) {
      fprintf(fp,"@prefix lv2:  <http://lv2plug.in/ns/lv2core#> .\n");
      fprintf(fp,"@prefix rdf:  <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n");
      fprintf(fp,"@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n");
      //fprintf(fp,"@prefix ui:   <http://lv2plug.in/ns/extensions/ui#> .\n");
      fprintf(fp,"\n");
      fprintf(fp,"<urn:%s/%s>\n",descriptor->getAuthor(),descriptor->getName()); //"urn:skei.audio/kode_debug";
      fprintf(fp,"  a lv2:Plugin ;\n");// , lv2:InstrumentPlugin ;\n");
      fprintf(fp,"  lv2:binary <%s.so>  ;\n",descriptor->getName()); // TODO: this.filename?
      fprintf(fp,"  rdfs:seeAlso <%s.ttl> .\n",descriptor->getName());
      fprintf(fp,"\n");
      //#ifdef KODE_PLUGIN_EDITOR
      //fprintf(fp,"urn:%s/%s#ui\n",author,name);
      //fprintf(fp,"	a ui:X11UI ;\n");
      //fprintf(fp,"	ui:binary <%s.so>  ;\n",name);
      //fprintf(fp,"	lv2:extensionData ui:idle .\n");
      //fprintf(fp,"\n");
      //#endif
      fflush(fp);
      fclose(fp);
    }
    char buffer[KODE_MAX_PATH_LENGTH] = {0};
    buffer[0] = 0;
    strcat(buffer,descriptor->getName());
    strcat(buffer,".ttl");
    fp = fopen(buffer,"w");
    if (fp) {

      fprintf(fp,"@prefix doap:   <http://usefulinc.com/ns/doap#> .\n");
      fprintf(fp,"@prefix lv2:    <http://lv2plug.in/ns/lv2core#> .\n");
      fprintf(fp,"@prefix rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n");
      fprintf(fp,"@prefix rdfs:   <http://www.w3.org/2000/01/rdf-schema#> .\n");
      fprintf(fp,"@prefix atom:   <http://lv2plug.in/ns/ext/atom#> .\n");
      fprintf(fp,"@prefix midi:   <http://lv2plug.in/ns/ext/midi#> .\n");
      //fprintf(fp,"@prefix ui:     <http://lv2plug.in/ns/extensions/ui#> .\n");
      fprintf(fp,"@prefix urid:   <http://lv2plug.in/ns/ext/urid#> .\n");
      fprintf(fp,"\n");
      //fprintf(fp,"<%s>\n",descriptor->getURI());
      fprintf(fp,"<urn:%s/%s>\n",descriptor->getAuthor(),descriptor->getName()); //"urn:skei.audio/kode_debug";
      fprintf(fp,"  a lv2:Plugin ;\n");// , lv2:InstrumentPlugin ;\n");
      fprintf(fp,"  doap:name           \"%s\" ;\n",descriptor->getName());
      fprintf(fp,"  lv2:optionalFeature lv2:hardRTCapable ;\n");
      fprintf(fp,"  lv2:requiredFeature urid:map ;\n");
      //fprintf(fp,"	ui:ui <http://infamousplugins.sourceforge.net/plugs.html#stuck_ui> ;
      //fprintf(fp,"<urn:%s/%s#ui>\n",author,name);
      uint32_t numin     = descriptor->getNumInputs();
      uint32_t numout    = descriptor->getNumOutputs();
      uint32_t numpar    = descriptor->getNumParameters();
      uint32_t numports  = numin + numout + numpar;
      if (descriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
        numports += 1;
      }

      uint32_t i = 0;
      uint32_t p = 0;

      char symbol[64];
      char name[64];

      // every port symbol must be unique and a valid C identifier
      // and the indices must start at 0 and be contiguous

      if (numports > 0) {
        fprintf(fp,"  lv2:port [\n");
        for (i=0; i<numin; i++) {

          //const char* name = "Input ";
          //name += (int)i;
          //const char* symbol = name;
          //symbol.makeValidCSymbol();

          sprintf(name,"Input %i",i);
          strcpy(symbol,name);

          fprintf(fp,"    a lv2:AudioPort , lv2:InputPort ;\n");
          fprintf(fp,"    lv2:index         %i ;\n",p);
          fprintf(fp,"    lv2:symbol        \"%s\" ;\n",symbol);
          fprintf(fp,"    lv2:name          \"%s\" ;\n",name);
          p++;
          if (p < numports) fprintf(fp,"	] , [\n");
          else fprintf(fp," ] .\n");
        }

        for (i=0; i<numout; i++) {

          //const char* name = "Output ";
          //name += (int)i;
          //const char* symbol = name;
          //symbol.makeValidCSymbol();

          sprintf(name,"Output %i",i);
          strcpy(symbol,name);

          fprintf(fp,"    a lv2:AudioPort , lv2:OutputPort ;\n");
          fprintf(fp,"    lv2:index         %i ;\n",p);
          fprintf(fp,"    lv2:symbol        \"%s\" ;\n",symbol);
          fprintf(fp,"    lv2:name          \"%s\" ;\n",name);
          p++;
          if (p < numports) fprintf(fp,"  ] , [\n");
          else fprintf(fp," ] .\n");
        }

        for (i=0; i<numpar; i++) {
          KODE_Parameter* par = descriptor->getParameter(i);

          //const char* name = par->getName();
          //const char* symbol = name;
          //symbol.makeValidCSymbol();

          strcpy(name,par->getName());
          strcpy(symbol,name);

          fprintf(fp,"    a lv2:InputPort , lv2:ControlPort ;\n");
          fprintf(fp,"    lv2:index         %i ;\n",p);
          fprintf(fp,"    lv2:symbol        \"%s\" ;\n",symbol);
          fprintf(fp,"    lv2:name          \"%s\" ;\n",name);
          fprintf(fp,"    lv2:default       %f ;\n",    par->getDefValue());
          fprintf(fp,"    lv2:minimum       %f ;\n",    par->getMinValue());
          fprintf(fp,"    lv2:maximum       %f ;\n",    par->getMaxValue());
          p++;
          if (p < numports) fprintf(fp," ] , [\n");
          else fprintf(fp," ] .\n");
        }
        if (descriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
          //fprintf(fp,"		a lv2:InputPort , atom:AtomPort ;\n");
          fprintf(fp,"    a atom:AtomPort , lv2:InputPort ;\n");
          fprintf(fp,"    atom:bufferType   atom:Sequence ;\n");
          fprintf(fp,"    atom:supports     midi:MidiEvent ;\n");
          fprintf(fp,"    lv2:designation   lv2:control ;\n");
          fprintf(fp,"    lv2:index         %i ;\n",p);
          fprintf(fp,"    lv2:symbol        \"midi_in\" ;\n");
          fprintf(fp,"    lv2:name          \"Midi in\" ;\n");
          p++;
          if (p < numports) fprintf(fp," ] , [\n");
          else fprintf(fp," ] .\n");
        }
      }
      fprintf(fp,"\n");
      fflush(fp);
      fclose(fp);
    }

  }

#endif // KODE_PLUGIN_LV2_DUMP_TTL


#endif // 0