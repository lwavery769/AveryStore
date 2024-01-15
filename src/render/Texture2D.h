#pragma once
#include <fstream>
#include <glad/glad.h>
namespace Odyssey{
class Texture2D
{
public:
	Texture2D() {}
	Texture2D(uint32_t width, uint32_t height);
	Texture2D(const std::string path, bool flipped = 1);
	~Texture2D();
	void Create(const std::string& filepath);
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }
	uint32_t GetRendererID() const { return m_RendererID; }
	const std::string& GetName() const { return m_Name; }
	uint32_t m_RendererID;
	void SetData(void* data, uint32_t size);
	void Bind(uint32_t slot = 0) const;
	bool IsLoaded() const { return m_IsLoaded; }
	bool operator==(const Texture2D& other) const  { return m_RendererID == ((Texture2D&)other).m_RendererID; }
private:
	int width, height, channels;
	std::string m_Path, m_Name;
	bool m_IsLoaded = false;
	uint32_t m_Width, m_Height;

	GLenum m_InternalFormat, m_DataFormat;
};
}