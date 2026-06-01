#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "monitor.hpp"
#include <darkmoon/utils/keys.hpp>
#include <darkmoon/utils/color.hpp>
#include <darkmoon/utils/math.hpp>

#include "resource_manager.hpp"

enum struct WindowMode{
    Windowed,
    Borderless,
    Fullscreen
};

struct Window{
    // Create window, load OpenGL functions pointers and configure OpenGL
    Window(int width, int height, const char* title, Window* sharedContext = nullptr);
    Window() {};
    ~Window(){ Close(); };

    // --------------- //
    // Basic Functions //
    // --------------- //

    // Destroy the window and close it
    void Close();
    // Return whether the window should close
    bool ShouldClose(){ return m_window && glfwWindowShouldClose(m_window); };
    // Iconify the window (minimize it)
    void Iconify(){ glfwIconifyWindow(m_window); };
    // Maximife the window
    void Maximize(){ glfwMaximizeWindow(m_window); };
    // Restore the window to its previous size and state
    void Restore(){ glfwRestoreWindow(m_window); };
    // Hide the window from view
    void Hide(){ glfwHideWindow(m_window); };
    // Show the window
    void Show(){ 
        SetSize({static_cast<float>(m_windowedWidth), static_cast<float>(m_windowedHeight)});
        SetPosition({static_cast<float>(m_windowedX), static_cast<float>(m_windowedY)});
        glfwShowWindow(m_window); 
    };
    // Focus on the window (bring it to the front)
    void Focus(){ 
        glfwFocusWindow(m_window); 
    };
    void Current(){
        glfwMakeContextCurrent(m_window);
    }
    // Return if the window was resized
    bool WasResized(){
        return m_resized;
    }

    // ------- //
    // Drawing //
    // ------- //

    // Prepares the OpenGL context for rendering by setting up the projection matrix and defining the viewport size
    void BeginDrawing();
    void BeginDrawing(Color color);
    // Swaps the buffers to display the rendered content and processes any pending events
    void EndDrawing();
    void EndDrawingNoPoll();
    // Clear background with color
    void ClearBackground(Color color);

    // ------- //
    // Shaders //
    // ------- //

    Shader* CreateShader(const char* vertexPath = "", const char* fragmentPath = "", const char* geometryPath = ""){
        Current();
        return m_resourceManager.loadResource<Shader>(vertexPath, fragmentPath, geometryPath);
    }

    Shader* CreateShader(const std::string& vertexCode = "", const std::string& fragmentCode  = "", const std::string& geometryCode = ""){
        Current();
        return m_resourceManager.loadResource<Shader>((vertexCode + fragmentCode + geometryCode).c_str(), vertexCode, fragmentCode, geometryCode);
    }

    Shader* GetBasicShader2D(){ return m_shaders["basic2D"]; };
    Shader* GetBasicTextureShader2D(){ return m_shaders["basicTexture2D"]; };
    Shader* GetBasicFontShader2D(){ return m_shaders["basicFont2D"]; };

    // --------------- //
    // Input Functions //
    // --------------- //

    void PollEvents();

    // Keyboard input

    void SetExitKey(int key){ m_exitKey = key; };
    char GetLastCharPressed(){ return static_cast<char>(m_input.lastChar); };

    bool IsKeyPressed(int key){ return (m_input.keysLast[key] == false && m_input.keys[key]); };
    bool IsKeyReleased(int key){ return (m_input.keysLast[key] && m_input.keys[key] == false); };
    bool IsKeyDown(int key){ return (m_input.keysLast[key] && m_input.keys[key]); };
    bool IsKeyUp(int key){ return (m_input.keysLast[key] == false && m_input.keys[key] == false); };
    
    // Mouse input

    // Set custom cursor (Normal size: 16 x 16 x 4)
    void SetCustomCursor(const char* cursorPath);
    void ResetCursor();

    int GetCursorPositionX();
    int GetCursorPositionY();
    Vector2Df GetCursorPosition();

    void SetCursorPositionX(int xpos);
    void SetCursorPositionY(int ypos);
    void SetCursorPosition(Vector2Df position);
    
    bool IsCursorHover(){ return glfwGetWindowAttrib(m_window, GLFW_HOVERED); };

