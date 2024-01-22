#include "mypch.h"
#include "XMLClass.h"
#include "Entity.h"
#include "Components.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include "rapidXML/rapidxml.hpp"
#include <tchar.h>
	using namespace rapidxml;
	using namespace std;
namespace ALStore {

	static std::map<std::string, int> localMapXC;
	XMLClass::XMLClass() {}
	XMLClass::~XMLClass() {}
	bool XMLClass::LoadXML(std::string filePath) {
		enum {
			Translation = 1, Rotation = 2, Scale = 3,
			Color = 4, Size = 5, Texture = 6, Name = 7
		};
		if (localMapXC.empty())
		{
			localMapXC["Translation"] = Translation;
			localMapXC["Rotation"] = Rotation;
			localMapXC["Scale"] = Scale;
			localMapXC["Color"] = Color;
			localMapXC["Size"] = Size;
			localMapXC["Texture"] = Texture;
			localMapXC["Name"] = Name;
		}
		xml_document<> doc;
		xml_node<>* root_node;
		ifstream theFile(filePath);
		vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		doc.parse<0>(&buffer[0]);
		root_node = doc.first_node("entities");
		xml_node<>* nodeFrame = doc.first_node()->first_node();

		while (nodeFrame)
		{
			printf("ID = %s\n", nodeFrame->first_attribute("ID")->value());
			printf("TagComponent = %s\n", nodeFrame->first_node("TagComponent")->value());

			//TransformComponent
			xml_node<>* TransNode = nodeFrame->first_node()->next_sibling();
			for (xml_node<>* child = TransNode->first_node(); child; child = child->next_sibling()) {
				if (child->type() == node_element) {
					int val = localMapXC[child->name()];
					switch (val)
					{
					case Translation: {printf("Translation = %s\n", child->value()); break; }
					case Rotation: {printf("Rotation = %s\n", child->value()); break;	}
					case Scale: {printf("Scale = %s\n", child->value()); break;	}
					default:  break;
					}
				}
			}
			//SpriteRendererComponent
			xml_node<>* SRCnode = nodeFrame->first_node()->next_sibling()->next_sibling();
			for (xml_node<>* child = SRCnode->first_node(); child; child = child->next_sibling()) {
				if (child->type() == node_element) {
					int val = localMapXC[child->name()];
					switch (val)
					{
					case Color: {printf("SRC color = %s\n", child->value()); break; }
					case Size: {printf("SRC Size = %s\n", child->value()); break;	}
					case Texture: {printf("SRC Texture = %s\n", child->value()); break;	}
					case Name: {printf("SRC name = %s\n", child->value()); break; }
					default:  break;
					}
				}
			}
			nodeFrame = nodeFrame->next_sibling();
		}
		return true;
	}

	bool XMLClass::SaveXML(std::string filePath) {
	/*	auto view = m_Scene->m_Registry.view<SpriteRendererComponent, TransformComponent>();
		// we iterate over all entities with a sprite and transform component
		// the logic inside the lambda is the same as in the previous for loop
		view.each([](auto& s, auto& t) {
			s.dst.x = tt.pos_x;
			s.dst.y = t.pos_y;
		
				//SerializeEntity(out, entity);
			});*/
		return true;
	}
}