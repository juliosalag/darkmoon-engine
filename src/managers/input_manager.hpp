#pragma once

#include "windows_manager.hpp"

#include <iostream>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <unordered_set>

namespace DarkMoon {
    struct InputManager {
    public:
        // Update
        void updateBeginFrame();
        void updateEndFrame();

        // ------------------------ //
        // Input Handling Functions //
        // ------------------------ //

        // Input-related functions: keyboard
        bool isKeyPressed(int key);
        bool isKeyPressedRepeat(int key);
        bool isKeyDown(int key);
        bool isKeyReleased(int key);
        bool isKeyUp(int key);
        bool isAnyKeyPressed();
        bool isAnyKeyDown();
        bool isAnyKeyReleased();

        void setExitKey(int key);

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        // Input-related functions: gamepad
        bool isGamePadAvailable(int gamepad);
        const char* getGamePadName(int gamepad);
        bool isGamepadButtonPressed(int gamepad, int button);
        bool isGamepadButtonDown(int gamepad, int button);
        bool isGamepadButtonReleased(int gamepad, int button);
        bool isGamepadButtonUp(int gamepad, int button);
        int getGamepadAxisCount(int gamepad);
        float getGamepadAxisMovement(int gamepad, int axis);

        // Input-related functions: mouse
        bool isMouseButtonPressed(int button);
        bool isMouseButtonDown(int button);
        bool isMouseButtonReleased(int button);
        bool isMouseButtonUp(int button);
        int getMouseX(GLFWwindow* window);
        int getMouseY(GLFWwindow* window);
        void setMousePosition(GLFWwindow* window, int x, int y);

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        // Instance
        static InputManager& getInstance() {
            static InputManager instance;
            return instance;
        }

    private:
        // Exit key
        int m_exitKey = { GLFW_KEY_ESCAPE };

        // Key States
        int m_keyStates[GLFW_KEY_LAST] = { GLFW_RELEASE };
        int m_keyReleaseStates[GLFW_KEY_LAST] = { GLFW_RELEASE };
        int m_keyPressedStates[GLFW_KEY_LAST] = { GLFW_RELEASE };

        // Gamepad State
        GLFWgamepadstate m_gamepadStates {};
        GLFWgamepadstate m_gamepadStatesprev {};
        GLFWgamepadstate m_gamepadPressedStates {};
        GLFWgamepadstate m_gamepadReleasedStates {};

        // Mouse Button States
        int m_mouseButtonStates[GLFW_MOUSE_BUTTON_LAST] = { GLFW_RELEASE };
        int m_mouseButtonReleaseStates[GLFW_MOUSE_BUTTON_LAST] = { GLFW_RELEASE };
        int m_mouseButtonPressedStates[GLFW_MOUSE_BUTTON_LAST] = { GLFW_RELEASE };
    };
}