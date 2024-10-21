#pragma once

#include <entt/entt.hpp>
#include <sol/sol.hpp>


 namespace ENGINE_CORE::ECS
 {

    class Registry
	{

	private:
		std::unique_ptr<entt::registry> m_pRegistry;


	public:
		Registry();
		~Registry() = default;

		inline entt::registry& GetRegistry() { return *m_pRegistry; }
		inline entt::entity CreateEntity() { return m_pRegistry->create(); }

		template <typename TContext>
		TContext AddToContext(TContext context);
		
		template <typename TContext>
		TContext& GetContext();
		// entt provides: TContext
		// object map that takes any type of variable (each Registry have one)
		// TContext has to be movable and constructable
	};

    template <typename TContext>
	inline TContext Registry::AddToContext(TContext context)
	{
		return m_pRegistry->ctx().emplace<TContext>(context);
	}

	template <typename TContext>
	inline TContext& Registry::GetContext()
	{
		return m_pRegistry->ctx().get<TContext>();
	}

 }