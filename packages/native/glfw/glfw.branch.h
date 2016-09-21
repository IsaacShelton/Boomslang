
#ifndef GLFW3_WRAP_H_INCLUDED
#define GLFW3_WRAP_H_INCLUDED

#include <vector>

// Glfw Constants
#define boomslang_GLFW_NOT_INITIALIZED                boomslang_Integer(GLFW_NOT_INITIALIZED)
#define boomslang_GLFW_NO_CURRENT_CONTEXT             boomslang_Integer(GLFW_NO_CURRENT_CONTEXT)
#define boomslang_GLFW_INVALID_ENUM                   boomslang_Integer(GLFW_INVALID_ENUM)
#define boomslang_GLFW_INVALID_VALUE                  boomslang_Integer(GLFW_INVALID_VALUE)
#define boomslang_GLFW_OUT_OF_MEMORY                  boomslang_Integer(GLFW_OUT_OF_MEMORY)
#define boomslang_GLFW_API_UNAVAILABLE                boomslang_Integer(GLFW_API_UNAVAILABLE)
#define boomslang_GLFW_VERSION_UNAVAILABLE            boomslang_Integer(GLFW_VERSION_UNAVAILABLE)
#define boomslang_GLFW_PLATFORM_ERROR                 boomslang_Integer(GLFW_PLATFORM_ERROR)
#define boomslang_GLFW_FORMAT_UNAVAILABLE             boomslang_Integer(GLFW_FORMAT_UNAVAILABLE)
#define boomslang_GLFW_NO_WINDOW_CONTEXT              boomslang_Integer(GLFW_NO_WINDOW_CONTEXT)

#define boomslang_GLFW_JOYSTICK_1                     boomslang_Integer(GLFW_JOYSTICK_1)
#define boomslang_GLFW_JOYSTICK_2                     boomslang_Integer(GLFW_JOYSTICK_2)
#define boomslang_GLFW_JOYSTICK_3                     boomslang_Integer(GLFW_JOYSTICK_3)
#define boomslang_GLFW_JOYSTICK_4                     boomslang_Integer(GLFW_JOYSTICK_4)
#define boomslang_GLFW_JOYSTICK_5                     boomslang_Integer(GLFW_JOYSTICK_5)
#define boomslang_GLFW_JOYSTICK_6                     boomslang_Integer(GLFW_JOYSTICK_6)
#define boomslang_GLFW_JOYSTICK_7                     boomslang_Integer(GLFW_JOYSTICK_7)
#define boomslang_GLFW_JOYSTICK_8                     boomslang_Integer(GLFW_JOYSTICK_8)
#define boomslang_GLFW_JOYSTICK_9                     boomslang_Integer(GLFW_JOYSTICK_9)
#define boomslang_GLFW_JOYSTICK_10                    boomslang_Integer(GLFW_JOYSTICK_10)
#define boomslang_GLFW_JOYSTICK_11                    boomslang_Integer(GLFW_JOYSTICK_11)
#define boomslang_GLFW_JOYSTICK_12                    boomslang_Integer(GLFW_JOYSTICK_12)
#define boomslang_GLFW_JOYSTICK_13                    boomslang_Integer(GLFW_JOYSTICK_13)
#define boomslang_GLFW_JOYSTICK_14                    boomslang_Integer(GLFW_JOYSTICK_14)
#define boomslang_GLFW_JOYSTICK_15                    boomslang_Integer(GLFW_JOYSTICK_15)
#define boomslang_GLFW_JOYSTICK_16                    boomslang_Integer(GLFW_JOYSTICK_16)
#define boomslang_GLFW_JOYSTICK_LAST                  boomslang_Integer(GLFW_JOYSTICK_LAST)

#define boomslang_GLFW_RELEASE                        boomslang_Integer(GLFW_RELEASE)
#define boomslang_GLFW_PRESS                          boomslang_Integer(GLFW_PRESS)
#define boomslang_GLFW_REPEAT                         boomslang_Integer(GLFW_REPEAT)

#define boomslang_GLFW_ARROW_CURSOR                   boomslang_Integer(GLFW_ARROW_CURSOR)
#define boomslang_GLFW_IBEAM_CURSOR                   boomslang_Integer(GLFW_IBEAM_CURSOR)
#define boomslang_GLFW_CROSSHAIR_CURSOR               boomslang_Integer(GLFW_CROSSHAIR_CURSOR)
#define boomslang_GLFW_HAND_CURSOR                    boomslang_Integer(GLFW_HAND_CURSOR)
#define boomslang_GLFW_HRESIZE_CURSOR                 boomslang_Integer(GLFW_HRESIZE_CURSOR)
#define boomslang_GLFW_VRESIZE_CURSOR                 boomslang_Integer(GLFW_VRESIZE_CURSOR)

