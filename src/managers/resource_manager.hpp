#pragma once

#include <memory>
#include <unordered_map>

#include "../resources/resource.hpp"
#include "../resources/resource_texture.hpp"

struct ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    ~ResourceManager() { unloadAllResources(); }

    template<typename T, typename... Args>
    T* loadResource(const char* filePath, Args&&... args) {

        // Check if the resource is already loaded

        for(auto& pair : m_resources)
            if(pair.second->getFilePath() == filePath && 
               pair.second->getFileType() == typeid(T).hash_code())
                return getResource<T>(pair.first);

        // Load a new resource if it is not already loaded

        auto resource = std::make_unique<T>(nextID, typeid(T).hash_code(), filePath, std::forward<Args>(args)...);
        if(resource->isLoaded()) {
            nextID++;
            auto rawPtr = resource.get();
            m_resources[nextID] = std::move(resource);

            return rawPtr; 
        }
        else
            return nullptr;
    }

    template<typename T>
    T* getResource(const std::size_t& id) {
        auto it = m_resources.find(id);
        if(it != m_resources.end())
            return static_cast<T*>(it->second.get());

        return nullptr;            
    }

    void unloadResource(const std::size_t& id) {
        auto it = m_resources.find(id);
        if(it != m_resources.end())
            m_resources.erase(it);
    }

    void unloadAllResources() { m_resources.clear(); }

private:
    ResourceManager() = default;

    inline static std::size_t nextID{ 0 };
    std::unordered_map<std::size_t, std::unique_ptr<Resource>> m_resources;
};