#pragma once

#include "resource.hpp"

struct ResourceAudio : public Resource {
private:
    void setup() override;

public:
    ResourceAudio(std::size_t idResource, std::size_t fileType,const char* filePath, void* engine);

    ~ResourceAudio() override;

    void unload() override;
    void play();
    void stop();
    void pause();
    void setVolume(float vol);
    void setLooping(bool loop);
    bool isPlaying() const;

private:
    struct Impl;
    Impl* m_impl { nullptr };
};