#pragma once

#include "def.h"

typedef struct {
	double x, y, z;
} Grad;


NOISE_API void init_simplex(int);
NOISE_API float simplex(float x);
NOISE_API float simplex2d(float x, float y, int octaves, float persistence);
NOISE_API float simplex3d(float x, float y, float z, int octaves, float persistence);