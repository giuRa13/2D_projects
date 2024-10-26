add_library(ENGINE_RENDERING
    "include/Rendering/Core/Camera2D.hpp"
    "src/Camera2D.cpp"
    "include/Rendering/Essentials/Shader.hpp"
    "src/Shader.cpp"
    "include/Rendering/Essentials/ShaderLoader.hpp"
    "src/ShaderLoader.cpp"
    "include/Rendering/Essentials/Texture.hpp"
    "src/Texture.cpp"
    "include/Rendering/Essentials/TextureLoader.hpp"
    "src/TextureLoader.cpp"
    "include/Rendering/Essentials/Vertex.hpp"
    "include/Rendering/Essentials/BatchTypes.hpp"
    "include/Rendering/Core/BatchRenderer.hpp"
    "src/BatchRenderer.cpp"
    "include/Rendering/Core/Batcher.hpp"
    )


target_include_directories(
    ENGINE_RENDERING PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    
target_link_libraries(ENGINE_RENDERING
    PRIVATE ENGINE_LOGGER
    PUBLIC glm::glm glad::glad soil2)
    
target_compile_options(
    ENGINE_RENDERING PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)