// Glfw Classes
#define boomslang_GlfwWindow                          GLFWwindow
#define boomslang_GlfwMonitor                         GLFWmonitor
#define boomslang_GlfwGammaRamp                       GLFWgammaramp
#define boomslang_GlfwGlProc                          GLFWglproc
#define boomslang_GlfwImage                           GLFWimage

typedef struct {
    boomslang_Integer boomslang_width;
    boomslang_Integer boomslang_height;
    boomslang_Integer boomslang_redBits;
    boomslang_Integer boomslang_greenBits;
    boomslang_Integer boomslang_blueBits;
    boomslang_Integer boomslang_refreshRate;
} boomslang_GlfwVideoMode;

// Glfw Functions
#define boomslang_glfwCreateWindow(a,b,c,d,e)         glfwCreateWindow(a,b,c,d,e)
#define boomslang_glfwDestroyWindow(a)                glfwDestroyWindow(a)
#define boomslang_glfwWindowShouldClose(a)            glfwWindowShouldClose(a)
#define boomslang_glfwInit()                          glfwInit()
#define boomslang_glfwTerminate()                     glfwTerminate()
#define boomslang_glfwPollEvents()                    glfwPollEvents()
#define boomslang_glfwSwapBuffers(a)                  glfwSwapBuffers(a)
#define boomslang_glfwMakeContextCurrent(a)           glfwMakeContextCurrent(a)
#define boomslang_glfwSwapInterval(a)                 glfwSwapInterval(a)
#define boomslang_glfwGetPrimaryMonitor()             glfwGetPrimaryMonitor()
#define boomslang_glfwGetCurrentContext()             glfwGetCurrentContext()
#define boomslang_glfwExtensionSupported(a)           glfwExtensionSupported(a)
#define boomslang_glfwGetProcAddress(a)               glfwGetProcAddress(a)
#define boomslang_glfwGetMonitors(a)                  glfwGetMonitors(a)
#define boomslang_glfwGetMonitorPos(a,b,c)            glfwGetMonitorPos(a,b,c)
#define boomslang_glfwGetMonitorPhysicalSize(a,b,c)   glfwGetMonitorPhysicalSize(a,b,c)
#define boomslang_glfwGetMonitorName(a)               boomslang_String(glfwGetMonitorName(a))
#define boomslang_glfwSetGamma(a,b)                   glfwSetGamma(a,b)
#define boomslang_glfwGetGammaRamp(a)                 glfwGetGammaRamp(a)
#define boomslang_glfwSetGammaRamp(a,b)               glfwSetGammaRamp(a,b)
#define boomslang_glfwDefaultWindowHints()            glfwDefaultWindowHints()
#define boomslang_glfwWindowHint(a,b)                 glfwWindowHint(a,b)
#define boomslang_glfwSetWindowShouldCloses(a,b)      glfwSetWindowShouldClose(a,b)
#define boomslang_glfwSetWindowTitle(a,b)             glfwSetWindowTitle(a,b)
#define boomslang_glfwSetWindowIcon(a,b,c)            glfwSetWindowIcon(a,b,c)
#define boomslang_glfwGetWindowPos(a,b,c)             glfwGetWindowPos(a,b,c)
#define boomslang_glfwSetWindowPos(a,b,c)             glfwSetWindowPos(a,b,c)
#define boomslang_glfwGetWindowSize(a,b,c)            glfwGetWindowSize(a,b,c)
#define boomslang_glfwSetWindowSizeLimits(a,b,c,d,e)  glfwSetWindowSizeLimits(a,b,c,d,e)
#define boomslang_glfwSetWindowAspectRatio(a,b,c)     glfwSetWindowAspectRatio(a,b,c)
#define boomslang_glfwSetWindowSize(a,b,c)            glfwSetWindowSize(a,b,c)
#define boomslang_glfwGetWindowFrameSize(a,b,c,d,e)   glfwGetWindowFrameSize(a,b,c,d,e)
#define boomslang_glfwIconifyWindow(a)                glfwIconifyWindow(a)
#define boomslang_glfwRestoreWindow(a)                glfwRestoreWindow(a)
#define boomslang_glfwMaximizeWindow(a)               glfwMaximizeWindow(a)
#define boomslang_glfwShowWindow(a)                   glfwShowWindow(a)
#define boomslang_glfwHideWindow(a)                   glfwHideWindow(a)
#define boomslang_glfwFocusWindow(a)                  glfwFocusWindow(a)
#define boomslang_glfwGetWindowMonitor(a)             glfwGetWindowMonitor(a)
#define boomslang_glfwSetWindowMonitor(a,b,c,d,e,f,g) glfwSetWindowMonitor(a,b,c,d,e,f,g)
#define boomslang_glfwGetWindowAttrib(a,b)            glfwGetWindowAttrib(a,b)
#define boomslang_glfwSetWindowUserPointer(a,b)       glfwSetWindowUserPointer(a,b)
#define boomslang_glfwGetWindowUserPointer(a)         glfwGetWindowUserPointer(a)
#define boomslang_glfwSetWindowPosCallback(a,b)       glfwSetWindowPosCallback(a,b)
#define boomslang_glfwSetWindowSizeCallback(a,b)      glfwSetWindowSizeCallback(a,b)
#define boomslang_glfwSetWindowCloseCallback(a,b)     glfwSetWindowCloseCallback(a,b)
#define boomslang_glfwSetWindowRefreshCallback(a,b)   glfwSetWindowRefreshCallback(a,b)
#define boomslang_glfwSetWindowFocusCallback(a,b)     glfwSetWindowFocusCallback(a,b)
#define boomslang_glfwSetWindowIconifyCallback(a,b)   glfwSetWindowIconifyCallback(a,b)
#define boomslang_glfwSetFramebufferSizeCallback(a,b) glfwSetFramebufferSizeCallback(a,b)
#define boomslang_glfwWaitEvents()                    glfwWaitEvents()
#define boomslang_glfwWaitEventsTimeout(a)            glfwWaitEventsTimeout(a)
#define boomslang_glfwPostEmptyEvent()                glfwPostEmptyEvent()
#define boomslang_glfwGetKey(a,b)                     boomslang_Integer(glfwGetKey(a,b))
#define boomslang_glfwGetInputMode(a,b)               glfwGetInputMode(a,b)
#define boomslang_glfwGetKeyName(a,b)                 boomslang_String(glfwGetKeyName(a,b))
#define boomslang_glfwGetMouseButton(a,b)             glfwGetMouseButton(a,b)
#define boomslang_glfwGetCursorPos(a,b,c)             glfwGetCursorPos(a,b,c)
#define boomslang_glfwSetCursorPos(a,b,c)             glfwSetCursorPos(a,b,c)
#define boomslang_glfwCreateCursor(a,b,c)             glfwCreateCursor(a,b,c)
#define boomslang_glfwCreateStandardCursor(a)         glfwCreateStandardCursor(a)
#define boomslang_glfwDestroyCursor(a)                glfwDestroyCursor(a)
#define boomslang_glfwSetCursor(a,b)                  glfwSetCursor(a,b)
#define boomslang_glfwSetTime(a)                      glfwSetTime(a)
#define boomslang_glfwGetTime()                       boomslang_Double(glfwGetTime())
#define boomslang_glfwGetClipboardString()            boomslang_String(glfwGetClipboardString())
#define boomslang_glfwJoystickPresent(a)              boomslang_Integer(glfwJoystickPresent(a))
#define boomslang_glfwGetJoystickName(a)              boomslang_String(glfwGetJoystickName(a))

