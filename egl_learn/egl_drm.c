#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

#include <gbm.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <drm/drm.h>


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


static void check_egl(const char *msg)
{
    EGLint err = eglGetError();
    if (err != EGL_SUCCESS) {
        fprintf(stderr, "%s failed: 0x%x\n", msg, err);
        exit(1);
    }
}


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

const char* connector_type_str(uint32_t type) {
    switch(type) {
        case DRM_MODE_CONNECTOR_VGA: return "VGA";
        case DRM_MODE_CONNECTOR_DVII: return "DVI-I";
        case DRM_MODE_CONNECTOR_DVID: return "DVI-D";
        case DRM_MODE_CONNECTOR_DVIA: return "DVI-A";
        case DRM_MODE_CONNECTOR_Composite: return "Composite";
        case DRM_MODE_CONNECTOR_SVIDEO: return "SVIDEO";
        case DRM_MODE_CONNECTOR_LVDS: return "LVDS";
        case DRM_MODE_CONNECTOR_Component: return "Component";
        case DRM_MODE_CONNECTOR_9PinDIN: return "9PinDIN";
        case DRM_MODE_CONNECTOR_DisplayPort: return "DP";
        case DRM_MODE_CONNECTOR_HDMIA: return "HDMI-A";
        case DRM_MODE_CONNECTOR_HDMIB: return "HDMI-B";
        case DRM_MODE_CONNECTOR_TV: return "TV";
        case DRM_MODE_CONNECTOR_eDP: return "eDP";
        default: return "Unknown";
    }
}


