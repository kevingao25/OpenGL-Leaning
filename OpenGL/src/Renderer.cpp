#include "Renderer.h"
#include <iostream>

// Clear any previous error that we have before calling the tested function
void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// Logging the error flag from OpenGL (hexInt)
bool GLLogCall(const char* function, const char* file, int line) {
	// Wrap in while loop because there might have multiple error flags
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " <<
			file << ":" << line << std::endl;
		return false;
	}
	return true;
}