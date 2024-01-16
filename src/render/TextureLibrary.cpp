#include "mypch.h"
#include "TextureLibrary.h"
#include "Texture2D.h"

	struct TextureData
	{
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
		std::shared_ptr<ALStore::Texture2D> WhiteTexture;
		std::array<std::shared_ptr<ALStore::Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};  static TextureData s_TextData;
namespace ALStore {
	TextureLibrary::TextureLibrary() {

	}
	TextureLibrary::~TextureLibrary() {}
	void TextureLibrary::init()
	{
		s_TextData.WhiteTexture = std::make_shared<Texture2D>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_TextData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_TextData.WhiteTexture->Bind();
		int32_t samplers[s_TextData.MaxTextureSlots];
		for (uint32_t i = 0; i < s_TextData.MaxTextureSlots; i++)
			samplers[i] = i;
		s_TextData.TextureSlots[0] = s_TextData.WhiteTexture;
	}
	void TextureLibrary::Add(const std::string& name, const std::shared_ptr<Texture2D>& texture)
	{
		if (Exists(name))
		{
			AL_CORE_ERROR("Texture already exists!");
		}
		else m_Textures[name] = texture;
		s_TextData.TextureSlots[s_TextData.TextureSlotIndex] = texture;
		s_TextData.TextureSlotIndex++;;
	}
	int TextureLibrary::MapPos(const std::string& name) {
		return distance(m_Textures.begin(), m_Textures.find(name));
	}
	void TextureLibrary::Add(const std::shared_ptr<Texture2D>& texture)
	{
		auto& name = texture->GetName();
		Add(name, texture);

	}
	void TextureLibrary::Load(const std::string& filepath) {
		std::shared_ptr<Texture2D> texture;
		texture = std::make_shared<Texture2D>(filepath, 1);
		Add(texture);
	}
	std::shared_ptr<Texture2D> TextureLibrary::Get(const std::string& name)
	{
		if (m_Textures.contains(name))
		{
			float textureIndex = 0.0f;
			for (uint32_t i = 1; i < s_TextData.TextureSlotIndex; i++)
			{
				if (*s_TextData.TextureSlots[i].get() == *m_Textures[name].get())
				{
					glActiveTexture(GL_TEXTURE + i);
					textureIndex = (float)i; return m_Textures[name];
					break;
				}
			}

			int i = MapPos(name);
			// activate the texture unit first before binding texture
		   //glBindTexture(GL_TEXTURE_2D, texture);
		   //m_Textures[name]->Bind(MapPos(name));
		   //return m_Textures[name];
		}
		else AL_CORE_CRITICAL("Texture map caan't find {0}", name);
	}

	bool TextureLibrary::Exists(const std::string& name) const
	{
		return m_Textures.find(name) != m_Textures.end();
	}
}