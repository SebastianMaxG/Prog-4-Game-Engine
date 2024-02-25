#pragma once
#include "BaseComponent.h"
#include "Transform.h"
namespace dae
{
	class VisualComponent : public dae::BaseComponent
	{
	public:
		VisualComponent(GameObject* m_GameObjectPtr, Transform transform = Transform());
		virtual void Update(double deltaTime) override;
		virtual void Render() const override = 0;

	protected:
		Transform m_Transform;
	};
}
