#pragma once
#include "entity.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"
#include "../managers/windows_manager.hpp"
#include "../managers/input_manager.hpp"
#include "../resources/resource_texture.hpp"   

#include <GL/gl.h>
#include <glm/glm.hpp>

#include <chrono>

static constexpr double K_PI2 = 3.14159265358979323846;

namespace DarkMoon {

    struct Entity2D : Entity {
    protected:
        GLuint m_VAO = {}, m_VBO = {}, m_EBO = {};
        glm::mat4 m_transMatrix = {};

        virtual void changeVAO(glm::mat4&) {};
    };

    struct Pixel : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;

    public:
        Color color = { D_BLACK };

        Pixel(Color c);
        ~Pixel();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Line : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;

    public:
        glm::vec2 startPos = {}, endPos = {};
        Color color = { D_BLACK };

        Line(glm::vec2 sP, glm::vec2 eP, Color c);
        ~Line();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Triangle : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;

    public:
        glm::vec2 v1 = {}, v2 = {}, v3 = {};
        Color color = { D_BLACK };

        Triangle(glm::vec2 v_1, glm::vec2 v_2, glm::vec2 v_3, Color c);
        ~Triangle();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Rectangle : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;
    public:
        glm::vec2 position = {};
        glm::vec2 size = {};
        Color color = { D_BLACK };

        Rectangle(glm::vec2 p, glm::vec2 s, Color c);
        ~Rectangle();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Circle : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;
        void changeVAO(glm::mat4& transMatrix, bool forceUpdate = false);
    public:
        glm::vec2 position = {};
        float radius = { 10.0f };
        float oldRadius = { 10.0f };
        int segments = { 20 };
        int oldSegments = { 20 };
        std::vector<float> vertices{};
        Color color = { D_BLACK };
        RenderManager& rm = RenderManager::getInstance();

        Circle(glm::vec2 p, float rad, int seg, Color c);
        ~Circle();

