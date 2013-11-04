#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define VALIDATE 0

#define IMG_FILE "depth.pgm"
#define MESH_FILE "mesh.dat"
#define NORM_FILE "norm.dat"

#define get(d,x,y) d[(x) + (y) * MESH_SIZE]

#define noise() (((2.0 * rand() * R[0]) / RAND_MAX) - R[0])

#define set(d,x,y,v) \
{ \
	get(d,x,y) = v + noise();\
	if( v < min ) { \
		min = v; \
	} \
	if( v > max ){ \
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
	
	norm[0] = norm[0] / div;
	norm[1] = norm[1] / div;
	norm[2] = norm[2] / div;
}

void main(int argc , char** args) {
	// Loop Index
	int i,j;

	// Temporary Vector
	float tmp_vec[3];
	
	// Used with normals
	float vec1[3];
	float vec2[3];
	float vec3[3];
	float norm[3];
	
	// Initial Corners
	float C[4];
	
	// Rougness parameters
	double R[2];
	
	
	float min = 1e32;
	float max = -1e32;

	
	// Check data
	if( argc != 8 ) {
		printf( "%s MESH_SIZE TL TR BL BR H DIV\n", args[0]);
		return;
	}
	
	//srand (time(NULL));
	
	int MESH_SIZE,MAX_SIZE,MAX_INDEX;
	
	// Size
	sscanf( args[1] , "%d" , &MESH_SIZE );	
	MAX_SIZE = MESH_SIZE * MESH_SIZE;
	MAX_INDEX = MESH_SIZE - 1;
	
	if( (MAX_INDEX - 1) & (MAX_INDEX ) ) {
		printf("MESH_SIZE should be a power of 2 plus 1.");
		return;
	}
	
	// Corners
	for( i = 0; i < 4 ; i++ ) {
		sscanf( args[2 + i] , "%f" , &C[i] );
	}
	// Rougness
	for( i = 0; i < 2 ; i++ ) {
		sscanf( args[6 + i] , "%lf" , &R[i] );
	}
	
	printf("Configuration : \n");
	printf("MESH_SIZE : %d\n" , MESH_SIZE );
	printf("MAX_SIZE : %d\n" , MAX_SIZE );
	printf("Corners : %0.5f , %0.5f , %0.5f , %0.5f \n" , C[0] , C[1] , C[2] , C[3] );
	printf("Roughness : %0.5lf , %0.5lf \n" , R[0] , R[1] );
	

	float* data = (float*)calloc( MAX_SIZE , sizeof(float) );
	if( !data || data == NULL) {
		printf("Could not allocate!");
		return;
	}
	
	// Initial points
	for( i = 0 ; i < 2 ;i++ ) {
		for( j = 0 ; j < 2 ; j++ ) {
			set( data , i * MAX_INDEX , j * MAX_INDEX  , C[j * 2 + i]);
		}
	}
	
	
	
	// Generate!
	int size = MAX_INDEX;
	while( size > 1) {
		int offset = size >> 1;
		// Square
		int x = 0,y;
		for( x = 0 ; x < MAX_INDEX ; x += size ) {
			for( y = 0 ; y < MAX_INDEX ; y += size ) {
				double comp = get(data, x , y ) + get( data , x + size , y ) + get( data , x , y + size ) + get( data , x + size , y + size );
				set( data , x + offset , y + offset , comp * 0.25 );
			}
		}
		// Diamond
		x = 0;
		for( x = 0; x <= MESH_SIZE ; x += offset ) {
			for( y = (x + offset) % size ; y < MESH_SIZE ; y += size ) {
				double comp = get( data , ( (x - offset) + MESH_SIZE) % MESH_SIZE , y );
				comp += get( data , (x + offset) % MESH_SIZE , y );
				comp += get( data , x , ((y - offset) + MESH_SIZE) % MESH_SIZE);
				comp += get( data , x , (y + offset) % MESH_SIZE);
				set( data , x , y , comp * 0.25 );
			}
		}
		size = offset;
		R[0] = R[0] / R[1];
	}
	
	printf("Max value : %f\n" , max);
	printf("Min value : %f\n" , min );

	// Output
	FILE* fp_mesh = NULL;
	FILE* fp_norm = NULL;
	FILE* fp_image = NULL;
	
	// Open and test for errors
	if( !(fp_mesh = fopen( MESH_FILE , "wb" )) ) {
		free( data );
		printf("Error with mesh file.\n");
		return;
	}else if( !(fp_norm = fopen( NORM_FILE , "wb" )) ) {
		fclose( fp_mesh );
		free( data );
		printf("Error with normal file.\n");
		return;
	}else if( !(fp_image = fopen( IMG_FILE , "wb" )) ) {
		fclose( fp_mesh );fclose( fp_norm );
		free( data );
		printf("Error with image file.\n");
		return;
	}
	fwrite( &MESH_SIZE , sizeof( int ) , 1 , fp_mesh );
	fwrite( &MESH_SIZE , sizeof( int ) , 1 , fp_norm );
	
	// Keep tally of what was written
	long long int mesh_written = sizeof(int);
	long long int norm_written = sizeof(int);
	long long int img_written = fprintf( fp_image , "P2 %d %d 255" , MESH_SIZE , MESH_SIZE );
	
	// Write the data
	for( i = 0 ; i < MESH_SIZE ; i++) {
		img_written += fprintf( fp_image , "\n" );
		for( j = 0 ; j < MESH_SIZE ; j++ ) {
			float t = (get( 
			data , i , j ) - min) / (max - min);			
			// Image
			img_written += fprintf( fp_image , "%3d " , (int)(t * 255) );		
			// Mesh
			fwrite( &t , sizeof(float), 1 , fp_mesh );
			mesh_written += sizeof(float);			
			// Normals
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
				norm_written += 3 * sizeof(float);
				
				// bottom and left
				vec2[0] = i - 1;
				vec2[1] = j;
				vec2[2] = get( data , i - 1, j);
				normal( vec1 , vec3 , vec2 , norm );
				
				fwrite( norm , sizeof(float), 3 , fp_norm );
				norm_written += 3 * sizeof(float);
			}
		}
	}
	fclose( fp_mesh );
	fclose( fp_norm );
	fclose( fp_image );
	
	printf("Wrote %lld bytes to %s \n"	, mesh_written , MESH_FILE );
	printf("Wrote %lld bytes to %s \n"	, norm_written , NORM_FILE );
	printf("Wrote %lld bytes to %s \n"	, img_written , IMG_FILE );
	
#if VALIDATE == 1
	// Validation Testing
	if( !(fp_mesh = fopen( MESH_FILE , "rb" )) ) {
		free( data );
		printf("Could not create the file.\n");
		return;
	}else if(!(fp_norm = fopen( NORM_FILE , "rb" ))) {
		fclose( fp_mesh );
		free( data );
		printf("Error with fp_norm\n");
		return;
	}
	
	fread( &size , sizeof(int) , 1 , fp_mesh );
	if( size != MESH_SIZE ) {
		printf("Mesh : Mesh Size");
	}
	fread( &size , sizeof(int) , 1 , fp_norm );
	if( size != MESH_SIZE ) {
		printf("Norm : Mesh Size");
	}	
	for( i = 0 ; i < MESH_SIZE ; i++ ) {
		for( j = 0 ; j < MESH_SIZE ; j++ ) {
			float t,x = (get( data , i , j ) - min) / (max - min);
			fread( &t , sizeof(float), 1 , fp_mesh );
			if( t != x ) {
				printf("t=%0.5f x=%0.5f\n" , t , x );
			}			
			if( i > 0 & j > 0) {
				// Top Left
				vec1[0] = i - 1;
				vec1[1] = j - 1;
				vec1[2] = get( data , i - 1, j - 1 );
				// Bottom Right
				vec3[0] = i;
				vec3[1] = j;
				vec3[2] = get( data , i , j );
				// Bottom and Left
				vec2[0] = i;
				vec2[1] = j - 1;
				vec2[2] = get( data , i , j - 1 );
				fread( tmp_vec , sizeof(float), 3 , fp_norm );
				normal( vec1 , vec2 , vec3 , norm );				
				if( norm[0] != tmp_vec[0] ) {
					printf("norm[0]=%0.5f vec[0]=%0.5f\n" , norm[0] , tmp_vec[0] );
				}				
				// Top and Right
				vec2[0] = i - 1;
				vec2[1] = j;
				vec2[2] = get( data , i - 1, j);
				normal( vec1 , vec3 , vec2 , norm );
				
				fread( tmp_vec , sizeof(float), 3 , fp_norm );
				
				if( norm[0] != tmp_vec[0] ) {
					printf("norm[0]=%0.5f vec[0]=%0.5f\n" , norm[0] , tmp_vec[0] );
				}
				
			}
		}
	}
	// Clean up
	fclose( fp_mesh );
	fclose( fp_norm );
#endif
	free( data );
	return;
}