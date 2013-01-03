#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "controls.hpp"

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

glm::mat4 getViewMatrix() {
    return viewMatrix;
}

glm::mat4 getProjectionMatrix() {
    return projectionMatrix;
}

const glm::vec3 direction (0, 0, 0);
float distance = 100;
float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;
float initialFoV = 45.0f;
float speed = 0.01;
int scroll_speed = 10;


void computeMatricesFromInputs() {
    static int lastX;
    static int lastY;
    static bool isMoving = false;
    static int mouseWheel = 0;

    int newMouseWheel = glfwGetMouseWheel();
    distance += (mouseWheel - newMouseWheel) * scroll_speed;
    mouseWheel = newMouseWheel;

    if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (isMoving) {
            int curX;
            int curY;
            glfwGetMousePos(&curX, &curY);
            if (cos(verticalAngle) < 0) {
                horizontalAngle += (curX - lastX) * speed;
            } else {
                horizontalAngle -= (curX - lastX) * speed;
            }
            verticalAngle += (curY - lastY) * speed;
            lastX = curX;
            lastY = curY;
        } else {
            isMoving = true;
            glfwGetMousePos(&lastX, &lastY);
        }
    } else {
        isMoving = false;
    }

    glm::vec3 position (
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle));
    position *= distance;

    projectionMatrix = glm::perspective(initialFoV, 4.0f/3.0f, 0.1f, 1000.1f);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (cos(verticalAngle) < 0) {
        up = -up;
    }

    viewMatrix = glm::lookAt(position, direction, up);

}
