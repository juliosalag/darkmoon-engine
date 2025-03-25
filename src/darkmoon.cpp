#include "darkmoon.hpp"

bool DarkMoonEngine::initWindow(int width, int height, const char* title){
    return m_windowManager.initWindow(width, height, title);
}

void DarkMoonEngine::closeWindow(){
    m_windowManager.closeWindow();    
}

bool DarkMoonEngine::windowShouldClose(){
    return m_windowManager.windowShouldClose();
}

void DarkMoonEngine::beginDrawing(){
    m_windowManager.beginDrawing();
}

void DarkMoonEngine::endDrawing(){
    m_windowManager.endDrawing();
}

void DarkMoonEngine::clearBackground(Color color){
    m_windowManager.clearBackground(color);
}