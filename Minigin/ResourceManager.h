#pragma once
#include <string>
#include <memory>
#include "Singleton.h"
#include <SDL_mixer.h>
namespace lsmf
{
	class Texture2D;
	class Font;
	class AudioClip;

	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::shared_ptr<AudioClip> LoadAudioClip(const std::string& file) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
	};
}
