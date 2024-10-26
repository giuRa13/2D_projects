add_library(ENGINE_SOUNDS
    "include/Sounds/Essentials/SoundParams.hpp"
    "include/Sounds/Essentials/Music.hpp"
    "src/Music.cpp"
    "include/Sounds/MusicPlayer/MusicPlayer.hpp"
    "src/MusicPlayer.cpp"
)

target_include_directories(
    ENGINE_SOUNDS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ENGINE_SOUNDS
    PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
    PUBLIC ENGINE_UTILITIES)

target_compile_options(
        ENGINE_SOUNDS PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)