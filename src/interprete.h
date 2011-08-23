#pragma once
#include "memory.h"

struct ProgramData {
  DataPointerFlags data[16];
  uint32_t reg[16];
  uint8_t validity;
  DataPointer program;
  bool ended;  
  int ip;
};

ProgramData programData;

int interprete() {
}

void start_interprete(CapabilityExecute exe) {
  if (exe.cap.program == 2) {
    //execute some internal stuff [KERNEL ETC.]
    //TODO
  }
  else {
    programData.data[0] = exe.cap.data;
    programData.data[1] = exe.data;
    programData.validity = 2;
    programData.program = exe.cap.program;
    programData.ip = 0;
    programData.ended = false;
    while (!programData.ended)
      interprete();
  }
}