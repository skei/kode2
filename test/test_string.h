
#include "base/kode.h"
#include "base/types/kode_dynamic_string.h"

KODE_DynamicString str;

int main() {
  str = "hello world ";
  str += 32;
  str = str + "_00";
  str.upperCase();
  str.replace(' ','-');
  str.replace("WORLD","QWE");
  const char* cstr = str;
  KODE_Print("%s\n",cstr);
  return 0;
}
