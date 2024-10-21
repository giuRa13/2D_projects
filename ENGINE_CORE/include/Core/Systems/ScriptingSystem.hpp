#pragma once

#include "Core/ECS/Registry.hpp"
#include <sol/sol.hpp>


namespace ENGINE_CORE::Systems
{

    class ScriptingSystem
    {

    private:
        ENGINE_CORE::ECS::Registry& m_Registry;
        bool m_bMainLoaded;
    
    public:
    	ScriptingSystem( ENGINE_CORE::ECS::Registry& registry );
        ~ScriptingSystem() = default;

        bool LoadMainScript( sol::state& lua );
        void Update();
        void Render();

    };

}