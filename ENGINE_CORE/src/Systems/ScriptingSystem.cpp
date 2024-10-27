#include "Core/Systems/ScriptingSystem.hpp"
#include "Core/ECS/Components/ScriptComponent.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"
#include "Core/ECS/Components/SpriteComponent.hpp"
#include "Core/ECS/Components/AnimationComponent.hpp"
#include "Core/ECS/Components/BoxColliderComponent.hpp"
#include "Core/ECS/Components/CircleColliderComponent.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/Scripting/GlmLuaBindings.hpp"
#include "Core/Scripting/InputManager.hpp"
#include "Core/Scripting/SoundBindings.hpp"
#include "Core/Scripting/RendererBindings.hpp"
#include "Core/Resources/AssetManager.hpp"
#include <EngineUtils/Timer.hpp>
#include <Logger/Logger.hpp>


using namespace ENGINE_CORE::ECS;

namespace ENGINE_CORE::Systems
{

    ScriptingSystem::ScriptingSystem(ENGINE_CORE::ECS::Registry& registry)
        : m_Registry(registry), m_bMainLoaded{false}
    { }


    bool ScriptingSystem::LoadMainScript(sol::state& lua)
    {
        try
        {
            auto result = lua.safe_script_file("./assets/scripts/main.lua");
        }
        catch(const sol::error& err)
        {
            ENGINE_ERROR("Error loading the Main Lua Script: {0}", err.what());
            return false;
        }

        sol::table main_lua = lua["main"];

        sol::optional<sol::table> bUpdateExists = main_lua[1];
        if(bUpdateExists == sol::nullopt)
        {
            ENGINE_ERROR("There is no update function in main.lua");
            return false;
        }
        sol::table update_script = main_lua[1];
        sol::function update = update_script["update"];

        sol::optional<sol::table> bRenderExists = main_lua[2];
        if(bRenderExists == sol::nullopt)
        {
            ENGINE_ERROR("There is no render function in main.lua");
            return false;
        }
        sol::table render_script = main_lua[2];
        sol::function render = render_script["render"];
        
        ENGINE_CORE::ECS::Entity mainLuaScript{m_Registry, "main_script", ""};
        mainLuaScript.AddComponent<ENGINE_CORE::ECS::ScriptComponent>(
            ENGINE_CORE::ECS::ScriptComponent{
                .update = update,
                .render = render
            }
        );

        m_bMainLoaded = true;
        return true;
    }


    void ScriptingSystem::Update()
    {
        if(!m_bMainLoaded)
        {
            ENGINE_ERROR("Main Lua Script has not been loaded");
            return;
        }

        auto view = m_Registry.GetRegistry().view<ENGINE_CORE::ECS::ScriptComponent>();

        for (const auto& entity : view)
        {
            ENGINE_CORE::ECS::Entity ent{m_Registry, entity};
            if(ent.GetName() != "main_script")
                continue;

            auto& script = ent.GetComponent<ENGINE_CORE::ECS::ScriptComponent>();
            auto error = script.update(entity);
            if(!error.valid())
            {
                sol::error err = error;
                ENGINE_ERROR("Error running the Update Script: {0}", err.what());
            }
        }

        auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
		lua->collect_garbage();
    }


    void ScriptingSystem::Render()
    {
        if(!m_bMainLoaded)
        {
            ENGINE_ERROR("Main Lua Script has not been loaded");
            return;
        }

        auto view = m_Registry.GetRegistry().view<ENGINE_CORE::ECS::ScriptComponent>();

        for (const auto& entity : view)
        {
            ENGINE_CORE::ECS::Entity ent{m_Registry, entity};
            if(ent.GetName() != "main_script")
                continue;

            auto& script = ent.GetComponent<ENGINE_CORE::ECS::ScriptComponent>();
            auto error = script.render(entity);
            if(!error.valid())
            {
                sol::error err = error;
                ENGINE_ERROR("Error running the Render Script: {0}", err.what());
            }
        }

        auto& lua = m_Registry.GetContext<std::shared_ptr<sol::state>>();
		lua->collect_garbage();
    }


    auto create_timer = [](sol::state& lua) {
        using namespace ENGINE_UTIL;
        lua.new_usertype<Timer>(
            "Timer",
            sol::call_constructor,
            sol::constructors<Timer()>(),
            "start", &Timer::Start,
            "stop", &Timer::Stop,
            "pause", &Timer::Pause,
            "resume", &Timer::Resume,
            "is_paused", &Timer::IsPaused,
            "is_running", &Timer::IsRunning,
            "elapsed_ms", &Timer::ElapsedMS,
            "elapsed_sec", &Timer::ElapsedSec,
            "restart", [](Timer& timer) {
                if(timer.IsRunning())
                    timer.Stop();
                timer.Start();
            }
        );
    };


    void ScriptingSystem::RegisterLuaBinding(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
    {
        ENGINE_CORE::Scripting::GLMBindings::CreateGLMBindings(lua);
        ENGINE_CORE::InputManager::CreateLuaInputBindings(lua);
        ENGINE_RESOURCES::AssetManager::CreateLuaAssetManager(lua, registry);
        ENGINE_CORE::Scripting::SoundBinder::CreateSoundBind(lua, registry);
        ENGINE_CORE::Scripting::RendererBinder::CreateRenderingBind(lua, registry);

        create_timer(lua);

        Registry::CreateLuaRegistryBind(lua, registry);
        Entity::CreateLuaEntityBinding(lua, registry);
        TransformComponent::CreateLuaTransformBind(lua);
        SpriteComponent::CreateSpriteLuaBind(lua, registry);
        AnimationComponent::CreateAnimationLuaBind(lua);
        BoxColliderComponent::CreateLuaBoxColliderBind(lua);
        CircleColliderComponent::CreateLuaCircleColliderBind(lua);

        Entity::RegisterMetaComponent<TransformComponent>();
        Entity::RegisterMetaComponent<SpriteComponent>();
        Entity::RegisterMetaComponent<AnimationComponent>();
        Entity::RegisterMetaComponent<BoxColliderComponent>();
        Entity::RegisterMetaComponent<CircleColliderComponent>();

        Registry::RegistryMetaComponent<TransformComponent>();
        Registry::RegistryMetaComponent<SpriteComponent>();
        Registry::RegistryMetaComponent<AnimationComponent>();
        Registry::RegistryMetaComponent<BoxColliderComponent>();
        Registry::RegistryMetaComponent<CircleColliderComponent>();
    }


    void ScriptingSystem::RegisterLuaFunctions(sol::state& lua)
    {
        lua.set_function(
            "run_script", [&](const std::string& path)
            {
                try
                {
                    lua.safe_script_file(path);
                }
                catch(const sol::error& error)
                {
                    ENGINE_ERROR("Error loading Lua Script: {}", error.what());
                    return false;
                }

                return true;
                
            }
        );
    }
}