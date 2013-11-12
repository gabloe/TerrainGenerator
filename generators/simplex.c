#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define F2 0.5*(sqrt(3.0)-1.0)
#define G2 (3.0-sqrt(3.0))/6.0
#define F3 1.0/3.0
#define G3 1.0/6.0
#define F4 (sqrt(5.0)-1.0)/4.0
#define G4 (5.0-sqrt(5.0))/20.0

typedef struct {
   double x, y, z;
} Grad;

double max = -32000;
double min = 32000;

#ifndef _max
#define _max(x,y) (x)>(y)?(x):(y)
#define _min(x,y) (x)<(y)?(x):(y)
#endif

double noise2D(double, double, short*, short*, Grad*);
double noise3D(double, double, double, short*, short*, Grad*);

short* p;
short* perm;
short* permMod12;

// Writes the data to a file.
void _write(const char* fname, double *data, short MESH_SIZE) {
   FILE* fp_img = NULL;
   if (!(fp_img = fopen(fname, "wt"))) {
      printf("Error with mesh file.\n");
      return;
   }
   long long int img_written = fprintf(fp_img, "P2 %d %d 255", MESH_SIZE, MESH_SIZE);
   for (int i = 0; i < MESH_SIZE; i++) {
      img_written += fprintf(fp_img, "\n");
      for (int j = 0; j < MESH_SIZE; j++) {
         double t = (data[i + (j*MESH_SIZE)] - min)/(max-min);
         img_written += fprintf(fp_img, "%3d ", _max(0,_min(255,(int)(t * 255))));
      }
   }
   fclose(fp_img);
}

// Returns 1 if a value is found within a particular array.  0 otherwise.
int in(short* arr, short val, int size) {
   int i=0;
   while(i<size) {
      if (arr[i] == val) {
         return 1;
      }
      ++i;
   }
   return 0;
}

// Generates all values between low and high randomly, given a particular seed value for srand.
short* genPermutations(int low, int high, int seed) {
   int range = high - low;
   short* p = (short*) calloc(sizeof(short),range);
   int i = 1;
   srand(seed);
   short v = rand() % range;
   p[0] = v;
   while (i<range) {
      v = rand() % range;
      while (in(p,v,i) == 1) {
         v = rand() % range;
      }
      p[i] = v;
      ++i;
   }
   return p;
}

// Floor function.  Faster than the standard math library.  Specified by Ken Perlin.
int fastFloor(double x) {
   int xx = (int)x;
   return x<xx ? xx-1 : xx;
}

// Dot product functions for 2d and 3d, respectively.
double dot2(Grad g, double x, double y) { return g.x*x + g.y*y; }
double dot3(Grad g, double x, double y, double z) { return g.x*x + g.y*y + g.z*z; }

// Generates a raw simplex noise value at a particular <x,y,z> coordinate.  Remember, this value is not scaled.
double raw3D(double x, double y, double z, short* perm, short* permMod12, Grad* gradients) {
    double n0, n1, n2, n3;
    double s = (x+y+z)*F3;
    int i = fastFloor(x+s);
    int j = fastFloor(y+s);
    int k = fastFloor(z+s);
    double t = (i+j+k)*G3;
    double X0 = i-t;
    double Y0 = j-t;
    double Z0 = k-t;
    double x0 = x-X0;
    double y0 = y-Y0;
    double z0 = z-Z0;
    int i1, j1, k1;
    int i2, j2, k2;

    if(x0>=y0) {
      if(y0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; }
        else if(x0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; }
        else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; }
      }
    else {
      if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; }
      else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; }
      else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; }
    }
    double x1 = x0 - i1 + G3;
    double y1 = y0 - j1 + G3;
    double z1 = z0 - k1 + G3;
    double x2 = x0 - i2 + 2.0*G3;
    double y2 = y0 - j2 + 2.0*G3;
    double z2 = z0 - k2 + 2.0*G3;
    double x3 = x0 - 1.0 + 3.0*G3;
    double y3 = y0 - 1.0 + 3.0*G3;
    double z3 = z0 - 1.0 + 3.0*G3;
    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int gi0 = permMod12[ii+perm[jj+perm[kk]]];
    int gi1 = permMod12[ii+i1+perm[jj+j1+perm[kk+k1]]];
    int gi2 = permMod12[ii+i2+perm[jj+j2+perm[kk+k2]]];
    int gi3 = permMod12[ii+1+perm[jj+1+perm[kk+1]]];
    double t0 = 0.6 - x0*x0 - y0*y0 - z0*z0;
    if(t0<0) n0 = 0.0;
    else {
      t0 *= t0;
      n0 = t0 * t0 * dot3(gradients[gi0], x0, y0, z0);
    }
    double t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
    if(t1<0) n1 = 0.0;
    else {
      t1 *= t1;
      n1 = t1 * t1 * dot3(gradients[gi1], x1, y1, z1);
    }
    double t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
    if(t2<0) n2 = 0.0;
    else {
      t2 *= t2;
      n2 = t2 * t2 * dot3(gradients[gi2], x2, y2, z2);
    }
    double t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
    if(t3<0) n3 = 0.0;
    else {
      t3 *= t3;
      n3 = t3 * t3 * dot3(gradients[gi3], x3, y3, z3);
    }
    return 32.0*(n0 + n1 + n2 + n3);
}

