#include <iostream>

#include <darkmoon.hpp>

int main() {
    std::cout << "Hello world\n";

    DarkMoonEngine dm;

    dm.m_resourceManager.loadResource<ResourceTexture>("../assets/defaultTexture.png");
}