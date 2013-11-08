#pragma once

#if !defined(__gl_h_) || defined(__GL_H__) || defined(__X_GL_H)
	#include <gl/glew.h>
#endif

class RenderObject
{
public:
	RenderObject();
	~RenderObject();

	GLuint* getIndices();
	GLfloat* getVertices();
	GLfloat* getNormals();

	GLuint getNumIndices();
	GLuint getNumVertices();
	GLuint getNumNormals();

	GLenum getDisplayMode();
};

