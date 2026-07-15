#include "../libs/miniaudio.h"
#include <darkmoon/resources/resource_audio.hpp>
#include <iostream>
#include <vector>

struct ResourceAudio::Impl {
    ma_engine*              engine       { nullptr };
    std::vector<ma_sound>   pool         {};
    std::size_t             poolSize     { 4 };
    std::size_t             nextVoice    { 0 };
    bool                    initialized  { false };
    float                   volume       { 1.0f };
    bool                    looping      { false };

    // Voices that were playing at the moment pause() was called.
    // miniaudio keeps a sound's playback cursor when you call
    // ma_sound_stop() on it, so we just remember WHICH voices to
    // restart later, without ever seeking them back to frame 0.
    std::vector<std::size_t> pausedVoices {};

    std::size_t pickVoice() {
        for (std::size_t i = 0; i < poolSize; ++i) {
            std::size_t idx = (nextVoice + i) % poolSize;
            if (!ma_sound_is_playing(&pool[idx]))
                return idx;
        }
        return nextVoice;
    }
};

ResourceAudio::ResourceAudio(std::size_t idResource, std::size_t fileType, const char* filePath, void* engine, std::size_t poolSize)
    : Resource(idResource, fileType, filePath)
{
    m_impl           = new Impl();
    m_impl->engine   = static_cast<ma_engine*>(engine);
    m_impl->poolSize = poolSize;
    m_impl->pool.resize(poolSize);
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

    for (std::size_t i = 0; i < m_impl->poolSize; ++i) {
        ma_result result = ma_sound_init_from_file(
            m_impl->engine,
            m_filePath.c_str(),
            MA_SOUND_FLAG_DECODE,
            nullptr, nullptr,
            &m_impl->pool[i]
        );

        if (result != MA_SUCCESS) {
            std::cerr << "[ResourceAudio] Pool[" << i << "] failed: " << m_filePath << " (error " << result << ")\n";
            m_impl->poolSize = i;
            break;
        }
    }

    if (m_impl->poolSize == 0) return;

    m_impl->initialized = true;
    m_isLoaded          = true;
    std::cout << "[LOAD] Audio ID: " << m_idResource << " pool = " << m_impl->poolSize << " (" << m_filePath << ")\n";
}

void ResourceAudio::unload() {
    if (m_impl && m_impl->initialized) {
        for (std::size_t i = 0; i < m_impl->poolSize; ++i)
            ma_sound_uninit(&m_impl->pool[i]);

        m_impl->initialized = false;
        m_isLoaded          = false;
        m_impl->pausedVoices.clear();
        std::cout << "[UNLOAD] Audio ID: " << m_idResource << " (" << m_filePath << ")\n";
    }
}

void ResourceAudio::play() {
    if (!m_impl->initialized) return;

    if (!m_impl->pausedVoices.empty()) {
        resume();
        return;
    }

    std::size_t idx = m_impl->pickVoice();
    m_impl->nextVoice = (idx + 1) % m_impl->poolSize;

    ma_sound* s = &m_impl->pool[idx];
    ma_sound_stop(s);
    ma_sound_seek_to_pcm_frame(s, 0);
    ma_sound_set_volume(s, m_impl->volume);
    ma_sound_set_looping(s, m_impl->looping ? MA_TRUE : MA_FALSE);
    ma_sound_start(s);
}

void ResourceAudio::stop() {
    if (!m_impl->initialized) return;
    for (std::size_t i = 0; i < m_impl->poolSize; ++i) {
        ma_sound_stop(&m_impl->pool[i]);
        ma_sound_seek_to_pcm_frame(&m_impl->pool[i], 0);
    }
    m_impl->pausedVoices.clear();
}

void ResourceAudio::pause() {
    if (!m_impl->initialized) return;

    m_impl->pausedVoices.clear();
    for (std::size_t i = 0; i < m_impl->poolSize; ++i) {
        if (ma_sound_is_playing(&m_impl->pool[i])) {
            ma_sound_stop(&m_impl->pool[i]);
            m_impl->pausedVoices.push_back(i);
        }
    }
}

void ResourceAudio::resume() {
    if (!m_impl->initialized) return;
    for (std::size_t idx : m_impl->pausedVoices)
        ma_sound_start(&m_impl->pool[idx]);
    m_impl->pausedVoices.clear();
}

void ResourceAudio::setVolume(float v) {
    m_impl->volume = v;
    if (!m_impl->initialized) return;
    for (std::size_t i = 0; i < m_impl->poolSize; ++i)
        ma_sound_set_volume(&m_impl->pool[i], v);
}

void ResourceAudio::setLooping(bool l) {
    m_impl->looping = l;
    if (!m_impl->initialized) return;
    for (std::size_t i = 0; i < m_impl->poolSize; ++i)
        ma_sound_set_looping(&m_impl->pool[i], l ? MA_TRUE : MA_FALSE);
}

bool ResourceAudio::isPlaying() const {
    if (!m_impl->initialized) return false;
    for (std::size_t i = 0; i < m_impl->poolSize; ++i)
        if (ma_sound_is_playing(&m_impl->pool[i])) return true;
    return false;
}

bool ResourceAudio::isPaused() const {
    return !m_impl->pausedVoices.empty();
}