// Glfw Custom Implementations
inline boomslang_GlfwVideoMode boomslang_glfwGetVideoMode(boomslang_GlfwMonitor* m){
    const GLFWvidmode* v = glfwGetVideoMode(m);
    return boomslang_GlfwVideoMode{v->width, v->height, v->redBits, v->greenBits, v->blueBits, v->refreshRate};
}

// Glfw Callbacks
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_Integer, boomslang_Integer, boomslang_Integer, boomslang_Integer);} __bwrap__GLFWkeybinding;
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_UnsignedInteger);} __bwrap__GLFWcharbinding;
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_UnsignedInteger, boomslang_Integer);} __bwrap__GLFWcharmodsbinding;
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_Integer, boomslang_Integer, boomslang_Integer);} __bwrap__GLFWmousebtnbinding;
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_Double, boomslang_Double);} __bwrap__GLFWcurposbinding;
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_Integer);} __bwrap__GLFWcurenterbinding;
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_Double, boomslang_Double);} __bwrap__GLFWscrollbinding;
typedef struct {GLFWwindow* window;void (*callback)(GLFWwindow*, boomslang_Integer);} __bwrap__GLFWdropbinding;
std::vector<__bwrap__GLFWkeybinding> __bwrap__GLFWkeycallbacks; std::vector<__bwrap__GLFWcharbinding> __bwrap__GLFWcharcallbacks;
std::vector<__bwrap__GLFWcharmodsbinding> __bwrap__GLFWcharmodscallbacks; std::vector<__bwrap__GLFWmousebtnbinding> __bwrap__GLFWmousebtncallbacks;
std::vector<__bwrap__GLFWcurposbinding> __bwrap__GLFWcurposcallbacks; std::vector<__bwrap__GLFWcurenterbinding> __bwrap__GLFWcurentercallbacks;
std::vector<__bwrap__GLFWscrollbinding> __bwrap__GLFWscrollcallbacks; std::vector<__bwrap__GLFWdropbinding> __bwrap__GLFWdropcallbacks;

