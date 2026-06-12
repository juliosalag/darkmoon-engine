#include "../libs/miniaudio.h"

#include <darkmoon/managers/audio_manager.hpp>

struct AudioManager::Impl {
    ma_engine engine {};
    bool      initialized { false };
};

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() {
    m_impl = new Impl();
    ma_result result = ma_engine_init(nullptr, &m_impl->engine);
    if (result != MA_SUCCESS) {
        m_impl->initialized = false;
        return;
    }
    m_impl->initialized = true;
}

AudioManager::~AudioManager() {
    if (m_impl->initialized)
        ma_engine_uninit(&m_impl->engine);
    delete m_impl;
}

void* AudioManager::get() {
    return m_impl->initialized ? &m_impl->engine : nullptr;
}

bool AudioManager::isInitialized() const {
    return m_impl->initialized;
}