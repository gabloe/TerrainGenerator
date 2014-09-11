#include "RenderObject.h"

#include <string>
#include <cmath>

#include <cassert>

#include <iostream>

Vec3 meanWeightEqual(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3) {
	Vec3 L1 = p2 - p1;
	Vec3 L2 = p3 - p1;
	Vec3 N = L1.cross(L2);
	N.normalize();
	return N;
}

Vec3 meanWeightAngle(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3) {
	Vec3 L1 = p2 - p1;
	Vec3 L2 = p3 - p1;
	Vec3 N = L1.cross(L2);
	float alpha = asin(N.getMagnitude() / (L1.getMagnitude()*L2.getMagnitude()));

	N.normalize();
	N *= alpha;
	return N;
}

Vec3 meanWeightSineEdge(const Vec3 &p1, const Vec3 &p2, const Vec3 &p3) {
	Vec3 L1 = p2 - p1;
	Vec3 L2 = p3 - p1;
	Vec3 N = L1.cross(L2);
	float bottom =L1.getMagnitude()*L2.getMagnitude();
	float alpha = asin(N.getMagnitude() / (L1.getMagnitude()*L2.getMagnitude()));

	N.normalize();
	N *= alpha;
	return N;
}

GLfloat *computeNormals(GLfloat *vertices, int number_vertices, GLuint *indices, int number_indices) {
	GLfloat *normals = new GLfloat[number_vertices];
	memset(normals, 0, sizeof(GLfloat)* number_vertices);

	GLfloat *divisors = new GLfloat[number_vertices/3];

	for (int i = 0; i < number_indices; i += 3) {

		int i1 = 3 * indices[i + 0];
		int i2 = 3 * indices[i + 1];
		int i3 = 3 * indices[i + 2];

		Vec3 p1(vertices + i1);
		Vec3 p2(vertices + i2);
		Vec3 p3(vertices + i3);

		Vec3 N = (p2 - p1).cross(p3 - p1);
		N.normalize();

		float t1 = N * (p2 - p1);
		if (abs(t1) >  0.00001) {
			std::cout << "T1 Failure: " << t1 << std::endl;
			std::exit(-1);
		}

		t1 = N * (p3 - p1);
		if (abs(t1) >  0.00001) {
			std::cout << "T2 Failure: " << t1 << std::endl;
			std::exit(-1);
		}

		normals[i1 + 0] += N.getX();
		normals[i1 + 1] += N.getY();
		normals[i1 + 2] += N.getZ();

		normals[i2 + 0] += N.getX();
		normals[i2 + 1] += N.getY();
		normals[i2 + 2] += N.getZ();

		normals[i3 + 0] += N.getX();
		normals[i3 + 1] += N.getY();
		normals[i3 + 2] += N.getZ();
	}

	for (int i = 0; i < number_vertices; i += 3) {
		float x = normals[i + 1]; x *= x;
		float y = normals[i + 2]; y *= y;
		float z = normals[i + 3]; z *= z;
		float len = sqrt(x + y + z);

		normals[i + 1] /= len;
		normals[i + 2] /= len;
		normals[i + 3] /= len;
	}

	delete divisors;
	return normals;
}

RenderObject::RenderObject(Shader &shader, GLfloat* vertices, int number_vertices, GLuint* indices, int number_indices) {

	_shader = &shader;
	shader.load();

	_number_vertices = number_vertices;
	_number_indices = number_indices;
	_raw_data = vertices;
	GLfloat *normals = computeNormals(vertices, number_vertices, indices, number_indices);

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

	std::string *str_p1 = new std::string("p1");
	std::string *str_p2 = new std::string("p2");
	std::string *str_p3 = new std::string("p3");

	if ((_p1_index = shader.getUniform(*str_p1)) < 0) {
		std::cout << "Error: Could not load p1" << std::endl;
	}

	if ((_p2_index = shader.getUniform(*str_p2)) < 0) {
		std::cout << "Error: Could not load p2" << std::endl;
	}

	if ((_p3_index = shader.getUniform(*str_p3)) < 0) {
		std::cout << "Error: Could not load p3" << std::endl;
	}

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

int RenderObject::getNumberVertices() const {
	return _number_vertices;
}

const GLfloat *RenderObject::getRawData() const { return _raw_data; }

void RenderObject::render(const Mat4 &projection, const Mat4& translate, const Vec3 &camera, const Vec3 &p1, const Vec3 &p2, const Vec3 &p3 ) {
	// Load the shader program
	_shader->load();

	// Load uniforms
	glUniformMatrix4fv(_projection_index, 1, GL_FALSE, projection.getData());
	glUniformMatrix4fv(_translation_index, 1, GL_FALSE, translate.getData());
	glUniform3fv(_camera_index, 1, camera.getData());

	glUniform3fv(_p1_index, 1, p1.getData());
	glUniform3fv(_p2_index, 1, p2.getData());
	glUniform3fv(_p3_index, 1, p3.getData());
	
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