inline void boomslang_glfwSetKeyCallback(GLFWwindow* window, void (*func)(GLFWwindow*, boomslang_Integer, boomslang_Integer, boomslang_Integer, boomslang_Integer)){
    glfwSetKeyCallback(window, [](GLFWwindow* w, int a, int b, int c, int d)->void {
        for(std::size_t i = 0; i < __bwrap__GLFWkeycallbacks.size(); i++){
            if(__bwrap__GLFWkeycallbacks[i].window == w){
                __bwrap__GLFWkeycallbacks[i].callback(w, boomslang_Integer(a), boomslang_Integer(b), boomslang_Integer(c), boomslang_Integer(d));
                break;
            }
        }
    });
    for(std::size_t i = 0; i < __bwrap__GLFWkeycallbacks.size(); i++){
        if(__bwrap__GLFWkeycallbacks[i].window == window){
            __bwrap__GLFWkeycallbacks[i].callback = func;
            return;
        }
    }
    __bwrap__GLFWkeycallbacks.push_back( __bwrap__GLFWkeybinding{window, func} );
}
inline void boomslang_glfwSetCharCallback(GLFWwindow* window, void (*func)(GLFWwindow*, boomslang_UnsignedInteger)){
    glfwSetCharCallback(window, [](GLFWwindow* w, unsigned int a)->void {
        for(std::size_t i = 0; i < __bwrap__GLFWcharcallbacks.size(); i++){
            if(__bwrap__GLFWcharcallbacks[i].window == w){
                __bwrap__GLFWcharcallbacks[i].callback(w, boomslang_UnsignedInteger(a));
                break;
            }
        }
    });
    for(std::size_t i = 0; i < __bwrap__GLFWcharcallbacks.size(); i++){
        if(__bwrap__GLFWcharcallbacks[i].window == window){
            __bwrap__GLFWcharcallbacks[i].callback = func;
            return;
        }
    }
    __bwrap__GLFWcharcallbacks.push_back( __bwrap__GLFWcharbinding{window, func} );
}
inline void boomslang_glfwSetCharModsCallback(GLFWwindow* window, void (*func)(GLFWwindow*, boomslang_UnsignedInteger, boomslang_Integer)){
    glfwSetCharModsCallback(window, [](GLFWwindow* w, unsigned int a, int b)->void {
        for(std::size_t i = 0; i < __bwrap__GLFWcharmodscallbacks.size(); i++){
            if(__bwrap__GLFWcharmodscallbacks[i].window == w){
                __bwrap__GLFWcharmodscallbacks[i].callback(w, boomslang_UnsignedInteger(a), boomslang_Integer(b));
                break;
            }
        }
    });
    for(std::size_t i = 0; i < __bwrap__GLFWcharmodscallbacks.size(); i++){
        if(__bwrap__GLFWcharmodscallbacks[i].window == window){
            __bwrap__GLFWcharmodscallbacks[i].callback = func;
            return;
        }
    }
    __bwrap__GLFWcharmodscallbacks.push_back( __bwrap__GLFWcharmodsbinding{window, func} );
}
inline void boomslang_glfwSetMouseButtonCallback(GLFWwindow* window, void (*func)(GLFWwindow*, boomslang_Integer, boomslang_Integer, boomslang_Integer)){
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int a, int b, int c)->void {
        for(std::size_t i = 0; i < __bwrap__GLFWmousebtncallbacks.size(); i++){
            if(__bwrap__GLFWmousebtncallbacks[i].window == w){
                __bwrap__GLFWmousebtncallbacks[i].callback(w, boomslang_Integer(a), boomslang_Integer(b), boomslang_Integer(c));
                break;
            }
        }
    });
    for(std::size_t i = 0; i < __bwrap__GLFWmousebtncallbacks.size(); i++){
        if(__bwrap__GLFWmousebtncallbacks[i].window == window){
            __bwrap__GLFWmousebtncallbacks[i].callback = func;
            return;
        }
    }
    __bwrap__GLFWmousebtncallbacks.push_back( __bwrap__GLFWmousebtnbinding{window, func} );
}
inline void boomslang_glfwSetCursorPosCallback(GLFWwindow* window, void (*func)(GLFWwindow*, boomslang_Double, boomslang_Double)){
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double a, double b)->void {
        for(std::size_t i = 0; i < __bwrap__GLFWcurposcallbacks.size(); i++){
            if(__bwrap__GLFWcurposcallbacks[i].window == w){
                __bwrap__GLFWcurposcallbacks[i].callback(w, boomslang_Double(a), boomslang_Double(b));
                break;
            }
        }
    });
    for(std::size_t i = 0; i < __bwrap__GLFWcurposcallbacks.size(); i++){
        if(__bwrap__GLFWcurposcallbacks[i].window == window){
            __bwrap__GLFWcurposcallbacks[i].callback = func;
            return;
        }
    }
    __bwrap__GLFWcurposcallbacks.push_back( __bwrap__GLFWcurposbinding{window, func} );
}
inline void boomslang_glfwSetCursorEnterCallback(GLFWwindow* window, void (*func)(GLFWwindow*, boomslang_Integer)){
    glfwSetCursorEnterCallback(window, [](GLFWwindow* w, int a)->void {
        for(std::size_t i = 0; i < __bwrap__GLFWcurentercallbacks.size(); i++){
            if(__bwrap__GLFWcurentercallbacks[i].window == w){
                __bwrap__GLFWcurentercallbacks[i].callback(w, boomslang_Integer(a));
                break;
            }
        }
    });
    for(std::size_t i = 0; i < __bwrap__GLFWcurentercallbacks.size(); i++){
        if(__bwrap__GLFWcurentercallbacks[i].window == window){
            __bwrap__GLFWcurentercallbacks[i].callback = func;
            return;
        }
    }
    __bwrap__GLFWcurentercallbacks.push_back( __bwrap__GLFWcurenterbinding{window, func} );
}
inline void boomslang_glfwSetScrollCallback(GLFWwindow* window, void (*func)(GLFWwindow*, boomslang_Double, boomslang_Double)){
    glfwSetScrollCallback(window, [](GLFWwindow* w, double a, double b)->void {
        for(std::size_t i = 0; i < __bwrap__GLFWscrollcallbacks.size(); i++){
            if(__bwrap__GLFWscrollcallbacks[i].window == w){
                __bwrap__GLFWscrollcallbacks[i].callback(w, boomslang_Double(a), boomslang_Double(b));
                break;
            }
        }
    });
    for(std::size_t i = 0; i < __bwrap__GLFWscrollcallbacks.size(); i++){
        if(__bwrap__GLFWscrollcallbacks[i].window == window){
            __bwrap__GLFWscrollcallbacks[i].callback = func;
            return;
        }
    }
    __bwrap__GLFWscrollcallbacks.push_back( __bwrap__GLFWscrollbinding{window, func} );
}

