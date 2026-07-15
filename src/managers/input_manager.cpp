#include "input_manager.hpp"

namespace DarkMoon {

    // Input-related functions: keyboard

    bool InputManager::isKeyPressed(int key) { return m_keyPressedStates[key]; }
    bool InputManager::isKeyReleased(int key) { return m_keyReleaseStates[key]; }
    bool InputManager::isKeyPressedRepeat(int key) { return m_keyStates[key] == GLFW_REPEAT; }
    bool InputManager::isKeyDown(int key) {
        return m_keyStates[key] == GLFW_PRESS || m_keyStates[key] == GLFW_REPEAT;
    }
    bool InputManager::isKeyUp(int key) { return m_keyStates[key] == GLFW_RELEASE; }

    void InputManager::setExitKey(int key) { m_exitKey = key; }

    void InputManager::keyCallback(GLFWwindow* window, int key, int, int action, int) {
        auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

        // Update the state of the key
        if (key >= 0) {
            im->m_keyStates[key] = action;

            if (action == GLFW_RELEASE)
                im->m_keyReleaseStates[key] = true;
            else if (action == GLFW_PRESS)
                im->m_keyPressedStates[key] = true;
        }

        // Mark the GLFW window to be closed
        if (key == im->m_exitKey)
            glfwSetWindowShouldClose(WindowsManager::getInstance().getWindow(), GLFW_TRUE);
    }

    bool InputManager::isAnyKeyPressed() {
        for (int i = 0; i < GLFW_KEY_LAST; i++) {
            if (m_keyPressedStates[i])
                return true;
        }

        for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
        {
            if (m_gamepadPressedStates.buttons[i])
                return true;
        }

        return false;
    }

    bool InputManager::isAnyKeyDown() {
        for (int i = 0; i < GLFW_KEY_LAST; i++) {
            if (m_keyStates[i] == GLFW_PRESS || m_keyStates[i] == GLFW_REPEAT)
                return true;
        }

        return false;
    }

    bool InputManager::isAnyKeyReleased() {
        for (int i = 0; i < GLFW_KEY_LAST; i++) {
            if (m_keyReleaseStates[i])
                return true;
        }

        return false;
    }

    // Input-related functions: gamepad

    bool InputManager::isGamePadAvailable(int gamepad) {
        return glfwJoystickPresent(gamepad);
    }

    const char* InputManager::getGamePadName(int gamepad) {
        const char* name = glfwGetJoystickName(gamepad);
        return name ? name : "Uknown";
    }

    bool InputManager::isGamepadButtonPressed(int, int button) {
        return m_gamepadPressedStates.buttons[button];
    }

    bool InputManager::isGamepadButtonDown(int, int button) {
        return m_gamepadStates.buttons[button];
    }

    bool InputManager::isGamepadButtonReleased(int, int button) {
        return m_gamepadReleasedStates.buttons[button];
    }

    bool InputManager::isGamepadButtonUp(int, int button) {
        return !m_gamepadStates.buttons[button];
    }

    int InputManager::getGamepadAxisCount(int gamepad) {
        int axes;
        glfwGetJoystickAxes(gamepad, &axes);
        return axes;
    }

    float InputManager::getGamepadAxisMovement(int, int axis) {
        return m_gamepadStates.axes[axis];
    }

    // Update

    void InputManager::updateBeginFrame() {};

    void InputManager::updateEndFrame() {
        // Update mouse state
        for (auto& sta : m_mouseButtonReleaseStates) sta = false;
        for (auto& sta : m_mouseButtonPressedStates) sta = false;

        // Update key state
        for (auto& sta : m_keyReleaseStates) sta = false;
        for (auto& sta : m_keyPressedStates) sta = false;

        // Update gamepad state
        for (auto& sta : m_gamepadReleasedStates.buttons) sta = false;
        for (auto& sta : m_gamepadPressedStates.buttons) sta = false;

        int jid = 0;
        if (glfwGetGamepadState(jid, &m_gamepadStates)) {
            for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
                if (m_gamepadStates.buttons[i] && !m_gamepadStatesprev.buttons[i]) {
                    m_gamepadPressedStates.buttons[i] = true;
                }
                if (!m_gamepadStates.buttons[i] && m_gamepadStatesprev.buttons[i]) {
                    m_gamepadReleasedStates.buttons[i] = true;
                }
            }

            m_gamepadStatesprev = m_gamepadStates;
        }
    }

    // Input-related functions: mouse
    bool InputManager::isMouseButtonPressed(int button) { return m_mouseButtonPressedStates[button]; }
    bool InputManager::isMouseButtonDown(int button) { return m_mouseButtonStates[button] == GLFW_PRESS; }
    bool InputManager::isMouseButtonReleased(int button) { return m_mouseButtonReleaseStates[button]; }
    bool InputManager::isMouseButtonUp(int button) { return m_mouseButtonStates[button] == GLFW_RELEASE; }

    int InputManager::getMouseX(GLFWwindow* window) {
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);
        return static_cast<int>(posX);
    }

    int InputManager::getMouseY(GLFWwindow* window) {
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);
        return static_cast<int>(posY);
    }

    void InputManager::setMousePosition(GLFWwindow* window, int x, int y) { glfwSetCursorPos(window, x, y); }

    void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int) {
        auto* im = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

        if (button >= 0) {
            im->m_mouseButtonStates[button] = action;

            if (action == GLFW_RELEASE)
                im->m_mouseButtonReleaseStates[button] = true;
            else if (action == GLFW_PRESS)
                im->m_mouseButtonPressedStates[button] = true;
        }
    }
}