// Generates a raw simplex noise value at a particular <x,y> coordinate.  Remember, this value is not scaled.
double raw2D(double x, double y, short* perm, short* permMod12, Grad* gradients) {
   double n0,n1,n2;
   double s = (x+y)*F2;
   int i = fastFloor(x+s);
   int j = fastFloor(y+s);
   double t = (i+j)*G2;
   double X0 = i-t;
   double Y0 = j-t;
   double x0 = x-X0;
   double y0 = y-Y0;
   int i1, j1;
   if (x0>y0) {i1=1;j1=0;}
   else {i1=0;j1=1;}
   double x1 = x0 - i1 + G2;
   double y1 = y0 - j1 + G2;
   double x2 = x0 - 1.0 + 2.0 * G2;
   double y2 = y0 - 1.0 + 2.0 * G2;
   int ii = i & 255;
   int jj = j & 255;
   int gi0 = permMod12[ii+perm[jj]];
   int gi1 = permMod12[ii+i1+perm[jj+j1]];
   int gi2 = permMod12[ii+1+perm[jj+1]];
   double t0 = 0.5 - x0*x0-y0*y0;
   if(t0<0) n0 = 0.0;
   else {
      t0 *= t0;
      n0 = t0 * t0 * dot2(gradients[gi0], x0, y0);
   }
   double t1 = 0.5 - x1*x1-y1*y1;
   if(t1<0) n1 = 0.0;
   else {
      t1 *= t1;
      n1 = t1 * t1 * dot2(gradients[gi1], x1, y1);
   }
   double t2 = 0.5 - x2*x2-y2*y2;
   if(t2<0) n2 = 0.0;
   else {
      t2 *= t2;
      n2 = t2 * t2 * dot2(gradients[gi2], x2, y2);
   }
   return 70.0 * (n0 + n1 + n2);
}

// Generates the 2d noise value at a particular <x,y> coordinate.
// Each octave doubles the frequency of the previous octave.  Higher octaves increase the level of detail, but have worse performance.
// Persistence scales the frequency of the wave function.
double noise2D(double x, double y, short* perm, short* permMod12, Grad* gradients, int octaves, float persistence) {
   double total = 0;
   for (int i=0;i<octaves;++i) {
      double freq = pow(2.0,i);
      double amp = pow(persistence,i);
      total += raw2D(x*freq,y*freq,perm,permMod12,gradients) * amp;
   }
   if (total > max) {
	   max = total;
   }
   else if (total < min) {
	   min = total;
   }
   return total;
}

