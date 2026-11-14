add_library(factory_model INTERFACE)
target_include_directories(factory_model INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/factory>
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:include/factory>)
target_compile_features(factory_model INTERFACE cxx_std_20)

add_subdirectory(kernel)
add_subdirectory(api)
