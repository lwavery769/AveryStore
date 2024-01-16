#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm/ext.hpp"

namespace ALStore {

	class Maths {
	public:
		static glm::mat4 getTransform(glm::vec2 size, glm::vec3 position) {

			glm::vec3 entRotation = { 0.0f, 0.0f, 0.0f };
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), entRotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), entRotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), entRotation.z, { 0, 0, 1 });

			glm::mat4 transform = glm::translate(
				glm::mat4(1.0f), position)
				* rotation
				* scale;
			return transform;
		}

	};
}