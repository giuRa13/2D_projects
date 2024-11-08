#pragma once

#include <memory>

namespace ENGINE_RENDERING
{
    class RectBatchRenderer;
    class Camera2D;
}


namespace ENGINE_EDITOR
{

    class GridSystem
    {

    private:
        std::unique_ptr<ENGINE_RENDERING::RectBatchRenderer> m_pBatchRenderer;


    public:
        GridSystem();
        ~GridSystem() = default;

        void Update(/*SceneObject& currentScene,*/ ENGINE_RENDERING::Camera2D& camera);

    };

}