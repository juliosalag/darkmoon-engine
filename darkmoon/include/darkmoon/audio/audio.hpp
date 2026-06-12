#pragma once

#include <darkmoon/resources/resource_audio.hpp>
#include <darkmoon/managers/resource_manager.hpp>
#include <darkmoon/managers/audio_manager.hpp>

class Audio {
public:
    explicit Audio(const char* filePath) {
        void* engine = AudioManager::getInstance().get();
        m_resource = ResourceManager::getInstance().loadResource<ResourceAudio>(filePath, engine);
    }

    void play()                { if (m_resource) m_resource->play();           }
    void stop()                { if (m_resource) m_resource->stop();           }
    void pause()               { if (m_resource) m_resource->pause();          }
    void setVolume(float vol)  { if (m_resource) m_resource->setVolume(vol);   }
    void setLooping(bool loop) { if (m_resource) m_resource->setLooping(loop); }
    bool isPlaying()     const { return m_resource && m_resource->isPlaying(); }
    bool isLoaded()      const { return m_resource && m_resource->isLoaded();  }

private:
    ResourceAudio* m_resource { nullptr };
};