#pragma once

#include <resource_manager.hpp>

struct DarkMoonEngine{
public:
    DarkMoonEngine();
    ~DarkMoonEngine();

private:
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};