#include "BSP.h"

typedef struct {
	BSP_NODE *left, *right;
	RenderObject *obj;
} BSP_NODE;

BSP_NODE head;

BSP::BSP()
{
}


BSP::~BSP()
{
}

void BSP::insert(RenderObject &obj) {
	if (head.obj == 0) {
		head.obj = &obj;
		return;
	}
}
