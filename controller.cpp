#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glfw.h>

#include <iostream>

#include "controller.h"

using namespace std;
using namespace glm;

Controller::Controller(Model_MD2& model)
    : model(model) {
    position = glm::vec3(0);
    initial_rotation =  glm::rotate(glm::mat4(1.0), 90.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
    model_matrix = initial_rotation;
    is_moving = false;
    is_turning = false;
    is_sitting = false;
    direction_in_degree = 0;
}

glm::core::type::mat4 Controller::getModelMatrix() {
    //glm::mat4 rotation = glm::rotate(initial_rotation, direction, glm::vec3(0.0f, 0.0f, -1.0f));
    //return glm::translate(rotation, position);
    //glm::mat4 translation = glm::translate(initial_rotation, glm::vec3(100, 100, 100));
    glm::mat4 translation = glm::translate(initial_rotation, position);
    return glm::rotate(translation, direction_in_degree, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Controller::process() {
    if(glfwGetKey(68) == GLFW_PRESS) { // d
        direction_in_degree -= ROTATION_SPEED;
    }

    if(glfwGetKey(65) == GLFW_PRESS) { // a
        direction_in_degree += ROTATION_SPEED;
    }

    if (is_moving) {
        float angle = direction_in_degree * 3.14 / 180;
        //cout << "angle: " << angle << " cos: " << cos(angle) << " sin: " << sin(angle) << endl;
        glm::vec3 shift = glm::vec3(cos(angle), sin(angle), 0) * MOVING_SPEED;
        position = position + shift;
    }

    if(glfwGetKey(69) == GLFW_PRESS && !is_moving) { // e
       model.Do(Model_MD2::FLIP);
    }

    if(glfwGetKey(87) == GLFW_PRESS) { // w
        if (!is_moving) {
           is_moving = true;
           model.Do(Model_MD2::RUN, true);
        }
    } else {
        if (is_moving) {
            is_moving = false;
            model.Do(Model_MD2::STAND);
        }
    }

    if(glfwGetKey(32) == GLFW_PRESS) { // space
        if (is_moving) {
            model.Do(Model_MD2::JUMP, false, Model_MD2::RUN);
        } else {
            model.Do(Model_MD2::JUMP, false, Model_MD2::STAND);
        }
    }
}
