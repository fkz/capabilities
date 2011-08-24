
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)
add_executable(test1 dataobject.cpp ../src/memory.cpp)
target_link_libraries(test1 libboost_unit_test_framework.a)

add_test(my_test test1)