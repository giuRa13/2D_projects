#pragma once

#include "../displays/IDisplay.hpp"
#include <Core/ECS/Registry.hpp>



namespace ENGINE_EDITOR
{

    class SceneDisplay : public IDisplay
    {

    private:
        bool m_bPlayScene, m_bSceneLoaded;

        void LoadScene();
        void UnloadScene();
        void RenderScene();

    public:
        SceneDisplay();
        ~SceneDisplay() = default;

        virtual void Draw() override;
        virtual void Update() override;

    };

}