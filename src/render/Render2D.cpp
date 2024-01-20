#include "mypch.h"
#include "Render2D.h"
#include <glad/glad.h>

namespace ALStore {
	//Render2D::SceneData* Render2D::s_SceneData = new Render2D::SceneData;
	Render2D::Render2D() {}
	Render2D::~Render2D() {}
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		int EntityID;
	};
	struct Renderer2DStorage
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<Shader> flatColorShader;
		std::shared_ptr<Shader> TextureShader;
		std::shared_ptr<Texture2D> WhiteTexture;
		std::shared_ptr<Texture2D> TruckTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 0; // 0 = white texture
		glm::vec4 QuadVertexPositions[4];
		//Renderer2D::Statistics Stats;
	};

	static Renderer2DStorage s_Data;
	void Render2D::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		//	HZ_PROFILE_FUNCTION("Render Init");
		s_Data.QuadVertexArray.reset(new VertexArray());
		s_Data.QuadVertexBuffer.reset(new VertexBuffer(s_Data.MaxVertices * sizeof(QuadVertex)));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float,  "a_TexIndex" },
			{ ShaderDataType::Int,    "a_EntityID" }
				});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(new IndexBuffer(quadIndices, s_Data.MaxIndices));
		s_Data.QuadVertexArray->SetIndexBuffer(squareIB);
		delete[] quadIndices;

		s_Data.WhiteTexture.reset(new Texture2D(1, 1));
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_Data.TextureSlotIndex++;
		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader.reset(new Shader("assets/shaders/TextureShader.glsl"));
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// Set all texture slots to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.flatColorShader.reset(new Shader("assets/shaders/flatColor.glsl"));
		s_Data.TruckTexture.reset(new Texture2D("assets/textures/Truck2.png"));
		s_Data.TextureSlots[1] = s_Data.TruckTexture;
		s_Data.TextureSlotIndex++;
	}
	void Render2D::BeginScene(OrthographicCamera& camera)
	{
		//HZ_PROFILE_FUNCTION();
		//s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		//s_Data.TextureSlotIndex = 1;
	}

	void Render2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}
	void Render2D::Flush()
	{
		//HZ_PROFILE_FUNCTION();
		if (s_Data.QuadIndexCount == 0)	return; // Nothing to draw
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);// s_Data.TruckTexture->GetRendererID());
		DrawIndexed();
	}
	void Render2D::DrawIndexed()
	{
		uint32_t count = s_Data.QuadIndexCount ? s_Data.QuadVertexArray->GetIndexBuffer()->GetCount() : s_Data.QuadIndexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
/*	void Render2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		//DrawQuad(transform, src.Color, entityID);
		if (src.Texture) {
			DrawTexture(transform, src, src.Color, entityID);
			//src.Texture->unBind();
		}
		else
			DrawSprite(transform, src.Color, entityID);//
	}*/
	void Render2D::DrawSprite(const glm::mat4& transform, const glm::vec4& color, int entityID) {
		//HZ_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)	Flush();

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;	//s_Data.Stats.QuadCount++;
	}
	/*void Render2D::DrawTexture(const glm::mat4& transform, SpriteRendererComponent& src, const glm::vec4& tintColor, int entityID)
	{
		//HZ_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };;
		bool flipped = 1;
		if (!flipped) {
			constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		}
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)  Flush();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *src.Texture.get()) {
				textureIndex = (float)i; break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots) Flush();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = src.Texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;

		//s_Data.Stats.QuadCount++;
	}*/
	void Render2D::DrawTexture(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tintColor) 
	{
		//HZ_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		glm::vec2* textureCoords = new glm::vec2[4];
		textureCoords[0] = { 0.0f, 0.0f };
		textureCoords[1] = { 1.0f, 0.0f };
		textureCoords[2] = { 1.0f, 1.0f };
		textureCoords[3] = { 0.0f, 1.0f };
		//const float tilingFactor = 1.0f;
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) Flush();
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DStorage::MaxTextureSlots)
				Flush();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}
		/*std::string name = texture->GetName();
		texIndex = (float)textLib.MapPos(name);
		texture->Bind(texIndex);*/
		/*for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get()){texIndex = (float)i; break;}
			if (texIndex == 0.0f){
				texIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}*/


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
	}
	void Render2D::DrawTile(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tileCoords, const glm::vec4& tintColor) {
		//HZ_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		float x = tileCoords.x; float y = tileCoords.y;
		constexpr float sheetW = 2560; constexpr float sheetH = 1664;
		float spriteW = 128; float spriteH = 128;
		glm::vec2* txtrCrds = new glm::vec2[4];
		txtrCrds[0] = { (x * spriteW) / sheetW, (y * spriteH) / sheetH };
		txtrCrds[1] = { ((x + 1) * spriteW) / sheetW, (y * spriteH) / sheetH };
		txtrCrds[2] = { ((x + 1) * spriteW) / sheetW, ((y + 1) * spriteH) / sheetH };
		txtrCrds[3] = { (x * spriteW) / sheetW, ((y + 1) * spriteH) / sheetH };
		//const float tilingFactor = 1.0f;
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices) Flush();
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DStorage::MaxTextureSlots)
				Flush();
			
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		/*for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get()){texIndex = (float)i; break;}
			if (texIndex == 0.0f){
				texIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}*/


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = txtrCrds[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		delete[] txtrCrds;
	}
}