#include "Core/Systems/AnimationSystem.hpp"
#include "Core/ECS/Components/AnimationComponent.hpp"
#include "Core/ECS/Components/SpriteComponent.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"
#include "Core/CoreUtilities/CoreUtilities.hpp"
#include <SDL2/SDL.h>

using namespace ENGINE_CORE::ECS;


namespace ENGINE_CORE::Systems
{

    AnimationSystem::AnimationSystem(ENGINE_CORE::ECS::Registry& registry)
        : m_Registry{registry}
    { }


    void AnimationSystem::Update()
    {
        auto view = m_Registry.GetRegistry().view<AnimationComponent, SpriteComponent, TransformComponent>();

        if (view.size_hint() < 1)
			return;

		auto& camera = m_Registry.GetContext<std::shared_ptr<ENGINE_RENDERING::Camera2D>>();

        for (auto entity : view)
        {
            const auto& transform = view.get<TransformComponent>(entity);
            auto& sprite = view.get<SpriteComponent>(entity);
            auto& animation = view.get<AnimationComponent>(entity);

            if (!ENGINE_CORE::EntityInView(transform, sprite.width, sprite.height, *camera))
				continue;

            if(animation.numFrames <= 0)
                continue;

            if (!animation.bLooped && animation.currentFrame >= animation.numFrames -1)
                continue;

            animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameRate / 1000) % animation.numFrames;

            if(animation.bVertical)
            {
                sprite.uvs.v = animation.currentFrame * sprite.uvs.uv_width;
                sprite.uvs.u = animation.frameOffset * sprite.uvs.uv_width;
            }
            else
            {
                sprite.uvs.u = (animation.currentFrame * sprite.uvs.uv_width) + (animation.frameOffset * sprite.uvs.uv_width);
            }
        }
    }
}