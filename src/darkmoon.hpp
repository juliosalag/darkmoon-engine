#pragma once

#include "./managers/resource_manager.hpp"

struct DarkMoonEngine{
public:
    DarkMoonEngine() {};
    ~DarkMoonEngine() {};

    ResourceManager& m_resourceManager = ResourceManager::getInstance();
private:
};