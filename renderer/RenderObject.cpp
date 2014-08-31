#include "RenderObject.h"

#include <string>
#include <cmath>

bool close(Vec3 &a, Vec3 &b){
	float x = a.getX() - b.getX();
	float y = a.getY() - b.getY();
	float z = a.getZ() - b.getZ();

	return abs(x) < 0.0001 && abs(y) < 0.0001 && abs(z) < 0.0001;

}
Vec3 helper(Vec3 &a, Vec3 &b, Vec3 &c) {
	Vec3 t1 = b - a;
	Vec3 t2 = c - a;

	Vec3 N = t1.cross(t2);

	N.normalize();
	return N;
}

RenderObject::RenderObject(Shader &shader, GLfloat* vertices, int number_vertices, GLuint* indices, int number_indices) {

	_shader = &shader;
	shader.load();

	_number_vertices = number_vertices;
	_number_indices = number_indices;

	GLfloat *normals = new GLfloat[number_vertices];
	memset(normals, 0, sizeof(GLfloat) * number_vertices);

	for (int i = 0; i < number_indices; i += 3) {
		
		int i1 = 3 * indices[i + 0];
		int i2 = 3 * indices[i + 1];
		int i3 = 3 * indices[i + 2];

		Vec3 p1(vertices + i1);
		Vec3 p2(vertices + i2);
		Vec3 p3(vertices + i3);

		Vec3 t1 = helper(p1, p2, p3);
		//Vec3 t2 = helper(p2, p3, p1);
		//Vec3 t3 = helper(p3, p1, p2);

		/*
		std::cout << t1 << std::endl;
		std::cout << t2 << std::endl;
		std::cout << t3 << std::endl;
		*/

		normals[i1 + 0] += t1.getX();
		normals[i1 + 1] += t1.getY();
		normals[i1 + 2] += t1.getZ();

		normals[i2 + 0] += t1.getX();
		normals[i2 + 1] += t1.getY();
		normals[i2 + 2] += t1.getZ();

		normals[i3 + 0] += t1.getX();
		normals[i3 + 1] += t1.getY();
		normals[i3 + 2] += t1.getZ();

	}

	// Copy over the position data
	glGenBuffers(1, &_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, number_vertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);


	// Copy over the normals
	glGenBuffers(1, &_normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, number_vertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

	// If we have indices copy them over too
	if (indices) {
		glGenBuffers(1, &_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, number_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}
	std::string *a = new std::string("v_Position");
	std::string *b = new std::string("v_Normal");

	std::string *x = new std::string("camera");
	std::string *y = new std::string("projection");
	std::string *z = new std::string("translate");
	
	if ((_position_index = shader.getVariable(*a)) < 0) {
		std::cout << "Error: Could not load v_Position" << std::endl;
	}

	if ((_normal_index = shader.getVariable(*b)) < 0) {
		std::cout << "Error: Could not load v_Normal" << std::endl;
	}

	if ((_camera_index = shader.getUniform(*x)) < 0) {
		std::cout << "Error: Could not load camera" << std::endl;
	}

	if ((_projection_index = shader.getUniform(*y)) < 0) {
		std::cout << "Error: Could not load projection" << std::endl;
	}

	if ((_translation_index = shader.getUniform(*z)) < 0) {
		std::cout << "Error: Could not load translate" << std::endl;
	}

	if (GLenum err = glGetError()) {
		std::cout << "Error creating render object" << std::endl;
		std::cout << glewGetErrorString(err) << std::endl;
	}

	delete normals;

	delete a;
	delete b;

	delete x;
	delete y;
	delete z;

	shader.unload();

}

RenderObject::~RenderObject() {}

void RenderObject::render(const Mat4 &projection, const Mat4& translate, const Vec3 &camera) {
	// Load the shader program
	_shader->load();

	// Load uniforms
	glUniformMatrix4fv(_projection_index, 1, GL_FALSE, projection.getData());
	glUniformMatrix4fv(_translation_index, 1, GL_FALSE, translate.getData());
	glUniform3fv(_camera_index, 1, camera.getData());
	
	// Turn on variables
	glEnableVertexAttribArray(_position_index);
	glEnableVertexAttribArray(_normal_index);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glVertexAttribPointer(
		_position_index,// attribute
		3,				// how many elements per item
		GL_FLOAT,		// the element type
		GL_FALSE,		// normalized
		0,				// stride
		(void*)0);		// offset

	glVertexAttribPointer(
		_normal_index,// attribute
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
