#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath, int& width, int& height);

// Load a .TGA file using GLFW's own loader
GLuint loadTGA_glfw(const char * imagepath, GLint mag_filter, GLint min_filter);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);


#endif
