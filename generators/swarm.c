#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void swarm(short*, int*, int, int);
void display(short*, int);
void smooth(short*, int);
void normalize(short*, int);
void out( const char* , short*, int );

short _min = 32000;
short _max = -32000;

#define max(x,y) (x)>(y)?(x):(y)
#define min(x,y) (x)<(y)?(x):(y)

#define set(d,p,v) { \
	d[p] = v; \
	if( v < _min ) _min = v; \
	if( v > _max ) _max = v; \
}

typedef struct {
	int mx;
	int my;
} Direction;

int main(int argc, char** argv) {
	int MESH_SIZE = 8;
	int NUM_PARTICLES = 64;
	int ITERATIONS = 10;
	int NUM_PEAKS = 3;

	sscanf(argv[1], "%d", &MESH_SIZE);
	sscanf(argv[2], "%d", &NUM_PARTICLES);
	sscanf(argv[3], "%d", &ITERATIONS);
	sscanf(argv[4], "%d", &NUM_PEAKS);

	short* grid = (short*)calloc(sizeof(short), MESH_SIZE * MESH_SIZE);
	//srand(time(NULL));
	srand(512);

	// Distribute the particles around the mesh
	int j = 0;
	while (j < NUM_PARTICLES) {
		int loc = rand() % (MESH_SIZE*MESH_SIZE);
		set( grid , loc , grid[loc] + 1 );
		++j;
	}

	// Generate random peak points
	int p = 0;
	int* peaks = (int*)calloc(sizeof(int), NUM_PEAKS);
	while (p < NUM_PEAKS) {
		peaks[p] = rand() % (MESH_SIZE*MESH_SIZE - 1);
		++p;
	}

	//display(grid, MESH_SIZE);

	int i = 0;
	while (i < ITERATIONS) {
		swarm(grid,peaks,MESH_SIZE,NUM_PEAKS);
		++i;
	}


	printf("Min: %d Max:%d\n" , _min , _max );
	normalize(grid, MESH_SIZE);
	out( "file.pgm" , grid, MESH_SIZE);

	free(grid);
	free(peaks);
	return 1;
}

void normalize(short* grid, int size) {
	int i = 0;
	while (i < size*size){
		grid[i] = (short)max( 0 , min( 255 , 255 * (grid[i] - _min) / (_max - _min) ) );
		++i;
	}
}

// Perform 1 iteration of particle swarm
void swarm(short* grid, int* peaks, int size, int numPeaks) {
	int i = 0;
	Direction * dir_vec = (Direction*)calloc(sizeof(Direction),size * size);
	while (i < size*size) {
		// Translate grid location to 2d
		int ix = i % size;
		int iy = (int)i / size;

		// Find the closest peak position
		int j = 0;
		int closest_px = 0;
		int closest_py = 0;
		float closest_dist = 1e16f;
		while (j < numPeaks) {
			int peak_pos = peaks[j];
			int px = peak_pos % size;
			int py = (int)peak_pos / size;
			float dist = sqrt(pow(ix - px, 2.f) + pow(iy - py, 2.f));
			if (dist < closest_dist) {
				closest_px += px;
				closest_py += py;
				closest_dist = dist;
			}
			++j;
		}

		dir_vec[i].mx = 0;
		dir_vec[i].my = 0;
		// Compute directional vector
		if (closest_px > ix) { // Right
			dir_vec[i].mx = 1;
		} else if (closest_px < ix) {
			dir_vec[i].mx = -1;
		}
		if (closest_py > iy) { // Right
			dir_vec[i].my = 1;
		}
		else if (closest_py < iy) {
			dir_vec[i].my = -1;
		}
		int r = rand();
		if (r % 3 == 0) {
			dir_vec[i].mx *= -1;
			dir_vec[i].my *= -1;
		}
		++i;
	}

	// Move particles
	int j = 0;
	while (j < size*size) {
		if ((dir_vec[j].mx != 0 || dir_vec[j].my != 0) && grid[j] > 0) {
			if (j + dir_vec[j].mx + dir_vec[j].my * size >= 0 && j + dir_vec[j].mx + dir_vec[j].my * size < size*size) {
				set(grid,j,grid[j]-1);
				set(grid, j + dir_vec[j].mx + dir_vec[j].my * size, grid[j + dir_vec[j].mx + dir_vec[j].my * size] + 1);
			}
		}
		++j;
	}
	free(dir_vec);
	return;
}

void out( const char* filename , short *data , int size ) {
	int i,j;
	FILE *fp;
	if( !(fp = fopen( filename , "w" ) ) ) {
		printf( "Image File Failure\n" );
		return;
	}
	printf( "%d\n" , size );
	fprintf( fp , "P2 %d %d 255" , size , size );

	for( i = 0 ; i < size ; i++ ) {
		fprintf( fp , "\n" );
		for( j = 0 ; j < size ; j++ ) {
			fprintf( fp , "%d " , data[i * size + j] );
		}
	}
	fclose( fp );
}

void display(short* grid, int size) {
	int i = 0;
	while (i < size*size) {
		if (i % size == 0) {
			printf("\n");
		}
		printf("%d ", grid[i]);
		++i;
	}
	printf("\n");
	return;
}
