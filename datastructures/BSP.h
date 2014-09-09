#pragma once

#include "Def.h"

#include "../renderer/RenderObject.h"

DATASTRUCTS_API class BSP
{
private:

public:
	BSP();
	~BSP();

	void insert(RenderObject&);
	void collision(float, float) const;

};

