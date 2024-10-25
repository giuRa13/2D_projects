#pragma oncce

#include <sol/sol.hpp>
#include <SDL2/SDL.h>


namespace ENGINE_CORE::ECS
{

    struct AnimationComponent
    {
        int numFrames{1}, frameRate{1}, frameOffset{0}, currentFrame{0}; 
        int startTime{ static_cast<int>(SDL_GetTicks()) };
        bool bVertical{false}, bLooped{false};

        static void CreateAnimationLuaBind(sol::state& lua);
    };

}