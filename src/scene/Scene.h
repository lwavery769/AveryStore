#pragma once

#include "../../include/entt/single_include/entt/entt.hpp"
#include "render/Render2D.h"
#include "render/Texture2D.h"
#include "render/TextureLibrary.h"
#include "rapidXML/rapidxml.hpp"
#include <map>
#include <tchar.h>
#include "core/Timestep.h"
using namespace rapidxml;
namespace ALStore {
	class Entity;
	static std::map<std::string, int> localMap;
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		// TEMP
		entt::registry& Reg() { return m_Registry; }
		void LoadXML(std::string filePath);
		void drawMap(); void drawRoads();
		void OnUpdate(Timestep ts);
		glm::vec2 m_size;
		glm::vec3 m_position;
		glm::vec4 tintColor = { 1.0, 1.0, 1.0, 1.0 };
		glm::vec2 coords = { 0.0f, 0.0f };
	
	private:
		entt::registry m_Registry;
		xml_document<> doc; xml_node<>* root_node;
		TextureLibrary s_TextureLibrary;
		std::shared_ptr<Texture2D> m_SpriteSheet;
		Render2D draw;
		int divisor, dividend, quotient, remainder;
		glm::vec2 getCoords(int dividend, int divisor);
		size_t m_MapWidth, m_MapHeight;
		friend class Entity;
		friend class XMLClass;
	};

}