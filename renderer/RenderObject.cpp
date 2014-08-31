#include "RenderObject.h"

RenderObject::RenderObject() {}
RenderObject::~RenderObject() {}

// Set
void RenderObject::setVertices(GLfloat *vert, int num) { this->vert = vert; this->num_vert = num; }
void RenderObject::setNormals(GLfloat *norms, int num) { this->norms = norms; this->num_norm = num; }
void RenderObject::setIndices(GLuint *inds, int num) { this->inds = inds; this->num_ind = num; }
void RenderObject::setPosition(Vec3 p) { this->position = p; }

void RenderObject::setMode(GLenum mode) { this->mode = mode; }
void RenderObject::setShaderProgram(ShaderProgram* p) { this->program = p; }

void RenderObject::moveToGPU() {
	program->load();

	glGenBuffers(1, &v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBufferData(GL_ARRAY_BUFFER, num_vert * sizeof(float), vert, GL_DYNAMIC_DRAW);
	

	glGenBuffers(1, &i_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_ind * sizeof(GLuint), inds, GL_STATIC_COPY);
	
	v_position = glGetAttribLocation(program->getProgram(), "v_Position");
	p_mat = glGetUniformLocation(program->getProgram(), "projection");
	
	program->unload();
	
}

// Get
GLuint* RenderObject::getIndices() { return this->inds; }
GLfloat* RenderObject::getVertices() { return this->vert; }
GLfloat* RenderObject::getNormals() { return this->norms; }

GLuint RenderObject::getNumIndices() { return this->num_ind; }
GLuint RenderObject::getNumVertices() { return this->num_vert; }
GLuint RenderObject::getNumNormals() { return this->num_norm; }

GLuint RenderObject::getVertexBufferIndex() { return v_buffer; }
GLuint RenderObject::getIndexBufferIndex() { return i_buffer; }
GLuint RenderObject::getShaderPosition() { return v_position; }

GLenum RenderObject::getDisplayMode() { return this->mode; }

Vec3 RenderObject::getPosition() { return this->position; }

ShaderProgram* RenderObject::getShaderProgram() { return this->program; }

void RenderObject::setProjectionMatrix(Mat4* mat) {
	this->proj = mat;
}

void RenderObject::render() {
	// Load the shader program
	program->load();

	// Turn on variables
	glEnableVertexAttribArray(v_position);

	// Select the data
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer);

	glVertexAttribPointer(v_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glUniformMatrix4fv(p_mat, 1, false, proj->getData());

	// Draw
	glDrawElements(GL_TRIANGLES, num_ind, GL_UNSIGNED_INT, 0);

	// Unload the shader program
	program->unload();
}