inline void boomslang_glfwGetFramebufferSize(GLFWwindow* window, boomslang_Integer* a, boomslang_Integer* b){
    int w, h;

    glfwGetFramebufferSize(window, &w, &h);
    *a = w;
    *b = h;
}
inline void boomslang_glfwGetFramebufferSize(GLFWwindow* window, boomslang_Double* a, boomslang_Double* b){
    int w, h;

    glfwGetFramebufferSize(window, &w, &h);
    *a = w;
    *b = h;
}

#define boomslang_GLFW_UNKNOWN boomslang_Integer(GLFW_UNKNOWN)
#define boomslang_GLFW_KEY_APOSTROPHE boomslang_Integer(GLFW_KEY_APOSTROPHE)
#define boomslang_GLFW_KEY_SPACE boomslang_Integer(GLFW_KEY_SPACE)
#define boomslang_GLFW_KEY_COMMA boomslang_Integer(GLFW_KEY_COMMA)
#define boomslang_GLFW_KEY_MINUS boomslang_Integer(GLFW_KEY_MINUS)
#define boomslang_GLFW_KEY_PERIOD boomslang_Integer(GLFW_KEY_PERIOD)
#define boomslang_GLFW_KEY_SLASH boomslang_Integer(GLFW_KEY_SLASH)
#define boomslang_GLFW_KEY_0 boomslang_Integer(GLFW_KEY_0)
#define boomslang_GLFW_KEY_1 boomslang_Integer(GLFW_KEY_1)
#define boomslang_GLFW_KEY_2 boomslang_Integer(GLFW_KEY_2)
#define boomslang_GLFW_KEY_3 boomslang_Integer(GLFW_KEY_3)
#define boomslang_GLFW_KEY_4 boomslang_Integer(GLFW_KEY_4)
#define boomslang_GLFW_KEY_5 boomslang_Integer(GLFW_KEY_5)
#define boomslang_GLFW_KEY_6 boomslang_Integer(GLFW_KEY_6)
#define boomslang_GLFW_KEY_7 boomslang_Integer(GLFW_KEY_7)
#define boomslang_GLFW_KEY_8 boomslang_Integer(GLFW_KEY_8)
#define boomslang_GLFW_KEY_9 boomslang_Integer(GLFW_KEY_9)
#define boomslang_GLFW_KEY_SEMICOLON boomslang_Integer(GLFW_KEY_SEMICOLON)
#define boomslang_GLFW_KEY_EQUAL boomslang_Integer(GLFW_KEY_EQUAL)
#define boomslang_GLFW_KEY_A boomslang_Integer(GLFW_KEY_A)
#define boomslang_GLFW_KEY_B boomslang_Integer(GLFW_KEY_B)
#define boomslang_GLFW_KEY_C boomslang_Integer(GLFW_KEY_C)
#define boomslang_GLFW_KEY_D boomslang_Integer(GLFW_KEY_D)
#define boomslang_GLFW_KEY_E boomslang_Integer(GLFW_KEY_E)
#define boomslang_GLFW_KEY_F boomslang_Integer(GLFW_KEY_F)
#define boomslang_GLFW_KEY_G boomslang_Integer(GLFW_KEY_G)
#define boomslang_GLFW_KEY_H boomslang_Integer(GLFW_KEY_H)
#define boomslang_GLFW_KEY_I boomslang_Integer(GLFW_KEY_I)
#define boomslang_GLFW_KEY_J boomslang_Integer(GLFW_KEY_J)
#define boomslang_GLFW_KEY_K boomslang_Integer(GLFW_KEY_K)
#define boomslang_GLFW_KEY_L boomslang_Integer(GLFW_KEY_L)
#define boomslang_GLFW_KEY_M boomslang_Integer(GLFW_KEY_M)
#define boomslang_GLFW_KEY_N boomslang_Integer(GLFW_KEY_N)
#define boomslang_GLFW_KEY_O boomslang_Integer(GLFW_KEY_O)
#define boomslang_GLFW_KEY_P boomslang_Integer(GLFW_KEY_P)
#define boomslang_GLFW_KEY_Q boomslang_Integer(GLFW_KEY_Q)
#define boomslang_GLFW_KEY_R boomslang_Integer(GLFW_KEY_R)
#define boomslang_GLFW_KEY_S boomslang_Integer(GLFW_KEY_S)
#define boomslang_GLFW_KEY_T boomslang_Integer(GLFW_KEY_T)
#define boomslang_GLFW_KEY_U boomslang_Integer(GLFW_KEY_U)
#define boomslang_GLFW_KEY_V boomslang_Integer(GLFW_KEY_V)
#define boomslang_GLFW_KEY_W boomslang_Integer(GLFW_KEY_W)
#define boomslang_GLFW_KEY_X boomslang_Integer(GLFW_KEY_X)
#define boomslang_GLFW_KEY_Y boomslang_Integer(GLFW_KEY_Y)
#define boomslang_GLFW_KEY_Z boomslang_Integer(GLFW_KEY_Z)
#define boomslang_GLFW_KEY_LEFT_BRACKET boomslang_Integer(GLFW_KEY_LEFT_BRACKET)
#define boomslang_GLFW_KEY_BACKSLASH boomslang_Integer(GLFW_KEY_BACKSLASH)
#define boomslang_GLFW_KEY_RIGHT_BRACKET boomslang_Integer(GLFW_KEY_RIGHT_BRACKET)
#define boomslang_GLFW_KEY_GRAVE_ACCENT boomslang_Integer(GLFW_KEY_GRAVE_ACCENT)
#define boomslang_GLFW_KEY_WORLD_1 boomslang_Integer(GLFW_KEY_WORLD_1)
#define boomslang_GLFW_KEY_WORLD_2 boomslang_Integer(GLFW_KEY_WORLD_2)
#define boomslang_GLFW_KEY_ESCAPE boomslang_Integer(GLFW_KEY_ESCAPE)
#define boomslang_GLFW_KEY_ENTER boomslang_Integer(GLFW_KEY_ENTER)
#define boomslang_GLFW_KEY_TAB boomslang_Integer(GLFW_KEY_TAB)
#define boomslang_GLFW_KEY_BACKSPACE boomslang_Integer(GLFW_KEY_BACKSPACE)
#define boomslang_GLFW_KEY_INSERT boomslang_Integer(GLFW_KEY_INSERT)
#define boomslang_GLFW_KEY_DELETE boomslang_Integer(GLFW_KEY_DELETE)
#define boomslang_GLFW_KEY_RIGHT boomslang_Integer(GLFW_KEY_RIGHT)
#define boomslang_GLFW_KEY_LEFT boomslang_Integer(GLFW_KEY_LEFT)
#define boomslang_GLFW_KEY_DOWN boomslang_Integer(GLFW_KEY_DOWN)
#define boomslang_GLFW_KEY_UP boomslang_Integer(GLFW_KEY_UP)
#define boomslang_GLFW_KEY_PAGE_UP boomslang_Integer(GLFW_KEY_PAGE_UP)
#define boomslang_GLFW_KEY_PAGE_DOWN boomslang_Integer(GLFW_KEY_PAGE_DOWN)
#define boomslang_GLFW_KEY_HOME boomslang_Integer(GLFW_KEY_HOME)
#define boomslang_GLFW_KEY_END boomslang_Integer(GLFW_KEY_END)
#define boomslang_GLFW_KEY_CAPS_LOCK boomslang_Integer(GLFW_KEY_CAPS_LOCK)
#define boomslang_GLFW_KEY_SCROLL_LOCK boomslang_Integer(GLFW_KEY_SCROLL_LOCK)
#define boomslang_GLFW_KEY_NUM_LOCK boomslang_Integer(GLFW_KEY_NUM_LOCK)
#define boomslang_GLFW_KEY_PRINT_SCREEN boomslang_Integer(GLFW_KEY_PRINT_SCREEN)
#define boomslang_GLFW_KEY_PAUSE boomslang_Integer(GLFW_KEY_PAUSE)
#define boomslang_GLFW_KEY_F1 boomslang_Integer(GLFW_KEY_F1)
#define boomslang_GLFW_KEY_F2 boomslang_Integer(GLFW_KEY_F2)
#define boomslang_GLFW_KEY_F3 boomslang_Integer(GLFW_KEY_F3)
#define boomslang_GLFW_KEY_F4 boomslang_Integer(GLFW_KEY_F4)
#define boomslang_GLFW_KEY_F5 boomslang_Integer(GLFW_KEY_F5)
#define boomslang_GLFW_KEY_F6 boomslang_Integer(GLFW_KEY_F6)
#define boomslang_GLFW_KEY_F7 boomslang_Integer(GLFW_KEY_F7)
#define boomslang_GLFW_KEY_F8 boomslang_Integer(GLFW_KEY_F8)
#define boomslang_GLFW_KEY_F9 boomslang_Integer(GLFW_KEY_F9)
#define boomslang_GLFW_KEY_F10 boomslang_Integer(GLFW_KEY_F10)
#define boomslang_GLFW_KEY_F11 boomslang_Integer(GLFW_KEY_F11)
#define boomslang_GLFW_KEY_F12 boomslang_Integer(GLFW_KEY_F12)
#define boomslang_GLFW_KEY_F13 boomslang_Integer(GLFW_KEY_F13)
#define boomslang_GLFW_KEY_F14 boomslang_Integer(GLFW_KEY_F14)
#define boomslang_GLFW_KEY_F15 boomslang_Integer(GLFW_KEY_F15)
#define boomslang_GLFW_KEY_F16 boomslang_Integer(GLFW_KEY_F16)
#define boomslang_GLFW_KEY_F17 boomslang_Integer(GLFW_KEY_F17)
#define boomslang_GLFW_KEY_F18 boomslang_Integer(GLFW_KEY_F18)
#define boomslang_GLFW_KEY_F19 boomslang_Integer(GLFW_KEY_F19)
#define boomslang_GLFW_KEY_F20 boomslang_Integer(GLFW_KEY_F20)
#define boomslang_GLFW_KEY_F21 boomslang_Integer(GLFW_KEY_F21)
#define boomslang_GLFW_KEY_F22 boomslang_Integer(GLFW_KEY_F22)
#define boomslang_GLFW_KEY_F23 boomslang_Integer(GLFW_KEY_F23)
#define boomslang_GLFW_KEY_F24 boomslang_Integer(GLFW_KEY_F24)
#define boomslang_GLFW_KEY_F25 boomslang_Integer(GLFW_KEY_F25)
#define boomslang_GLFW_KEY_KP_0 boomslang_Integer(GLFW_KEY_KP_0)
#define boomslang_GLFW_KEY_KP_1 boomslang_Integer(GLFW_KEY_KP_1)
#define boomslang_GLFW_KEY_KP_2 boomslang_Integer(GLFW_KEY_KP_2)
#define boomslang_GLFW_KEY_KP_3 boomslang_Integer(GLFW_KEY_KP_3)
#define boomslang_GLFW_KEY_KP_4 boomslang_Integer(GLFW_KEY_KP_4)
#define boomslang_GLFW_KEY_KP_5 boomslang_Integer(GLFW_KEY_KP_5)
#define boomslang_GLFW_KEY_KP_6 boomslang_Integer(GLFW_KEY_KP_6)
#define boomslang_GLFW_KEY_KP_7 boomslang_Integer(GLFW_KEY_KP_7)
#define boomslang_GLFW_KEY_KP_8 boomslang_Integer(GLFW_KEY_KP_8)
#define boomslang_GLFW_KEY_KP_9 boomslang_Integer(GLFW_KEY_KP_9)
#define boomslang_GLFW_KEY_KP_DECIMAL boomslang_Integer(GLFW_KEY_KP_DECIMAL)
#define boomslang_GLFW_KEY_KP_DIVIDE boomslang_Integer(GLFW_KEY_KP_DIVIDE)
#define boomslang_GLFW_KEY_KP_MULTIPLY boomslang_Integer(GLFW_KEY_KP_MULTIPLY)
#define boomslang_GLFW_KEY_KP_SUBTRACT boomslang_Integer(GLFW_KEY_KP_SUBTRACT)
#define boomslang_GLFW_KEY_KP_ADD boomslang_Integer(GLFW_KEY_KP_ADD)
#define boomslang_GLFW_KEY_KP_ENTER boomslang_Integer(GLFW_KEY_KP_ENTER)
#define boomslang_GLFW_KEY_KP_EQUAL boomslang_Integer(GLFW_KEY_KP_EQUAL)
#define boomslang_GLFW_KEY_LEFT_SHIFT boomslang_Integer(GLFW_KEY_LEFT_SHIFT)
#define boomslang_GLFW_KEY_LEFT_CONTROL boomslang_Integer(GLFW_KEY_LEFT_CONTROL)
#define boomslang_GLFW_KEY_LEFT_ALT boomslang_Integer(GLFW_KEY_LEFT_ALT)
#define boomslang_GLFW_KEY_LEFT_SUPER boomslang_Integer(GLFW_KEY_LEFT_SUPER)
#define boomslang_GLFW_KEY_RIGHT_SHIFT boomslang_Integer(GLFW_KEY_RIGHT_SHIFT)
#define boomslang_GLFW_KEY_RIGHT_CONTROL boomslang_Integer(GLFW_KEY_RIGHT_CONTROL)
#define boomslang_GLFW_KEY_RIGHT_ALT boomslang_Integer(GLFW_KEY_RIGHT_ALT)
#define boomslang_GLFW_KEY_RIGHT_SUPER boomslang_Integer(GLFW_KEY_RIGHT_SUPER)
#define boomslang_GLFW_KEY_MENU boomslang_Integer(GLFW_KEY_MENU)
#define boomslang_GLFW_KEY_LAST boomslang_Integer(GLFW_KEY_LAST)

