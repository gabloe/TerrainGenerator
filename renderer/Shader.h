#pragma once
#include "GL/glew.h"

#include <string>

#include "Def.h"

// Errors that can happen when you try to open a shader
enum SHADER_ERROR { NO_SHADER_ERROR , FRAGMENT, VERTEX, PROGRAM };

class RENDERER_API Shader {
private:
	GLuint _shader_program;
	GLuint _vertex_id, _fragment_id;
	std::string *_vertex_source = 0,*_fragment_source = 0;
	size_t _vertex_code_length, _fragment_code_length;

	SHADER_ERROR _error;

public:
	Shader(const std::string&, const std::string&);
	~Shader();

	GLuint getVariable(std::string&);
	GLuint getUniform(std::string&);

	GLenum load();
	GLboolean isValid();
	GLenum unload();

	SHADER_ERROR getError();

};

