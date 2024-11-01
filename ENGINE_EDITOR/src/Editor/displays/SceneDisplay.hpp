#pragma once

#include <Core/ECS/Registry.hpp>



namespace ENGINE_EDITOR
{

    class SceneDisplay
    {

    private:
        ENGINE_CORE::ECS::Registry& m_Registry;


    public:
        SceneDisplay(ENGINE_CORE::ECS::Registry& registry);
        ~SceneDisplay() = default;

        void Draw();

    };

}