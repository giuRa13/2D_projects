#pragma once

//#include "Core/ECS/Components/AllComponents.hpp"
#include "Core/ECS/Components/TransformComponent.hpp"
#include <Rendering/Core/Camera2D.hpp>



namespace ENGINE_CORE 
{

	bool EntityInView(const ENGINE_CORE::ECS::TransformComponent& transform, float width, float height, const ENGINE_RENDERING::Camera2D& camera);

	glm::mat4 RSTModel(const ENGINE_CORE::ECS::TransformComponent& transform, float width, float height);

}