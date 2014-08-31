#pragma once

#include "Def.h"

#include "RenderObject.h"
#include "../math/vec3.h"

class RENDERER_API ObjectGenerator
{
public:

	static RenderObject* createSquare(Shader &s, Vec3 &position);
	static RenderObject* createTriangle(Shader &s, Vec3 &position);
	static RenderObject* createCircle(Shader &s, Vec3 &position, unsigned int radius);

	static RenderObject* createCube(Shader &s, Vec3 &position);
	static RenderObject* createSphere(Shader &s, Vec3 &position, unsigned int radius);
	static RenderObject* createPyramid(Shader &s, Vec3 &position);

};

