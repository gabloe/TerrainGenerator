#pragma once

#include <cstdio>

unsigned long long write( const char* filename , int width , int height , float* data , float min , float max ) {
	int written = 0;
	FILE *fp;
	if( !(fp = fopen( filename , "w" ) ) ) {
		printf("Error, could not open the file.\n");
	}else {
		written = fprintf( fp , "P2 %d %d 255" , width , height );
		int pos = 0;
		while( pos < height * width ) {
			if( pos % width == 0 ) { 
				fprintf( fp , "\n" );
			}
			int v = 255 * min( 1 , max( (data[pos] - min) / (max - min) , 0 ) );
			written += fprintf( fp , "%3d " , v );
			pos++;
		}
		fclose( fp );
	}
	return written;
}