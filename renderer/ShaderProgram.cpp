#include "ShaderProgram.h"

#include <iostream>
#include <fstream>

using namespace std;

unsigned long long getFileLength(ifstream& file)
{
	if (!file.good()) return 0;

	unsigned long long pos = file.tellg();
	file.seekg(0, ios::end);
	unsigned long long len = file.tellg();
	file.seekg(ios::beg);

	return (unsigned long)len;
}

int loadshader(const char* filename, GLchar** ShaderSource, GLint* len)
{
	ifstream file;
	file.open(filename, ios::in); // opens as ASCII!
	if (!file) return -1;

	unsigned long long size = getFileLength(file);

	if (size == 0) return -2;   // Error: Empty File 

	GLchar *source = new GLchar[size + 1];
	if (source == 0) return -3;   // can't reserve memory

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value... 
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


void unloadshader(GLchar** ShaderSource)
{
	if (*ShaderSource != 0)
		delete[] * ShaderSource;
	*ShaderSource = 0;
}

void printError(GLint object ,int blen ) {
	GLsizei slen = 0;
	GLchar* compiler_log = (GLchar*)malloc(blen);
	glGetInfoLogARB(object, blen, &slen, compiler_log);
	cout << "compiler_log:\n" << compiler_log << endl;
	free(compiler_log);
}

int compile(GLuint ShaderObject) {
	GLint blen = 0;
	GLint compiled;

	glCompileShaderARB(ShaderObject);
	
	glGetObjectParameterivARB(ShaderObject, GL_COMPILE_STATUS, &compiled);
	if (compiled){
		return 1;
	}	
	glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH, &blen);
	printError(ShaderObject, blen);

	return 0;
}

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

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	loadshader(vert, &this->v_src, &this->v_len);
	loadshader(frag, &this->f_src, &this->f_len);

	
	glShaderSourceARB(v, 1, &vert, &this->v_len);
	glShaderSourceARB(f, 1, &frag, &this->f_len);

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

}


ShaderProgram::~ShaderProgram()
{
	unloadshader(&this->v_src);
	unloadshader(&this->f_src);
}

