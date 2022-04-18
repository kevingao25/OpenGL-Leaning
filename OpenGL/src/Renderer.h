#pragma once
#include <GL/glew.h>


// --------------------------------------------------------------------------------------
// OpenGL dealing with errors

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))	// #x gets the string of the funciton name

// Clear any previous error that we have before calling the tested function
void GLClearError();

// Logging the error flag from OpenGL (hexInt)
bool GLLogCall(const char* function, const char* file, int line);

