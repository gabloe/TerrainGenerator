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
	glBufferData(GL_ARRAY_BUFFER, num_vert, vert, GL_DYNAMIC_DRAW);
	

	glGenBuffers(1, &i_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, i_buffer);
	glBufferData(GL_ARRAY_BUFFER, num_ind, inds, GL_STATIC_DRAW);
	
	v_position = glGetAttribLocation(program->getProgram(), "v_Position");
	
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

void RenderObject::render() {
	program->load();
	glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer);
	glVertexAttribPointer(v_position, 3, GL_FLOAT, false, 0, 0);
	glDrawElements(GL_TRIANGLES, num_vert / 3, GL_UNSIGNED_SHORT, 0);
	program->unload();
}