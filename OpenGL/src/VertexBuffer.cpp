#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	// Generate vertex buffer
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));	// Generate buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));	// Bind/select buffer
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW)); // Allocate memory
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::bind()
{
}

void VertexBuffer::unbind()
{
}
