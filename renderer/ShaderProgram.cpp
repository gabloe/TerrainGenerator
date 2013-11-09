#include "ShaderProgram.h"

#include <iostream>
#include <fstream>

using namespace std;

/*
	Given a file stream returns the length of the file
*/
unsigned long long getFileLength(ifstream& file)
{
	if (!file.good()) return 0;

	unsigned long long pos = file.tellg();
	file.seekg(0, ios::end);
	unsigned long long len = file.tellg();
	file.seekg(ios::beg);

	return (unsigned long)len;
}

/*
	Loads the shader source code from a file in an array. The length 
	of the source code is saved into len.
*/
int loadshader(const char* filename, GLchar** ShaderSource, GLint* len)
{
	ifstream file;
	file.open(filename, ios::in); // opens as ASCII!
	if (!file) return -1;

	unsigned long long size = getFileLength(file);

	if (size == 0) return -2;   // Error: Empty File 

	GLchar *source = new GLchar[size + 1];
	if (source == 0) return -3;   // can't reserve memory

	source[size] = 0;

	unsigned int i = 0;
	while (file.good())
	{
		source[i] = file.get();       // get character from file.
		if (!file.eof())
			i++;
	}

	source[i] = 0;  // 0-terminate it at the correct position

	file.close();

	*ShaderSource = source;
	*len = (GLint)(size);

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
void printError(GLint object ,int blen ) {
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
	GLint blen = 0;
	GLint compiled;

	glCompileShaderARB(ShaderObject);
	
	glGetObjectParameterivARB(ShaderObject, GL_COMPILE_STATUS, &compiled);
	if (compiled){
		return 0;
	}

	glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH, &blen);
	printError(ShaderObject, blen);

	return 1;
}

/*
*/
int link(GLuint *program, GLuint f, GLuint v){
	GLint blen;
	GLint linked; 
	*program = glCreateProgram();
	glAttachShader(*program, v);
	glAttachShader(*program, f);
	glLinkProgram(*program);
	
	glGetProgramiv(*program, GL_LINK_STATUS, &linked);
	if (linked){
		return 1;
	}

	glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &blen);
	printError(*program, blen);
	return 0;
}

ShaderProgram::ShaderProgram(const char* vert, const char* frag)
{

	program = 0;
	v = 0;
	f = 0;

	if (loadshader(vert, &this->v_src, &this->v_len) != 0) {
		printf("Could not load the Vertex Shader file: %s" , vert );
		return;
	}
	
	if (loadshader(frag, &this->f_src, &this->f_len) != 0) {
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
		// Cleanup
	}


	this->error = NO_SHADER_ERROR;
}


ShaderProgram::~ShaderProgram()
{
	unloadshader(&this->v_src);
	unloadshader(&this->f_src);
}

