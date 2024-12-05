#pragma once

#include <memory>
#include <unordered_map>

#include "../resources/resource.hpp"

struct ResourceInfo {
    std::unique_ptr<Resource> resource;
    std::string filePath;
    std::size_t fileType;

    ResourceInfo(std::unique_ptr<Resource> r, const std::string& fPath, std::size_t fType)
        : resource(std::move(r)), filePath(fPath), fileType(fType) {};
};

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
            if(pair.second.filePath == filePath && pair.second.fileType == typeid(T).hash_code())
                return getResource<T>(pair.first);

        // Load a new resource if it is not already loaded

        auto resource = std::make_unique<T>(nextID, std::forward<Args>(args)...);
        if(resource->load(filePath)) {
            nextID++;
            auto rawPtr = resource.get();
            m_resources[nextID] = ResourceInfo(std::move(resource), filePath, typeid(T).hash_code());

            return rawPtr; 
        }
        else
            return nullptr;
    }

    template<typename T>
    T* getResource(const std::size_t& id) {
        auto it = m_resources.find(id);
        if(it != m_resources.end())
            return static_cast<T*>(it->second.resource.get());

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
    std::unordered_map<std::size_t, ResourceInfo> m_resources;
};