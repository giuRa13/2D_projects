#include "Core/ECS/Entity.hpp"
#include "Core/ECS/Components/Identification.hpp"


namespace ENGINE_CORE::ECS
{

	Entity::Entity(Registry& registry)
		: Entity(registry, "GameObject", "")
	{ }
	
    
    Entity::Entity(Registry& registry, const std::string& name, const std::string& group)
		: m_Registry(registry), m_Entity{registry.CreateEntity()}, m_sName{name}, m_sGroup{group}
	{
		AddComponent<Identification>(Identification{
				.name = name,
				.group = group,
				.entity_id = static_cast<int32_t>(m_Entity)
			}
		);
	}


	Entity::Entity(Registry& registry, const entt::entity& entity)
		: m_Registry(registry), m_Entity(entity), m_sName{""}, m_sGroup{""}
	{
		
		//if(m_Registry.GetRegistry().all_of<Identification>(m_Entity))
		if (HasComponent<Identification>())
		{
			const auto& id = GetComponent<Identification>();
			//auto id = GetComponent<Identification>();
			m_sName = id.name;
			m_sGroup = id.group;
		}
	}

}