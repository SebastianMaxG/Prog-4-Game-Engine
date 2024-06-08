#pragma once
#include <string>
#include <SDL_mixer.h>

namespace lsmf
{
    class AudioClip final
    {
    public:
        explicit AudioClip(const std::string& fullPath);
        ~AudioClip();

        AudioClip(const AudioClip&) = delete;
        AudioClip(AudioClip&&) = delete;
        AudioClip& operator= (const AudioClip&) = delete;
        AudioClip& operator= (const AudioClip&&) = delete;

        Mix_Chunk* GetChunk() const;
        void Play() const;
        void SetVolume(float volume);
        bool IsLoaded () const { return m_chunk != nullptr; }

    private:
        Mix_Chunk* m_chunk;
    };
}
