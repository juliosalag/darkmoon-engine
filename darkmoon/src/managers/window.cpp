#include <darkmoon/managers/window.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#define STB_IMAGE_IMPLEMENTATION
#include "./../libs/stb_image.h"

#pragma GCC diagnostic pop

#ifdef _WIN32
    #include <intrin.h>
    #include <windows.h>
#else
    #include <immintrin.h>
    #include <time.h>
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
    glfwSetDropCallback(m_window, drop_callback);
    glfwSetJoystickCallback(joystick_callback);

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

    // --------------------------------- //
    // Scan already-connected gamepads   //
    // --------------------------------- //
    for(int jid = 0; jid <= GLFW_JOYSTICK_LAST; ++jid){
        m_input.gamepads[jid].connected = (glfwJoystickIsGamepad(jid) == GLFW_TRUE);
        if(m_input.gamepads[jid].connected)
            std::cout << "[OK] Gamepad " << jid << " found at startup: "
                      << (glfwGetGamepadName(jid) ? glfwGetGamepadName(jid) : "unknown") << "\n";
    }

    SetIconDefault();
}

void Window::Close(){ 
    //m_resourceManager.unloadAllResources();
    #ifdef _WIN32
        timeEndPeriod(1);
    #endif
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    //if(m_window) glfwDestroyWindow(m_window); 
};

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

