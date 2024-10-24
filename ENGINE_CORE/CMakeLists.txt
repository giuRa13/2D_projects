add_library(ENGINE_CORE
    "include/Core/ECS/Entity.hpp"
    "include/Core/ECS/Entity.inl"
    "src/ECS/Entity.cpp"
    "include/Core/ECS/Registry.hpp"
    "include/Core/ECS/Registry.inl"
    "src/ECS/Registry.cpp"
    "include/Core/ECS/MetaUtilities.hpp"
    "src/ECS/MetaUtilities.cpp"
    
    # Components
    "include/Core/ECS/Components/Identification.hpp"
    "include/Core/ECS/Components/SpriteComponent.hpp"
    "src/ECS/Components/SpriteComponent.cpp"
    "include/Core/ECS/Components/TransformComponent.hpp"
    "src/ECS/Components/TransformComponent.cpp"
    "include/Core/ECS/Components/ScriptComponent.hpp"
    "src/ECS/Components/AnimationComponent.cpp"
    "include/Core/ECS/Components/AnimationComponent.hpp"
    "include/Core/ECS/Components/BoxColliderComponent.hpp"
    "src/ECS/Components/BoxColliderComponent.cpp"
    "include/Core/ECS/Components/CircleColliderComponent.hpp"
    "src/ECS/Components/CircleColliderComponent.cpp"

    # Resources
    "include/Core/Resources/AssetManager.hpp"
    "src/Resources/AssetManager.cpp"

    #Scripting
    "include/Core/Scripting/GlmLuaBindings.hpp"
    "src/Scripting/GlmLuaBindings.cpp"
    "include/Core/Scripting/InputManager.hpp"
    "src/Scripting/InputManager.cpp"

    #Systems
    "include/Core/Systems/ScriptingSystem.hpp"
    "src/Systems/ScriptingSystem.cpp"
    "include/Core/Systems/RenderSystem.hpp"
    "src/Systems/RenderSystem.cpp"
    "include/Core/Systems/AnimationSystem.hpp"
    "src/Systems/AnimationSystem.cpp"
    )



target_include_directories(
    ENGINE_CORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(ENGINE_CORE PUBLIC
    glm::glm EnTT::EnTT sol2 glad::glad SDL2::SDL2
    #$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    #$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    ENGINE_LOGGER ENGINE_UTILITIES 
	ENGINE_RENDERING ENGINE_WINDOW)

target_compile_options(
    ENGINE_CORE PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILE_FLAGS}>)