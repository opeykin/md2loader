#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

class Controller {
public:
    Controller(Model_MD2& model);
    glm::mat4 getModelMatrix();
    void process();

private:
    Model_MD2& model;
    glm::mat4 model_matrix;
    glm::mat4 initial_rotation;
    bool is_moving;
    bool is_turning;
    bool is_sitting;
    float direction_in_degree;
    glm::vec3 position;

    static const float ROTATION_SPEED = 0.3;
    static const float MOVING_SPEED = 0.1;
};

#endif // CONTROLLER_H
