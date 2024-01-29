#include "mypch.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"
#include "render/Render2D.h"
#include "render/Maths.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/string_cast.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
using namespace std;

namespace ALStore {
	static const uint32_t s_MapWidth = 24;
	static const char* s_MapTiles =
		"WWWWWSSSSSSSSSSWWWWWWWWW"
		"WWWWTGGGGGGGGGGUWWWWWWWW"
		"WWWWTGGGGGGGGGGRSSSWWWWW"
		"WWWWTGGGGGGGGGGGGGGUWWWW"
		"WWWWTGGGGGGGGGGGGGGUWWWW"
		"WWWWTGGGGGGGGGGGGGGUWWWW"
		"WWWWTGGGGGGGGGGGGGGUWWWW"
		"WWWWTGGGGGGGGGGGGGGUWWWW"
		"WWWWTGGGGGGGGGGGGQVWWWWW"
		"WWWWWVVVVVVVVVVVVWWWWWWW";

	static void DoMath(const glm::mat4& transform)
	{

	}
	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{

	}

	Scene::Scene() : m_size({ 1.0, 1.0 }), m_position({ 0.0, 0.0, 0.0 })
	{
		m_SpriteSheet = std::make_shared<Texture2D>("assets/textures/RPGpack_sheet_2X.png");

#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name) {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
	void Scene::LoadXML(std::string filePath) {
		//HZ_PROFILE_FUNCTION();
		enum {
			Translation = 1, Rotation = 2, Scale = 3,
			Color = 4, Size = 5, Texture = 6, Name = 7
		};
		if (localMap.empty())
		{
			localMap["Translation"] = Translation;
			localMap["Rotation"] = Rotation;
			localMap["Scale"] = Scale;
			localMap["Color"] = Color;
			localMap["Size"] = Size;
			localMap["Texture"] = Texture;
			localMap["Name"] = Name;
		}
		ifstream theFile(filePath);
		vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		doc.parse<0>(&buffer[0]);

		root_node = doc.first_node("entities");
		xml_node<>* nodeFrame = doc.first_node()->first_node();

		while (nodeFrame)
		{
			printf("ID = %s\n", nodeFrame->first_attribute("ID")->value());
			//entity.AddComponent<IDComponent>((uint64_t)nodeFrame->first_attribute("ID")->value());
			printf("TagComponent = %s\n", nodeFrame->first_node("TagComponent")->value());
			//entity.AddComponent<TagComponent>(nodeFrame->first_node("TagComponent")->value());
	//		 { 
//				m_Registry.create(), this 

			Entity entity = CreateEntityWithUUID((uint64_t)nodeFrame->first_attribute("ID")->value(),
				nodeFrame->first_node("TagComponent")->value());
			//	};

				//TransformComponent
			glm::vec3 entTranslation, entRotation, entScale;
			xml_node<>* TransNode = nodeFrame->first_node()->next_sibling();
			for (xml_node<>* child = TransNode->first_node(); child; child = child->next_sibling()) {
				if (child->type() == node_element) {
					int val = localMap[child->name()];
					switch (val)
					{
					case Translation: {
						entTranslation = glm::make_vec3(child->value());
						printf("Translation = %s\n", child->value()); break; }
					case Rotation: {
						entRotation = glm::make_vec3(child->value());
						printf("Rotation = %s\n", child->value()); break;	}
					case Scale: {
						entScale = glm::make_vec3(child->value());
						printf("Scale = %s\n", child->value()); break;	}
					default:  break;
					}
				}
			}
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), entRotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), entRotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), entRotation.z, { 0, 0, 1 });

			glm::mat4 entTransform = glm::translate(
				glm::mat4(1.0f), entTranslation)
				* rotation
				* glm::scale(glm::mat4(1.0f), entScale);
			entity.GetComponent<TransformComponent>().Transform = entTransform;

			//SpriteRendererComponent
			entity.AddComponent<SpriteRendererComponent>();
			xml_node<>* SRCnode = nodeFrame->first_node()->next_sibling()->next_sibling();
			for (xml_node<>* child = SRCnode->first_node(); child; child = child->next_sibling()) {
				if (child->type() == node_element) {
					int val = localMap[child->name()];
					switch (val)
					{
					case Color: {
						entity.GetComponent<SpriteRendererComponent>().Color = glm::make_vec4(child->value());
						printf("SRC color = %s\n", child->value()); break; }
					case Size: {
						entity.GetComponent<SpriteRendererComponent>().Size = glm::make_vec2(child->value());
						printf("SRC Size = %s\n", child->value()); break;
					}
					case Texture: {
						std::string str(child->value());
						s_TextureLibrary.Load("assets/textures/" + str);
						std::shared_ptr<Texture2D> enttTexture = s_TextureLibrary.Get(str);
						//entity.GetComponent<SpriteRendererComponent>().mapPOS = s_TextureLibrary.MapPos(str);
						entity.GetComponent<SpriteRendererComponent>().Texture = enttTexture;
						printf("SRC Texture = %s\n", child->value()); break;	}
					case Name: {
						entity.GetComponent<SpriteRendererComponent>().texNamne = child->value();
						printf("SRC name = %s\n", child->value()); break; }
					default:  break;
					}
				}
			}
			nodeFrame = nodeFrame->next_sibling();
		}
	}
	void Scene::drawMap() {
		//HZ_PROFILE_FUNCTION();
		float xCoord = 0.0f; float yCoord = 0.0f;
		m_SpriteSheet->Bind();
		m_MapWidth = s_MapWidth; m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
		for (uint32_t y = 0; y < m_MapHeight; y++) {
			for (uint32_t x = 0; x < m_MapWidth; x++) {
				char tileType = s_MapTiles[x + y * m_MapWidth];
				switch (tileType) {
				case 'D': xCoord = 6.0f; yCoord = 11.0f; break;
				case 'G': xCoord = 1.0f; yCoord = 11.0f; break;
				case 'Q': xCoord = 10.0f; yCoord = 12.0f; break;
				case 'R': xCoord = 10.0f; yCoord = 10.0f; break;
				case 'S': xCoord = 11.0f; yCoord = 10.0f; break;
				case 'T': xCoord = 12.0f; yCoord = 11.0f; break;
				case 'U': xCoord = 10.0f; yCoord = 11.0f; break;
				case 'V': xCoord = 11.0f; yCoord = 12.0f; break;
				case 'W': xCoord = 11.0f; yCoord = 11.0f; break;
				case 'X': xCoord = 12.0f; yCoord = 10.0f; break;
				case 'Y': xCoord = 12.0f; yCoord = 12.0f; break;
				default: break;
				}
				m_position = { x - m_MapWidth / 2.0f, m_MapHeight - y - m_MapHeight / 2.0f, 0.0f };
				//quotient = dividend / divisor;remainder = dividend % divisor;		
				coords = { xCoord, yCoord };
				draw.DrawTile(Maths::getTransform(m_size, m_position), m_SpriteSheet, coords, tintColor);
			}
		}
		drawRoads();
	}
	void Scene::drawRoads() {
		float xCoord = 14.0f; float yCoord = 9.0f; m_position = { -2.0f,0.0f,0.0f }; glm::vec2 rSize = { 0.5,0.5 };
		coords = { xCoord, yCoord };
		for (int i = 0; i < 16; i++) {
			m_position.x += .5f;
			draw.DrawTile(Maths::getTransform(rSize, m_position), m_SpriteSheet, coords, tintColor);
		}
	}
	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			//Render2D::DrawSprite(transform, sprite.Color, -1);
		}

		/*   entt::registry& reg = m_ActiveScene->Reg();
   for (auto&& [entity, trnf, txt]
	   : reg.group<TransformComponent>(entt::get<SpriteRendererComponent>).each()) {
		   txt.Texture->Bind();
   }
   auto view = reg.view<IDComponent>();
   for (auto entity : view)
   {
	   IDComponent& entID = view.get<IDComponent>(entity);

   }
   auto group = reg.group<TransformComponent>(entt::get<SpriteRendererComponent>);

   e_Truck2.GetComponent<SpriteRendererComponent>().Texture->Bind();
   m_Renderer->DrawSprite(e_Truck2.GetComponent<TransformComponent>().Transform, e_Truck2.GetComponent<SpriteRendererComponent>(), e_Truck2.GetComponent<IDComponent>().eID);
*/
	}

}