    void DisableCursor(){ glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); };
    void HideCursor(){ glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); };
    void EnableCursor(){ glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); };

    bool IsMouseButtonPressed(int button){ return (m_input.mouseButtonsLast[button] == false && m_input.mouseButtons[button]); };
    bool IsMouseButtonReleased(int button){ return (m_input.mouseButtonsLast[button] && m_input.mouseButtons[button] == false); };
    bool IsMouseButtonDown(int button){ return (m_input.mouseButtonsLast[button] && m_input.mouseButtons[button]); };
    bool IsMouseButtonUp(int button){ return (m_input.mouseButtonsLast[button] == false && m_input.mouseButtons[button] == false); };
    
    int GetXoffsetScroll(){ return m_input.xoffset; };
    int GetYoffsetScroll(){ return m_input.yoffset; };

    // Gamepad / Joystick input

    // Is the gamepad connected? (id 0..15)
    bool IsGamepadAvailable(int id = 0) const {
        if(id < 0 || id > GLFW_JOYSTICK_LAST) return false;
        return m_input.gamepads[id].connected;
    }

    // Name of the gamepad (may be nullptr if not connected)
    const char* GetGamepadName(int id = 0) const {
        return glfwGetGamepadName(id);
    }

    // Pressed  = was released, now pressed (rising edge)
    bool IsGamepadButtonPressed(int button, int id = 0) const {
        if(id < 0 || id > GLFW_JOYSTICK_LAST) return false;
        if(!m_input.gamepads[id].connected) return false;
        return !m_input.gamepads[id].buttonsLast[button] &&
                m_input.gamepads[id].buttons[button];
    }
    // Released = was pressed, now released (falling edge)
    bool IsGamepadButtonReleased(int button, int id = 0) const {
        if(id < 0 || id > GLFW_JOYSTICK_LAST) return false;
        if(!m_input.gamepads[id].connected) return false;
        return  m_input.gamepads[id].buttonsLast[button] &&
               !m_input.gamepads[id].buttons[button];
    }
    // Down = was and still is pressed
    bool IsGamepadButtonDown(int button, int id = 0) const {
        if(id < 0 || id > GLFW_JOYSTICK_LAST) return false;
        if(!m_input.gamepads[id].connected) return false;
        return m_input.gamepads[id].buttonsLast[button] &&
               m_input.gamepads[id].buttons[button];
    }
    // Up = was and still is released
    bool IsGamepadButtonUp(int button, int id = 0) const {
        if(id < 0 || id > GLFW_JOYSTICK_LAST) return false;
        if(!m_input.gamepads[id].connected) return false;
        return !m_input.gamepads[id].buttonsLast[button] &&
               !m_input.gamepads[id].buttons[button];
    }

    // Raw axis value in [-1.0, 1.0]
    // Axes: GLFW_GAMEPAD_AXIS_LEFT_X/Y, RIGHT_X/Y,
    //       GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
    float GetGamepadAxis(int axis, int id = 0) const {
        if(id < 0 || id > GLFW_JOYSTICK_LAST) return 0.0f;
        if(!m_input.gamepads[id].connected) return 0.0f;
        return m_input.gamepads[id].axes[axis];
    }

    // Axis delta relative to previous frame
    float GetGamepadAxisDelta(int axis, int id = 0) const {
        if(id < 0 || id > GLFW_JOYSTICK_LAST) return 0.0f;
        if(!m_input.gamepads[id].connected) return 0.0f;
        return m_input.gamepads[id].axes[axis] -
               m_input.gamepads[id].axesLast[axis];
    }

    // Axis value with deadzone applied (returns 0.0 if within deadzone)
    float GetGamepadAxisDeadzone(int axis, float deadzone = 0.1f, int id = 0) const {
        float v = GetGamepadAxis(axis, id);
        return (v > deadzone || v < -deadzone) ? v : 0.0f;
    }

    // Left stick as Vector2Df with deadzone
    Vector2Df GetGamepadLeftStick(float deadzone = 0.1f, int id = 0) const {
        return { GetGamepadAxisDeadzone(GLFW_GAMEPAD_AXIS_LEFT_X,  deadzone, id),
                 GetGamepadAxisDeadzone(GLFW_GAMEPAD_AXIS_LEFT_Y,  deadzone, id) };
    }
    // Right stick as Vector2Df with deadzone
    Vector2Df GetGamepadRightStick(float deadzone = 0.1f, int id = 0) const {
        return { GetGamepadAxisDeadzone(GLFW_GAMEPAD_AXIS_RIGHT_X, deadzone, id),
                 GetGamepadAxisDeadzone(GLFW_GAMEPAD_AXIS_RIGHT_Y, deadzone, id) };
    }
    // Left trigger normalized to [0.0, 1.0] (GLFW returns [-1, 1])
    float GetGamepadLeftTrigger(float deadzone = 0.05f, int id = 0) const {
        return std::max(0.0f, (GetGamepadAxis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,  id) + 1.0f) * 0.5f - deadzone);
    }
    // Right trigger normalized to [0.0, 1.0]
    float GetGamepadRightTrigger(float deadzone = 0.05f, int id = 0) const {
        return std::max(0.0f, (GetGamepadAxis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, id) + 1.0f) * 0.5f - deadzone);
    }

    // ---------- //
    // FPS & Time //
    // ---------- //
    void SetTargetFPS(int fps);
    int GetTargetFPS() const { return m_targetFPS; };
    int  GetFPS() const { return m_fps; }
    float GetDeltaTime() const { return m_deltaTime; }
    double GetTime() const { return glfwGetTime(); }

    // --------- //
    // Clipboard //
    // --------- //

    const char* GetClipboardString(){ return glfwGetClipboardString(m_window); };
    void SetClipboardString(const char* text){ glfwSetClipboardString(m_window, text); }

    // --------- //
    // Path drop //
    // --------- //

    void SetDropCallback(std::function<void(int, const char**)> callback){ m_dropCallback = callback; };
    const std::string& GetLastDroppedPath(){ return m_lastDroppedPath; };
    bool IsFileDropped(){ return !m_lastDroppedPath.empty(); };
    void ClearDroppedPath(){ m_lastDroppedPath = ""; };

    // ------- //
    // Setters //
    // ------- //

    // Set the window to fullscreen mode
    void SetFullscreen(Monitor monitor = glfwGetPrimaryMonitor());
    // Set the window to borderless mode
    void SetBorderless(Monitor monitor = glfwGetPrimaryMonitor());
    // Set the window to windowed mode
    void SetWindowed();
    // Set the window mode (Fullscreen, Borderless and Windowed)
    void SetWindowMode(WindowMode mode, Monitor monitor = glfwGetPrimaryMonitor());
    // Set the window size
    void SetSize(int width, int height);
    // Set the window size (vector)
    void SetSize(Vector2Df size);
    // Set the window position on the screen
    void SetPosition(int x, int y);
    // Set the window position on the screen (vector)
    void SetPosition(Vector2Df position);
    // Set the window title
    void SetTitle(const char* title);
    // Set the window icon from the specified file path
    void SetIcon(const char* iconPath);
    void SetIconDefault();
    // Set the window opacity (0.0 to 1.0)
    void SetOpacity(float opacity);

    // ------- //
    // Getters //
    // ------- //

    WindowMode GetWindowMode(){ return m_mode; };
    GLFWwindow* GetWindow(){ return m_window; };

    int GetWidth();
    int GetHeight();
    Vector2Df GetSize();

    int GetPositionX();
    int GetPositionY();
    Vector2Df GetPosition();

    float GetOpacity(){ return glfwGetWindowOpacity(m_window); };

    int GetExitKey(){ return m_exitKey; };

