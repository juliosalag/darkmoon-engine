#pragma once

struct Window{
/*
// Variable de clase

- id (autoincremental)
- GLFWwindow*

// ------------------------------------------

// Constructor

IMPORTANTE --> Las window creation hints se ponen por defecto en el glfwInit y hay que
setearlas con los valores que queramos antes de la creacion de la ventana y del contexto:

https://www.glfw.org/docs/latest/window_guide.html#window_hints

- width, height, Title, NULL, NULL
glfwCreateWindow

// ------------------------------------------

// Destructor

glfwDestroyWindow

// ------------------------------------------

// Event processing

toda esta parte es la relacionada con el input, que se va a encargar otro sistema: INPUT SYSTEM

// ------------------------------------------

// Funciones: window properties and events

- Window closing and close flag
- Window size
- Framebuffer size
- Window content scale

- Window position
- Window title
- Window icon
- Window monitor
- Window iconification
- Window maximization
- Window visibility
- Window input focus


- Window transparency
- Window attributes

// ------------------------------------------

Swap buffers y renderizado se encargara otro sistema: RENDER SYSTEM

*/
};