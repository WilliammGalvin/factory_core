add_library(factory_core STATIC simulator.cpp serialize.cpp)
add_library(factory::core ALIAS factory_core)

target_link_libraries(factory_core
    PUBLIC  factory_model
    PRIVATE factory_kernel)
