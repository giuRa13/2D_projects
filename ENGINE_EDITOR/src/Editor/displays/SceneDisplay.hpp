#pragma once

#include "../displays/IDisplay.hpp"
#include <Core/ECS/Registry.hpp>



namespace ENGINE_EDITOR
{

    class SceneDisplay : public IDisplay
    {

    private:
        ENGINE_CORE::ECS::Registry& m_Registry;


    public:
        SceneDisplay(ENGINE_CORE::ECS::Registry& registry);
        ~SceneDisplay() = default;

        virtual void Draw() override;

    };

}