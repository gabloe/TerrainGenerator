#pragma once

#include "Def.h"

#ifndef __APPLE__
#include "GL/glew.h"
#endif

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
	RenderObject();
	~RenderObject();

	// Set
	void setVertices(GLfloat *vert, int num);
	void setNormals(GLfloat *norms, int num);
	void setIndices(GLuint *inds, int num);
	void setPosition(Vec3 p);

	void setMode(GLenum mode);
	void setShaderProgram(ShaderProgram* p);


	// Get
	GLuint* getIndices();
	GLfloat* getVertices();
	GLfloat* getNormals();

	GLuint getNumIndices();
	GLuint getNumVertices();
	GLuint getNumNormals();

	GLenum getDisplayMode();

	Vec3 getPosition();

	ShaderProgram* getShaderProgram();
};

