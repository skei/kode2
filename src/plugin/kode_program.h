#ifndef kode_program_included
#define kode_program_included
//----------------------------------------------------------------------

//struct KODE_ProgramOptions {
//  bool canAutomate = true;
//};

//----------------------------------------------------------------------

class KODE_Program {
public:
  //KODE_ProgramOptions options;
  const char* name = "";
  float       num_parameters;
  float*      parameter_values;
};

//----------------------------------------------------------------------

typedef KODE_Array<KODE_Program*> KODE_Programs;

//----------------------------------------------------------------------
#endif
