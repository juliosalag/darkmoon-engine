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

| Platform | Status |
|----------|--------|
| Windows  | Supported |
| Linux    | Supported |
| Web      | In progress |

## Features

### Window & Input
Full window lifecycle management built on GLFW, with support for windowed, borderless, and exclusive fullscreen modes. The `Window` class handles OpenGL context creation, shader loading, FPS capping, delta time, and event polling out of the box. Multiple windows with shared contexts are supported.

Input covers keyboard, mouse, and gamepad, with pressed/released/down/up states for all of them. Additional utilities include clipboard access, custom cursor support, file drag-and-drop, and scroll events.

### 2D Rendering
Primitives: pixel, line, triangle, rectangle, circle, and regular polygons.

Texture and asset support: static textures, animated textures (spritesheets), tilesets, and TrueType font rendering. All resources are managed through a centralized `ResourceManager` with support for multiple texture and font formats.

### Camera2D
2D camera system with offset, target tracking, zoom, and rotation.

### Custom Shaders
Create shaders from file paths or inline GLSL source strings. Built-in shaders cover basic 2D geometry, textured quads, and font rendering.

### Entity Component System (ECS)
Header-only, C++23 ECS built around a `SlotMap`-backed component storage and compile-time bitmask filtering. Define your component and tag lists in a `types.hpp` file and get zero-overhead iteration via `forEach` and `forEachAny`.

Key characteristics:
- Components stored in packed `SlotMap` arrays no gaps, O(1) insert/erase/access
- Bitmask queries resolved entirely at compile time using template metaprogramming
- Safe entity destruction during iteration via a `deathSet` pattern
- Tags are zero-size flags with no storage cost

### Audio
> In progress.

### Utilities
Built-in math types (`Vector2Df`, GLM integration), color constants, key/button enums, and memory utilities.

## Dependencies

| Library | Purpose |
|---------|---------|
| [GLFW](https://www.glfw.org/) | Window creation and input |
| [GLAD](https://glad.dav1d.de/) | OpenGL loader |
| [GLM](https://github.com/g-truc/glm) | Math |
| [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) | Image loading |
| [stb_truetype](https://github.com/nothings/stb/blob/master/stb_truetype.h) | Font rendering |

## Installation

> Installation instructions coming soon.

## Usage

> Usage guide and examples coming soon. In the meantime, refer to the [full documentation](https://juliosalag.github.io/darkmoon/index.html).
