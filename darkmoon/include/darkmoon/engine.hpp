#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <map>
#include <optional>

#include <darkmoon/managers/resource_manager.hpp>
#include <darkmoon/managers/window.hpp>
#include <darkmoon/managers/monitor.hpp>

#include <darkmoon/2D/2D.hpp>

#include <darkmoon/utils/keys.hpp>
#include <darkmoon/utils/math.hpp>

struct DarkMoonEngine{
public:
    DarkMoonEngine();
    ~DarkMoonEngine() { 
        m_resourceManager.unloadAllResources();
        glfwTerminate(); 
    };

    // Returns the primary (preferred) monitor
    Monitor GetPrimaryMonitor() const {
        return Monitor::getPrimary();
    }
 
    // Returns all currently connected monitors
    std::vector<Monitor> GetMonitors() const {
        return Monitor::getAll();
    }
 
    // Returns true (once per event) if a monitor was connected.
    bool MonitorConnected() const {
        return Monitor::wasConnected();
    }
 
    // Returns true (once per event) if a monitor was disconnected.
    bool MonitorDisconnected() const {
        return Monitor::wasDisconnected();
    }

private:
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};