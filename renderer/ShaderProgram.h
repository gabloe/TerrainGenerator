#pragma once

#include <GL/glew.h>

enum SHADER_ERROR { FRAGMENT, VERTEX, PROGRAM };

class ShaderProgram
{
private:
	GLuint program;
	GLuint v, f;
	GLchar* v_src;
	GLchar* f_src;
	GLint v_len, f_len;

	int error;
	void cleanup();
public:
	ShaderProgram(const char* vert , const char* frag);
	~ShaderProgram();

	GLuint getVertexShader() { return v; }
	GLuint getFragmentShader() { return f; }
	GLuint getProgram(){ return program; }
	SHADER_ERROR getError();

	
};

