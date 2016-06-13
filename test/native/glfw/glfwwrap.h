
#ifndef GLFW3_WRAP_H_INCLUDED
#define GLFW3_WRAP_H_INCLUDED

#define boomslang_GlfwWindow                  GLFWwindow
#define boomslang_GlfwMonitor                 GLFWmonitor
#define boomslang_GlfwVidMode                 GLFWvidmode
#define boomslang_GlfwGammaRamp               GLFWgammaramp
#define boomslang_glfwCreateWindow(a,b,c,d,e) glfwCreateWindow(a,b,c,d,e)
#define boomslang_glfwDestroyWindow(a)        glfwDestroyWindow(a)
#define boomslang_glfwWindowShouldClose(a)    glfwWindowShouldClose(a)
#define boomslang_glfwInit()                  glfwInit()
#define boomslang_glfwTerminate()             glfwTerminate()
#define boomslang_glfwPollEvents()            glfwPollEvents()
#define boomslang_glfwSwapBuffers(a)          glfwSwapBuffers(a)
#define boomslang_glfwMakeContextCurrent(a)   glfwMakeContextCurrent(a)
#define boomslang_glfwSwapInterval(a)         glfwSwapInterval(a)
#define boomslang_glClearColor(a,b,c,d)       glClearColor(a,b,c,d)
#define boomslang_glClear()                   glClear(GL_COLOR_BUFFER_BIT)

#endif // GLFW3_WRAP_H_INCLUDED
