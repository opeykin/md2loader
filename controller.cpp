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
    glm::mat4 translation = glm::translate(initial_rotation, position);
    return glm::rotate(translation, direction_in_degree, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Controller::process() {
    Model_MD2::AnimationType animation = Model_MD2::STAND;
    bool new_animation = false;

    if(glfwGetKey(68) == GLFW_PRESS) { // d
        direction_in_degree -= ROTATION_SPEED;
    }

    if(glfwGetKey(65) == GLFW_PRESS) { // a
        direction_in_degree += ROTATION_SPEED;
    }

    if (glfwGetKey(87) == GLFW_PRESS) {
        is_moving = true;
    } else {
        if (is_moving) {
            new_animation = true;
            if (is_sitting) {
                animation = Model_MD2::CROUCH_STAND;
            } else {
                animation = Model_MD2::STAND;
            }
        }
        is_moving = false;
    }

    if(glfwGetKey(69) == GLFW_PRESS) { // e
        animation = Model_MD2::FLIP;
        new_animation = true;
    }

    if(glfwGetKey(32) == GLFW_PRESS) { // space
        animation = Model_MD2::JUMP;
        new_animation = true;
    }

    if(glfwGetKey(49) == GLFW_PRESS) { // space
        animation = Model_MD2::ATTACK;
        new_animation = true;
    }

    if(glfwGetKey(50) == GLFW_PRESS) { // space
        animation = Model_MD2::SALUTE;
        new_animation = true;
    }
    if(glfwGetKey(51) == GLFW_PRESS) { // space
        animation = Model_MD2::WAVE;
        new_animation = true;
    }

    if(glfwGetKey(52) == GLFW_PRESS) { // space
        animation = Model_MD2::POINT;
        new_animation = true;
    }

    if(glfwGetKey(53) == GLFW_PRESS) { // space
        animation = Model_MD2::BOOM;
        new_animation = true;
    }

    if (glfwGetKey(81) == GLFW_PRESS) {
        is_sitting = true;
        animation = Model_MD2::CROUCH_STAND;
        new_animation = true;
    } else {
        is_sitting = false;
    }

    if (is_moving) {
        float radians = direction_in_degree * 3.14 / 180;
        if (is_sitting) {
            model.Do(Model_MD2::CROUCH_WALK, true);
            move(radians, CR_SPEED);
        } else {
            model.Do(Model_MD2::RUN, true);
            move(radians, RUNING_SPEED);
        }
    } else if (new_animation) {
        model.Do(animation);
    }
}

void Controller::move(float radians, float speed) {
    glm::vec3 shift = glm::vec3(cos(radians), sin(radians), 0) * speed;
    position = position + shift;
}