#define boomslang_GLFW_MOD_SHIFT boomslang_Integer(GLFW_MOD_SHIFT)
#define boomslang_GLFW_MOD_CONTROL boomslang_Integer(GLFW_MOD_CONTROL)
#define boomslang_GLFW_MOD_ALT boomslang_Integer(GLFW_MOD_ALT)
#define boomslang_GLFW_MOD_SUPER boomslang_Integer(GLFW_MOD_SUPER)

#define boomslang_GLFW_MOUSE_BUTTON_1 boomslang_Integer(GLFW_MOUSE_BUTTON_1)
#define boomslang_GLFW_MOUSE_BUTTON_2 boomslang_Integer(GLFW_MOUSE_BUTTON_2)
#define boomslang_GLFW_MOUSE_BUTTON_3 boomslang_Integer(GLFW_MOUSE_BUTTON_3)
#define boomslang_GLFW_MOUSE_BUTTON_4 boomslang_Integer(GLFW_MOUSE_BUTTON_4)
#define boomslang_GLFW_MOUSE_BUTTON_5 boomslang_Integer(GLFW_MOUSE_BUTTON_5)
#define boomslang_GLFW_MOUSE_BUTTON_6 boomslang_Integer(GLFW_MOUSE_BUTTON_6)
#define boomslang_GLFW_MOUSE_BUTTON_7 boomslang_Integer(GLFW_MOUSE_BUTTON_7)
#define boomslang_GLFW_MOUSE_BUTTON_8 boomslang_Integer(GLFW_MOUSE_BUTTON_8)
#define boomslang_GLFW_MOUSE_BUTTON_LAST boomslang_Integer(GLFW_MOUSE_BUTTON_LAST)
#define boomslang_GLFW_MOUSE_BUTTON_LEFT boomslang_Integer(GLFW_MOUSE_BUTTON_LEFT)
#define boomslang_GLFW_MOUSE_BUTTON_RIGHT boomslang_Integer(GLFW_MOUSE_BUTTON_RIGHT)
#define boomslang_GLFW_MOUSE_BUTTON_MIDDLE boomslang_Integer(GLFW_MOUSE_BUTTON_MIDDLE)

// Not Supported Yet
// glfwSetDropCallback because no String Lists yet
// glfwGetTimerValue because no uint64_t class yet
// glfwGetTimerFrequency because no uint64_t class yet
// glfwSetClipboardString because no const char* class yet
// glfwGetJoystickAxes because no const float* class yet
// glfwGetJoystickButtons because no const unsigned char* class yet

#endif // GLFW3_WRAP_H_INCLUDED
