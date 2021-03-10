
#define KODE_NO_GUI

//----------

//#include <stdio.h>
//#include <unistd.h>

#include "kode.h"
#include "3d/kode_mdl_converter.h"

//----------

  //model.tscn
  //model_skins.png
  //model_normal_frames.png
  //model_vertex_frames.png

int main(int argc, char** argv) {

  int   opt         = 0;
  bool  arg_dump    = false;
  char* arg_input_path  = KODE_NULL;
  char* arg_output_name = KODE_NULL;

  if (argc == 1) {
    printf("Usage:\n");
    printf("  -i <path>   path to mdl file\n");
    printf("  -n <name    name of output model/ files\n");
    printf("  -d          dump/inspect mdl file\n");
    exit(1);
  }


  while ((opt = getopt(argc,argv,"di:n:")) != -1) {
    switch(opt) {

      case 'd':
        arg_dump = true;
        break;

      case 'i':
        arg_input_path = optarg;
        break;

      case 'n':
        arg_output_name = optarg;
        break;

      case '?':
        if (optopt == 'i') {
          printf("* missing input path\n");
          exit(2);
        } else if (optopt == 'o') {
          printf("* missing output name\n");
          exit(3);
        } else {
          printf("* invalid option '%c'",optopt);
          exit(4);
      }
      break;
    }
  }

  //------------------------------
  //
  //------------------------------

  KODE_MdlConverter converter;
  bool res = converter.read(arg_input_path);
  if (!res) {
    printf("* Error! Couldn't read '%s'\n",arg_input_path);
    exit(1);
  }
  if (arg_dump) {
    converter.dump();
  }
  else {
    res = converter.write(arg_output_name);
    if (!res) {
      printf("* Error! Couldn't write '%s'\n",arg_output_name);
      //exit(1);
    }
  }
  converter.cleanup();
  return 0;
}
