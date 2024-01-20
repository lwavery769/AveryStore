#pragma once

#include "../../include/entt/single_include/entt/entt.hpp"

#include "Componets.h"
#include "core/Timestep.h"

namespace ALStore {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};

}