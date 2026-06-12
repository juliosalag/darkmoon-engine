#include "../libs/miniaudio.h"

#include <darkmoon/resources/resource_audio.hpp>
#include <iostream>

struct ResourceAudio::Impl {
    ma_engine* engine  { nullptr };
    ma_sound   sound   {};
    bool       initialized { false };
};

ResourceAudio::ResourceAudio(std::size_t idResource, std::size_t fileType,
                             const char* filePath, void* engine)
    : Resource(idResource, fileType, filePath)
{
    m_impl = new Impl();
    m_impl->engine = static_cast<ma_engine*>(engine);
    setup();
}

ResourceAudio::~ResourceAudio() {
    unload();
    delete m_impl;
}

void ResourceAudio::setup() {
    if (!m_impl->engine) {
        std::cerr << "[ResourceAudio] No engine for: " << m_filePath << "\n";
        return;
    }

    ma_result result = ma_sound_init_from_file(
        m_impl->engine,
        m_filePath.c_str(),
        MA_SOUND_FLAG_DECODE,
        nullptr, nullptr,
        &m_impl->sound
    );

    if (result != MA_SUCCESS) {
        std::cerr << "[ResourceAudio] Failed to load: " << m_filePath
                  << " (error " << result << ")\n";
        return;
    }

    m_impl->initialized = true;
    m_isLoaded = true;

   std::cout << "[LOAD] Audio ID: " << m_idResource << " (" << m_filePath << ")\n";
}

void ResourceAudio::unload() {
    if (m_impl && m_impl->initialized) {
        ma_sound_uninit(&m_impl->sound);
        m_impl->initialized = false;
        m_isLoaded = false;
        std::cout << "[UNLOAD] Audio ID: " << m_idResource << " (" << m_filePath << ")\n";
    }
}

void ResourceAudio::play()             { if (m_impl->initialized) ma_sound_start(&m_impl->sound); }
void ResourceAudio::stop()             { if (m_impl->initialized) ma_sound_stop(&m_impl->sound);  }
void ResourceAudio::pause()            { if (m_impl->initialized) ma_sound_stop(&m_impl->sound);  }
void ResourceAudio::setVolume(float v) { if (m_impl->initialized) ma_sound_set_volume(&m_impl->sound, v); }
void ResourceAudio::setLooping(bool l) { if (m_impl->initialized) ma_sound_set_looping(&m_impl->sound, l ? MA_TRUE : MA_FALSE); }
bool ResourceAudio::isPlaying() const  { return m_impl->initialized && ma_sound_is_playing(&m_impl->sound); }