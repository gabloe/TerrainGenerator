#pragma once

#include <GL/glew.h>

class RenderObject
{

private:
	GLuint num_vert, num_norm, num_ind;
	GLenum mode;

	GLfloat *vert;
	GLfloat *norms;
	GLuint *inds;

public:
	RenderObject() {}
	~RenderObject() {}

	void setVertices(GLfloat *vert, int num) { this->vert = vert; this->num_vert = num; }
	void setNormals(GLfloat *norms, int num) { this->norms = norms; this->num_norm = num; }
	void setIndices(GLuint *inds, int num) { this->inds = inds; this->num_ind = num; }
	void setMode(GLenum mode) { this->mode = mode; }

	GLuint* getIndices() { return this->inds; }
	GLfloat* getVertices() { return this->vert; }
	GLfloat* getNormals() { return this->norms; }

	GLuint getNumIndices() { return this->num_ind; }
	GLuint getNumVertices() { return this->num_vert; }
	GLuint getNumNormals() { return this->num_norm; }

	GLenum getDisplayMode() { return this->mode; }
};

