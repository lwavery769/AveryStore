#pragma once
#include "Texture2D.h"

namespace ALStore {
	class TextureLibrary
	{
	public:
		TextureLibrary();
		~TextureLibrary();
		void init();
		void Add(const std::string& name, const  std::shared_ptr<Texture2D>& texture);
		void Add(const  std::shared_ptr<Texture2D>& texture);
		void Load(const std::string& filepath);
		int MapPos(const std::string& name);
		std::shared_ptr<Texture2D> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Texture2D> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		Texture2D tex2d;
		std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
	};

}