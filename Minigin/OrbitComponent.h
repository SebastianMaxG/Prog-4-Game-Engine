#pragma once
#include <glm/vec3.hpp>

#include "BaseComponent.h"
namespace dae
{

	class OrbitComponent : public BaseComponent
	{
	public:
		
		OrbitComponent(GameObject* m_GameObjectPtr, double radius, double speed, double angleOffset);


		~OrbitComponent() override = default;
		OrbitComponent(const OrbitComponent& other) = delete;
		OrbitComponent(OrbitComponent&& other) = delete;
		OrbitComponent& operator=(const OrbitComponent& other) = delete;
		const OrbitComponent& operator=(OrbitComponent&& other) = delete;

	private:

		void FixedUpdate(double deltaTime) override;
		//void Update(double) override;

		double m_Radius;
		double m_Speed;
		double m_AngleOffset;
		double m_Angle;

	};

}

