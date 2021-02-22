
#define KODE_NO_GUI

//----------

#include "kode.h"
#include "3d/kode_mdl_converter.h"

//----------

//const char* mdl_path  = "../data/meshes/baron.mdl";
//const char* name = "baron";

int main(int argc, char** argv) {

  if (argc != 3) {
    KODE_DPrint("Usage: convert <mdl_path> <name>\n");
    exit(1);
  }

  char* mdl = argv[1];
  char* name = argv[2];

  KODE_MdlConverter converter;


  bool res = converter.read(mdl);

  if (!res) {
    KODE_DPrint("Error! Couldn't read %s\n",mdl);
    exit(1);
  }

  res = converter.write(name);

  if (!res) {
    KODE_DPrint("Error! Couldn't write %s\n",name);
    exit(1);
  }

  //importer.write_obj("baron",0,0);
  converter.cleanup();

  return 0;
}
