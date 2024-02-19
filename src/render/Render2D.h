#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "OrthographicCamera.h"
#include "../scene/Components.h"

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
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int UUID = -1, bool flipped = false);
		static void DrawSprite(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawTexture(const glm::mat4& transform, SpriteRendererComponent& src, const glm::vec4& tintColor, int entityID, bool flipped);
		void DrawTexture(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tintColor);
		void DrawTile(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tileCoords, const glm::vec4& tintColor);
	private:
		glm::vec4 uColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}