/* ------------------------------------------------------------ */
/* Main                                                         */
/* ------------------------------------------------------------ */
int main(void)
{
    int drm_fd;
    drmModeRes *res;
    drmModeConnector *conn = NULL;
    drmModeEncoder *enc = NULL;
    drmModeModeInfo mode;
    uint32_t crtc_id, conn_id;

    // 按实际环境配置 DRM 节点
    drm_fd = open("/dev/dri/card1", O_RDWR | O_CLOEXEC);
    if (drm_fd < 0) {
        perror("open /dev/dri/card1");
        return 1;
    }

    res = drmModeGetResources(drm_fd);
    if (!res) {
        perror("drmModeGetResources");
        return 1;
    }


    printf("----------------------List  Info--------------------------\n");

    printf("Total connectors: %d\n", res->count_connectors);
    for (int i = 0; i < res->count_connectors; i++) {
        conn = drmModeGetConnector(drm_fd, res->connectors[i]);
        printf("Connector %d: %s-%d, status=%d, encoder_nums=%d\n",
            conn->connector_id,
            connector_type_str(conn->connector_type),
            conn->connector_type_id,
            conn->connection,
            conn->count_encoders);

        for (int j = 0; j<conn->count_encoders; ++j) {
            drmModeEncoder *enc = drmModeGetEncoder(drm_fd, conn->encoders[j]);
            printf("    Encoder %d: encoder_id: %d type=%d, possible CRTCs=0x%x\n", j, conn->encoders[j], enc->encoder_type, enc->possible_crtcs);
        }
    }
    printf("----------------------------------------------------------\n\n");



    /* -------------------------------------------------------- */
    /* Find connected connector                                 */
    /* -------------------------------------------------------- */
    for (int i = 0; i < res->count_connectors; i++) {
        conn = drmModeGetConnector(drm_fd, res->connectors[i]);
        if (conn->connection == DRM_MODE_CONNECTED &&
            conn->count_modes > 0) {
            break;
        }
        drmModeFreeConnector(conn);
        conn = NULL;
    }

    if (!conn) {
        fprintf(stderr, "No connected connector found\n");
        return 1;
    }

    conn_id = conn->connector_id;
    mode = conn->modes[0];


    /* -------------------------------------------------------- */
    /* Find encoder + CRTC                                      */
    /* -------------------------------------------------------- */
    enc = drmModeGetEncoder(drm_fd, conn->encoder_id);
    if (!enc) {
        fprintf(stderr, "No encoder\n");
        return 1;
    }

    crtc_id = enc->crtc_id;

    printf("Using connector %u, CRTC %u, mode %dx%d\n",
           conn_id, crtc_id,
           mode.hdisplay, mode.vdisplay);




    /* -------------------------------------------------------- */
    /* GBM                                                      */
    /* -------------------------------------------------------- */
    struct gbm_device *gbm_dev = gbm_create_device(drm_fd);
    if (!gbm_dev) {
        fprintf(stderr, "gbm_create_device failed\n");
        return 1;
    }

    struct gbm_surface *gbm_surf =
        gbm_surface_create(gbm_dev,
                           mode.hdisplay,
                           mode.vdisplay,
                           GBM_FORMAT_XRGB2101010,
                           GBM_BO_USE_SCANOUT |
                           GBM_BO_USE_RENDERING);

    if (!gbm_surf) {
        fprintf(stderr, "gbm_surface_create failed\n");
        return 1;
    }

    /* -------------------------------------------------------- */
    /* EGL                                                      */
    /* -------------------------------------------------------- */
    const char *exts = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    if (!strstr(exts, "EGL_KHR_platform_gbm")) {
        fprintf(stderr, "EGL_KHR_platform_gbm not supported\n");
        return 1;
    }

    EGLDisplay egl_dpy =
        eglGetPlatformDisplay(EGL_PLATFORM_GBM_KHR,
                              gbm_dev, NULL);
    if (egl_dpy == EGL_NO_DISPLAY) {
        check_egl("eglGetPlatformDisplay");
    }

    if (!eglInitialize(egl_dpy, NULL, NULL)) {
        check_egl("eglInitialize");
    }

    static const EGLint cfg_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE,   10,
        EGL_GREEN_SIZE, 10,
        EGL_BLUE_SIZE,  10,
        EGL_ALPHA_SIZE, 0,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLConfig cfg;
    EGLint num_cfg;

    if (!eglChooseConfig(egl_dpy, cfg_attribs, &cfg, 1, &num_cfg)
        || num_cfg != 1) {
        check_egl("eglChooseConfig");
    }
    EGLint visual_id;
    eglGetConfigAttrib(egl_dpy, cfg, EGL_NATIVE_VISUAL_ID, &visual_id);
    
    printf("EGL native visual ID: 0x%x\n", visual_id);

    static const EGLint ctx_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLContext ctx =
        eglCreateContext(egl_dpy, cfg, EGL_NO_CONTEXT, ctx_attribs);
    check_egl("eglCreateContext");

    EGLSurface egl_surf =
        eglCreateWindowSurface(egl_dpy, cfg,
                               (EGLNativeWindowType)gbm_surf,
                               NULL);
    check_egl("eglCreateWindowSurface");

    if (!eglMakeCurrent(egl_dpy, egl_surf, egl_surf, ctx)) {
        check_egl("eglMakeCurrent");
    }


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

    glViewport(0, 0, 640, 480);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(egl_dpy, egl_surf);


    /* -------------------------------------------------------- */
    /* DRM scan out                                              */
    /* -------------------------------------------------------- */
    struct gbm_bo *bo = gbm_surface_lock_front_buffer(gbm_surf);
    if (!bo) {
        fprintf(stderr, "gbm_surface_lock_front_buffer failed\n");
        return 1;
    }

    uint32_t fb;
    uint32_t handle = gbm_bo_get_handle(bo).u32;
    uint32_t stride = gbm_bo_get_stride(bo);

    if (drmModeAddFB(drm_fd,
                     mode.hdisplay,
                     mode.vdisplay,
                     30, 32,
                     stride,
                     handle,
                     &fb)) {
        perror("drmModeAddFB");
        return 1;
    }

    if (drmModeSetCrtc(drm_fd, crtc_id,
                       fb, 0, 0,
                       &conn_id, 1,
                       &mode)) {
        perror("drmModeSetCrtc");
        return 1;
    }

    printf("TRIANGLE should now be visible. Sleeping...\n");
    sleep(5);

    return 0;
}
