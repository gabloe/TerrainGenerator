#pragma once

#include "Def.h"

#include <iostream>

#ifndef __APPLE__
#include "GL/glew.h"
#else
#include "GL/gl3w.h"
#endif

enum SHADER_ERROR { NO_SHADER_ERROR , FRAGMENT, VERTEX, PROGRAM };

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
	GLuint getProgram(){ return this->program; }
	SHADER_ERROR getError() { return this->error; }

	GLenum load() {
		glUseProgram(this->program); 
		return glGetError();
	};
	
	GLuint isValid() { return glIsProgram(this->program) == GL_TRUE; }
	void unload(){ glUseProgram(0); };

};

