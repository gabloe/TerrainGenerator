#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

float min = 1e32;
float max = -1e32;

#define SQR 3


#define get(d,x,y) d[(x) + (y) * MESH_SIZE]

#define noise() (((2 * rand() * h) / RAND_MAX) - h)

#define set(d,x,y,v) \
{ \
	get(d,x,y) = v + noise();\
	if( v < min ) { \
		min = v; \
	}else if( v > max ){ \
		max = v;\
	} \
}

#define max(x,y) ((x)>(y))?(x):(y)
#define min(x,y) ((x)<(y))?(x):(y)

void normal( float* vec1 , float* vec2 , float* vec3 , float* norm ) {
	float ux,uy,uz;
	ux = vec2[0] - vec1[0];
	uy = vec2[1] - vec1[1];
	uz = vec2[2] - vec1[2];
	
	float vx,vy,vz;
	vx = vec3[0] - vec1[0];
	vy = vec3[1] - vec1[1];
	vz = vec3[2] - vec1[2];
	
	norm[0] = uy * vz - uz * vy;
	norm[1] = uz * vx - ux * vz;
	norm[2] = ux * vy - uy * vx;
	
	float div = sqrt( norm[0] * norm[0] + norm[1]*norm[1] + norm[2]*norm[2] );
	
	//*
	norm[0] = norm[0] / div;
	norm[1] = norm[1] / div;
	norm[2] = norm[2] / div;
	// */
//	printf("x=%0.5f y=%0.5f z=%0.5f\n" , norm[0] , norm[1] , norm[2] );
	
}