// Generates the 3d noise value at a particular <x,y,z> coordinate.
// Each octave doubles the frequency of the previous octave.  Higher octaves increase the level of detail, but have worse performance.
// Persistence scales the frequency of the wave function.
double noise3D(double x, double y, double z, short* perm, short* permMod12, Grad* gradients, int octaves, float persistence) {
   double total = 0;
   for (int i=0;i<octaves;++i) {
      double freq = pow(2.0,i);
      double amp = pow(persistence,i);
      total += raw3D(x*freq,y*freq,z*freq,perm,permMod12,gradients) * amp;
   }
   if (total > max) {
	   max = total;
   }
   else if (total < min) {
	   min = total;
   }
   return total;
}

// Produces 2d simplex noise (square of noise)
double* gen2DNoise(int mesh_size,float point_dist,short* perm,short* permMod12,Grad* gradients,int octaves,float pers) {
   double* narr = (double*)malloc(mesh_size*mesh_size*sizeof(double));
   for (int i=0;i<mesh_size;++i) {
      for (int j=0;j<mesh_size;++j) {
	  narr[i + j*mesh_size] = noise2D(i*point_dist, j*point_dist, perm, permMod12, gradients,octaves,pers);
      }
   }
   return narr;
}

// Produces 3-dimensional simplex noise (cube of noise)
double* gen3DNoise(int mesh_size,float point_dist,short* perm,short* permMod12,Grad* gradients,int octaves,float pers) {
   double* narr = (double*)malloc(mesh_size*mesh_size*mesh_size*sizeof(double));
   for (int i=0;i<mesh_size;++i) {
      for (int j=0;j<mesh_size;++j) {
         for (int k=0;k<mesh_size;++k) {
	        narr[i + mesh_size * (j + mesh_size * k)] = noise3D(i*point_dist, j*point_dist, k*point_dist, perm, permMod12, gradients, octaves, pers);
         }
      }
   }
   return narr;
}

double* select2D(double* first, double* second, double* control, int mesh_size) {
	double* out = (double*)calloc(sizeof(double),mesh_size * mesh_size);
	for (int i = 0; i < mesh_size * mesh_size; ++i){
		if (control[i] >= 0.5) out[i] = second[i];
		else out[i] = first[i];
		if (out[i] > max) {
			max = out[i];
		}
		else if (out[i] < min) {
			min = out[i];
		}
	}
	return out;
}

double* add2D(double* first, double* second, int mesh_size) {
	double* out = (double*)calloc(sizeof(double), mesh_size * mesh_size);
	for (int i = 0; i < mesh_size * mesh_size; ++i){
		out[i] = first[i] + second[i];
		if (out[i] > max) {
			max = out[i];
		}
		else if (out[i] < min) {
			min = out[i];
		}
	}
	return out;
}

double* max2D(double* first, double* second, int mesh_size) {
	double* out = (double*)calloc(sizeof(double), mesh_size * mesh_size);
	for (int i = 0; i < mesh_size * mesh_size; ++i){
		out[i] = _max(first[i], second[i]);
		if (out[i] > max) {
			max = out[i];
		}
		else if (out[i] < min) {
			min = out[i];
		}
	}
	return out;
}

double* mult2D(double* first, double* second, int mesh_size) {
	double* out = (double*)calloc(sizeof(double), mesh_size * mesh_size);
	for (int i = 0; i < mesh_size * mesh_size; ++i){
		out[i] = first[i]*second[i];
		if (out[i] > max) {
			max = out[i];
		}
		else if (out[i] < min) {
			min = out[i];
		}
	}
	return out;
}

void generatePermutations(int seed) {
	if (p) { free(p); }
	if (perm) { free(perm); }
	if (permMod12) { free(permMod12); }
	p = genPermutations(0, 256, seed);
	perm = (short*)calloc(sizeof(short), 512);
	permMod12 = (short*)calloc(sizeof(short), 512);
	// Generate permutations from 0 to 255 inclusive.  Copy them twice for wrapping.
	for (int i = 0; i<512; ++i) {
		perm[i] = p[i & 255];
		permMod12[i] = (short)(perm[i] % 12);
	}
}

