#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

namespace dae
{
	class SpriteComponent final: public BaseComponent
	{
	public:
		SpriteComponent(GameObject* m_GameObjectPtr, const std::string& filename);
		SpriteComponent(GameObject* m_GameObjectPtr, std::shared_ptr<Texture2D> newTexture);
		//delete copy/move constructors and assignment operator
		SpriteComponent(SpriteComponent&& other) = delete;
		SpriteComponent(const SpriteComponent& other) = delete;
		const SpriteComponent& operator=(SpriteComponent&& other) = delete;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;

		~SpriteComponent() override = default;



		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> newTexture);

	private:
		std::shared_ptr<Texture2D> m_texture{};

	};

}