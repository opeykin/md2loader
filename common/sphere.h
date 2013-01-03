#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <math.h>
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using std::vector;

void createSphere(vector<vec3>& vertices, vector<GLushort>& indices, vector<vec2>& texcoords,
             float radius, unsigned int rings, unsigned int sectors);


#endif // SPHERE_H
