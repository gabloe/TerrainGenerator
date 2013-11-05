#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "my_random.h"

typedef struct Point {
	int x;
	int y;
	int dirt;
	short dx;
	short dy;
}  Point;


void swarm(Point*, int,int);
void display(short*, int);
void smooth(short*, int);
void normalize(short*, int , short , short);
void out( const char* , short*, int , short , short );

#define max(x,y) (x)>(y)?(x):(y)
#define min(x,y) (x)<(y)?(x):(y)
#define set(d,p,v) d[p] = (v)

#define sgn(x) (x)<0?-1:((x)==0?0:1);

#if CMWC == 1
#define MAX_RAND 0xEFFFFFFF
#define my_rand() rand_cmwc()
#else
#define MAX_RAND RAND_MAX
#define my_rand() rand()
#endif

#define uniform(low,high) (int)(low + high * ( my_rand() / (double)MAX_RAND ) )

int main(int argc, char** argv) {
	srand( time( 0 ) );
	init_rand( rand() );
	
	// Index variables
	int j;

	// Used for normalization
	short _min = 32000;
	short _max = -32000;

	// Configuration Parameters
	int MESH_SIZE,NUM_PARTICLES,ITERATIONS,NUM_PEAKS,MAX_SIZE,MAX_INDEX;

	// Read in the paramaters
	sscanf(argv[1], "%d", &MESH_SIZE);
	sscanf(argv[2], "%d", &NUM_PARTICLES);
	sscanf(argv[3], "%d", &ITERATIONS);
	sscanf(argv[4], "%d", &NUM_PEAKS);
	
	MAX_SIZE = MESH_SIZE * MESH_SIZE;
	MAX_INDEX = MAX_SIZE - 1;

	// Print Configuration
	printf( "Mesh Size : %d\n", MESH_SIZE);
	printf( "Number of Particles : %d\n", NUM_PARTICLES);
	printf( "Iterations : %d\n", ITERATIONS);
	printf( "Number of Peaks : %d\n", NUM_PEAKS);
	
	// Generate peaks
	int* peaks = (int*)calloc(sizeof(int), NUM_PEAKS);
	for( j = 0 ; j < NUM_PEAKS ; j++ ) {
		peaks[j] = (int)uniform(0 , MAX_INDEX );
		printf( "Peak Position : %d\n" , peaks[j] );
	}
	
	int *count = (int*)calloc( sizeof(int) , NUM_PEAKS ); 
	
	// Generate particles
	Point* particles = (Point*)calloc( sizeof( Point ) , NUM_PARTICLES );
	for( j = 0 ; j < NUM_PARTICLES ; j++ ) {
		int p = (int)uniform( 0 , NUM_PEAKS - 1 );
		count[p]++;
		int l = (int)uniform( 0 , MAX_INDEX );
		particles[j].x = l % MESH_SIZE;
		particles[j].y = l / MESH_SIZE;
		particles[j].dirt = uniform( 0 , 50 );
		particles[j].dx = peaks[p] % MESH_SIZE;
		particles[j].dy = peaks[p] / MESH_SIZE;
	}
	for( j = 0 ; j < NUM_PEAKS ; j++ ) {
		printf( "%d : %d\n" , j , count[j] );
	}
	free( count );
	
	// Run!
	for ( j = 0 ; j < ITERATIONS ; j++ ) {
		swarm(particles,NUM_PARTICLES,MESH_SIZE);
	}
	
	// Copy over particles to grid	
	short* grid = (short*)calloc( sizeof(short) , MAX_SIZE );	
	for( j = 0 ; j < NUM_PARTICLES ; j++ ) {
		int pos = particles[j].x + particles[j].y * MESH_SIZE;
		if( pos > MAX_SIZE ) {
			printf("Error : %d\n" , pos );
		}
		grid[pos]++;
	}free( particles );
	
	int max_ind = -1;
	for( j = 0 ; j < MAX_SIZE ; j++ ) {
		if( grid[j] < _min ) _min = grid[j];
		if( grid[j] > _max ) {max_ind = j;_max = grid[j];}
	}
	printf("Min: %d Max:%d\n" , _min , _max );	
	printf("x: %d y: %d\n" , max_ind % MESH_SIZE , max_ind / MESH_SIZE );
	
	out( "file.pgm" , grid, MESH_SIZE , _min , _max );

	free(grid);
	free(peaks);
	return 1;
}

// Instead of swarm
void swarm(Point* par, int points , int size ) {
	int i = 0;
	for( i = 0 ; i <  points ; i++ ) {
		int r = my_rand() % 3;
		if( r == 1 ) {
			par[i].x += sgn( par[i].dx - par[i].x );
		}else {
			par[i].y += sgn( par[i].dy - par[i].y );
		}
		
		//par[i].x += uniform( -1 , 1 );
		//par[i].y += uniform( -1 , 1 );
		
		par[i].x = min( size - 1, max( 0 , par[i].x ) );
		par[i].y = min( size - 1, max( 0 , par[i].y ) );
	}
}

void out( const char* filename , short *data , int size , short _min , short _max  ) {
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
			fprintf( fp , "%d " , min( 255 , max( 0 , (255  * (data[i * size + j] - _min) ) / (_max - _min ) ) ) );
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
