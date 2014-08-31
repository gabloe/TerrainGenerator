#include "RenderObject.h"

#include <string>

RenderObject::RenderObject(Shader &shader, GLfloat* vertices, int number_vertices, GLuint* indices, int number_indices) {

	_shader = &shader;
	shader.load();

	_number_vertices = number_vertices;
	_number_indices = number_indices;

	glGenBuffers(1, &_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, number_vertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	if (indices) {
		glGenBuffers(1, &_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}
	std::string *a = new std::string("v_Position");
	std::string *b = new std::string("projection");
	std::string *c = new std::string("translate");

	if ((_position_index = shader.getVariable(*a)) < 0) {
		std::cout << "Error: Could not load v_Position" << std::endl;
	}
	if ((_projection_index = shader.getUniform(*b)) < 0) {
		std::cout << "Error: Could not load projection" << std::endl;
	}
	if ((_translation_index = shader.getUniform(*c)) < 0) {
		std::cout << "Error: Could not load translate" << std::endl;
	}

	if (glGetError()) {
		std::cout << "Error creating render object" << std::endl;
	}

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
	glUniformMatrix4fv(_projection_index, 1, GL_FALSE, projection.getData());
	glUniformMatrix4fv(_translation_index, 1, GL_FALSE, translate.getData());
	
	// Turn on variables
	glEnableVertexAttribArray(_position_index);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glVertexAttribPointer(
		_position_index,// attribute
		3,				// how many elements per item
		GL_FLOAT,		// the element type
		GL_FALSE,		// normalized
		0,				// stride
		(void*)0);		// offset
	
	if (_number_indices) {
		// Draw
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
		glDrawElements(
			GL_TRIANGLES,		// type to draw
			_number_indices,	// how mant indices
			GL_UNSIGNED_INT,	// index data type
			(void*)0);			// offset
	}else {
		glDrawArrays(GL_TRIANGLES, 0, _number_vertices / 3);
	}
	

	glDisableVertexAttribArray(_position_index);

	// Unload the shader program
	_shader->unload();
}