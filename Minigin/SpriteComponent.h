#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

namespace lsmf
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

		void Update(double deltaTime) override;

		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> newTexture);

		void SetFrames(int rows, int columns, int nrFrames, double frameTime);

		void SetColumn(int column) { m_currentColumn = column; }
		void SetFrame(int frame = 0)
		{
			m_currentFrame = frame;
			m_currentTime = 0;
		}

		glm::vec2 GetTextureSize() const { return { m_width, m_height }; }

		void SetZ(float z) { m_Z = z; }
	private:
		std::shared_ptr<Texture2D> m_texture{};

		int m_rows = 1;
		int m_columns = 1;
		int m_currentFrame = 0;

		int m_width = 0;
		int m_height = 0;

		int m_currentColumn = 0;

		int m_nrFrames = 1;
		float m_Z = 0;
		double m_frameTime = 0;
		double m_currentTime = 0;

	};

}