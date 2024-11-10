add_executable(ENGINE_EDITOR
    "src/main.cpp"
    "src/Application.cpp"
    "src/Application.hpp"
    "src/Editor/displays/IDisplay.hpp"
    "src/Editor/displays/SceneDisplay.hpp"
    "src/Editor/displays/SceneDisplay.cpp"
    "src/Editor/displays/LogDisplay.hpp"
    "src/Editor/displays/LogDisplay.cpp"
    "src/Editor/utilities/editor_textures.hpp"
    "src/Editor/utilities/EditorFramebuffer.hpp"
    "src/Editor/displays/TilesetDisplay.hpp"
    "src/Editor/displays/TilesetDisplay.cpp"
    "src/Editor/systems/GridSystem.hpp"
    "src/Editor/systems/GridSystem.cpp"
    "src/Editor/displays/TilemapDisplay.hpp"
    "src/Editor/displays/TilemapDisplay.cpp"
    "src/Editor/displays/AssetDisplay.hpp"
    "src/Editor/displays/AssetDisplay.cpp"
    "src/Editor/utilities/EditorUtilities.hpp"
    "src/Editor/scenes/SceneObject.hpp"
    "src/Editor/scenes/SceneObject.cpp"
    "src/Editor/scenes/SceneManager.hpp"
    "src/Editor/scenes/SceneManager.cpp"
    )

target_include_directories(
    ENGINE_EDITOR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    
target_link_libraries(ENGINE_EDITOR 
    PRIVATE ENGINE_WINDOW ENGINE_RENDERING ENGINE_LOGGER ENGINE_CORE ENGINE_SOUNDS ENGINE_PHYSICS
    PUBLIC glm::glm glad::glad soil2 EnTT::EnTT sol2 imgui::imgui)

add_resources(TARGET Assets IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets OUT_DIR assets)
add_dependencies(ENGINE_EDITOR Copy-Assets)

set_target_properties(ENGINE_EDITOR PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/")

target_compile_options(
    ENGINE_EDITOR PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)