#pragma once

#include "components/node.hpp"
#include "components/entity.hpp"
#include "components/entity_model.hpp"
#include "components/entity_camera.hpp"

#include "components/entities2D.hpp"
#include "components/entities3D.hpp"

#include "managers/resource_manager.hpp"
#include "managers/windows_manager.hpp"
#include "managers/render_manager.hpp"
#include "managers/input_manager.hpp"
#include "managers/animator_manager.hpp"
#include "resources/resource_shader.hpp"

#include "utils/keys.hpp"

namespace DarkMoon {
    struct DarkMoonEngine {
    public:
        DarkMoonEngine();
        ~DarkMoonEngine() {};

        // ---------------------- //
        // Node-related functions //
        // ---------------------- //

        void DrawTree();

        // Create node in parentNode
        Node* CreateNode(const char* nodeName, Node* parentNode);
        Node* CreateNodeCopy(Node* copyNode, Node* parentNode);

        // 2D

        // Create pixel in node
        Node* CreatePixel(glm::vec2 position, Color color, const char* nodeName, Node* parentNode);
        // Create line in node
        Node* CreateLine(glm::vec2 startPos, glm::vec2 endPos, Color color, const char* nodeName, Node* parentNode);
        // Create triangle in node
        Node* CreateTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, Color color, const char* nodeName, Node* parentNode);
        // Create rectangle in node
        Node* CreateRectangle(glm::vec2 position, glm::vec2 size, Color color, const char* nodeName, Node* parentNode);
        // Create circle in node
        Node* CreateCircle(glm::vec2 position, float radius, int segments, Color color, const char* nodeName, Node* parentNode);
        // Create texture 2D in node
        Node* CreateTexture2D(glm::vec2 position, const char* filePath, Color color, const char* nodeName, Node* parentNode);
        // Create animated texture 2D in node
        Node* CreateAnimatedTexture2D(glm::vec2 position, std::vector<Texture*> filePaths, Color color, float frameDuration, int currentFrame, const char* nodeName, Node* parentNode);

        // GUI

        // Create text in node
        Node* CreateText(glm::vec2 position, std::string text, Font* font, int fontSize, Color color, Aligned align, bool charByChar, const char* nodeName, Node* parentNode);
        // Create a text box in node
        Node* CreateTextBox(glm::vec2 position, glm::vec2 size, Color boxColor, std::string text, Font* font, int fontSize, Color textColor, Aligned verticalAligned, Aligned horizontalAligned, bool charByChar, const char* nodeName, Node* parentNode);
        // Create a button in node
        Node* CreateButton(glm::vec2 position, glm::vec2 size, std::string text, Font* font, int fontSize, Color textColor, Aligned verticalAligned, Aligned horizontalAligned, Color normalColor, Color hoverColor, Color clickColor, const char* nodeName, Node* parentNode);
        // Create a slider in node
        Node* CreateSlider(glm::vec2 position, glm::vec2 size, float value, Color backColor, Color sliderColor, const char* nodeName, Node* parentNode);
        // Create option slider in node
        Node* CreateOptionSlider(glm::vec2 pos, glm::vec2 sz, Color bCol, std::string txt, Font* f, int fS, int fsArrows, Color tCol, Aligned verAl, Aligned horAl, Color nColor, Color hColor, Color cColor, std::vector<std::string> opts, std::string firstOption, const char* nodeName, Node* parentNode);
        // Create float slider in node
        Node* CreateFloatSlider(glm::vec2 pos, glm::vec2 sz, Color bCol, std::string txt, Font* f, int fS, int fsArrows, Color tCol, Aligned verAl, Aligned horAl, Color nColor, Color hColor, Color cColor, float initialValue, const char* nodeName, Node* parentNode);
        // Create checkbox in node
        Node* CreateCheckbox(glm::vec2 pos, float size, bool checked, Color bCol, Color nCol, Color hCol, const char* nodeName, Node* parentNode);
        // CReate checkbox with pointer in node
        Node* CreateCheckboxPtr(glm::vec2 pos, float size, bool* checked, Color bCol, Color nCol, Color hCol, const char* nodeName, Node* parentNode);

        // 3D

        // Create point 3D in node
        Node* CreatePoint3D(glm::vec3 position, float pointSize, Color color, const char* nodeName, Node* parentNode);
        // Create line 3D in node
        Node* CreateLine3D(glm::vec3 startPos, glm::vec3 endPos, float lineSize, Color color, const char* nodeName, Node* parentNode);
        // Create grid 3D in node
        Node* CreateGrid(int slices, float spacing, Color color, const char* nodeName, Node* parentNode);
        // Create plane 3D (XZ) in node
        Node* CreatePlane(glm::vec3 centerPos, glm::vec2 size, Color color, const char* nodeName, Node* parentNode);
        // Create cube in node
        Node* CreateCube(glm::vec3 position, glm::vec3 size, Color color, const char* nodeName, Node* parentNode);
        // Create cube wires in node
        Node* CreateCubeWires(glm::vec3 position, glm::vec3 size, Color color, const char* nodeName, Node* parentNode);
        // Create model in node
        Node* CreateModel(const char* filePath, Color tint, const char* nodeName, Node* parentNode);
        // Create billboard in node
        Node* CreateBillboard(const char* filePath, glm::vec3 position, glm::vec2 size, const char* nodeName, Node* parentNode);

