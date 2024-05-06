#include "OrbitComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <iostream>

dae::OrbitComponent::OrbitComponent(GameObject* m_GameObjectPtr, double radius, double speed, double angleOffset)
	:BaseComponent(m_GameObjectPtr)
	, m_Radius(radius)
	, m_Speed(speed)
	, m_AngleOffset(angleOffset)
	, m_Angle(0)
{
}

void dae::OrbitComponent::FixedUpdate(double deltaTime)
{
	m_Angle += m_Speed * deltaTime;
	if (m_Angle > 360)
	{
				m_Angle -= 360;
	}
	//cast to float
	float x = static_cast<float> (m_Radius * cos(m_Angle * 3.14159265358979323846 / 180));
	float y = static_cast<float> (m_Radius * sin(m_Angle * 3.14159265358979323846 / 180));
	GetGameObject()->GetTransform()->Translate({x,y,0});
}

//void dae::OrbitComponent::Update(double)
//{
//
//	//std::cout << "Update OrbitComponent" << std::endl;
//
//}