int main(int argc, char** argv) {
   int MESH_SIZE = 64;
   int OCTAVES = 5;
   float PERS = 0.5f;
   float POINT_DIST = 0.005f;
   int DIMENSIONS = 2;
   int SEED = (int)time(NULL);
   if( argc != 7 ) {
      printf( "%s MESH_SIZE OCTAVES PERSISTENCE POINTDIST DIMENSIONS SEED\n", argv[0]);
      return 0;
   }
   sscanf(argv[1],"%d",&MESH_SIZE );
   sscanf(argv[2],"%d",&OCTAVES);
   sscanf(argv[3],"%f",&PERS );
   sscanf(argv[4],"%f",&POINT_DIST );
   sscanf(argv[5],"%d",&DIMENSIONS );
   sscanf(argv[6],"%d",&SEED);

   printf("Description:\n");
   printf("OCTAVES - Scales the level of detail, 1 being worst detail and 5 being best detail.\n");
   printf("PERSISTENCE - Scales the amplitude of the wave function.\n");
   printf("POINTDIST - Scales the distance between point samples, essentially zooms in or out.\n");
   printf("DIMENSIONS - Number of dimensions of data (2 or 3).\n");
   printf("SEED - Seed for the random number generator.\n\n");
   printf("Configuration:\n");
   printf("OCTAVES - %d.\n",OCTAVES);
   printf("PERSISTENCE - %3f.\n",PERS);
   printf("POINTDIST - %3f.\n",POINT_DIST);
   printf("DIMENSIONS - %d.\n", DIMENSIONS);
   printf("SEED - %d.\n\n", SEED);

   printf("Generating permutations.\n", POINT_DIST);
   generatePermutations(SEED);
   printf("done.\n\n", POINT_DIST);

   printf("Computing gradients.\n", POINT_DIST);
   Grad* gradients3D = (Grad*)malloc(12 * sizeof(Grad));
   // Generate gradients
   int cnt = 0;
   for (int i=-1;i<2;i+=2) {
      for (int j=-1;j<2;j+=2) {
         gradients3D[cnt].x = (double)i;
	 gradients3D[cnt].y = (double)j;
         gradients3D[cnt++].z=0.0;
      }
   }
   for (int i=-1;i<2;i+=2) {
      for (int j=-1;j<2;j+=2) {
	 gradients3D[cnt].x = (double)i;
	 gradients3D[cnt].z = (double)j;
         gradients3D[cnt++].y=0.0;
      }
   }
   for (int i=-1;i<2;i+=2) {
      for (int j=-1;j<2;j+=2) {
         gradients3D[cnt].y=(double)i;
	 gradients3D[cnt].z = (double)j;
         gradients3D[cnt++].x=0.0;
      }
   }
   printf("done.\n\n", POINT_DIST);
   printf("Generating simplex mesh.\n", POINT_DIST);
   double* narr = NULL;
   double* narrOne = NULL;
   double* narrTwo = NULL;
   if (DIMENSIONS == 2) {
	  narr = gen2DNoise(MESH_SIZE, POINT_DIST, perm, permMod12, gradients3D, OCTAVES, PERS);
      printf("done.\n\n", POINT_DIST);
      printf("Writing output file.\n");
	  _write("out.pgm", narr, MESH_SIZE);
      printf("done.\n");
   } else if (DIMENSIONS == 3) {
      narr = gen3DNoise(MESH_SIZE,POINT_DIST,perm,permMod12,gradients3D,OCTAVES,PERS);
      printf("done.\n\n", POINT_DIST);
      printf("What format do we output 3D noise? :/\n");
      //_write("out.pgm",narr,MESH_SIZE);
      printf("done.\n", POINT_DIST);
   } else {
     printf("We do not support %d dimensions at the point.", DIMENSIONS);
   }
   if (narr) {
      free(narr);
   }
   if (narrOne) {
	   free(narrOne);
   }
   if (narrTwo) {
	   free(narrTwo);
   }
   if (p) {
	   free(p);
   }
   if (gradients3D) {
	   free(gradients3D);
   }
   if (perm) {
	   free(perm);
   }
   if (permMod12) {
	   free(permMod12);
   }
   return 0;
}
