#pragma once
#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"
#include "render/Maths.h"
#include "render/Texture2D.h"
#include "render/Render2D.h"

	class Delivery
	{
	public:
		Delivery();
		~Delivery();
		void init();
		void updatePosition();
		bool GetTrk() { return m_Deliver; }
		void SetTrk(bool dlv) { m_Deliver = dlv; }
		void Render();
	private:
		glm::vec2 truck2_size = { .451f, .750f };
		glm::vec3 truck2_position = { 1.2f, 0.041f, 0.0f };
		glm::vec4 m_TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		bool m_Flipped, m_Deliver;
		std::shared_ptr<ALStore::Texture2D> m_DelTruckTxt;
		//std::shared_ptr<
		ALStore::Render2D draw;
	};
