#pragma once
#include "Scene.h"
namespace ALStore {
	class XMLClass
	{
	public:
		XMLClass();
		~XMLClass();
		bool LoadXML(std::string filePath);
		bool SaveXML(std::string filePath);
	private:
		std::shared_ptr<Scene> m_Scene;

	};
}