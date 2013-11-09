#include "ShaderProgram.h"

#include <iostream>
#include <fstream>

using namespace std;

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
	GLchar *source = new GLchar[size + 1];
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
		delete[] * ShaderSource;
	*ShaderSource = 0;
}

/*
	Prints any error from the object passed in
	*/
void printError(GLint object, int blen) {
	GLsizei slen = 0;
	GLchar* compiler_log = (GLchar*)malloc(blen);
	glGetInfoLogARB(object, blen, &slen, compiler_log);
	cout << "compiler_log:\n" << compiler_log << endl;
	free(compiler_log);
}

/*
	Compiles a Shader Object (vertex/fragment)
	*/
int compile(GLuint ShaderObject) {
	GLint blen = 0, compiled;
	glCompileShaderARB(ShaderObject);
	glGetObjectParameterivARB(ShaderObject, GL_COMPILE_STATUS, &compiled);
	if (compiled)	return 0;
	glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH, &blen);
	printError(ShaderObject, blen);
	return 1;
}

/*
*/
int link(GLuint *program, GLuint f, GLuint v){
	GLint blen, linked;
	*program = glCreateProgram();
	glAttachShader(*program, v);
	glAttachShader(*program, f);
	glLinkProgram(*program);
	glGetProgramiv(*program, GL_LINK_STATUS, &linked);
	if (!linked){
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &blen);
		printError(*program, blen);
	}
	return linked;
}

ShaderProgram::ShaderProgram(const char* vert, const char* frag) : program(0), v(0), f(0)
{
	// No error yet
	this->error = NO_SHADER_ERROR;

	// Load Shader Source code
	if (loadshader(vert, &this->v_src, &this->v_len) != 0) {
		printf("Could not load the Vertex Shader file: %s", vert);
		return;
	}
	else if (loadshader(frag, &this->f_src, &this->f_len) != 0) {
		printf("Could not load the Fragment Shader file: %s", frag);
		return;
	}

	// Create palce for Shaders 
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	// Copy over source code
	glShaderSourceARB(v, 1, &vert, &this->v_len);
	glShaderSourceARB(f, 1, &frag, &this->f_len);

	// Try to compile!
	if (!compile(v)) {
		error = VERTEX;
		return;
	}
	if (!compile(f)) {
		error = FRAGMENT;
		return;
	}

	if (!link(&program, f, v)) {
		error = PROGRAM;
	}

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

	unloadshader(&this->v_src);
	unloadshader(&this->f_src);

}

