// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include "common/sphere.h"
#include "common/texture.hpp"

#include "model.h"
#include "controller.h"

using std::cout;
using std::endl;

int main( void )
{
    cout << "starting\n";



    glewExperimental = GL_TRUE;
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		glfwTerminate();
		return -1;
	}

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

    glfwSetWindowTitle( "task2" );
	glfwEnable( GLFW_STICKY_KEYS );
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);
/*
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
*/
    GLuint programID = LoadShaders( "vertexshader", "fragmentshader" );
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint textureID  = glGetUniformLocation(programID, "myTextureSampler");

    glUseProgram(programID);

    Model_MD2 model;
    Controller controller(model);
    model.Do(Model_MD2::STAND);

    cout << "created\n";

    model.Load(
                "skin/rat.md2",
                "skin/skin.bmp"
                );

    do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        ModelMatrix = controller.getModelMatrix();
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glUniform1i(textureID, 0);

        controller.process();
        model.processAnimation();
        model.Draw();
        glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

glDeleteProgram(programID);

	glfwTerminate();

	return 0;
}

