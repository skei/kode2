
#define KODE_GUI_XCB

#include "kode.h"
#include "plugin/kode_descriptor.h"

int main(void) {
  KODE_Descriptor descriptor;
  uint8_t* id = descriptor.getLongId();
  KODE_DPrint("%02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",id[0],id[1],id[2],id[3],id[4],id[5],id[6],id[7],id[8],id[9],id[10],id[11],id[12],id[13],id[14],id[15]);
  const char* version = descriptor.getVersionText();
  KODE_DPrint("%s\n",version);
return 0;
}
