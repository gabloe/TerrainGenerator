#pragma once

#include "Def.h"

#include "GL/glew.h"

#include "Shader.h"

#include "../math/vec3.h"
#include "../math/mat4.h"


class RENDERER_API RenderObject
{

private:
	Mat4 *_projection;
	Mat4 _translation;

	GLuint _number_vertices, _number_indices;

	// Pointers to variables for shaders
	GLuint _vertex_buffer;
	GLuint _normal_buffer;
	GLuint _index_buffer;
	GLuint _position_index;
	GLuint _normal_index;
	GLuint _projection_index;
	GLuint _translation_index;

	Shader* _shader;

public:
	RenderObject(Shader &shader, GLfloat* vertices, int number_vertices, GLuint* indices, int number_indices);
	~RenderObject();

	void render(const Mat4&,const Mat4&);
	
};

