#pragma once

#include "Def.h"

#include "GL/glew.h"

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

	GLuint v_buffer;
	GLuint i_buffer;
	GLuint v_position;

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

	void moveToGPU();

	// Get
	GLuint* getIndices();
	GLfloat* getVertices();
	GLfloat* getNormals();

	GLuint getNumIndices();
	GLuint getNumVertices();
	GLuint getNumNormals();

	GLuint getVertexBufferIndex();
	GLuint getIndexBufferIndex();
	GLuint getShaderPosition();

	GLenum getDisplayMode();

	void render();
	Vec3 getPosition();

	ShaderProgram* getShaderProgram();
};

