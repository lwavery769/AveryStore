#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "OrthographicCamera.h"
//#include "scene/Components.h"

namespace ALStore {
	class Render2D
	{
	public:
		Render2D();
		~Render2D();
		void Init();
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();
		static void DrawIndexed();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		//void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int UUID = -1);
		static void DrawSprite(const glm::mat4& transform, const glm::vec4& color, int entityID);
		//void DrawTexture(const glm::mat4& transform, SpriteRendererComponent& src, const glm::vec4& tintColor, int entityID);
		void DrawTexture(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tintColor);
		void DrawTile(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tileCoords, const glm::vec4& tintColor);
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}