#pragma once

#include "Def.h"

#include <GL/glew.h>

enum SHADER_ERROR { NO_SHADER_ERROR, FRAGMENT, VERTEX, PROGRAM };

class RENDERER_API ShaderProgram
{
private:
	GLuint program;
	GLuint v, f;
	GLchar* v_src;
	GLchar* f_src;
	GLint v_len, f_len;

	SHADER_ERROR error;

public:
	ShaderProgram(const char* vert, const char* frag);
	~ShaderProgram();

	GLuint getVertexShader() { return v; }
	GLuint getFragmentShader() { return f; }
	GLuint getProgram(){ return program; }
	SHADER_ERROR getError() { return this->error; }

	void load() { glUseProgram(this->program); };
	void unload(){ glUseProgram(0); };

};

