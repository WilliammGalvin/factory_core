add_library(factory_kernel STATIC kernel.cpp)
target_include_directories(factory_kernel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
