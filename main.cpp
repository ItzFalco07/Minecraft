#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GL/glut.h>

// imgui
#include "./packages/imgui/imgui.h"
#include "./packages/imgui/imgui_impl_glfw.h"
#include "./packages/imgui/imgui_impl_opengl3.h"


using namespace glm;

void display();
float rotationAngle = 0.0f;

float deltaTime = 1.0f; // difference between last frame and current frame
float lastFrame = 0.0f; // time of last frame

vec3 cameraPosition = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.05f * deltaTime;
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();



    GLFWwindow* window = glfwCreateWindow(800, 450, "Minecraft", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Initialize ImGui with GLFW and OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        processInput(window); // Capture input
        if (rotationAngle == 360.0f) {
            rotationAngle = 0.0f;
        }
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
        rotationAngle += 0.5f;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    // imgui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    int newWidth = width;
    int newHeight = (width * 9) / 16;

    if (newHeight > height) {
        newHeight = height;
        newWidth = (height * 16) / 9;
    }

    glfwSetWindowSize(window, newWidth, newHeight);
    glViewport(0, 0, newWidth, newHeight);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPosition += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPosition -= cameraUp * cameraSpeed;
}

void drawCube() {
    glPushMatrix();
    glRotatef(rotationAngle, 1.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);

    // Front face (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Top face (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Bottom face (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Left face (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // Right face (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    glEnd();

    glPopMatrix();
}

void display() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Clear the buffer
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 16.0 / 9.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up camera using glm
    mat4 view = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glLoadMatrixf(value_ptr(view));

    drawCube();

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug Menu");

    ImGui::Text("Rotation Angle: %.2f", rotationAngle);
    ImGui::Text("Delta Time: %.4f", deltaTime);
    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)",
                cameraPosition.x, cameraPosition.y, cameraPosition.z);

    // Collapsible Section
    if (ImGui::CollapsingHeader("Controls")) {
        ImGui::Text("W/A/S/D - Move");
        ImGui::Text("Space - Up, Shift - Down");
    }

    ImGui::End();

    // Render ImGui on top
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {

}