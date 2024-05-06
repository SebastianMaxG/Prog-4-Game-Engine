#pragma once
#include "Transform.h"
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	//class BaseComponent;

	class TransformComponent final: public BaseComponent
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

		const Transform& GetLocalTransform() const;
		const Transform& GetWorldTransform() const;

		void SetLocalTransform(const Transform& transform);
		void SetWorldTransform(const Transform& transform);

		void SetIsRoot(bool isRoot);
		void SetDirty();
	private:

		Transform m_LocalTransform;
		mutable Transform m_WorldTransform;
		mutable bool m_IsDirty = true;
		bool m_IsRoot = true;

	};
}

