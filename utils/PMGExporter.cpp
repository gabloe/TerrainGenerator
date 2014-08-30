#include "def.h"

#define _CRT_SECRURE_NO_WARNINGS

#include <cstdio>

float _min(float a, float b) { return a < b ? a : b; }
float _max(float a, float b) { return a > b ? a : b; }

unsigned long long UTIL_API write(const char* filename, int width, int height, float* data, float min, float max) {
	int written = 0;
	FILE *fp;
	if (!(fopen_s(&fp,filename, "w"))) {
		printf("Error, could not open the file.\n");
	}
	else {
		written = fprintf(fp, "P2 %d %d 255", width, height);
		int pos = 0;
		while (pos < height * width) {
			if (pos % width == 0) {
				fprintf(fp, "\n");
			}
			int v = (int)(255 * _min(1, _max((data[pos] - min) / (max - min), 0)));
			written += fprintf(fp, "%3d ", v);
			pos++;
		}
		fclose(fp);
	}
	return written;
}