#pragma once

struct Resource {
protected:
    std::size_t m_id;
    std::string m_filePath;
    bool m_isLoaded { false };

    virtual void setup() {};

public:
    virtual ~Resource() {};
    virtual bool load(const char* filePath) = 0;
    virtual void unload() {};

    // Getters

    bool isLoaded() { return m_isLoaded; };
    std::size_t getID() { return m_id; };
    std::string getFilePath() { return m_filePath; };
};