#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *vertexShaderSource = 
"attribute vec3 aPos;\n"
"void main() {\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

const char *fragmentShaderSource = 
"precision mediump float;\n"
"void main() {\n"
"   gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\0";


void checkCompile(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, NULL, info);
        printf("Shader compile error: %s\n", info);
        exit(-1);
    }
}

void checkLink(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        printf("Program link error: %s\n", info);
        exit(-1);
    }
}


int main() {
    Display *x_disp = XOpenDisplay(NULL);
    if (!x_disp) { printf("Cannot open X display\n"); return -1; }

    // 1. 初始化 EGL
    // The set of supported platforms and their behavior is defined by extensions.
    // To detect if a particular platform is supported, 
    // clients should query the EGL_EXTENSIONS string of EGL_NO_DISPLAY using eglQueryString
    const char *exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    if (!strstr(exts, "EGL_KHR_platform_x11")) {
        fprintf(stderr, "EGL_KHR_platform_x11 not supported\n");
        return -1;
    }
    EGLDisplay eglDpy = eglGetPlatformDisplay(EGL_PLATFORM_X11_KHR, (void *)x_disp, NULL);

    if (eglDpy == EGL_NO_DISPLAY) { printf("No EGL display\n"); return -1; }
    eglInitialize(eglDpy, NULL, NULL);

    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,8, EGL_GREEN_SIZE,8, EGL_BLUE_SIZE,8, EGL_ALPHA_SIZE,8,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    eglChooseConfig(eglDpy, configAttribs, &config, 1, &numConfigs);

    // 2. 创建 EGLSurface 并 关联 Window
    XSetWindowAttributes swa;
    Window root = DefaultRootWindow(x_disp);
    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
    Window win = XCreateWindow(x_disp, root, 0,0, 640,480,0,
                               CopyFromParent, InputOutput, CopyFromParent,
                               CWEventMask, &swa);
    XMapWindow(x_disp, win);

    EGLSurface eglSurf = eglCreateWindowSurface(eglDpy, config, (EGLNativeWindowType)win, NULL);

    // 3. 创建 EGLContext
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext eglCtx = eglCreateContext(eglDpy, config, EGL_NO_CONTEXT, contextAttribs);

    eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);


    // 4. OpenGL shaders & VBO
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompile(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompile(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkLink(shaderProgram);
    glUseProgram(shaderProgram);


    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);


    // 6. eglSwapBuffers 可视化 render buffer
    glViewport(0, 0, 640, 480);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(eglDpy, eglSurf);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor   = glGetString(GL_VENDOR);
    const GLubyte* version  = glGetString(GL_VERSION);

    printf("GL_TRIANGLES displayed!\n");

    printf("Renderer: %s\n", renderer);
    printf("Vendor: %s\n", vendor);
    printf("GL Version: %s\n", version);

    printf("EGL Vendor: %s\n", eglQueryString(eglDpy, EGL_VENDOR));
    printf("EGL Version: %s\n", eglQueryString(eglDpy, EGL_VERSION));
    printf("EGL Renderer: %s\n", eglQueryString(eglDpy, EGL_CLIENT_APIS));

    getchar();

    eglDestroySurface(eglDpy, eglSurf);
    eglDestroyContext(eglDpy, eglCtx);
    eglTerminate(eglDpy);
    XDestroyWindow(x_disp, win);
    XCloseDisplay(x_disp);

    return 0;
}
