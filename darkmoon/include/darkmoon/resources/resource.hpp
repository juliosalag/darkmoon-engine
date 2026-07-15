#pragma once

#include <string>
#include <iostream>

struct Resource {
protected:
    std::size_t m_idResource { 0 };
    std::size_t m_fileType {};
    std::string m_filePath {};
    bool m_isLoaded { false };

    Resource(std::size_t idResource, std::size_t fileType, const char* filePath)
            : m_idResource(idResource), m_fileType(fileType), m_filePath(filePath) {};

    virtual void setup() {};

public:
    virtual ~Resource() {};
    virtual void unload() {};

    // Getters

    bool isLoaded() { return m_isLoaded; };
    std::size_t getIDResource() { return m_idResource; };
    std::string getFilePath() { return m_filePath; };
    std::size_t getFileType() { return m_fileType; };
};