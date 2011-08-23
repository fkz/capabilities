#include "dataobject.h"
#include "interprete.h"

void run () {
  //TODO: do some more extended scheduling
  while (exe_size[exe_actual]) {
    uint32_t last_exe_actual = exe_actual;
    exe_actual = (exe_actual + 1) % 2;
    exe_size[exe_actual] = 0;
    for (size_t i = 0; i != exe_size[last_exe_actual]; ++i) {
      start_interprete (exe[i][last_exe_actual]);
    }
  }
}