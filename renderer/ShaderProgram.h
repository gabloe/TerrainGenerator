#pragma once

#define RENDERER_API __declspec(dllexport) 

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
	__declspec( dllexport) ShaderProgram(const char* vert , const char* frag);
	__declspec(dllexport) ~ShaderProgram();

	GLuint getVertexShader() { return v; }
	GLuint getFragmentShader() { return f; }
	GLuint getProgram(){ return program; }
	__declspec(dllexport) SHADER_ERROR getError();

	
};

