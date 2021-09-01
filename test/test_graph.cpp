
#define KODE_GUI_CAIRO

//----------------------------------------------------------------------

#include "kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

int main() {

  KODE_Window* window = new KODE_Window(800,600,"test_graph");

  KODE_GraphWidget* graph = new KODE_GraphWidget( KODE_FRect() );
  graph->layout.alignment = KODE_WIDGET_ALIGN_FILL_CLIENT;
  window->appendWidget(graph);

  KODE_GraphModule* module;

  for (uint32_t i=0; i<10; i++) {
    module = new KODE_GraphModule();
    uint32_t inum = KODE_RandomRangeInt(1,5);
    uint32_t onum = KODE_RandomRangeInt(0,5);
    module->numInputs = inum;
    module->numOutputs = onum;
    for (uint32_t j=0; j<inum; j++) module->inputs[j] =  KODE_RandomRangeInt(0,1);
    for (uint32_t j=0; j<onum; j++) module->outputs[j] = KODE_RandomRangeInt(0,1);
    graph->addModule(module, 10 + i*50, 10 + i*50, "module" );
  }


//  module = new KODE_GraphModule();
//    module->numInputs = 2;
//    module->numOutputs = 2;
//    graph->addModule(module,300,200,"module2");

  //KODE_GraphWire*   wire;
  //wire = new KODE_GraphWire(inmod,inpin,outmodule,outpin);

  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
