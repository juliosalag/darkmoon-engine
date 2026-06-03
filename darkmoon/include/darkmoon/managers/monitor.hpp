#pragma once

#include <string>
#include <vector>
#include <functional>

#include <GLFW/glfw3.h>

// Supporting types

struct VideoMode {
    int width;
    int height;
    int redBits;
    int greenBits;
    int blueBits;
    int refreshRate;

    explicit VideoMode(const GLFWvidmode& m)
        : width(m.width), height(m.height),
          redBits(m.redBits), greenBits(m.greenBits), blueBits(m.blueBits),
          refreshRate(m.refreshRate) {}
};

struct PhysicalSize {
    int widthMM;
    int heightMM;
};

struct ContentScale {
    float x;
    float y;
};

struct VirtualPosition {
    int x;
    int y;
};

struct WorkArea {
    int x;
    int y;
    int width;
    int height;
};

struct GammaRamp {
    std::vector<unsigned short> red;
    std::vector<unsigned short> green;
    std::vector<unsigned short> blue;

    // Construct from a raw GLFW gamma ramp
    explicit GammaRamp(const GLFWgammaramp& ramp)
        : red(ramp.red, ramp.red + ramp.size),
          green(ramp.green, ramp.green + ramp.size),
          blue(ramp.blue, ramp.blue + ramp.size) {}

    // Construct an empty ramp of a given size
    explicit GammaRamp(unsigned int size)
        : red(size, 0), green(size, 0), blue(size, 0) {}

    unsigned int size() const { return static_cast<unsigned int>(red.size()); }

    // Convert back to GLFWgammaramp for use with GLFW (caller owns nothing extra)
    GLFWgammaramp toGLFW() const {
        GLFWgammaramp r{};
        r.size  = size();
        r.red   = const_cast<unsigned short*>(red.data());
        r.green = const_cast<unsigned short*>(green.data());
        r.blue  = const_cast<unsigned short*>(blue.data());
        return r;
    }
};

// ------- //
// Monitor //
// ------- //

class Monitor {
public:
    // Wraps an existing GLFWmonitor pointer.  Passing nullptr yields an
    // invalid monitor; check isValid() before using.
    explicit Monitor(GLFWmonitor* handle = nullptr) : m_handle(handle) {}

    // Returns whether this object wraps a real, connected monitor.
    bool isValid() const { return m_handle != nullptr; }
    explicit operator bool() const { return isValid(); }

    // Raw GLFW handle, for interop with GLFW window functions etc.
    GLFWmonitor* handle() const { return m_handle; }

    // Retrieval helpers (static) //

    // Returns all currently connected monitors.
    static std::vector<Monitor> getAll() {
        int count = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        std::vector<Monitor> result;
        result.reserve(count);
        for (int i = 0; i < count; ++i)
            result.emplace_back(monitors[i]);
        return result;
    }

    // Returns the primary (preferred) monitor.
    static Monitor getPrimary() {
        return Monitor(glfwGetPrimaryMonitor());
    }

    // Monitor configuration callback //

    // Signature: void callback(Monitor monitor, int event)
    // event is GLFW_CONNECTED or GLFW_DISCONNECTED.
    using MonitorCallback = std::function<void(Monitor, int)>;

    // Call initCallback() once from DarkMoonEngine's constructor to
    // register the internal GLFW callback. After that, poll wasConnected() /
    // wasDisconnected() each frame both return true only once per event and
    // then reset automatically.
    static void initCallback() {
        glfwSetMonitorCallback([](GLFWmonitor* /*mon*/, int event) {
            if (event == GLFW_CONNECTED)
                s_connected = true;
            else if (event == GLFW_DISCONNECTED)
                s_disconnected = true;
        });
    }

    // Returns true (once) when any monitor has been connected
    static bool wasConnected() {
        if (s_connected) {
            s_connected = false;
            return true;
        }
        return false;
    }
 
    // Returns true (once) when any monitor has been disconnected
    static bool wasDisconnected() {
        if (s_disconnected) {
            s_disconnected = false;
            return true;
        }
        return false;
    }

    // Video modes //

    // Returns all video modes supported by this monitor.
    std::vector<VideoMode> getVideoModes() const {
        int count = 0;
        const GLFWvidmode* modes = glfwGetVideoModes(m_handle, &count);
        std::vector<VideoMode> result;
        result.reserve(count);
        for (int i = 0; i < count; ++i)
            result.emplace_back(modes[i]);
        return result;
    }

    // Returns the current video mode of this monitor.
    VideoMode getCurrentVideoMode() const {
        return VideoMode(*glfwGetVideoMode(m_handle));
    }

    // Physical size //
    PhysicalSize getPhysicalSize() const {
        PhysicalSize s{};
        glfwGetMonitorPhysicalSize(m_handle, &s.widthMM, &s.heightMM);
        return s;
    }

    // Content scale //
    ContentScale getContentScale() const {
        ContentScale s{};
        glfwGetMonitorContentScale(m_handle, &s.x, &s.y);
        return s;
    }

    // Virtual position //
    VirtualPosition getPosition() const {
        VirtualPosition p{};
        glfwGetMonitorPos(m_handle, &p.x, &p.y);
        return p;
    }

    // Work area //
    WorkArea getWorkArea() const {
        WorkArea a{};
        glfwGetMonitorWorkarea(m_handle, &a.x, &a.y, &a.width, &a.height);
        return a;
    }

    // Human-readable name //
    std::string getName() const {
        const char* name = glfwGetMonitorName(m_handle);
        return name ? name : "";
    }

    // User pointer //
    void  setUserPointer(void* ptr) { glfwSetMonitorUserPointer(m_handle, ptr); }
    void* getUserPointer() const    { return glfwGetMonitorUserPointer(m_handle); }

    // Gamma ramp //

    // Reads the current gamma ramp from the monitor.
    GammaRamp getGammaRamp() const {
        return GammaRamp(*glfwGetGammaRamp(m_handle));
    }

    // Uploads a gamma ramp to the monitor.
    void setGammaRamp(const GammaRamp& ramp) {
        GLFWgammaramp r = ramp.toGLFW();
        glfwSetGammaRamp(m_handle, &r);
    }

    // Lets GLFW generate and apply a standard gamma curve from an exponent.
    // gamma = 1.0 restores the default (sRGB-like) behaviour.
    void setGamma(float gamma) {
        glfwSetGamma(m_handle, gamma);
    }

    // Comparison //
    bool operator==(const Monitor& other) const { return m_handle == other.m_handle; }
    bool operator!=(const Monitor& other) const { return !(*this == other); }

private:
    GLFWmonitor* m_handle = nullptr;

    inline static bool s_connected    = false;
    inline static bool s_disconnected = false;
};