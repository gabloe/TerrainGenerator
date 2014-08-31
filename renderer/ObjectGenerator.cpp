#include "ObjectGenerator.h"


//  2D Objects
RENDERER_API RenderObject* ObjectGenerator::createSquare(Shader &s, Vec3 &position) {
	GLfloat *verts = new GLfloat[12];
	GLuint *inds = new GLuint[6];
	int pos = 0;
	for(int y = 0; y < 2;++y) {
		for(int x = 0; x < 2; ++x) {
			verts[pos + 0] = (GLfloat)(x - 1);	// x
			verts[pos + 1] = (GLfloat)(y - 1);	// y
			verts[pos + 2] = (GLfloat)(0);		// z
			pos += 3;
		}
	}
	inds[0] = 0;
	inds[1] = 1;
	inds[2] = 2;

	inds[3] = 1;
	inds[4] = 4;
	inds[5] = 2;

	return new RenderObject(s,verts,12,inds,6);
}

RENDERER_API RenderObject* ObjectGenerator::createTriangle(Shader &s, Vec3 &position) {
	return 0;
}

RENDERER_API RenderObject* ObjectGenerator::createCircle(Shader &s, Vec3 &position, unsigned int radius) {
	return 0;
}

// 3D Objects
RENDERER_API RenderObject* ObjectGenerator::createCube(Shader &s, Vec3 &position) {
	return 0;
}

RENDERER_API RenderObject* ObjectGenerator::createPyramid(Shader &s, Vec3 &position) {
	return 0;
}

RENDERER_API RenderObject* ObjectGenerator::createSphere(Shader &s, Vec3 &position, unsigned int radius) {
	return 0;
}


