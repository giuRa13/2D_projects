add_library(ENGINE_UTILITIES
"include/SDL_Wrappers.hpp"
"src/SDL_Wrappers.cpp"
"include/Timer.hpp"
"src/Timer.cpp"
)

target_include_directories(
    ENGINE_UTILITIES PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ENGINE_UTILITIES PUBLIC SDL2::SDL2 glad::glad) 

target_compile_options(
    ENGINE_UTILITIES PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)