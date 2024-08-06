#include "AudioClip.h"

#include <iostream>
#include <stdexcept>

lsmf::AudioClip::AudioClip(const std::string& fullPath)
{
    m_chunk = Mix_LoadWAV(fullPath.c_str());
    if (m_chunk == nullptr)
    {
        throw std::runtime_error(std::string("Failed to load audio clip: ") + Mix_GetError());
    }
}

lsmf::AudioClip::~AudioClip()
{
    Mix_FreeChunk(m_chunk);
}

Mix_Chunk* lsmf::AudioClip::GetChunk() const
{
    return m_chunk;
}

void lsmf::AudioClip::Play() const
{
    if (Mix_PlayChannel(-1, m_chunk, 0) == -1)
    {
        //throw std::runtime_error(std::string("Failed to play audio clip: ") + Mix_GetError());
        std::cout << "Failed to play audio clip: " << Mix_GetError() << std::endl;
    }
}

void lsmf::AudioClip::SetVolume(float volume)
{
    Mix_VolumeChunk(m_chunk, static_cast<int>(volume * MIX_MAX_VOLUME));
}