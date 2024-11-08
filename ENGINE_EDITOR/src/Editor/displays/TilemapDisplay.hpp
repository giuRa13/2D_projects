#pragma once

#include "IDisplay.hpp"
#include <string>

namespace ENGINE_RENDERING
{
    class Camera2D;
}


namespace ENGINE_EDITOR
{

    class TilemapDisplay : public IDisplay
    {

    private:
        std::unique_ptr<ENGINE_RENDERING::Camera2D> m_pTilemapCam;

        void RenderTilemap();


    public:
        TilemapDisplay();
        ~TilemapDisplay() = default;

        virtual void Draw() override;
        virtual void Update() override;


    };

}