#include "RenderObject.h"

#include <string>

RenderObject::RenderObject(Shader &shader, GLfloat* vertices, int number_vertices, GLuint* indices, int number_indices) {
	_shader = &shader;

	shader.load();

	glGenBuffers(1, &_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, number_vertices* sizeof(GLfloat), vertices, GL_STATIC_DRAW);


	glGenBuffers(1, &_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_indices * sizeof(GLuint), indices, GL_STATIC_COPY);

	std::string *a = new std::string("v_Position");
	std::string *b = new std::string("projection");
	std::string *c = new std::string("translate");

	_position_index = shader.getVariable(*a);
	_projection_index = shader.getUniform(*b);
	_projection_index = shader.getUniform(*c);

	delete a;
	delete b;
	delete c;

	shader.unload();

}
RenderObject::~RenderObject() {}


void RenderObject::render(const Mat4 &projection, const Mat4& translate) {
	// Load the shader program
	_shader->load();

	// Load uniforms
	glUniformMatrix4fv(_projection_index, 1, false, projection.getData());
	glUniformMatrix4fv(_translation_index, 1, false, translate.getData());

	// Select the data
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);

	// Turn on variables
	glEnableVertexAttribArray(_position_index);
	glVertexAttribPointer(_position_index,
		3,			// how many elements per item
		GL_FLOAT,	// the element type
		GL_FALSE,	// normalized
		0,			// stride
		0);			// offset
	
	// Draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
	glDrawElements(
		GL_TRIANGLES,		// type to draw
		_number_indices,	// how mant indices
		GL_UNSIGNED_INT,	// index data type
		(void*)0);			// offset

	// Unload the shader program
	_shader->unload();
}