#include "Registry.hpp"

namespace ENGINE_CORE::ECS
{

	Registry::Registry()
		: m_pRegistry{ nullptr }
	{
		m_pRegistry = std::make_unique<entt::registry>();
	}

}