#pragma once
#include <iostream>

namespace DarkMoon {
    struct Resource {
    protected:
        std::size_t m_id;        // ID of ResourceManager
        std::string m_filePath;  // FilePath of Resource
        bool m_isLoad{ false };

        virtual void setup() {};

    public:
        virtual ~Resource() {};
        virtual bool load(const char* fileName) = 0;
        virtual void unload() = 0;

        // Getters

        bool isLoaded() { return m_isLoad; };
        std::size_t getID() { return m_id; };
        std::string getFilePath() { return m_filePath; };
    };
}