        void draw(glm::mat4 transMatrix) override;
    };

    struct Texture2D : Entity2D {
    private:
        void changeVAO(glm::mat4& transMatrix) override;
    public:
        glm::vec2 position = {};
        Texture* texture = { nullptr };
        Color color = { D_WHITE };

        Texture2D(glm::vec2 pos, Texture* text, Color col);
        Texture2D(const Texture2D& other);
        ~Texture2D();

        void draw(glm::mat4 transMatrix) override;
    };

    struct AnimatedTexture2D : Entity {
        glm::vec2 position{};
        std::vector<Texture*> frames{};
        Color color{};
        float frameDuration{ .6f };
        float elapsedTime{ 0.0f };
        std::size_t currentFrame{ 0 };

        AnimatedTexture2D(glm::vec2 pos = { 0.0f, 0.0f }, Color col = D_WHITE, float framD = 1.0f, std::size_t currF = 0)
            : position(pos), color(col), frameDuration(framD), currentFrame(currF) {};

        void draw(glm::mat4 transMatrix) override {
            // Check actual texture
            // WindowsManager wm = WindowsManager::getInstance();

            elapsedTime += 1 / 60.0f;
            if (elapsedTime > frameDuration) {
                if (currentFrame + 1 > frames.size() - 1)
                    currentFrame = 0;
                else
                    currentFrame += 1;

                elapsedTime = 0;
            }
            auto texture = frames[currentFrame];

            // Draw texture
            RenderManager& rm = RenderManager::getInstance();

            rm.useShader(rm.shaders["texture"]);

            auto nColor = rm.normalizeColor(color);

            // Apply Transformation Matrix
            position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);

            // Top-left corner
            float auxWidth{ 0.0f }, auxHeight{ 0.0f };
            if (texture) {
                auxWidth = static_cast<float>(texture->getWidth()) * glm::length(glm::vec2(transMatrix[0][0], transMatrix[1][0]));
                auxHeight = static_cast<float>(texture->getHeight()) * glm::length(glm::vec2(transMatrix[1][1], transMatrix[1][0]));
            }

            // Define vertices and indices
            float vertices[] = {
                // positions                                                                         // colors                       // texture coords
                rm.normalizeX(position.x)           , rm.normalizeY(position.y)            , 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 0.0f,
                rm.normalizeX(position.x + auxWidth), rm.normalizeY(position.y)            , 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 0.0f,
                rm.normalizeX(position.x)           , rm.normalizeY(position.y + auxHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   0.0f, 1.0f,
                rm.normalizeX(position.x + auxWidth), rm.normalizeY(position.y + auxHeight), 0.0f,   nColor.x, nColor.y, nColor.z,   1.0f, 1.0f
            };

            GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

            // Create and configure VAO, VBO and EBO
            GLuint VBO, VAO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // Colors
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

            // Apply Transformation Matrix
            //GLuint transformLoc = glGetUniformLocation(rm.getShader()->getIDShader(), "transform");
            //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transMatrix));

            if (texture) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Draw Texture
                glBindTexture(GL_TEXTURE_2D, texture->getIDTexture());
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_BLEND);
            }

            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            rm.useShader(rm.shaders["color"]);
        };
    };

    // GUI
    enum struct Aligned { LEFT, CENTER, RIGHT, TOP, BOTTOM };

    struct Text : Entity {
        glm::vec2 position{};
        Font* font{};
        int fontSize{};
        Color color{};
        static constexpr float charSpeed{ 0.045f };

        float scale{};
        float maxWidth{};
        float maxHeight{};
        float timeElapsed{};
        size_t charIndex{};
        bool charIndexChanged{ false };
        std::vector<float> widths{};
        Aligned alignment{};

        GLuint VAO{}, VBO{};
        GLint textColorLocation{};
        RenderManager& rm = RenderManager::getInstance();
        WindowsManager& wm = WindowsManager::getInstance();
        float ratio{};
        int numLines{ 1 };
        bool charByChar{ false };
        bool bold{ false };
        bool italic{ false };

        Text(glm::vec2 pos = { 0.0f, 0.0f }, std::string txt = "", Font* f = nullptr, int fS = 10, Color col = D_BLACK, Aligned al = Aligned::LEFT, bool cbc = false)
            : position(pos), font(f), fontSize(fS), color(col), alignment(al), charByChar(cbc)
        {
            // Get ratio
            ratio = wm.getHeightRatio();

            setText(txt);

            // Buffers
            setupBuffers();

            // Get uniform location
            rm.useShader(rm.shaders["text"]);
            textColorLocation = glGetUniformLocation(rm.getShader()->getIDShader(), "textColor");
        };

        void setText(std::string text)
        {
            std::wstring textW{};
            textW.resize(text.size());
            std::mbstowcs(&textW[0], text.c_str(), text.size());
#ifdef _WIN32
            bool checkSpecial = false;
#endif
            if (font && !text.empty() && textW != this->text) {
                // Reset values
                widths.clear();
                maxWidth = 0.0f;
                maxHeight = 0.0f;
                numLines = 1;
                charIndex = 0;
                bold = false;
                italic = false;
                float lineWidth = position.x;

                // Seteamos la escala con el ratio
                scale = static_cast<float>(fontSize) / 34.0f * ratio;

                for (wchar_t c : textW) {
                    if (c == '\n') {
                        widths.push_back(lineWidth - position.x);
                        lineWidth = position.x;
                        if (maxWidth < lineWidth)
                            maxWidth = lineWidth;

                        numLines += 1;
                        continue;
                    }
#ifdef _WIN32
                    else if (checkSpecial)
                    {
                        c += 64;
                        checkSpecial = false;
                    }
                    else if (c == 195)
                    {
                        checkSpecial = true;
                        continue;
                    }
#endif
                    // Max Height
                    Character ch = font->characters[c];
                    auto chY = static_cast<float>(ch.size.y);
                    if (maxHeight < chY)
                        maxHeight = chY;
                    lineWidth += static_cast<float>(ch.advance >> 6) * scale;
                }
                maxHeight *= scale;
                widths.push_back(lineWidth - position.x);
                if (maxWidth < lineWidth)
                    maxWidth = lineWidth;

                this->text = textW;
            }
        }

        void setupBuffers() {
            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Generate VAO and VBO
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            // Configure VAO/VBO for texture quads
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }

        ~Text() {
            // Delete VAO and VBO
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        };

        void draw(glm::mat4 transMatrix) override {

            if (ratio != wm.getHeightRatio())
            {
                auto lastRatio = ratio;
                ratio = wm.getHeightRatio();
                scale = static_cast<float>(fontSize) / 34.0f * ratio;
                for (auto& w : widths)
                    w *= ratio / lastRatio;

                for (wchar_t& c : text) {
                    Character ch = font->characters[c];
                    auto chY = static_cast<float>(ch.size.y);
                    if (maxHeight < chY)
                        maxHeight = chY;
                }
                maxHeight *= scale;
            }

            rm.useShader(rm.shaders["text"]);

            position = glm::vec2(transMatrix[3][0], transMatrix[3][1]);

            if (!font)
                font = rm.defaultFont;
            if (text.empty())
                return;

            auto normColor = rm.normalizeColor(color);
            glUniform3f(textColorLocation, normColor.r, normColor.g, normColor.b);

            // Blend
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Assign the position
            float aux_x = position.x;
            float aux_y = position.y;

            if (alignment == Aligned::CENTER)
                aux_x -= widths[0] / 2;
            else if (alignment == Aligned::RIGHT)
                aux_x -= widths[0];

            // Iterate through all characters
            auto offSetY = maxHeight * 0.8f;
            if (numLines != 1)
            {
                offSetY = maxHeight / 2;
                aux_y -= maxHeight * static_cast<float>(numLines) * 0.6f;
            }

            float deltaTime = static_cast<float>(wm.getFrameTime());

            if (charByChar)
            {
                timeElapsed += deltaTime;
                if (timeElapsed >= charSpeed) {
                    timeElapsed -= charSpeed;
                    if (charIndex < text.size()) {
                        bold = false;
                        italic = false;
                        ++charIndex;

                        if (charIndex % static_cast<std::size_t>(8) == 0)
                            charIndexChanged = true;
                    }
                }
                else
                    charIndexChanged = false;
            }
            else
                charIndex = text.size();
#ifdef _WIN32
            bool checkSpecial = false;
#endif
            std::size_t k{ 1 }; // Pa la anchura de las líneas
            for (std::size_t i = 0; i < charIndex; ++i) {
                wchar_t c = text[i];
                if (c == '\n') {
                    // Reset the x position to the start of the line
                    aux_x = position.x;

                    if (alignment == Aligned::CENTER)
                        aux_x -= widths[k] / 2;
                    else
                        aux_x -= widths[k];

                    k += 1;
                    auto hRate = wm.getHeightRatio();
                    aux_y += maxHeight * 1.8f * hRate;
                    // Skip the rest of the loop
                    continue;
                }
                // Poner o quitar negrita
                else if (c == '\b')
                    bold = !bold;

                // Poner o quitar cursiva
                else if (c == '\t')
                    italic = !italic;

#ifdef _WIN32
                else if (checkSpecial)
                {
                    c += 64;
                    checkSpecial = false;
                }
                else if (c == 195)
                {
                    checkSpecial = true;
                    continue;
                }
#endif

                Character ch = font->characters[c];

                float posX = aux_x + static_cast<float>(ch.bearing.x) * scale;
                float posY = aux_y - static_cast<float>(ch.bearing.y) * scale + offSetY;

                float w = static_cast<float>(ch.size.x) * scale;
                float h = static_cast<float>(ch.size.y) * scale;

                // Update VBO for each character
                float vertices[6][4] = {
                    { posX    , posY + h, 0.0f, 1.0f },
                    { posX    , posY    , 0.0f, 0.0f },
                    { posX + w, posY    , 1.0f, 0.0f },

                    { posX    , posY + h, 0.0f, 1.0f },
                    { posX + w, posY    , 1.0f, 0.0f },
                    { posX + w, posY + h, 1.0f, 1.0f }
                };

                // Transformamos los vértices
                if (italic) {
                    static constexpr float shearAmount = -0.2f;
                    for (int i = 0; i < 6; i++) {
                        float shear = shearAmount * vertices[i][1];
                        vertices[i][0] += shear;
                    }
                }

                // Normalizamos los vértices
                for (int i = 0; i < 6; i++) {
                    vertices[i][0] = rm.normalizeX(vertices[i][0]);
                    vertices[i][1] = rm.normalizeY(vertices[i][1]);
                }

                // Movemos los caracteres a donde queremos que estén
                if (italic) {
                    static constexpr float adjustment = 0.181f;
                    for (int i = 0; i < 6; i++) {
                        vertices[i][0] += adjustment;
                    }
                }

                // Configure VAO/VBO for texture quads
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);

                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glBindTexture(GL_TEXTURE_2D, ch.textureID);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                if (bold)
                {
                    static constexpr float offset = 0.001f;

                    for (int l = 0; l < 6; l++)
                        vertices[l][0] += offset;

                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }

                // Advance cursors for next glyph
                aux_x += static_cast<float>(ch.advance >> 6) * scale;
            }

            // Clean up resources
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);

            rm.useShader(rm.shaders["color"]);
        };

        std::wstring getText() {
            return text;
        }

        void skipText() {
            charIndex = text.size();
        }

        bool isTextFinished() {
            return charIndex == text.size();
        }

        bool charChanged() {
            return charIndexChanged;
        }

        std::wstring text{};
    };

    struct TextBox : Entity {
        Rectangle box;
        Rectangle boxBackground;
        Text text;
        float padding = 10.0f;

        bool drawBox{ true };

        Aligned verAligned;
        Aligned horAligned;

        TextBox(glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            Color bCol = D_WHITE,
            std::string txt = "",
            Font* f = nullptr,
            int fS = 10,
            Color tCol = D_BLACK,
            Aligned verAl = Aligned::CENTER,
            Aligned horAl = Aligned::CENTER,
            bool cbc = false) :
            box(pos, sz, bCol), boxBackground({ pos.x - 1, pos.y - 1 }, { sz.x + 2, sz.y + 2 }, D_GRAY), text(pos, txt, f, fS, tCol, horAl, cbc), verAligned(verAl), horAligned(horAl) {};

        void draw(glm::mat4 transMatrix) override {
            if (drawBox) {
                // Rectangle background
                glm::mat4 aux = transMatrix;
                aux[3][0] -= 1;   aux[3][1] -= 1;
                boxBackground.draw(aux);
                // Rectangle
                box.draw(transMatrix);
            }
            else {
                boxBackground.position.x = transMatrix[3][0] - 1;
                boxBackground.position.y = transMatrix[3][1] - 1;

                box.position.x = transMatrix[3][0];
                box.position.y = transMatrix[3][1];
            }

            WindowsManager& wm = WindowsManager::getInstance();
            auto wRat = wm.getWidthRatio();
            auto hRat = wm.getHeightRatio();

            // Vertical Aligned
            switch (verAligned) {
            case Aligned::TOP:
                transMatrix[3][1] = box.position.y + padding;
                break;
            case Aligned::CENTER:
                transMatrix[3][1] = box.position.y + (box.size.y - text.maxHeight) * hRat / 2;
                break;
            case Aligned::BOTTOM:
                transMatrix[3][1] = box.position.y + box.size.y * hRat - padding;
                break;
            default:
                break;
            }

            // Horizontal Aligned
            switch (horAligned) {
            case Aligned::LEFT:
                transMatrix[3][0] = box.position.x + padding;
                break;
            case Aligned::CENTER:
                transMatrix[3][0] = box.position.x + box.size.x * wRat / 2;
                break;
            case Aligned::RIGHT:
                transMatrix[3][0] = box.position.x + box.size.x * wRat - padding;
                break;
            default:
                break;
            }

            // Text
            text.draw(transMatrix);
        };

    };

    enum struct ButtonState { NORMAL, HOVER, CLICK };

    struct Button : Entity {
        TextBox textBox;
        Color normalColor;
        Color hoverColor;
        Color clickColor;
        ButtonState state{ ButtonState::NORMAL };
        ButtonState prevState{ ButtonState::NORMAL };
        WindowsManager& wm = WindowsManager::getInstance();
        InputManager& im = InputManager::getInstance();
        bool isCurrent{ false };

        Button(glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            Color bCol = D_WHITE,
            std::string txt = "",
            Font* f = nullptr,
            int fS = 10,
            Color tCol = D_BLACK,
            Aligned verAl = Aligned::CENTER,
            Aligned horAl = Aligned::CENTER,
            Color nColor = D_WHITE,
            Color hColor = D_GRAY,
            Color cColor = D_BLACK) :
            textBox(pos, sz, bCol, txt, f, fS, tCol, verAl, horAl), normalColor(nColor), hoverColor(hColor), clickColor(cColor)
        {
            textBox.box.position = pos;
            checkMouse();
        };

        void draw(glm::mat4 transMatrix) override {
            checkMouse();
            textBox.draw(transMatrix);
        };

        void checkMouse()
        {
            glm::vec2 mPos = { im.getMouseX(wm.getWindow()), im.getMouseY(wm.getWindow()) };
            glm::vec2 topLeft = textBox.box.position;
            glm::vec2 bottomRight = { topLeft.x + textBox.box.size.x * wm.getWidthRatio(), topLeft.y + textBox.box.size.y * wm.getHeightRatio() };

            if (mPos.x >= topLeft.x && mPos.x <= bottomRight.x &&
                mPos.y >= topLeft.y && mPos.y <= bottomRight.y) {
                if (im.isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
                    if (!textBox.drawBox)
                        textBox.text.color = clickColor;
                    else
                        textBox.box.color = clickColor;
                    prevState = state;
                    state = ButtonState::CLICK;
                }
                else {
                    if (!textBox.drawBox)
                        textBox.text.color = hoverColor;
                    else
                        textBox.box.color = hoverColor;
                    prevState = state;
                    state = ButtonState::HOVER;
                }
            }
            else if (isCurrent)
            {
                if (!textBox.drawBox)
                    textBox.text.color = hoverColor;
                else
                    textBox.box.color = hoverColor;
                prevState = state;
                state = ButtonState::HOVER;

                isCurrent = false;
            }
            else {
                if (!textBox.drawBox)
                    textBox.text.color = normalColor;
                else
                    textBox.box.color = normalColor;
                prevState = state;
                state = ButtonState::NORMAL;
            }
        }
    };

    struct CheckBoxBase : Entity {
        TextBox textBox;
        Color normalColor;
        Color hoverColor;
        ButtonState state{ ButtonState::NORMAL };
        WindowsManager& wm = WindowsManager::getInstance();
        InputManager& im = InputManager::getInstance();

        CheckBoxBase(glm::vec2 pos = { 0.0f, 0.0f },
            float size = 50.0f,
            Color bCol = D_WHITE,
            Color nColor = D_WHITE,
            Color hColor = D_GRAY) :
            textBox(pos, { size, size }, bCol, "", nullptr, 18, D_BLACK, Aligned::CENTER, Aligned::CENTER),
            normalColor(nColor), hoverColor(hColor)
        {
            textBox.box.position = pos;
            checkMouse();
        };

        void draw(glm::mat4 transMatrix) override {
            checkMouse();
            if (checkChecked()) {
                textBox.text.setText("X");
            }
            else {
                textBox.text.setText(" ");
            }
            textBox.draw(transMatrix);
        };

        void checkMouse()
        {
            glm::vec2 mPos = { im.getMouseX(wm.getWindow()), im.getMouseY(wm.getWindow()) };
            glm::vec2 topLeft = textBox.box.position;
            glm::vec2 bottomRight = { topLeft.x + textBox.box.size.x, topLeft.y + textBox.box.size.y };

            if (mPos.x >= topLeft.x && mPos.x <= bottomRight.x &&
                mPos.y >= topLeft.y && mPos.y <= bottomRight.y) {
                if (im.isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)) {
                    state = ButtonState::CLICK;
                    toggleChecked();
                }
                else {
                    textBox.box.color = hoverColor;
                    state = ButtonState::HOVER;
                }
            }
            else {
                textBox.box.color = normalColor;
                state = ButtonState::NORMAL;
            }
        }

        bool isClicked() {
            return state == ButtonState::CLICK;
        }

        virtual bool checkChecked() = 0;
        virtual void toggleChecked() = 0;
    };

    struct CheckBox : CheckBoxBase {
        bool checked{ false };

        CheckBox(glm::vec2 pos = { 0.0f, 0.0f },
            float size = 50.0f,
            bool checked = false,
            Color bCol = D_WHITE,
            Color nColor = D_WHITE,
            Color hColor = D_GRAY) :
            CheckBoxBase(pos, size, bCol, nColor, hColor), checked(checked)
        {};

        bool checkChecked() override {
            return checked;
        }

        void toggleChecked() override {
            checked = !checked;
        }
    };

    struct CheckBoxPtr : CheckBoxBase {
        bool* checked{ nullptr };

        CheckBoxPtr(glm::vec2 pos = { 0.0f, 0.0f },
            float size = 50.0f,
            bool* checked = nullptr,
            Color bCol = D_WHITE,
            Color nColor = D_WHITE,
            Color hColor = D_GRAY) :
            CheckBoxBase(pos, size, bCol, nColor, hColor), checked(checked)
        {};

        bool checkChecked() override {
            return *checked;
        }

        void toggleChecked() override {
            if (checked) {
                *checked = !*checked;
            }
        }
    };

    struct Slider : Entity {
        Rectangle background;
        Rectangle boxBackground;
        Rectangle slider;
        float valor{}; // 0 -> 1
        WindowsManager& wm = WindowsManager::getInstance();
        InputManager& im = InputManager::getInstance();

        Slider(glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            float value = 0.5f,
            Color bCol = D_GRAY,
            Color sCol = D_WHITE) :
            background(pos, sz, bCol), boxBackground({ pos.x - 1, pos.y - 1 }, { sz.x + 2, sz.y + 2 }, D_GRAY), slider({ pos.x + 1, pos.y + 1 }, { sz.x - 2, sz.y - 2 }, sCol), valor{ value }
        {
            slider.position = { pos.x + 1, pos.y + 1 };
            checkMouse();
        };

        void draw(glm::mat4 transMatrix) override {
            // Rectangle box background
            glm::mat4 aux = transMatrix;
            aux[3][0] -= 1;   aux[3][1] -= 1;
            boxBackground.draw(aux);

            // Rectangle background
            background.draw(transMatrix);

            // Rectangle slider
            aux = transMatrix;
            aux[3][0] += 1;   aux[3][1] += 1;

            checkMouse();

            aux[0][0] = valor;
            slider.draw(aux);
        };

        void checkMouse()
        {
            glm::vec2 mPos = { im.getMouseX(wm.getWindow()), im.getMouseY(wm.getWindow()) };
            glm::vec2 topLeft = slider.position;
            glm::vec2 bottomRight = { topLeft.x + slider.size.x, topLeft.y + slider.size.y };

            if (mPos.x >= topLeft.x && mPos.x <= bottomRight.x &&
                mPos.y >= topLeft.y && mPos.y <= bottomRight.y)
                if (im.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                    valor = (mPos.x - topLeft.x) / (bottomRight.x - topLeft.x);
        }
    };

    struct OptionSlider : Entity {
        Button prevButton;
        TextBox textBox;
        Button nextButton;
        Text name{};
        std::vector<std::string> options{};
        std::size_t currentOption{};


        WindowsManager& wm = WindowsManager::getInstance();
        InputManager& im = InputManager::getInstance();

        OptionSlider(
            glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            Color bCol = D_WHITE,
            std::string txt = "",
            Font* f = nullptr,
            int fS = 10,
            int fsArrows = 20,
            Color tCol = D_BLACK,
            Aligned verAl = Aligned::CENTER,
            Aligned horAl = Aligned::CENTER,
            Color nColor = D_WHITE,
            Color hColor = D_GRAY,
            Color cColor = D_BLACK,
            std::vector<std::string> opts = { "Option 1", "Option 2", "Option 3" },
            std::string firstOption = "Option 1") :
            prevButton(pos, { sz.y, sz.y }, D_WHITE, "<", f, fsArrows, D_BLACK, verAl, Aligned::CENTER, nColor, hColor, cColor),
            textBox(pos, sz, bCol, "", f, fS, tCol, Aligned::TOP, horAl),
            nextButton(pos, { sz.y, sz.y }, D_WHITE, ">", f, fsArrows, D_BLACK, verAl, Aligned::CENTER, nColor, hColor, cColor),
            name(pos, txt, f, fS, tCol, horAl),
            options(opts),
            currentOption(0)
        {
            prevButton.textBox.drawBox = false;
            nextButton.textBox.drawBox = false;
            textBox.drawBox = false;

            textBox.box.position = pos;
            textBox.text.setText(opts[currentOption]);

            if (firstOption != "")
                findAndAssign(firstOption);

            checkMouse();
        };

        void draw(glm::mat4 transmatrix) override
        {
            auto wRat = wm.getWidthRatio();

            glm::mat4 aux = transmatrix;
            aux[3][0] -= prevButton.textBox.box.size.x * wRat * 1.1f;
            // aux[3][1] += 5;
            prevButton.draw(aux);

            aux = transmatrix;
            aux[3][0] += textBox.box.size.x * wRat * 1.015f;
            // aux[3][1] += 5;
            nextButton.draw(aux);

            // Text box
            textBox.draw(transmatrix);

            // Name
            if (name.text != L"")
            {
                aux = transmatrix;
                aux[3][0] -= textBox.box.size.x * wRat / 1.5f;
                name.draw(aux);
            }

            // Check mouse
            checkMouse();
        }

        void checkMouse()
        {
            if (nextButton.state == ButtonState::CLICK)
                nextOption();
            else if (prevButton.state == ButtonState::CLICK)
                prevOption();
        }

        void setOptions(std::vector<std::string> opts)
        {
            options = opts;
            textBox.text.setText(options[currentOption]);
        }

        void setCurrentOption(std::size_t option)
        {
            currentOption = option;
            textBox.text.setText(options[currentOption]);
        }

        void findAndAssign(std::string option)
        {
            for (std::size_t i = 0; i < options.size(); i++)
                if (options[i] == option)
                {
                    setCurrentOption(i);
                    break;
                }
        }

        virtual void nextOption()
        {
            // Pasamos a la siguiente opción y si estamos en la última volvemos a la primera
            currentOption = (currentOption + 1) % options.size();
            textBox.text.setText(options[currentOption]);
        }

        virtual void prevOption()
        {
            // Pasamos a la anterior opción y si estamos en la primera volvemos a la última
            currentOption = (currentOption - 1 + options.size()) % options.size();
            textBox.text.setText(options[currentOption]);
        }
    };

    struct FloatSlider : OptionSlider {
        float currentValue{};

        FloatSlider(
            glm::vec2 pos = { 0.0f, 0.0f },
            glm::vec2 sz = { 100.0f, 50.0f },
            Color bCol = D_WHITE,
            std::string txt = "",
            Font* f = nullptr,
            int fS = 10,
            int fsArrows = 20,
            Color tCol = D_BLACK,
            Aligned verAl = Aligned::CENTER,
            Aligned horAl = Aligned::CENTER,
            Color nColor = D_WHITE,
            Color hColor = D_GRAY,
            Color cColor = D_BLACK,
            float initialValue = 0.0f) :
            OptionSlider(pos, sz, bCol, txt, f, fS, fsArrows, tCol, verAl, horAl, nColor, hColor, cColor),
            currentValue(initialValue)
        {
            prevButton.textBox.drawBox = false;
            nextButton.textBox.drawBox = false;
            textBox.drawBox = false;

            textBox.box.position = pos;
            textBox.text.setText(std::to_string(static_cast<int>(currentValue * 100)));
            checkMouse();
        };

        void nextOption() override
        {
            // Incrementamos el valor actual y nos aseguramos de que no supere 1
            currentValue = std::min(currentValue + 0.01f, 1.0f);
            textBox.text.setText(std::to_string(static_cast<int>(currentValue * 100)));
        }

        void prevOption() override
        {
            // Decrementamos el valor actual y nos aseguramos de que no baje de 0
            currentValue = std::max(currentValue - 0.01f, 0.0f);
            textBox.text.setText(std::to_string(static_cast<int>(currentValue * 100)));
        }
    };
}