        // EXTRA

        // Create camera
        Camera* CreateCamera(const char* nodeName, Node* parentNode);
        // Get root node
        Node* GetRootNode();
        // Get default font
        Font* GetDefaultFont() { return m_renderManager.defaultFont; };
        // Get mouse ray cast
        Ray GetMouseRay();
        // Get view matrix
        glm::mat4 GetViewMatrix();

        // LIGHTS

        // Create point light in node
        Node* CreatePointLight(glm::vec3 position, Color color, const char* nodeName, Node* parentNode);
        // Create directional light in node
        Node* CreateDirectionalLight(glm::vec3 direction, Color color, const char* nodeName, Node* parentNode);
        // Create spot light in node
        Node* CreateSpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, Color color, const char* nodeName, Node* parentNode);
        // Toggle lights
        void ToggleLights() { m_renderManager.activeLights = !m_renderManager.activeLights; };
        // Activate lights
        void ActivateLights() { m_renderManager.activeLights = true; };
        // Deactivate lights
        void DeactivateLights() { m_renderManager.activeLights = false; };
        // Update lights
        void UpdateLights(Node* parentNode);
        // AuxUpdateLights
        void AuxUpdateLights(Node* parentNode);

        // SHADER CARTOON

        // Toggle shader cartoon
        void ToggleShaderCartoon() { m_renderManager.activeShaderCartoon = !m_renderManager.activeShaderCartoon; };
        // Activate shader cartoon
        void ActivateShaderCartoon() { m_renderManager.activeShaderCartoon = true; };
        // Deactivate shader cartoon
        void DeactivateShaderCartoon() { m_renderManager.activeShaderCartoon = false; };

        // Set valors shader cartoon
        float GetDepthThresholdValue() { return m_renderManager.depthThreshold; };
        void SetDepthThresholdValue(float depthThreshold) {
            m_renderManager.depthThreshold = depthThreshold;
        };
        float GetNormalThresholdValue() { return m_renderManager.normalThreshold; };
        void SetNormalThresholdValue(float normalThreshold) {
            m_renderManager.normalThreshold = normalThreshold;
        };
        glm::vec4 GetOutlineColor() { return m_renderManager.outlineColor; };
        void SetOutlineColor(glm::vec4 outlineColor) {
            m_renderManager.outlineColor = outlineColor;
        };

        // ------------------------ //
        // Window-related functions //
        // ------------------------ //

        // Initialize window and OpenGL context
        bool InitWindow(int width, int height, const char* title);
        // Close window and unload OpenGL context
        void CloseWindow();
        // Check if application should close
        bool WindowShouldClose();
        // Set window dimensions
        void SetWindowSize(int width, int height);
        // Get current screen width
        int GetScreenWidth();
        // Get current screen height
        int GetScreenHeight();
        // Get Monitor size
        glm::vec2 GetMonitorSize();
        // Check if window is fullscreen
        bool IsWindowFullscreen();
        // Toggle Fullscreen
        void ToggleFullscreen();

        // Get position from 3D world to 2D world
        glm::vec2 GetWorldToScreen(glm::vec3 pos3D);

        // Set exit key
        void SetExitKey(int key);

        // ------------------------- //
        // Drawing-related functions //
        // ------------------------- //

        // Set background color
        void ClearBackground(Color color);
        // Setup canvas to start drawing
        void BeginDrawing();
        // End canvas drawing and swap buffers
        void EndDrawing();
        // Draw the scene
        void Draw(Color clearColor = D_WHITE);

        // ------------------------ //
        // Texture Loading unctions //
        // ------------------------ //

        // Load texture from file into GPU memory
        Texture* LoadTexture2D(const char* filePath);
        // Load textures from file into GPU memory
        std::vector<Texture*> LoadTexture2DAnim(const char* filePath);
        // Unload texture data from CPU and GPU
        void UnloadTexture(Texture* texture);

        // -------------------------------------- //
        // Model 3D Loading and Drawing functions //
        // -------------------------------------- //

        // Load model from file into GPU memory
        std::unique_ptr<Model> LoadModel(const char* filePath);

        // Unload model data from CPU and GPU
        void UnloadModel(Model* model);
        // Unload mesh data from CPU and GPU
        void UnloadMesh(Mesh* mesh);

        // Play animation
        std::size_t PlayAnimation(Animation* panimation);
        // Stop animation
        void StopAnimation(std::size_t idanim);
        // Create animations
        std::vector<Animation*> CreateAnimations(const std::string& path, std::vector<BoneInfo>& vecbones);
        // Update animation
        void UpdateAnimation(float dt, float mult, std::size_t id);
        // Get animation time
        float GetAnimationTime(std::size_t id);
        // Interpolate Animation
        void InterpolateAnimation(std::size_t id, Animation* panimation);
        // Is interpolating
        bool IsInterpolating(std::size_t id);
        // Change Animation
        void ChangeAnimation(std::size_t id, Animation* panimation);
        // Has animation ended
        bool HasAnimationEnded(std::size_t id);
        // Reset animations
        void ResetAnimations();

        // --------------------------------- //
        // Input-related functions: keyboard //
        // --------------------------------- //

        // Check if a key has been pressed once
        bool IsKeyPressed(int key);
        // Check if a key has been pressed again
        bool IsKeyPressedRepeat(int key);
        // Check if a key is being pressed
        bool IsKeyDown(int key);
        // Check if a key has not been released once
        bool IsKeyReleased(int key);
        // Check if a key is not being pressed
        bool IsKeyUp(int key);
        // Check if any key is being pressed
        bool IsAnyKeyPressed();
        // Check if any key is down
        bool IsAnyKeyDown();
        // Check if any key has been released
        bool IsAnyKeyReleased();

        // ------------------------------ //
        // Input-related functions: mouse //
        // ------------------------------ //

        // Check if a mouse button has been pressed once
        bool IsMouseButtonPressed(int button);
        // Check if a mouse button is being pressed
        bool IsMouseButtonDown(int button);
        // Check if a mouse button has been released once
        bool IsMouseButtonReleased(int button);
        // Check if a mouse button is not being pressed
        bool IsMouseButtonUp(int button);
        // Get mouse position X
        int GetMouseX();
        // Get mouse position Y
        int GetMouseY();
        // Set mouse position XY
        void SetMousePosition(int x, int y);

        // -------------------------------- //
        // Input-related functions: gamepad //
        // -------------------------------- //

        // Check if gamepad is available
        bool IsGamepadAvailable(int gamepad);
        // Get gamepad internal name id
        const char* GetGamepadName(int gamepad);
        // Check is a gamepad button has been pressed once
        bool IsGamepadButtonPressed(int gamepad, int button);
        // Check is a gamepad button is being pressed
        bool IsGamepadButtonDown(int gamepad, int button);
        // Check is a gamepad button has been released once
        bool IsGamepadButtonReleased(int gamepad, int button);
        // Check is a gamepad button is not being pressed
        bool IsGamepadButtonUp(int gamepad, int button);
        // Get gamepad axis count for a gamepad
        int GetGamepadAxisCount(int gamepad);
        // Get axis movement value for a gamepad axis
        float GetGamepadAxisMovement(int gamepad, int axis);

        // ----------------------------- //
        // Loading / Unloading functions //
        // ----------------------------- //

        // Load font from file into GPU memory
        Font* LoadFont(const char* filePath);
        // Unload font from CPU and GPU
        void UnloadFont(Font* font);

        // Load shader from file into GPU memory
        Shader* LoadShader(const char* idShader, const char* vsFilePath = "", const char* fsFilePath = "", const char* gsFilePath = "");
        // Unload shader from CPU and GPU
        void UnloadShader(Shader* shader);

        // Load material from file into GPU memory
        Material* LoadMaterial(const char* filePath);
        // Unload material from CPU and GPU
        void UnloadMaterial(Material* material);

        // ------ //
        // Camera //
        // ------ //

        // Get actual camera
        Camera* GetCamera() { return m_renderManager.m_camera; }
        // Assigns camera
        void UseCamera(Camera* newCamera) { m_renderManager.setCamera(newCamera); }

        // ------------------------ //
        // Timing-related functions //
        // ------------------------ //

        // Set target FPS (max)
        void SetTargetFPS(int fps);
        // Get target FPS
        double GetTargetFPS();
        // Get time in seconds for last frame drawn
        double GetFrameTime();
        // Get elapsed time in seconds
        double GetTime();
        // Get current FPS
        int GetFPS();


    private:
        // Root node 
        std::unique_ptr<Node> m_rootNode;

        // Managers
        InputManager& m_inputManager = InputManager::getInstance();
        RenderManager& m_renderManager = RenderManager::getInstance();
        WindowsManager& m_windowsManager = WindowsManager::getInstance();
        ResourceManager& m_resourceManager = ResourceManager::getInstance();
        AnimationManager& m_animationManager = AnimationManager::getInstance();
    };
}