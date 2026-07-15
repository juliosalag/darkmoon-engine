#pragma once

class AudioManager {
public:
    static AudioManager& getInstance();

    void* get();
    bool  isInitialized() const;

private:
    AudioManager();
    ~AudioManager();
    AudioManager(const AudioManager&)            = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    struct Impl;
    Impl* m_impl { nullptr };
};