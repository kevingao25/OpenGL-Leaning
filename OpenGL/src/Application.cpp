#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


// --------------------------------------------------------------------------------------
// Shader program

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

// Parse the shader program from external files
static ShaderProgramSource ParseShader(const std::string& filepath) {

	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	// Add the lines to corresponding stringStream type
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) {
			// Set the stream type to VERTEX or FRAGMENT
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			if (type == ShaderType::NONE) {
				continue;
			}

			// 0 represents vertex string, 1 represents fragment string
			ss[(int)type] << line << '\n';
		}
	}

	// .str() copy string from the streambuffer
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();	// Pointer to the beginnign of the data
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Debug shade
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);	// iv:  i: integer, v: vector

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // alloca: allocate on stack
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);

		return 0;
	}
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);


	// Delete the obj shaders
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

// --------------------------------------------------------------------------------------

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(10);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}

	// Logging the current GL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		float positions[] = {
			-0.5f, -0.5f,	// 0
			0.5f,  -0.5f,	// 1
			0.5f,  0.5f,	// 2
			-0.5f, 0.5f		// 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Create a new vertex array
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		// Generate vertex buffer
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		// Tell OpenGL the layout of our buffer
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		// Create index buffer
		IndexBuffer ib(indices, 6);

		// Create the shader
		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		GLCall(glUseProgram(shader));

		// Uniform (pass data from cpp to shader)
		GLCall(int location = glGetUniformLocation(shader, "u_Color"));	// Get the location of the uniform from the shader id (shader name represents the id)
		ASSERT(location != -1);
		GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));	// Set the data into the shader

		float r = 0.0f;
		float increment = 0.05;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			// Use shader
			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));	// Set the data into the shader

			// Bind vertex array and index buffer
			GLCall(glBindVertexArray(vao));
			ib.bind();

			// Draw the triangle
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			// Change the colour
			if (r > 1.0f) increment = -0.05f;
			if (r < 0.0f) increment = 0.05;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glDeleteProgram(shader);
	}

	glfwTerminate();
	return 0;
}