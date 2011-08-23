#include <iostream>
#include "src/kernel.h"

int main(int argc, char **argv) {
  //some tests
  DataPointerFlags dp = create_data();
  dp = data_to_pointer(dp);
  data_set(dp, 10, 4);
  std::cout << (int)data_get(dp, 10) << std::endl;
  data_set(dp, 100, 32);
  std::cout << (int)data_get(dp, 105) << " " << (int)data_get(dp, 100) << " " << (int)data_get(dp, 10) << std::endl;
  
  
  run();
}
