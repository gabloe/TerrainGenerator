#include "Shader.h"

#include <iostream>
#include <fstream>

#include <stdlib.h>

using namespace std;

#define ERR printf("HERE\n");

#define HANDLE_ERROR() if( glGetError() != GL_NO_ERROR ) std::cout<< glGetError() <<std::endl;

#define CHECK_AND_RET(msg,ret,obj) \
{ \
	GLuint err = glGetError(); \
	if (err != GL_NO_ERROR) { \
		if (obj) printLog(obj); \
		std::cout << msg << gluErrorString(err) << std::endl; \
		_error = ret;  \
		return; \
	} \
} 

/*
 * Given a file stream returns the length of the file
 */
unsigned long long getFileLength(ifstream& file)
{
	if (!file.good()) return 0;
	file.seekg(0, ios::end);
	unsigned long long len = file.tellg();
	file.seekg(ios::beg);
	return (unsigned long)len;
}

/*
	Loads the shader source code from a file in an array. The length
	of the source code is saved into len.
	*/
int loadshader(const char* filename, std::string **ShaderSource, size_t* len) {
	ifstream file;

	// Open the file
	file.open(filename, ios::in);	// opens as ASCII!
	if (!file) return -1;

	// Get file length
	unsigned long long size = getFileLength(file);
	if (size == 0) return -2;		// Error: Empty File 

	// Create Memory
	GLchar *source = new GLchar[size + 1];
	if (source == 0) return -3;		// can't reserve memory

	// Read
	file.read(source, size);
	source[size] = 0;
	file.close();


	// Set "Return" values
	*ShaderSource = new string(source);
	*len = (GLint)(size);

	delete source;

	// Return no error
	return 0; // No Error
}

/*
	Frees Shader Source Code information
	*/
void unloadshader(GLchar** ShaderSource)
{
	if (*ShaderSource != 0)
		free( ShaderSource );
}

/*
	Prints any error from the object passed in
	*/
void printLog(GLint object) {
	int infoLogLength = 0;
	char infoLog[1024];
	if (glIsShader(object)) {
		glGetShaderInfoLog(object, 1024, &infoLogLength, infoLog);
	}
	else {
		glGetProgramInfoLog(object, 1024, &infoLogLength, infoLog);
	}
	if (infoLogLength > 1) {
		std::cout << infoLog << std::endl;
	}
}

int link(GLuint f, GLuint v){
	GLuint program = glCreateProgram();

	glAttachShader( program, v);
	glAttachShader( program, f);
	glLinkProgram( program );

	printLog(program);
	return program;
}

Shader::Shader(const std::string &base_dir, const std::string &name) {

	GLuint err = 0;
	// No error yet
	_error = NO_SHADER_ERROR;

	// Load vertex shader
	if (loadshader((base_dir + name + ".vert").data(), &_vertex_source, &_vertex_code_length) != 0) {
		std::cerr << "Could not load the Vertex Shader file: " << name << std::endl;
		return;
	} 
	// Load fragment shader
	if (loadshader((base_dir + name + ".frag").data(), &_fragment_source, &_fragment_code_length) != 0) {
		delete _vertex_source;
		std::cerr << "Could not load the Fragment Shader file: " << name << std::endl;
		return;
	}

	// Create place for Shaders 
	_vertex_id = glCreateShader(GL_VERTEX_SHADER);
	CHECK_AND_RET("glCreateShader(GL_VERTEX_SHADER) : ", VERTEX, 0);

	_fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	CHECK_AND_RET("glCreateShader(GL_FRAGMENT_SHADER) : ", VERTEX, 0);

	const GLchar *tmp_c = _vertex_source->data();
	glShaderSource(_vertex_id, 1, &tmp_c, NULL);
	CHECK_AND_RET("glShaderSource(v,1,&vs,NULL) : ", VERTEX, 0);

	tmp_c = _fragment_source->data();
	glShaderSource(_fragment_id, 1, &tmp_c, NULL);
	CHECK_AND_RET("glShaderSource(f,1,&fs,NULL) : ", FRAGMENT, 0);

	// Try to compile!
	glCompileShader(_fragment_id);
	CHECK_AND_RET("glCompileShader(_fragment_id) : ", FRAGMENT, _fragment_id);
        char logBuf[1024];
		glGetShaderInfoLog(_fragment_id, sizeof(logBuf), NULL, (GLchar*)logBuf);
        std::cout << logBuf << std::endl;

	glCompileShader(_vertex_id);
	CHECK_AND_RET("glCompileShader(_vertex_id) : ", VERTEX, _vertex_id);
        glGetShaderInfoLog( _vertex_id, sizeof(logBuf), NULL, (GLchar*)logBuf );
        std::cout << logBuf << std::endl;


	_shader_program = link(_fragment_id, _vertex_id);
	CHECK_AND_RET("link(fragment,vertex) : ", PROGRAM, 0);

	if (glIsProgram(this->_shader_program) != GL_TRUE) {
		std::cout << "Could not load program" << std::endl;
	}
	printLog(_shader_program);
}

/* Cleanup */
Shader::~Shader()
{
	if (_shader_program) {
		glDeleteProgram(_shader_program);
		if (_vertex_id) {
			glDetachShader(_shader_program, _vertex_id);
			glDeleteShader(_vertex_id);
		}
		if (_fragment_id) {
			glDetachShader(_shader_program, _fragment_id);
			glDeleteShader(_fragment_id);
		}
	}
	else {
		if (_vertex_id) glDeleteShader(_vertex_id);
		if (_fragment_id) glDeleteShader(_fragment_id);
	}

	_shader_program = 0;
	_fragment_id = 0;
	_vertex_id = 0;

}

GLuint Shader::load() {
		glUseProgram(_shader_program);
		return glGetError();
}

GLenum Shader::unload() {
	glUseProgram(0);
	return glGetError();
}

GLboolean Shader::isValid()  {
	return glIsProgram(_shader_program);
}

SHADER_ERROR Shader::getError() { 
	return _error; 
}

GLuint Shader::getVariable(std::string &name) {
	return glGetAttribLocation(_shader_program, name.data());
}

GLuint Shader::getUniform(std::string &name) {
	return glGetUniformLocation(_shader_program, name.data());
}

