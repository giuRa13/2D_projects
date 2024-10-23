#pragma once

#include <Windowing/Inputs/Keyboard.hpp>
#include <memory>
#include <sol/sol.hpp>

using namespace ENGINE_WINDOWING::Inputs;


namespace ENGINE_CORE
{

    class InputManager
    {
    private:
        std::unique_ptr<Keyboard> m_pKeyboard;

        InputManager();
        ~InputManager() = default;
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;

        static void RegisterLuaKeyNames(sol::state& lua);


    public:
        static InputManager& GetInstance();
        
        static void CreateLuaInputBindings(sol::state& lua);

        inline Keyboard& GetKeyboard() { return *m_pKeyboard; }

    };
    
} 


