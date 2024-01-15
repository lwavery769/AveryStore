#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
namespace Odyssey {
	class Render2D
	{
	public:
		Render2D();
		~Render2D();
		void Init();
		static void EndScene();
		static void Flush();
		static void DrawIndexed();
		void DrawTexture(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tintColor);
	private:

	};
}