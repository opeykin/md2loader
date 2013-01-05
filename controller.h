#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

class Controller {
public:
    Controller(Model_MD2& model);
    glm::mat4 getModelMatrix();
    void process();

private:
    void move(float radians, float speed);
    Model_MD2& model;
    glm::mat4 model_matrix;
    glm::mat4 initial_rotation;
    bool is_moving;
    bool is_turning;
    bool is_sitting;
    float direction_in_degree;
    glm::vec3 position;

    static const float ROTATION_SPEED = 0.3;
    static const float RUNING_SPEED = 0.12;
    static const float JUMPING_SPEED = 0.2;
    static const float CR_SPEED = 0.06;
};

#endif // CONTROLLER_H