void main(int argc , char** args) {
	if( argc != 7 ) {
		printf( "%s MESH_SIZE v1 v2 v3 v4 DIV\n", args[0]);
		return;
	}
	double h = 200;
	double DIV = 2;
	int MESH_SIZE,MAX_SIZE;
	
	sscanf( args[1] , "%d" , &MESH_SIZE );
	
	float v1,v2,v3,v4;
	sscanf( args[2] , "%f" , &v1 );
	sscanf( args[3] , "%f" , &v2 );
	sscanf( args[4] , "%f" , &v3 );
	sscanf( args[5] , "%f" , &v4 );
	
	sscanf( args[6] , "%lf" , &DIV );
	
	MAX_SIZE = MESH_SIZE * MESH_SIZE;
	
	printf("Configuration : \n");
	printf("MESH_SIZE : %d\n" , MESH_SIZE );
	printf("MAX_SIZE : %d\n" , MAX_SIZE );
	srand (time(NULL));

	float* data = (float*)calloc( MAX_SIZE , sizeof(float) );
	if( data == NULL ) {
		printf("Could not allocate!");
		return;
	}
	
	if( !data ) {
		printf("Could not allocate!");
		return;
	}
	// Initial points
	set( data , 0 , 0, v1 );
	set( data , MESH_SIZE - 1 , 0, v2 );
	set( data , 0 , MESH_SIZE - 1 , v3 );
	set( data , MESH_SIZE - 1 , MESH_SIZE - 1 , v4 );
	
	// Generate!
	int size = MESH_SIZE - 1;
	while( size > 1) {
		int offset = size >> 1;
		// printf("Size = %d Offset = %d\n" , size , offset );
		
		// Square
		int x = 0;
		while( x < MESH_SIZE - 1  ) {
			int y = 0;
			while( y < MESH_SIZE - 1 ) {
				double comp = get(data, x , y ) + get( data , x + size , y ) + get( data , x , y + size ) + get( data , x + size , y + size );
				set( data , x + offset , y + offset , comp * 0.25 );
				y += size;
			}
			x += size;
		}
		
		
		// Inner diamond
		x = 0;
		int i = 0;
		while( x < (MESH_SIZE - 1) ) {
			int y = (x + offset) % size;
			while( y < (MESH_SIZE - 1)) {
				
				double comp = get( data , (x - offset + MESH_SIZE) % MESH_SIZE , y );
				comp += get( data , (x + offset) % MESH_SIZE , y );
				comp += get( data , x , (y - offset + MESH_SIZE) % MESH_SIZE);
				comp += get( data , x , (y + offset) % MESH_SIZE);
				set( data , x , y , comp * 0.25 );
				y += size;
			}
			x += offset;
		}
		
		size = offset;
		h = h / DIV;
	}
	
	// Here
	FILE* fp = NULL;
	FILE* fp_norm = NULL;
	
	fp = fopen( "mesh" , "wb" );
	if( !fp ) {
		free( data );
		printf("Could not create the file.\n");
		return;
	}
	
	fp_norm = fopen( "norm" , "wb" );
	if( !fp_norm ) {
		fclose( fp );
		free( data );
		printf("Error with fp_norm\n");
		return;
	}
//	fprintf( fp , "P2 %d %d 255" , MESH_SIZE , MESH_SIZE );
	
	float read_vec[3];
	
	float vec1[3];
	float vec2[3];
	float vec3[3];
	float norm[3];
	
	fwrite( &MESH_SIZE , sizeof( int ) , 1 , fp );
	fwrite( &MESH_SIZE , sizeof( int ) , 1 , fp_norm );
	
	unsigned long long written = sizeof(int);
	
	int i = 0;
	while( i < MESH_SIZE) {
		int j = 0;
		while( j < MESH_SIZE ) {
			float t = (get( data , i , j ) - min) / (max - min);
			fwrite( &t , sizeof(float), 1 , fp );
			written += sizeof(float);
			
			// I know I have completed at least one block
			if( i > 0 & j > 0) {
				vec1[0] = i - 1;
				vec1[1] = j - 1;
				vec1[2] = get( data , i - 1, j - 1 );
				
				vec3[0] = i;
				vec3[1] = j;
				vec3[2] = get( data , i , j );
				
				// Top and right
				vec2[0] = i;
				vec2[1] = j - 1;
				vec2[2] = get( data , i , j - 1 );
				
				normal( vec1 , vec2 , vec3 , norm );
				
				fwrite( norm , sizeof(float), 3 , fp_norm );
				
				// bottom and left
				vec2[0] = i - 1;
				vec2[1] = j;
				vec2[2] = get( data , i - 1, j);
				normal( vec1 , vec3 , vec2 , norm );
				
				fwrite( norm , sizeof(float), 3 , fp_norm );
			}
			j++;
		}
		i++;
	}
	
	printf("Wrote %u bytes to file." , written );
	
	fclose( fp );
	fclose( fp_norm );
	
	fp = fopen( "mesh" , "rb" );
	if( !fp ) {
		free( data );
		printf("Could not create the file.\n");
		return;
	}
	
	fp_norm = fopen( "norm" , "rb" );
	if( !fp_norm ) {
		fclose( fp );
		free( data );
		printf("Error with fp_norm\n");
		return;
	}
	
	fread( &size , sizeof(int) , 1 , fp );
	if( size != MESH_SIZE ) {
		printf("Mesh : Mesh Size");
	}
	fread( &size , sizeof(int) , 1 , fp_norm );
	if( size != MESH_SIZE ) {
		printf("Norm : Mesh Size");
	}
	
	i = 0;
	while( i < MESH_SIZE) {
		int j = 0;
		while( j < MESH_SIZE ) {
			float x = (get( data , i , j ) - min) / (max - min);
			float t = 0;
			fread( &t , sizeof(float), 1 , fp );
			
			if( t != x ) {
				printf("t=%0.5f x=%0.5f\n" , t , x );
			}
			
			// I know I have completed at least one block
			if( i > 0 & j > 0) {
			
				vec1[0] = i - 1;
				vec1[1] = j - 1;
				vec1[2] = get( data , i - 1, j - 1 );
				
				vec3[0] = i;
				vec3[1] = j;
				vec3[2] = get( data , i , j );
				
				// Top and right
				vec2[0] = i;
				vec2[1] = j - 1;
				vec2[2] = get( data , i , j - 1 );
				
				fread( read_vec , sizeof(float), 3 , fp_norm );
				normal( vec1 , vec2 , vec3 , norm );
				
				if( norm[0] != read_vec[0] ) {
					printf("norm[0]=%0.5f vec[0]=%0.5f\n" , norm[0] , read_vec[0] );
				}
				
				// bottom and left
				vec2[0] = i - 1;
				vec2[1] = j;
				vec2[2] = get( data , i - 1, j);
				normal( vec1 , vec3 , vec2 , norm );
				
				fread( read_vec , sizeof(float), 3 , fp_norm );
				
				if( norm[0] != read_vec[0] ) {
					printf("norm[0]=%0.5f vec[0]=%0.5f\n" , norm[0] , read_vec[0] );
				}
				
			}
			j++;
		}
		i++;
	}
	
	fclose( fp );
	fclose( fp_norm );
	free( data );
	
	return;
}