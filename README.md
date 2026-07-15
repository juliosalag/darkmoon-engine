# DarkMoon Engine

Custom graphics engine made from scratch in **C++23** and **OpenGL**, specifically tailored for game development projects such as [Two Wonders](https://kaiwagames.itch.io/two-wonders). Version 2.0 is a complete rebuild focused on clean architecture, modularity, and ease of use.

```cpp
#include <darkmoon/graphics.hpp>

int main() {
    DarkMoonEngine dm{};
    Window win = Window(800, 600, "Hello World");

    while (!win.ShouldClose()) {
        win.BeginDrawing(BLACK);
        win.EndDrawing();
    }
}
```

## Platform Support

| Platform | Status      |
|----------|-------------|
| Windows  | Supported   |
| Linux    | Supported   |

## Features

### Window & Input

Full window lifecycle management built on GLFW, with support for windowed, borderless, and exclusive fullscreen modes. The `Window` class handles OpenGL context creation, shader loading, FPS capping, delta time, and event polling out of the box. Multiple windows with shared contexts are supported.

Input covers keyboard, mouse, and gamepad, with pressed/released/down/up states for all of them. Additional utilities include clipboard access, custom cursor support, file drag-and-drop, and scroll events.

### Monitor

Lightweight, non-owning wrapper around GLFW monitor handles. Enumerate connected displays, query supported video modes and physical size/DPI scale, react to connect/disconnect events, and read or write the monitor's gamma ramp.

### 2D Rendering

Primitives: pixel, line, triangle, rectangle, circle, and regular polygons, each with a filled and wireframe (`*Lines`) variant.

Higher-level drawables: static `Texture` with tint/opacity/rotation, `AnimatedTexture` for GIF-based sprite animations, `TileSet` for slicing a texture into an indexed grid, and `Text` for TrueType font rendering. All of them support drawing directly in screen space or through a `Camera2D`.

Every loadable resource (textures, animated textures, fonts, shaders) goes through a centralized `ResourceManager`, which deduplicates loads by file path and type and owns the full resource lifecycle.

### Camera2D

2D camera system with offset, target tracking, zoom, and rotation, plus exact `WorldToScreen` / `ScreenToWorld` conversions for mouse picking and UI-to-world logic.

### Custom Shaders

Create shaders from file paths or inline GLSL source strings. Built-in shaders cover basic 2D geometry, textured quads, and font rendering.

### Entity Component System (ECS)

Header-only, C++23 ECS built around a `SlotMap`-backed component storage and compile-time bitmask filtering. Define your component and tag lists in a `types.hpp` file and get zero-overhead iteration via `forEach` and `forEachAny`.

Key characteristics:
- Components stored in packed `SlotMap` arrays — no gaps, O(1) insert/erase/access
- Bitmask queries resolved entirely at compile time using template metaprogramming
- Safe entity destruction during iteration via a `deathSet` pattern
- Tags are zero-size flags with no storage cost

### Audio

Lightweight RAII wrapper on top of [miniaudio](https://miniaud.io/). Load a sound once and play, stop, loop, or fire overlapping copies of it through a per-object voice pool — ideal for effects that can be triggered rapidly (footsteps, hits, UI clicks). Backed internally by a shared `AudioManager` singleton, with support for WAV, MP3, FLAC, and OGG.

### Utilities

Built-in math types (`Vector2D`, `Vector2Df`, GLM integration), color constants, key/button/gamepad enums, and a `MemoryViewer` debug tool for hex/ASCII-dumping the raw memory of any object.

## Dependencies

| Library                                                                     | Purpose               |
|------------------------------------------------------------------------------|------------------------|
| [GLFW](https://www.glfw.org/)                                                | Window creation and input |
| [GLAD](https://glad.dav1d.de/)                                               | OpenGL loader          |
| [GLM](https://github.com/g-truc/glm)                                         | Math                   |
| [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)        | Image loading          |
| [stb_truetype](https://github.com/nothings/stb/blob/master/stb_truetype.h)  | Font rendering         |
| [miniaudio](https://miniaud.io/)                                             | Audio playback         |

## Installation

> Installation instructions coming soon.

## Usage

> Usage guide and examples coming soon. In the meantime, refer to the [full documentation](https://juliosalag.github.io/darkmoon/index.html).
