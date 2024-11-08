#pragma once

#include <Rendering/Buffers/FrameBuffer.hpp>
#include <map>


namespace ENGINE_EDITOR
{

    enum class FramebufferType
    {
        TILEMAP, SCENE, NO_TYPE
    };


    struct EditorFramebuffers
    {
        std::map <FramebufferType, std::shared_ptr<ENGINE_RENDERING::FrameBuffer>> mapFramebuffers;
    };

}