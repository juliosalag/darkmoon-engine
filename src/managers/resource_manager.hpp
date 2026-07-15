#pragma once
#include "../resources/resource.hpp"

#include <string>
#include <iostream>
#include <gif_lib.h>

#include <memory>
#include <unordered_map>

namespace DarkMoon {
    struct ResourceManager {
    public:

        static ResourceManager& getInstance() {
            static ResourceManager instance;
            return instance;
        }

        ~ResourceManager() {
            std::cout << "┌────────┐" << std::endl;
            std::cout << "│ Unload │" << std::endl;
            std::cout << "└────────┘" << std::endl;

            unloadAllResources();
        };

        template<typename T>
        T* getResource(const std::size_t& id) {
            auto it = m_resources.find(id);
            if (it != m_resources.end())
                return static_cast<T*>(it->second.get());

            return nullptr;
        }

        template<typename T, typename... Args>
        T* loadResource(const char* filePath, Args&&... args) {
            // Search if the resource is already loaded
            for (auto& pair : m_fileNames)
                if (pair.second == filePath && m_fileTypes[pair.first] == typeid(T).hash_code())
                    return getResource<T>(pair.first);

            // If the resource is not in memory, load it
            nextID++;
            auto resource = std::make_unique<T>(nextID, std::forward<Args>(args)...);
            if (resource->load(filePath)) {
                auto rawPtr = resource.get();
                m_resources[nextID] = std::move(resource);
                m_fileNames[nextID] = std::string(filePath);
                m_fileTypes[nextID] = typeid(T).hash_code();
                return rawPtr;
            }
            else {
                nextID--;
                return nullptr;
            }
        }

        std::vector<Texture*> loadGifResources(const char* filePath) {
            std::vector<Texture*> resources;

            // Check if the file is a GIF
            std::string pathStr(filePath);
            if (pathStr.substr(pathStr.find_last_of(".") + 1) == "gif") {
                // Load the GIF and create a texture for each frame
                int error = 0;
                GifFileType* gifFile = DGifOpenFileName(filePath, &error);
                if (gifFile == nullptr) {
                    return resources;
                }

                if (DGifSlurp(gifFile) == GIF_ERROR) {
                    DGifCloseFile(gifFile, &error);
                    return resources;
                }

                for (int i = 0; i < gifFile->ImageCount; i++) {
                    nextID++;
                    auto resource = std::make_unique<Texture>(nextID);
                    if (resource->loadFrame(gifFile, i)) {
                        auto rawPtr = resource.get();
                        m_resources[nextID] = std::move(resource);
                        m_fileNames[nextID] = std::string(filePath);
                        m_fileTypes[nextID] = typeid(Texture).hash_code();
                        resources.push_back(rawPtr);
                    }
                    else {
                        nextID--;
                    }
                }

                DGifCloseFile(gifFile, &error);
            }

            return resources;
        }

        void unloadResource(const std::size_t& id) {
            auto it = m_resources.find(id);
            if (it != m_resources.end())
                m_resources.erase(it);

            auto it2 = m_fileNames.find(id);
            if (it2 != m_fileNames.end())
                m_fileNames.erase(it2);

            auto it3 = m_fileTypes.find(id);
            if (it3 != m_fileTypes.end())
                m_fileTypes.erase(it3);
        }

        void unloadAllResources() {
            m_resources.clear();
            m_fileNames.clear();
            m_fileTypes.clear();
        }

    private:
        ResourceManager() = default;

        inline static std::size_t nextID{ 0 };
        std::unordered_map<std::size_t, std::unique_ptr<Resource>> m_resources;
        std::unordered_map<std::size_t, std::string> m_fileNames;
        std::unordered_map<std::size_t, std::size_t> m_fileTypes;
    };
}