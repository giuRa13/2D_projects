add_library(ENGINE_UTILITIES
"include/EngineUtils/SDL_Wrappers.hpp"
"src/SDL_Wrappers.cpp"
"include/EngineUtils/Timer.hpp"
"src/Timer.cpp"
"include/EngineUtils/RandomGenerator.hpp"
"src/RandomGenerator.cpp"
"include/EngineUtils/EngineUtilities.hpp"
)

target_include_directories(
    ENGINE_UTILITIES PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ENGINE_UTILITIES PUBLIC ENGINE_LOGGER SDL2::SDL2 SDL2_mixer glad::glad) 

target_compile_options(
    ENGINE_UTILITIES PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)