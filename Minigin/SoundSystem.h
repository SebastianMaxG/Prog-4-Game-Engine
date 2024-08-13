#pragma once
#include <cstdint>
#include <memory>
#include <SDL_events.h>
#include <string>
#include <unordered_map>
#include "Signal.h"
namespace lsmf
{
	namespace sound
	{
        inline std::jthread g_PlaySoundThread;
        inline signal::Signal<const std::string, float> PlaySoundSignal{g_PlaySoundThread};
        inline signal::Signal<> StopAllSoundsSignal{g_PlaySoundThread};
	}


	class AudioClip;

	class SoundSystem
    {
    public:
        SoundSystem()
        {
        	m_PlaySoundConnection = sound::PlaySoundSignal.Connect(this, &SoundSystem::PlaySound);
            m_StopAllSoundsConnection = sound::StopAllSoundsSignal.Connect(this, &SoundSystem::StopAllSounds);
		}
        virtual ~SoundSystem();

        SoundSystem(const SoundSystem& other) = delete;
        SoundSystem(SoundSystem&& other) = delete;
        SoundSystem& operator=(const SoundSystem& other) = delete;
        SoundSystem& operator=(SoundSystem&& other) = delete;


        virtual void PlaySound(const std::string file, float volume) = 0;
        virtual void StopAllSounds() = 0;
        virtual void UnloadAll() = 0;
        virtual void Unload(const std::string& file) = 0;
        virtual void Load (const std::string& file) = 0;
	protected:
        signal::Connection<const std::string, float>* m_PlaySoundConnection;
        signal::Connection<>* m_StopAllSoundsConnection;
    };

    class SDLSoundSystem : public SoundSystem
    {
    public:
        SDLSoundSystem();
        ~SDLSoundSystem() override;

        void PlaySound(const std::string file, float volume) override;
        void StopAllSounds() override;
        void UnloadAll() override;
        void Unload(const std::string& file) override;
        void Load(const std::string& file) override;
        void Mute(SDL_Event event);

    private:
        std::unordered_map<std::string, std::shared_ptr<AudioClip>> audioClipCache;
        bool muted = false;

    };

    class DebugSoundSystem : public SoundSystem
    {
    public:
		void PlaySound(const std::string file, float volume) override;
		void StopAllSounds() override;
		void UnloadAll() override;
		void Unload(const std::string& file) override;
		void Load(const std::string& file) override;
    };
}

