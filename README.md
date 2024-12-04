# DarkMoon Engine
Custom graphics engine made from scratch in **C++** and **OpenGL**, specifically tailored for [Two Wonders](https://kaiwagames.itch.io/two-wonders), designed to meet the technical and graphical needs of the project. The engine was created with the goal of handling complex scenes and optimizing the game's performance.

## Current Status

DarkMoon Engine is a graphics engine is currently in an unstable version, as my intention is to reorganize and rebuild it from scratch to improve its architecture and functionality.

This repository contains the code I developed while learning OpenGL and improving my knowledge of C++. While the engine meets the initial goals of supporting projects like **Two Wonders**, I acknowledge that there is room to optimize its design and address certain technical limitations.

My goal with this project is to share my learning, demonstrate progress in graphics engine development, and, above all, build a more robust and efficient version of the engine.

## Plans for version 2.0 (2D)

I am working on a version 2.0 that I plan to release in the coming weeks. This new version will focus on delivering a fully **functional 2D graphics engine**, with an improved architecture and a modular approach to make it easier to use in game development projects.

Some features included in version 2.0:
- Optimized rendering system for **sprites**, **textures**, and **2D visual effects**.
- More efficient **resource management**, with extended support for multiple texture and font formats.
- **Real-time lighting** with different types of lights: point, directional, and spotlights (possibly including shadows).
- Support for **2D animations** such as spritesheets and advanced particle effects.
- **Better documentation and detailed guides** for developers interested in using or contributing to the engine.

## Platform Suport

The goal for DarkMoon Engine is to be **multiplatform**, supporting operating systems such as **Windows** and **Linux**. Currently, the engine is only functional on **Linux** due to some platform-specific optimizations and dependencies.

## Dependencies

DarkMoon Engine requires the following libraries to build and run.  
Instructions for installing or using them in your development environment are linked below.

| Dependency       | Documentation                                |
| ---------------- | -------------------------------------------- |
| make             | [GNU Make Documentation](https://www.gnu.org/software/make/) |
| ccache           | [ccache README](https://ccache.dev/)        |
| glm              | [glm GitHub](https://github.com/g-truc/glm) |
| libgif           | [giflib GitHub](https://github.com/lecram/giflib) |
| glew             | [GLEW Documentation](http://glew.sourceforge.net/) |
| glfw3            | [GLFW Documentation](https://www.glfw.org/) |
| freetype2        | [Freetype Documentation](https://freetype.org/) |
| assimp           | [Assimp Documentation](https://github.com/assimp/assimp) |

## Installation and deployment

Clone the repository.
```sh
git clone https://github.com/juliosalag/darkmoon-engine.git
```

Run makefile.
```sh
make engine
```
