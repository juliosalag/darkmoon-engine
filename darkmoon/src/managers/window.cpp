#include <darkmoon/managers/window.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#define STB_IMAGE_IMPLEMENTATION
#include "./../libs/stb_image.h"

#pragma GCC diagnostic pop

#ifdef _WIN32
    #include <intrin.h>
#else
    #include <immintrin.h>
#endif

Window::Window(int width, int height, const char* title, Window* sharedContext){
    #ifdef _WIN32
        timeBeginPeriod(1);
    #endif
    // ------------------ // 
    // Create window GLFW //
    // ------------------ // 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, sharedContext == nullptr ? nullptr : sharedContext->GetWindow());
    if(!m_window){
        std::cerr << "[ERROR] Failed to create GLFW window\n";
        glfwTerminate();

        // return false;
    } 
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);
    glfwSetWindowUserPointer(m_window, this);

    // Callbacks
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCharCallback(m_window, char_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetWindowCloseCallback(m_window, window_close_callback);

    glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
    glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);

    std::cout << "[OK] GLFW window created successfully\n";

    // ------------------------------ //
    // Load OpenGL functions pointers //
    // ------------------------------ //

    if (sharedContext == nullptr){
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "[ERROR] Failed to initialize GLAD\n";
            if(m_window)
                glfwDestroyWindow(m_window);
            glfwTerminate();

            //return false;
        }
        
        std::cout << "[OK] GLAD initialization successful\n";
    }

    glViewport(0, 0, width, height);

    if(sharedContext == nullptr)
        LoadBasicShaders();
    else
        m_shaders = sharedContext->m_shaders;

    m_lastTime = glfwGetTime();
}

// ------- //
// Drawing //
// ------- //

