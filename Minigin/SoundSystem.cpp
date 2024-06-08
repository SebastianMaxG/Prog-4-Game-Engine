#include "SoundSystem.h"

#include <memory>
#include <stdexcept>

#include "AudioClip.h"
#include "ResourceManager.h"

namespace lsmf
{

    void SDLSoundSystem::PlaySound(const std::string& filename, float volume)
    {
        std::shared_ptr<AudioClip> audioClip;

        auto it = audioClipCache.find(filename);
        if (it != audioClipCache.end()) {
            audioClip = it->second;
        }
        else {
            audioClip = ResourceManager::GetInstance().LoadAudioClip(filename);
            audioClipCache[filename] = audioClip;
        }

        audioClip->SetVolume(volume);
        audioClip->Play();
    }

    void SDLSoundSystem::StopAllSounds()
    {
		Mix_HaltChannel(-1);
    }

    void SDLSoundSystem::UnloadAll()
    {
        audioClipCache.clear();
    }

    void SDLSoundSystem::Unload(const std::string& file)
    {
        auto it = audioClipCache.find(file);
        if (it != audioClipCache.end())
        {
            audioClipCache.erase(it);
        }
    }

    void SDLSoundSystem::Load(const std::string& file)
    {
        auto it = audioClipCache.find(file);
		if (it == audioClipCache.end())
		{
            audioClipCache[file] = ResourceManager::GetInstance().LoadAudioClip(file);
		}
    }
}
