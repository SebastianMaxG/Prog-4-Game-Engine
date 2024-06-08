#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include "Signal.h"
namespace lsmf
{
	namespace sound
	{
        inline std::jthread g_PlaySoundThread;
        inline signal::Signal<const std::string&, float> PlaySoundSignal{g_PlaySoundThread};
	}


	class AudioClip;

	class SoundSystem
    {
    public:
        SoundSystem()
        {
        	m_PlaySoundConnection = sound::PlaySoundSignal.Connect(this, &SoundSystem::PlaySound);
		}
        virtual ~SoundSystem() = default;

        SoundSystem(const SoundSystem& other) = delete;
        SoundSystem(SoundSystem&& other) = delete;
        SoundSystem& operator=(const SoundSystem& other) = delete;
        SoundSystem& operator=(SoundSystem&& other) = delete;


        virtual void PlaySound(const std::string& file, float volume) = 0;
        virtual void StopAllSounds() = 0;
        virtual void UnloadAll() = 0;
        virtual void Unload(const std::string& file) = 0;
        virtual void Load (const std::string& file) = 0;
	protected:
        signal::Connection<const std::string&, float>* m_PlaySoundConnection;
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

