#pragma once

#include <Windowing/Inputs/Keyboard.hpp>
#include <Windowing/Inputs/Mouse.hpp>
#include "Core/ECS/Registry.hpp"
#include <memory>
#include <sol/sol.hpp>

using namespace ENGINE_WINDOWING::Inputs;


namespace ENGINE_CORE
{

    class InputManager
    {
    private:
        std::unique_ptr<Keyboard> m_pKeyboard;
        std::unique_ptr<Mouse> m_pMouse;

        InputManager();
        ~InputManager() = default;
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;

        static void RegisterLuaKeyNames(sol::state& lua);
        static void RegisterLuaMouseBtnames(sol::state& lua);


    public:
        static InputManager& GetInstance();
        
        static void CreateLuaInputBindings(sol::state& lua, ENGINE_CORE::ECS::Registry& registry);

        inline Keyboard& GetKeyboard() { return *m_pKeyboard; }
        inline Mouse& GetMouse() { return *m_pMouse; }

    };
    
} 


