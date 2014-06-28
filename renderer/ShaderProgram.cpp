#include "ShaderProgram.h"

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
		error = ret;  \
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
int loadshader(const char* filename, GLchar** ShaderSource, GLint* len) {
	ifstream file;

	// Open the file
	file.open(filename, ios::in);	// opens as ASCII!
	if (!file) return -1;

	// Get file length
	unsigned long long size = getFileLength(file);
	if (size == 0) return -2;		// Error: Empty File 

	// Create Memory
	GLchar *source = (GLchar*)calloc( size + 1, sizeof( GLchar ) );
	if (source == 0) return -3;		// can't reserve memory

	// Read
	file.read(source, size);
	source[size] = 0;
	file.close();


	// Set "Return" values
	*ShaderSource = source;
	*len = (GLint)(size);

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

ShaderProgram::ShaderProgram(const char* vert, const char* frag) : program(0), v(0), f(0)
{

	GLuint err = 0;

	// No error yet
	this->error = NO_SHADER_ERROR;
	this->v_src = 0;
	this->f_src = 0;

	// Load Shader Source code
	if (loadshader( vert , &this->v_src, &this->v_len) != 0) {
		printf("Could not load the Vertex Shader file: %s", vert);
		return;
	}else if (loadshader(frag, &this->f_src, &this->f_len) != 0) {
		printf("Could not load the Fragment Shader file: %s", frag);
		return;
	}

	// Create place for Shaders 
	v = glCreateShader(GL_VERTEX_SHADER);
	CHECK_AND_RET("glCreateShader(GL_VERTEX_SHADER) : ", VERTEX, 0);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	CHECK_AND_RET("glCreateShader(GL_FRAGMENT_SHADER) : ", VERTEX, 0);

	// Copy over source code
	const char* vs = v_src,*fs = f_src;
	glShaderSource(v, 1, &vs, NULL);
	CHECK_AND_RET("glShaderSource(v,1,&vs,NULL) : ", VERTEX, 0);
	glShaderSource(f, 1, &fs, NULL);
	CHECK_AND_RET("glShaderSource(f,1,&fs,NULL) : ", FRAGMENT, 0);

	free(f_src);
	free(v_src);

	// Try to compile!
	glCompileShader(f);
	CHECK_AND_RET("glCompileShader(f) : ", FRAGMENT, f);
	glCompileShader(v);
	CHECK_AND_RET("glCompileShader(v) : ", VERTEX, v);

	this->program = link(f, v);
	CHECK_AND_RET("link(f,v) : ", PROGRAM, 0);

	if(glIsProgram(this->program) != GL_TRUE) {
		std::cout << "Is not a Program!" << std::endl;
	}

	printLog(this->program);
}

/* Cleanup */
ShaderProgram::~ShaderProgram()
{
	if (program) {
		glDeleteProgram(program);
		if (v) {
			glDetachShader(program, v);
			glDeleteShader(v);
		}
		if (f) {
			glDetachShader(program, f);
			glDeleteShader(f);
		}
	}
	else {
		if (v) glDeleteShader(v);
		if (f) glDeleteShader(f);
	}

	program = 0;
	f = 0;
	v = 0;	

}

