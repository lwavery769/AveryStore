#pragma once

#include "../../include/entt/single_include/entt/entt.hpp"

//#include "Componets.h"
#include "core/Timestep.h"

namespace ALStore {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
		friend class Entity;
	};

}