#pragma once

#include "Def.h"

#include "RenderObject.h"
#include "../math/vec3.h"

class RENDERER_API ObjectGenerator
{
public:
	ObjectGenerator();
	~ObjectGenerator();

	RenderObject* createSquare(Vec3 position);
	RenderObject* createTriangle(Vec3 position);
	RenderObject* createCircle(Vec3 position, unsigned int detail);

	RenderObject* createCube(Vec3 position);
	RenderObject* createSphere(Vec3 position, unsigned int detail);
	RenderObject* createPyramid(Vec3 position);

};

