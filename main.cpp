#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream> 

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

float deltaTime = 1.0f;
float lastFrame = 0.0f;
bool cursorLocked = true;
bool firstMouse = true;
float lastX = 400.0f, lastY = 225.0f;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float sensitivity = 0.1f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.05f;

struct nk_context *ctx;

void display();
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void drawDebugMenu();

void initCube();

static unsigned int CompileShader(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);

    const char* src = source.c_str(); // .c_str returns the memory adress and we store in char src (first charecter)

    glShaderSource(id, 1, &src, nullptr)
    glCompileShader(id);

    // error handling
    int result;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        std::cout << "some error in code" << std::endl;
    } else {
        std::cout << "compiled shaders" << std::endl;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
}

// ids of cube vbo and vao
unsigned int cubeVBO, cubeVAO; // never be negative

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

    // save cube verticies (VBO) with its VAO
    initCube();

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

void initCube() {
    float vertices[] = {
        // Front face (Red)
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        // Back face (Green)
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

        // Top face (Blue)
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

        // Bottom face (Yellow)
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,

        // Left face (Magenta)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,

        // Right face (Cyan)
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    };

    // Generate and bind VBO, VAO
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    // bind/select vao and vbo
    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

    // GL_STATIC_DRAW -> You’ll set the data once and use it many times.
    // GL_DYNAMIC_DRAW -> You’ll update the data frequently and use it many times.
    // GL_STREAM_DRAW -> You’ll set the data once and use it only a few times (e.g., per frame).
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // upload the data to the current buffer object

    // tell VAO about the verticies range
    glVertexAttribPointer(
        0, // location attibute (0 for verticies, 1 for color)
        3, // 3 components
        GL_FLOAT, // float values
        GL_FALSE, // do not normalize (convert to float)
        6 * sizeof(float), // size of a single vertex with its color 
        (void*)0 // start from 0 bits
    );

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float)) // start after 3rd component (3 * sizeof(float))
    );

    glEnableVertexAttribArray(0); // enable the 0th attribute (verticies)
    glEnableVertexAttribArray(1); // enable the 1st attribute (colors)

    // Unbind/deselect VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void drawCube() {
    // Draw the cube (inside the render loop)
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); // trangles (6 vertex in each side, total vertex = 6 * 6 = 36)
    glBindVertexArray(0);
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

    glm::mat4 view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glLoadMatrixf(glm::value_ptr(view));

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
    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = front;
}