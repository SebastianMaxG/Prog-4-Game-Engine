#pragma once
#include <glm/glm.hpp>

namespace lsmf
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);

		void SetPosition(const glm::vec3& pos);

		void Translate(const glm::vec3& pos);

		void SetScale(const glm::vec3& scale);

		void Rotate(float angle);

		void SetRotation(float angle);

		const glm::vec3& GetScale() const;
		float GetRotation() const;

		//make a minus operator for transform
		Transform operator-(const Transform& other) const;
		Transform operator+(const Transform& other) const;

	private:


		glm::vec3 m_Position{};
		glm::vec3 m_Scale{1,1,1};
		float m_Rotation{};
	};
}
