#pragma once

#define NOISE_API _declspec( dllexport )

NOISE_API float perlin(float x);
NOISE_API float perlin2d(float x, float y);
NOISE_API float perlin3d(float x, float y, float z);