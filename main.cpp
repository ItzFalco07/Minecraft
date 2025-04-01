#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream> 
#include <GL/glut.h>

// Nuklear includes with implementation
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "utils/nuklear.h"
#include "utils/nuklear_glfw_gl3.h"

using namespace glm;

float deltaTime = 1.0f;
float lastFrame = 0.0f;
bool cursorLocked = true;
bool firstMouse = true;
float lastX = 400.0f, lastY = 225.0f;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float sensitivity = 0.1f;

vec3 cameraPosition = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.05f;

struct nk_context *ctx;

void display();
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void drawDebugMenu();

int main() {
    // Step 1: Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Step 2: Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 450, "Minecraft", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Step 3: Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    std::cout << "GLFW and GLEW initialized successfully." << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Step 4: Initialize Nuklear
    ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
    if (!ctx) {
        std::cerr << "Failed to initialize Nuklear context" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    std::cout << "Nuklear context initialized." << std::endl;

    // Step 5: Set up font atlas
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    if (!atlas) {
        std::cerr << "Failed to begin font stash" << std::endl;
        nk_glfw3_shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    nk_glfw3_font_stash_end();
    std::cout << "Font stash set up successfully." << std::endl;

    glfwSetInputMode(window, GLFW_CURSOR, cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // declare speed and sensitivity
        sensitivity = 10.0f * deltaTime;
        cameraSpeed = 5.0f * deltaTime;

        // initialize nuklear frame (2d GUI on top of 3d)
        nk_glfw3_new_frame();

        // idk
        processInput(window);

        display();
        // display rendered image into screen
        glfwSwapBuffers(window);

        // handle events
        glfwPollEvents();
    }

    // Cleanup
    nk_glfw3_shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Function implementations (unchanged for brevity, but included for completeness)
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    static bool escapePressed = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escapePressed) {
        cursorLocked = !cursorLocked;
        glfwSetInputMode(window, GLFW_CURSOR, cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        escapePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) escapePressed = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraPosition += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cameraPosition -= cameraUp * cameraSpeed;
}

void drawCube() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(0.5f, 0.5f, 0.5f); glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glColor3f(1.0f, 0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f); glVertex3f(-0.5f, -0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, -0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glColor3f(0.0f, 1.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.5f); glVertex3f(0.5f, -0.5f, -0.5f); glVertex3f(0.5f, 0.5f, -0.5f); glVertex3f(0.5f, 0.5f, 0.5f);
    glEnd();
    glPopMatrix();
}

void drawCrosshair() {
    float lineSize = 10.0f;
    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-lineSize, 0); glVertex2f(lineSize, 0);
    glVertex2f(0, -lineSize); glVertex2f(0, lineSize);
    glEnd();
    glEnable(GL_DEPTH_TEST);
}

void drawGUI() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-width/2, width/2, -height/2, height/2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    drawCrosshair();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawDebugMenu() {
    int nuklear_window = nk_begin(ctx, "Debug Menu", nk_rect(10,10, 220, 200), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE);

    if (nuklear_window) {
        nk_layout_row_dynamic(ctx, 30, 1);

        nk_label(ctx, "Camera Position:", NK_TEXT_LEFT);
        // display coordinates
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "X: %.2f Y: %.2f Z: %.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        nk_label(ctx, buffer, NK_TEXT_LEFT);

        // display fps
        nk_label(ctx, "FPS:", NK_TEXT_LEFT);
        snprintf(buffer, sizeof(buffer), "%.2f", 1.0f / deltaTime);
        nk_label(ctx, buffer, NK_TEXT_LEFT);
    }

    nk_end(ctx);
}

void display() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (float)width / (float)height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mat4 view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glLoadMatrixf(value_ptr(view));

    drawCube();
    drawGUI();

    drawDebugMenu();
    nk_glfw3_render(NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    cameraYaw += xOffset;
    cameraPitch += yOffset;
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
    vec3 front;
    front.x = cos(radians(cameraYaw)) * cos(radians(cameraPitch));
    front.y = sin(radians(cameraPitch));
    front.z = sin(radians(cameraYaw)) * cos(radians(cameraPitch));
    cameraFront = front;
}