void Window::BeginDrawing(){
    glfwMakeContextCurrent(m_window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    glOrtho(0, width, 0, height, -1, 1);
}

void Window::BeginDrawing(Color color){
    BeginDrawing();
    ClearBackground(color);
}

void Window::EndDrawing(){
    // Calculate deltaTime
    double currentTime = glfwGetTime();
    m_deltaTime = static_cast<float>(currentTime - m_lastTime);

    // FPS cap: busy-wait (as raylib)
    if(m_targetFPS > 0){
        double targetFrameTime = 1.0 / static_cast<double>(m_targetFPS);
        double remaining = targetFrameTime - (glfwGetTime() - m_lastTime);

        if(remaining > 0.002){
            double sleepTime = remaining - 0.002;

            #ifdef _WIN32
                Sleep(static_cast<DWORD>(sleepTime * 1000.0));
            #else
                struct timespec ts;
                ts.tv_sec  = static_cast<time_t>(sleepTime);
                ts.tv_nsec = static_cast<long>((sleepTime - static_cast<double>(ts.tv_sec)) * 1e9);
                nanosleep(&ts, nullptr);
            #endif
        }

        while((glfwGetTime() - m_lastTime) < targetFrameTime) {}

        currentTime = glfwGetTime();
        m_deltaTime = static_cast<float>(currentTime - m_lastTime);
    }

    // Update fps counter
    m_fpsCounter++;
    m_fpsTimer += m_deltaTime;
    if(m_fpsTimer >= 0.1){
        m_fps        = static_cast<int>(m_fpsCounter / m_fpsTimer);
        m_fpsCounter = 0;
        m_fpsTimer  -= 0.1;
    }

    m_lastTime = currentTime;
    m_fps = m_targetFPS;

    // Swap and Events
    PollEvents();
    glfwSwapBuffers(m_window);
}

void Window::ClearBackground(Color color){
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// --------------- //
// Input Functions //
// --------------- //

void Window::PollEvents(){
    UpdateInput();
    glfwPollEvents();
}

void Window::SetCustomCursor(const char* cursorPath){
    int width, height, channels;
    unsigned char* image = stbi_load(cursorPath, &width, &height, &channels, 0);

    if(!image)
        std::cerr << "[ERROR]: Loading cursor image: \"" << cursorPath << "\"\n";

    GLFWimage glfw_img;
    glfw_img.width = width;
    glfw_img.height = height;
    glfw_img.pixels = image;

    glfwSetCursor(m_window, glfwCreateCursor(&glfw_img, 0, 0));  

    stbi_image_free(image);
}

void Window::ResetCursor(){
    glfwSetCursor(m_window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
}

int Window::GetCursorPositionX(){
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    return static_cast<int>(xpos);
}

int Window::GetCursorPositionY(){
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    return static_cast<int>(ypos);
}

Vector2D Window::GetCursorPosition(){
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    return {static_cast<int>(xpos), static_cast<int>(ypos)};
}

void Window::SetCursorPositionX(int xpos){
    glfwSetCursorPos(m_window, static_cast<double>(xpos), GetCursorPositionY());
}

void Window::SetCursorPositionY(int ypos){
    glfwSetCursorPos(m_window, GetCursorPositionX(), static_cast<double>(ypos));
}

void Window::SetCursorPosition(Vector2D position){
    glfwSetCursorPos(m_window, static_cast<double>(position.x), static_cast<double>(position.y));
}

// ---------- //
// FPS & Time //
// ---------- //
void Window::SetTargetFPS(int fps){
    m_targetFPS = (fps > 0) ? fps : 0;
}

// ------- //
// Setters //
// ------- //

void Window::SetFullscreen(Monitor monitor){

    if(m_mode == WindowMode::Windowed){
        glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowMonitor(m_window, monitor.monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    
    m_mode = WindowMode::Fullscreen;
}

void Window::SetBorderless(Monitor monitor){

    if(m_mode == WindowMode::Windowed){
        glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwSetWindowMonitor(m_window, monitor.monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);

    m_mode = WindowMode::Borderless;
}

void Window::SetWindowed(){
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwSetWindowMonitor(m_window, NULL, m_windowedX, m_windowedY, m_windowedWidth, m_windowedHeight, 0);

    m_mode = WindowMode::Windowed;
}

void Window::SetWindowMode(WindowMode mode, Monitor monitor){
    switch (mode){
        case WindowMode::Borderless :
            SetBorderless(monitor);
            break;
        case WindowMode::Fullscreen :
            SetFullscreen(monitor);
            break;
        case WindowMode::Windowed :
            SetWindowed();
            break;

        default:
            break;
    }
}

void Window::SetSize(int width, int height){
    if(m_mode == WindowMode::Windowed){
        m_windowedWidth = width;
        m_windowedHeight = height;
        glfwSetWindowSize(m_window, width, height);
    }
}

void Window::SetSize(Vector2D size){
    SetSize(size.x, size.y);
}

void Window::SetPosition(int x, int y){
    if(m_mode == WindowMode::Windowed){
        m_windowedX = x;
        m_windowedY = y;
        glfwSetWindowPos(m_window, x, y);  
    }
}

void Window::SetPosition(Vector2D position){
    SetPosition(position.x, position.y);
}

void Window::SetTitle(const char* title){
    glfwSetWindowTitle(m_window, title);
}

void Window::SetIcon(const char* iconPath){
    GLFWimage images[1];
    images[0].pixels = stbi_load(iconPath, &images[0].width, &images[0].height, 0, 4);

    glfwSetWindowIcon(m_window, 1, images),

    stbi_image_free(images[0].pixels); 
}

void Window::SetOpacity(float opacity){
    if(opacity >= 0.0f && opacity <= 1.0f)
        glfwSetWindowOpacity(m_window, opacity);
}

// ------- //
// Getters //
// ------- //

int Window::GetWidth(){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    
    return width;
}

int Window::GetHeight(){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    
    return height;
}

Vector2D Window::GetSize(){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    
    return Vector2D{width, height};
}

int Window::GetPositionX(){
    int xpos, ypos;
    glfwGetWindowPos(m_window, &xpos, &ypos);

    return xpos;
}

int Window::GetPositionY(){
    int xpos, ypos;
    glfwGetWindowPos(m_window, &xpos, &ypos);

    return ypos;
}

Vector2D Window::GetPosition(){
    int xpos, ypos;
    glfwGetWindowPos(m_window, &xpos, &ypos);

    return Vector2D{xpos, ypos};
}

// ------- //
// Shaders //
// ------- //

void Window::LoadBasicShaders(){

    // Basic Shader 2D //

    const std::string basicVertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main(){
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const std::string basicFragment = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 customColor;

        void main(){
            FragColor = customColor;
        }
    )";

    m_shaders["basic2D"] = CreateShader(basicVertex, basicFragment);

    // Basic Shader Texture 2D //

    const std::string basicTextureVertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec3 aTextCoord;

        out vec3 ourColor;
        out vec2 TextCoord;

        void main()
        {
        gl_Position = vec4(aPos, 1.0f);
        ourColor = aColor;
        TextCoord = vec2(aTextCoord.x, aTextCoord.y);
        }
    )";

    const std::string basicTextureFragment = R"(
        #version 330 core

        out vec4 FragColor;
        in vec3 ourColor;
        in vec2 TextCoord;
        uniform sampler2D texture1;

        void main()
        {
        FragColor = texture(texture1, TextCoord) * vec4(ourColor, 1.0);
        }
    )";

    m_shaders["basicTexture2D"] = CreateShader(basicTextureVertex, basicTextureFragment);

    // Basic Shader Font 2D 

    const std::string basicFontVertex = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aUV;

        out vec2 vUV;

        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            vUV = aUV;
        }
    )";

    const std::string basicFontFragment = R"(
        #version 330 core
        in vec2 vUV;
        out vec4 fragColor;

        uniform sampler2D uAtlas;
        uniform vec4      customColor;

        void main() {
            float alpha = texture(uAtlas, vUV).r;
            fragColor   = vec4(customColor.rgb, customColor.a * alpha);
        }
    )";

    m_shaders["basicFont2D"] = CreateShader(basicFontVertex, basicFontFragment);
}