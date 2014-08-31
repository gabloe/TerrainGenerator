#pragma once

#include "Def.h"

#include <iostream>

#include "GL/glew.h"

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

	GLuint getVertexShader();
	GLuint getFragmentShader();
	GLuint getProgram();
	SHADER_ERROR getError();

	GLenum load();
	GLboolean isValid();
	GLenum unload();

};

