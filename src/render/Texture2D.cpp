#include "mypch.h"
#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
namespace ALStore {
	Texture2D::Texture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		//HZ_PROFILE_FUNCTION();
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		bool gl4 = true;
		if (gl4)
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
		}
		else {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	Texture2D::Texture2D(const std::string path, bool flipped) {
		//HZ_PROFILE_FUNCTION();
		char ch = '//';
		size_t found;
		found = path.find_last_of(ch);
		m_Name = path.substr(found + 1);
		stbi_set_flip_vertically_on_load(flipped); //sprites aree flipped....
		stbi_uc* data = nullptr;
		{
			//HZ_PROFILE_SCOPE("stbi_load - Texture2D::Texture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		//unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data) {
			AL_CORE_ERROR("Error loading image file"); return;
		}
		else {
			m_IsLoaded = true;
			m_Width = width;
			m_Height = height;
			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;
			//HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			bool gl4 = true;
			if (gl4)
			{
				glGenTextures(1, &m_RendererID);
				glBindTexture(GL_TEXTURE_2D, m_RendererID);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else {
				glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
				glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
			}
		}

		stbi_image_free(data);
		AL_CORE_INFO("loaded texture, renderID = {0}, {1}", m_RendererID, m_Name);
		// return 0;
	}
	Texture2D::~Texture2D() { glDeleteTextures(1, &m_RendererID); }
	void Texture2D::Create(const std::string& filepath) {
		//HZ_PROFILE_FUNCTION();
		char ch = '//';
		size_t found;
		found = filepath.find_last_of(ch);
		m_Name = filepath.substr(found + 1);
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			//HZ_PROFILE_SCOPE("stbi_load - Texture2D::Texture2D(const std::string&)");
			data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}
		//unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data) { AL_CORE_ERROR("Error loading image file"); return; }
		else {
			m_IsLoaded = true;
			m_Width = width;
			m_Height = height;
			AL_CORE_INFO("loaded texture");
			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;
			//HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			bool gl4 = true;
			if (gl4)
			{
				glGenTextures(1, &m_RendererID);
				glBindTexture(GL_TEXTURE_2D, m_RendererID);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else {
				glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
				glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
			}
		}

		stbi_image_free(data);

	}
	void Texture2D::SetData(void* data, uint32_t size)
	{
		//HZ_PROFILE_FUNCTION();
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		//HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
	void Texture2D::Bind(uint32_t slot) const
	{
		////HZ_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}
}