add_library(ENGINE_WINDOW
    "include/Windowing/Inputs/Keyboard.hpp"
    "src/Keyboard.cpp"
    "include/Windowing/Inputs/Keys.hpp"
    "include/Windowing/Inputs/Button.hpp"
    "src/Button.cpp"
    "include/Windowing/Window/Window.hpp"
    "src/Window.cpp")

target_include_directories(
    ENGINE_WINDOW PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ENGINE_WINDOW PUBLIC ENGINE_UTILITIES ENGINE_LOGGER)

target_compile_options(
    ENGINE_WINDOW PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)