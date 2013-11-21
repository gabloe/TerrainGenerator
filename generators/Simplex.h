#pragma once

#define NOISE_API _declspec( dllexport )

NOISE_API float simplex(float x);
NOISE_API float simplex2d(float x, float y);
NOISE_API float simplex3d(float x, float y, float z);