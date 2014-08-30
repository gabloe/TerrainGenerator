#include "ObjectGenerator.h"


ObjectGenerator::ObjectGenerator()
{
}


ObjectGenerator::~ObjectGenerator()
{
}

RenderObject* createAndSet(GLfloat* vert , int num_v , GLfloat* norm , int num_n , GLuint* indices , int num_i , Vec3 pos) {
	RenderObject* res = new RenderObject();
	if (res) {

	}
	return res;
}

//  2D Objects
RenderObject* ObjectGenerator::createSquare(Vec3 position) {
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

	RenderObject *ret = new RenderObject();
	ret->setVertices(verts,12);
	ret->setIndices(inds,6);
	ret->setPosition(position);
	ret->setMode(GL_TRIANGLES);

	return ret;
}
RenderObject* ObjectGenerator::createTriangle(Vec3 position) {
	return 0;
}

RenderObject* ObjectGenerator::createCircle(Vec3 position, unsigned int detail) {
	RenderObject* res = new RenderObject();
	if (res) {

	}
	return res;
}

// 3D Objects
RenderObject* ObjectGenerator::createCube(Vec3 position) {
	RenderObject* res = new RenderObject();
	if (res) {

	}
	return res;
}

RenderObject* ObjectGenerator::createPyramid(Vec3 position) {
	return 0;
}

RenderObject* ObjectGenerator::createSphere(Vec3 position, unsigned int detail) {
	return 0;
}


