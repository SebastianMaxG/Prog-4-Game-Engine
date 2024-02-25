#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include <glm/glm.hpp>

namespace dae
{
	class TransformComponent final: public dae::BaseComponent
	{
	public:
		TransformComponent(GameObject* m_GameObjectPtr);

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;
		virtual ~TransformComponent() = default;

		void SetPosition(const glm::vec3& pos);

		void Translate(const glm::vec3& pos);

		void SetScale(const glm::vec3& scale);

		void Rotate(float angle);

		void SetRotation(float angle);

		const glm::vec3& GetPosition() const;
		const glm::vec3& GetScale() const;
		float GetRotation() const;
	private:

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		float m_Rotation;

	};
}