private:

    // ---------- //
    // Properties //
    // ---------- //

    struct GamepadState {
        bool  buttons    [GLFW_GAMEPAD_BUTTON_LAST + 1] { false };
        bool  buttonsLast[GLFW_GAMEPAD_BUTTON_LAST + 1] { false };
        float axes       [GLFW_GAMEPAD_AXIS_LAST   + 1] { 0.0f  };
        float axesLast   [GLFW_GAMEPAD_AXIS_LAST   + 1] { 0.0f  };
        bool  connected  { false };
    };

    struct Input{
        // Keyboard
        bool keys[GLFW_KEY_LAST] { false };
        bool keysLast[GLFW_KEY_LAST] { false };
        unsigned int lastChar {};
        
        // Mouse buttons
        bool mouseButtons[GLFW_MOUSE_BUTTON_LAST] {false};
        bool mouseButtonsLast[GLFW_MOUSE_BUTTON_LAST] {false};
        int xoffset {}, yoffset {};

        // Gamepad / Joystick
        GamepadState gamepads[GLFW_JOYSTICK_LAST + 1] {};
    };

    Input m_input {};
    WindowMode m_mode {};
    GLFWwindow* m_window {};
    int m_windowedX {}, m_windowedY {}, m_windowedWidth {}, m_windowedHeight {};
    int m_exitKey { KEY_ESCAPE };

    ResourceManager& m_resourceManager = ResourceManager::getInstance();

    // Path drop
    std::function<void(int, const char**)> m_dropCallback {};
    std::string m_lastDroppedPath {};

    // Shaders
    std::map<std::string, Shader*> m_shaders;

    // Resize
    bool m_resized = { false };

    void LoadBasicShaders();

    // FPS & Time
    int    m_targetFPS    { 60 };
    float  m_deltaTime    { 0.0f };
    int    m_fps          { 0 };
    double m_lastTime     { 0.0 };
    double m_fpsTimer     { 0.0 };
    int    m_fpsCounter   { 0 }; 

    // --------- //
    // Functions //
    // --------- //

    void UpdateInput(){
        for(int i=0; i<GLFW_KEY_LAST; i++)
            m_input.keysLast[i] = m_input.keys[i];

        for(int i=0; i<GLFW_MOUSE_BUTTON_LAST; i++)
            m_input.mouseButtonsLast[i] = m_input.mouseButtons[i];

        m_input.xoffset = 0;
        m_input.yoffset = 0;

        // Gamepads
        for(int jid = 0; jid <= GLFW_JOYSTICK_LAST; ++jid){
            auto& gp = m_input.gamepads[jid];

            gp.connected = (glfwJoystickIsGamepad(jid) == GLFW_TRUE);

            if(!gp.connected){
                std::fill(std::begin(gp.buttons),      std::end(gp.buttons),      false);
                std::fill(std::begin(gp.buttonsLast),  std::end(gp.buttonsLast),  false);
                std::fill(std::begin(gp.axes),         std::end(gp.axes),         0.0f);
                std::fill(std::begin(gp.axesLast),     std::end(gp.axesLast),     0.0f);
                continue;
            }

            std::copy(std::begin(gp.buttons), std::end(gp.buttons), std::begin(gp.buttonsLast));
            std::copy(std::begin(gp.axes),    std::end(gp.axes),    std::begin(gp.axesLast));

            GLFWgamepadstate state;
            if(glfwGetGamepadState(jid, &state) == GLFW_TRUE){
                for(int b = 0; b <= GLFW_GAMEPAD_BUTTON_LAST; ++b)
                    gp.buttons[b] = (state.buttons[b] == GLFW_PRESS);
                for(int a = 0; a <= GLFW_GAMEPAD_AXIS_LAST; ++a)
                    gp.axes[a] = state.axes[a];
            }
        }
    };

    // --------- //
    // Callbacks //
    // --------- //

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
        glfwMakeContextCurrent(window);
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        //std::cout << width << " - " << height << "\n";
        
        if(win){
            glViewport(0, 0, width, height);
            win->m_resized = true;
        }
    }

    static void key_callback(GLFWwindow* window, int key, int, int action, int) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if(key >= 0){
            if(action == GLFW_PRESS)
                win->m_input.keys[key] = true;
            else if(action == GLFW_RELEASE)
                win->m_input.keys[key] = false; 
        }
        
        if(key == win->GetExitKey() && action == GLFW_RELEASE){
            win->m_resourceManager.unloadAllResources();
            glfwSetWindowShouldClose(win->GetWindow(), GLFW_TRUE);
        }
    }

    static void char_callback(GLFWwindow* window, unsigned int codepoint) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        
        win->m_input.lastChar = codepoint;
    }

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if(button >= 0){
            if(action == GLFW_PRESS)
                win->m_input.mouseButtons[button] = true;
            else if(action == GLFW_RELEASE)
                win->m_input.mouseButtons[button] = false;
        }
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        win->m_input.xoffset = static_cast<int>(xoffset);
        win->m_input.yoffset = static_cast<int>(yoffset);
    }

    static void window_close_callback(GLFWwindow* window){
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        win->m_resourceManager.unloadAllResources();
    }

    static void drop_callback(GLFWwindow* window, int count, const char** paths){
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if(count > 0)
            win->m_lastDroppedPath = paths[count - 1]; // save the last
        if(win->m_dropCallback)
            win->m_dropCallback(count, paths);
    }

    static void joystick_callback(int jid, int event){
        if(event == GLFW_CONNECTED)
            std::cout << "[OK] Gamepad " << jid << " connected: "
                      << (glfwGetGamepadName(jid) ? glfwGetGamepadName(jid) : "unknown") << "\n";
        else if(event == GLFW_DISCONNECTED)
            std::cout << "[INFO] Gamepad " << jid << " disconnected\n";
    }
};