void Window::EndDrawingNoPoll(){
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
    m_resized = false;
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

Vector2Df Window::GetCursorPosition(){
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    return Vector2Df{static_cast<float>(xpos), static_cast<float>(ypos)};
}

void Window::SetCursorPositionX(int xpos){
    glfwSetCursorPos(m_window, static_cast<double>(xpos), GetCursorPositionY());
}

void Window::SetCursorPositionY(int ypos){
    glfwSetCursorPos(m_window, GetCursorPositionX(), static_cast<double>(ypos));
}

void Window::SetCursorPosition(Vector2Df position){
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

    const GLFWvidmode* mode = glfwGetVideoMode(monitor.handle());

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowMonitor(m_window, monitor.handle(), 0, 0, mode->width, mode->height, mode->refreshRate);
    
    m_mode = WindowMode::Fullscreen;
}

void Window::SetBorderless(Monitor monitor){

    if(m_mode == WindowMode::Windowed){
        glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor.handle());

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwSetWindowMonitor(m_window, monitor.handle(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);

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

void Window::SetSize(Vector2Df size){
    SetSize(static_cast<int>(size.x), static_cast<int>(size.y));
}

void Window::SetPosition(int x, int y){
    if(m_mode == WindowMode::Windowed){
        m_windowedX = x;
        m_windowedY = y;
        glfwSetWindowPos(m_window, x, y);  
    }
}

void Window::SetPosition(Vector2Df position){
    SetPosition(static_cast<int>(position.x), static_cast<int>(position.y));
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

void Window::SetIconDefault(){
    static const unsigned char icon[16 * 16 * 4] = {
        80,61,117,255,   68,58,111,255,   58,61,112,255,   64,72,121,255,   67,64,119,255,   62,57,113,255,   73,70,127,255,   60,63,118,255,   61,73,125,255,   44,54,107,255,   72,67,123,255,   69,58,116,255,   65,50,107,255,   73,64,119,255,   65,65,115,255,   56,62,110,255,
        70,55,110,255,   60,53,105,255,   59,67,116,255,   55,65,114,255,   65,62,117,255,   68,63,119,255,   58,57,114,255,   60,65,120,255,   53,63,116,255,   68,73,128,255,   63,55,112,255,   74,63,121,255,   74,67,121,255,   64,61,114,255,   60,62,111,255,   59,65,113,255,
        68,63,117,255,   66,69,120,255,   56,70,117,255,   51,65,112,255,   63,60,115,255,   69,61,118,255,   58,63,118,255,   64,74,127,255,   57,64,118,255,   70,69,126,255,   71,57,116,255,   68,57,115,255,   53,59,109,255,   54,66,114,255,   49,55,105,255,   71,74,125,255,
        54,54,106,255,   67,70,121,255,   51,63,111,255,   57,74,120,255,   55,65,116,255,   48,64,113,255,   45,73,120,255,   31,62,108,255,   43,67,115,255,   49,62,114,255,   66,69,122,255,   63,66,119,255,   52,68,117,255,   63,80,126,255,   64,64,116,255,   66,59,113,255,
        64,61,114,255,   66,63,116,255,   56,57,111,255,   53,69,118,255,   36,75,118,255,   47,107,143,255,  45,118,150,255,  38,111,143,255,  50,110,146,255,  59,108,148,255,  52,95,137,255,   33,68,110,255,   43,65,112,255,   57,64,116,255,   74,61,117,255,   68,44,104,255,
        57,69,117,255,   56,66,115,255,   57,64,116,255,   46,73,118,255,   47,113,147,255,  89,181,206,255,  74,173,196,255,  93,187,212,255,  113,194,223,255, 118,193,224,255, 120,195,226,255, 58,121,156,255,  41,76,118,255,   59,71,121,255,   56,47,102,255,   88,70,128,255,
        24,68,107,255,   68,107,148,255,  41,74,117,255,   50,100,137,255,  83,175,200,255,  109,220,239,255, 86,192,214,255,  102,200,225,255, 111,201,228,255, 107,197,224,255, 107,206,229,255, 96,182,209,255,  30,80,117,255,   44,71,116,255,   55,69,118,255,   50,60,111,255,
        48,113,145,255,  124,187,220,255, 23,82,116,255,   39,113,142,255,  95,202,222,255,  95,212,229,255,  98,204,226,255,  106,204,229,255, 106,200,226,255, 86,184,209,255,  9,118,139,255,   109,205,229,255, 22,85,120,255,   34,76,116,255,   148,179,223,255, 48,75,120,255,
        36,111,140,255,  93,173,200,255,  111,198,225,255, 26,125,148,255,  86,195,216,255,  76,185,206,255,  25,123,150,255,  103,197,225,255, 101,199,226,255, 84,186,209,255,  17,122,144,255,  103,199,223,255, 14,85,117,255,   128,182,220,255, 128,170,210,255, 32,69,111,255,
        15,80,112,255,   50,125,154,255,  83,179,203,255,  19,128,148,255,  95,206,226,255,  81,187,209,255,  23,121,148,255,  95,193,220,255,  99,205,229,255,  92,202,225,255,  93,202,223,255,  107,203,228,255, 108,178,212,255, 110,159,199,255, 47,80,125,255,   47,71,117,255,
        47,84,128,255,   37,91,129,255,   85,168,198,255,  96,198,221,255,  103,215,237,255, 81,193,215,255,  100,206,230,255, 102,212,235,255, 85,208,226,255,  83,208,226,255,  73,192,212,255,  97,193,218,255,  25,83,121,255,   48,75,122,255,   52,55,110,255,   75,67,126,255,
        42,75,120,255,   28,76,116,255,   47,126,157,255,  97,197,221,255,  91,203,225,255,  79,196,216,255,  12,121,144,255,  99,211,233,255,  89,212,230,255,  69,196,213,255,  93,214,233,255,  88,186,213,255,  34,87,127,255,   42,62,113,255,   77,67,127,255,   72,50,114,255,
        30,82,121,255,   39,102,137,255,  118,201,231,255, 99,199,223,255,  93,208,229,255,  90,207,227,255,  82,188,214,255,  12,116,143,255,  13,123,148,255,  101,217,240,255, 88,207,227,255,  96,196,222,255,  123,183,220,255, 52,78,127,255,   59,56,113,255,   71,53,115,255,
        31,102,134,255,  125,206,235,255, 77,175,200,255,  82,191,212,255,  92,207,228,255,  83,198,219,255,  98,202,229,255,  89,190,218,255,  79,180,208,255,  7,113,139,255,   96,208,230,255,  104,206,231,255, 117,193,225,255, 33,78,120,255,   49,59,112,255,   82,72,132,255,
        35,129,155,255,  69,174,196,255,  96,217,234,255,  80,206,221,255,  72,187,208,255,  100,209,232,255, 101,207,233,255, 94,198,225,255,  110,208,237,255, 107,205,234,255, 100,200,226,255, 84,186,211,255,  85,190,212,255,  111,190,221,255, 54,80,129,255,   51,52,109,255,
        91,197,219,255,  78,193,212,255,  86,220,232,255,  58,192,204,255,  96,208,230,255,  99,205,229,255,  78,184,210,255,  101,205,232,255, 83,178,208,255,  107,200,231,255, 105,199,227,255, 83,185,210,255,  95,212,230,255,  109,203,229,255, 25,62,107,255,   64,71,126,255,
    };

    GLFWimage image;
    image.width  = 16;
    image.height = 16;
    image.pixels = const_cast<unsigned char*>(icon);

    glfwSetWindowIcon(m_window, 1, &image);
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

Vector2Df Window::GetSize(){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    
    return Vector2Df{static_cast<float>(width), static_cast<float>(height)};
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

Vector2Df Window::GetPosition(){
    int xpos, ypos;
    glfwGetWindowPos(m_window, &xpos, &ypos);

    return Vector2Df{static_cast<float>(xpos), static_cast<float>(ypos)};
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
        in vec2 TextCoord;

        uniform sampler2D texture1;
        uniform vec4 customColor;

        void main()
        {
            FragColor = texture(texture1, TextCoord) * customColor;
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