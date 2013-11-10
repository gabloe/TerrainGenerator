#pragma once

#include "Def.h"

#include <GL/glew.h>
#include "ShaderProgram.h"

#include "../math/vec3.h"

class RENDERER_API RenderObject
{

private:

	Vec3 position;

	GLuint num_vert, num_norm, num_ind;
	GLenum mode;

	GLfloat *vert;
	GLfloat *norms;
	GLuint *inds;

	ShaderProgram* program;

public:
	RenderObject() {}
	~RenderObject() {}

	// Set
	void setVertices(GLfloat *vert, int num) { this->vert = vert; this->num_vert = num; }
	void setNormals(GLfloat *norms, int num) { this->norms = norms; this->num_norm = num; }
	void setIndices(GLuint *inds, int num) { this->inds = inds; this->num_ind = num; }
	void setPositon(Vec3 p) { this->position = p; }


	void setMode(GLenum mode) { this->mode = mode; }
	void setShaderProgram( ShaderProgram* p ) { this->program = p; }


	// Get
	GLuint* getIndices() { return this->inds; }
	GLfloat* getVertices() { return this->vert; }
	GLfloat* getNormals() { return this->norms; }

	GLuint getNumIndices() { return this->num_ind; }
	GLuint getNumVertices() { return this->num_vert; }
	GLuint getNumNormals() { return this->num_norm; }

	GLenum getDisplayMode() { return this->mode; }

	Vec3 getPosition() { return this->position;}

	ShaderProgram* getShaderProgram() { return this->program; }
};

