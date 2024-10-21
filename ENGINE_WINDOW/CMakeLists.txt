add_library(ENGINE_WINDOW
    "include/Windowing/Window/Window.hpp"
    "src/Window.cpp")

target_include_directories(
    ENGINE_WINDOW PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ENGINE_WINDOW PUBLIC ENGINE_UTILITIES)

target_compile_options(
    ENGINE_WINDOW PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)