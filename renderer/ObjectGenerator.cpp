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
	return 0;
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


