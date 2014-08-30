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


// Get
GLuint* RenderObject::getIndices() { return this->inds; }
GLfloat* RenderObject::getVertices() { return this->vert; }
GLfloat* RenderObject::getNormals() { return this->norms; }

GLuint RenderObject::getNumIndices() { return this->num_ind; }
GLuint RenderObject::getNumVertices() { return this->num_vert; }
GLuint RenderObject::getNumNormals() { return this->num_norm; }

GLenum RenderObject::getDisplayMode() { return this->mode; }

Vec3 RenderObject::getPosition() { return this->position; }

ShaderProgram* RenderObject::getShaderProgram() { return this->program; }