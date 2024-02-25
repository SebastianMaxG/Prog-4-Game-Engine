#pragma once
#include "BaseComponent.h"
#include "Transform.h"
namespace dae
{
	class PhysicsComponent : public dae::BaseComponent
	{
	public:
		PhysicsComponent(GameObject* m_GameObjectPtr);
		virtual void Update(double deltaTime) override = 0;
		virtual void FixedUpdate(double deltaTime) override = 0;



		//void Move(float x, float y);
		//void Move(float x, float y, float z);

		//void SetVelocity(float x, float y);
		//void SetVelocity(float x, float y, float z);

		//void SetAcceleration(float x, float y);
		//void SetAcceleration(float x, float y, float z);

		//void SetFriction(float x, float y);
		//void SetFriction(float x, float y, float z);

		//void SetGravity(float x, float y);
		//void SetGravity(float x, float y, float z);

		//void SetMaxVelocity(float x, float y);
		//void SetMaxVelocity(float x, float y, float z);
		//void SetMinVelocity(float x, float y);
		//void SetMinVelocity(float x, float y, float z);

		//void SetMaxAcceleration(float x, float y);
		//void SetMaxAcceleration(float x, float y, float z);
		//void SetMinAcceleration(float x, float y);
		//void SetMinAcceleration(float x, float y, float z);

		//void SetMaxFriction(float x, float y);
		//void SetMaxFriction(float x, float y, float z);
		//void SetMinFriction(float x, float y);
		//void SetMinFriction(float x, float y, float z);

		//void SetMaxGravity(float x, float y);
		//void SetMaxGravity(float x, float y, float z);
		//void SetMinGravity(float x, float y);
		//void SetMinGravity(float x, float y, float z);

	private:


		//float m_VelocityX = 0;
		//float m_VelocityY = 0;
		//float m_VelocityZ = 0;

		//float m_AccelerationX = 0;
		//float m_AccelerationY = 0;
		//float m_AccelerationZ = 0;

		//float m_FrictionX = 0;
		//float m_FrictionY = 0;
		//float m_FrictionZ = 0;

		//float m_GravityX = 0;
		//float m_GravityY = 0;
		//float m_GravityZ = 0;

		//float m_MaxVelocityX = 0;
		//float m_MaxVelocityY = 0;
		//float m_MaxVelocityZ = 0;

		//float m_MinVelocityX = 0;
		//float m_MinVelocityY = 0;
		//float m_MinVelocityZ = 0;

		//float m_MaxAccelerationX = 0;
		//float m_MaxAccelerationY = 0;
		//float m_MaxAccelerationZ = 0;

		//float m_MinAccelerationX = 0;
		//float m_MinAccelerationY = 0;
		//float m_MinAccelerationZ = 0;

		//float m_MaxFrictionX = 0;
		//float m_MaxFrictionY = 0;
		//float m_MaxFrictionZ = 0;

		//float m_MinFrictionX = 0;
		//float m_MinFrictionY = 0;
		//float m_MinFrictionZ = 0;

		//float m_MaxGravityX = 0;
		//float m_MaxGravityY = 0;
		//float m_MaxGravityZ = 0;

		//float m_MinGravityX = 0;
		//float m_MinGravityY = 0;
		//float m_MinGravityZ = 0;
	};

}