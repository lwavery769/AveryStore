#pragma once
#include "core/core.h"
#include "Scene.h"
#include "entt/single_include/entt/entt.hpp"

namespace ALStore {
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			AL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			bool* p;
			p = m_Scene->m_Registry.try_get();
			if (p != NULL) return true;
			else return false;
			//return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

	///operator bool() const { return m_EntityHandle != 0; }	operator bool() const { return m_EntityHandle != 0; }
	private:
		entt::entity m_EntityHandle;
		Scene* m_Scene = nullptr;
	};
}