
#define KODE_NO_GUI

//----------

#include "kode.h"
//#include "3d/kode_mdl_converter.h"
#include "3d/kode_mdl_importer.h"

//----------

int main(int argc, char** argv) {

  //if (argc != 2) {
  //  KODE_DPrint("Usage: convert_mdl <path>\n");
  //  exit(1);
  //}
  //char* path = argv[1];

  const char* path = "../data/meshes/baron.mdl";

  KODE_MdlImporter importer;

  bool res;

  res = importer.read(path);
  if (!res) {
    KODE_DPrint("Error! Couldn't import %s\n",path);
    exit(1);
  }

  res = importer.convert();
  importer.print_mdl_header();

  importer.cleanup();

  return 0;
}
