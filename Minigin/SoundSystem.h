#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace lsmf
{
	class AudioClip;

	class SoundSystem
    {
    public:
        virtual ~SoundSystem() = default;

        virtual void PlaySound(const std::string& file, float volume) = 0;
        virtual void StopAllSounds() = 0;
        virtual void UnloadAll() = 0;
        virtual void Unload(const std::string& file) = 0;
        virtual void Load (const std::string& file) = 0;

    };

    class SDLSoundSystem : public SoundSystem
    {
    public:
        void PlaySound(const std::string& file, float volume) override;
        void StopAllSounds() override;
        void UnloadAll() override;
        void Unload(const std::string& file) override;
        void Load(const std::string& file) override;

    private:
        std::unordered_map<std::string, std::shared_ptr<AudioClip>> audioClipCache;

    };

    class DebugSoundSystem : public SoundSystem
    {
    public:
		void PlaySound(const std::string& file, float volume) override;
		void StopAllSounds() override;
		void UnloadAll() override;
		void Unload(const std::string& file) override;
		void Load(const std::string& file) override;
    };
}

