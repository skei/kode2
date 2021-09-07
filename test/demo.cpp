
//#define KODE_GUI_XCB
#define KODE_GUI_CAIRO
//#define KODE_USE_CAIRO
//#define KODE_NO_GUI
//#define KODE_DEBUG_PRINT_TIME
//#define KODE_DEBUG_PRINT_THREAD
//#define KODE_DEBUG_PRINT_SOCKET

//nc -U -l -k /tmp/kode.socket

//----------

#include "kode.h"
#include "plugin/kode_plugin.h"

#include "demo/demo_descriptor.h"
#include "demo/demo_instance.h"

//----------

KODE_PLUGIN_ENTRYPOINT(demo_descriptor,demo_instance);
