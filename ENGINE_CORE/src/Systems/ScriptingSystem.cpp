#include "Core/Systems/ScriptingSystem.hpp"
#include "Core/ECS/Components/ScriptComponent.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"
#include "Core/ECS/Components/SpriteComponent.hpp"
#include "Core/ECS/Components/AnimationComponent.hpp"
#include "Core/ECS/Components/BoxColliderComponent.hpp"
#include "Core/ECS/Components/CircleColliderComponent.hpp"
#include "Core/ECS/Components/PhysicsComponent.hpp"
#include "Core/ECS/Components/TextComponent.hpp"
#include "Core/ECS/Components/RigidBodyComponent.hpp"
#include "Core/ECS/Entity.hpp"
#include "Core/ECS/MainRegistry.hpp"
#include "Core/Scripting/GlmLuaBindings.hpp"
#include "Core/Scripting/InputManager.hpp"
#include "Core/Scripting/SoundBindings.hpp"
#include "Core/Scripting/RendererBindings.hpp"
#include "Core/Scripting/UserDataBindings.hpp"
#include "Core/Scripting/ContactListenerBinds.hpp"
#include "Core/Resources/AssetManager.hpp"
#include <EngineUtils/Timer.hpp>
#include <EngineUtils/RandomGenerator.hpp>
#include "Core/CoreUtilities/CoreEngineData.hpp"
#include "Core/CoreUtilities/FollowCamera.hpp"
#include "Core/CoreUtilities/CoreUtilities.hpp"
#include "Core/States/State.hpp"
#include "Core/States/StateStack.hpp"
#include "Core/States/StateMachine.hpp"
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

    // LUA LOG //////////////////////////////////
    auto create_lua_logger = []( sol::state& lua ) {

        auto& logger = ENGINE_LOGGER::Logger::GetInstance();
        lua.new_usertype<ENGINE_LOGGER::Logger>(
            "Logger",
            sol::no_constructor,
            "log",
            [ & ]( const std::string_view message ) { logger.LuaLog( message ); },
            "warn",
            [ & ]( const std::string_view message ) { logger.LuaWarn( message ); },
            "error",
            [ & ]( const std::string_view message ) { logger.LuaError( message ); } );
        
        auto logResult = lua.safe_script( R"(
                    function ZZZ_Log(message, ...)
                        Logger.log(string.format(message, ...))
                    end
                )" );
        if ( !logResult.valid() )
        {
            ENGINE_ERROR( "Failed to initialize lua logs" );
        }

        auto warnResult = lua.safe_script( R"(
                    function ZZZ_Warn(message, ...)
                        Logger.warn(string.format(message, ...))
                    end
                )" );
        if ( !warnResult.valid() )
        {
            ENGINE_ERROR( "Failed to initialize lua warnings" );
        }

        auto errorResult = lua.safe_script( R"(
                    function ZZZ_Error(message, ...)
                        Logger.error(string.format(message, ...))
                    end
                )" );
        if ( !errorResult.valid() )
        {
            ENGINE_ERROR( "Failed to initialize lua errors" );
        }

        lua.set_function( "ENGINE_log", []( const std::string& message, const sol::variadic_args& args, sol::this_state s ) {
            try
            {
                sol::state_view L = s;
                sol::protected_function log = L[ "ZZZ_Log" ];
                auto result = log( message, args );
                if ( !result.valid() )
                {
                    sol::error error = result;
                    throw error;
                }
            }
            catch ( const sol::error& error )
            {
                ENGINE_ERROR( "Failed to get lua log: {}", error.what() );
            }
        } );

        lua.set_function( "ENGINE_warn", []( const std::string& message, const sol::variadic_args& args, sol::this_state s ) {
            try
            {
                sol::state_view L = s;
                sol::protected_function warn = L[ "ZZZ_Warn" ];
                auto result = warn( message, args );
                if ( !result.valid() )
                {
                    sol::error error = result;
                    throw error;
                }
            }
            catch ( const sol::error& error )
            {
                ENGINE_ERROR( "Failed to get lua warning: {}", error.what() );
            }
        } );

        lua.set_function( "ENGINE_error", []( const std::string& message, const sol::variadic_args& args, sol::this_state s ) {
            try
            {
                sol::state_view L = s;
                sol::protected_function err = L[ "ZZZ_Error" ];
                auto result = err( message, args );
                if ( !result.valid() )
                {
                    sol::error error = result;
                    throw error;
                }
            }
            catch ( const sol::error& error )
            {
                ENGINE_ERROR( "Failed to get lua errors: {}", error.what() );
            }
        } );

	auto assertResult = lua.safe_script( R"(
				ENGINE_assert = assert
				assert = function(arg1, message, ...)
					if not arg1 then 
						Logger.error(string.format(message, ...))
					end 
					ENGINE_assert(arg1)
				end
			)" );
    };


    void ScriptingSystem::RegisterLuaBinding(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
    {
        ENGINE_CORE::Scripting::GLMBindings::CreateGLMBindings(lua);
        ENGINE_CORE::InputManager::CreateLuaInputBindings(lua, registry);
        ENGINE_RESOURCES::AssetManager::CreateLuaAssetManager(lua);
        ENGINE_CORE::Scripting::SoundBinder::CreateSoundBind(lua);
        ENGINE_CORE::Scripting::RendererBinder::CreateRenderingBind(lua, registry);
        ENGINE_CORE::Scripting::UserDataBinder::CreateLuaUserData(lua);
        ENGINE_CORE::Scripting::ContactListenerBinder::CreateLuaContactListener(lua, registry.GetRegistry());
    
        ENGINE_CORE::FollowCamera::CreateLuaFollowCamera(lua, registry);
        create_timer(lua);
        create_lua_logger( lua );

        ENGINE_CORE::State::CreateLuaStateBind(lua);
        ENGINE_CORE::StateStack::CreateLuaStateStackBinds(lua);
        ENGINE_CORE::StateMachine::CreateLuaStateMachine(lua);

        Registry::CreateLuaRegistryBind(lua, registry);
        Entity::CreateLuaEntityBinding(lua, registry);
        TransformComponent::CreateLuaTransformBind(lua);
        SpriteComponent::CreateSpriteLuaBind(lua);
        AnimationComponent::CreateAnimationLuaBind(lua);
        BoxColliderComponent::CreateLuaBoxColliderBind(lua);
        CircleColliderComponent::CreateLuaCircleColliderBind(lua);
        PhysicsComponent::CreatePhysicsLuaBind(lua, registry.GetRegistry());
        TextComponent::CreateTextLuaBinding(lua);
        RigidBodyComponent::CreateRigidBodyBind(lua);

        Entity::RegisterMetaComponent<TransformComponent>();
        Entity::RegisterMetaComponent<SpriteComponent>();
        Entity::RegisterMetaComponent<AnimationComponent>();
        Entity::RegisterMetaComponent<BoxColliderComponent>();
        Entity::RegisterMetaComponent<CircleColliderComponent>();
        Entity::RegisterMetaComponent<PhysicsComponent>();
        Entity::RegisterMetaComponent<TextComponent>();
        Entity::RegisterMetaComponent<RigidBodyComponent>();

        Registry::RegistryMetaComponent<TransformComponent>();
        Registry::RegistryMetaComponent<SpriteComponent>();
        Registry::RegistryMetaComponent<AnimationComponent>();
        Registry::RegistryMetaComponent<BoxColliderComponent>();
        Registry::RegistryMetaComponent<CircleColliderComponent>();
        Registry::RegistryMetaComponent<PhysicsComponent>();
        Registry::RegistryMetaComponent<TextComponent>();
        Registry::RegistryMetaComponent<RigidBodyComponent>();

        ENGINE_CORE::Scripting::UserDataBinder::register_meta_user_data<ObjectData>();
    }


    void ScriptingSystem::RegisterLuaFunctions(sol::state& lua, ENGINE_CORE::ECS::Registry& registry)
    {
        auto& mainRegistry = MAIN_REGISTRY();

        lua.set_function(
            "ENGINE_run_script", [&](const std::string& path)
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

        lua.set_function("ENGINE_load_script_table", [&](const sol::table& scriptList){
            if(!scriptList.valid() )
            { 
                ENGINE_ERROR("Failed to load script list: Table is invalid"); 
                return;
            }
            for(const auto& [index, script] : scriptList)
            {
                try
                {
                    auto result = lua.safe_script_file(script.as<std::string>());
                    if(!result.valid())
                    {
                        sol::error error = result;
                        throw error;
                    }
                }
                catch(const sol::error& error)
                {
                   	ENGINE_ERROR( "Error loading the main lua script: {0}", error.what() );
		            return;
                }
            }
        });

        lua.set_function("ENGINE_get_ticks", [] { return SDL_GetTicks(); });

		//auto& assetManager = registry.GetContext<std::shared_ptr<ENGINE_RESOURCES::AssetManager>>();
        auto& assetManager = mainRegistry.GetAssetManager();

		lua.set_function("ENGINE_measure_text", [&](const std::string& text, const std::string& fontName) {
			const auto& pFont = assetManager.GetFont(fontName);
			if (!pFont)
			{
				ENGINE_ERROR("Failed to get font [{}] - Does not exist in asset manager!", fontName);
				return -1.f;
			}
			glm::vec2 position{ 0.f }, temp_pos{ position };
			for (const auto& character : text)
				pFont->GetNextCharPos(character, temp_pos);
			
			return std::abs((position - temp_pos).x);
			}
		);

		auto& engine = CoreEngineData::GetInstance();
		
        lua.set_function("GetDeltaTime", [&] { return engine.GetDeltaTime(); });
		lua.set_function("WindowWidth", [&] { return engine.WindowWidth(); });
		lua.set_function("WindowHeight", [&] { return engine.WindowHeight(); });
		lua.set_function("DisablePhysics", [&] { engine.DisablePhysics(); });
		lua.set_function("EnablePhysics", [&] { engine.EnablePhysics(); });
		lua.set_function("IsPhysicsEnabled", [&] { return engine.IsPhysicsEnabled(); });
        lua.set_function("DisableRenderColliders", [&] { engine.DisableColliderRender(); });
		lua.set_function("EnableRenderColliders", [&] { engine.EnableColliderRender(); });
		lua.set_function("IsRenderCollidersEnabled", [&] { return engine.RenderCollidersEnabled(); });


		lua.new_usertype<ENGINE_UTIL::RandomGenerator>(
			"Random",
			sol::call_constructor,
			sol::constructors<ENGINE_UTIL::RandomGenerator(uint32_t, uint32_t), ENGINE_UTIL::RandomGenerator()>(),
			"get_float", &ENGINE_UTIL::RandomGenerator::GetFloat,
			"get_int", &ENGINE_UTIL::RandomGenerator::GetInt
		);

        lua.set_function("ENGINE_EntityInView", [&](const TransformComponent& transform, float width, float height) {
			auto& camera = registry.GetContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>();
			return ENGINE_CORE::EntityInView(transform, width, height, *camera);
			}
		);


    }
}