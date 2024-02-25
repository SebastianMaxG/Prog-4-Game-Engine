#pragma once
#include "VisualComponent.h"
#include "Texture2D.h"

namespace dae
{
	class SpriteComponent : public VisualComponent
	{
	public:
		SpriteComponent(GameObject* m_GameObjectPtr, const std::string& filename, Transform transform = Transform());
		virtual void Render() const override;

		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_texture{};

	};

}