#include "ExportObject.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/*
   Serializes an object with vertices, normals, and shaders.
   outfile - Filename of resultant serialization
   i - Number of vertices
   j - Number of fields per vertex
   data - vertices
   norms - normals
   vshader - filename for vertex shader
   fshader - filename for fragment shader
*/
void exportObject(const char *outfile, const int i, const int j, float *data, float *norms, const char *vshader, const char *fshader) {
  
   FILE * file = fopen(outfile, "wb");

   // Get string lengths 
   int vsSize = strlen(vshader);
   int fsSize = strlen(fshader);
   
   // i is the data length
   fwrite( &i, sizeof(int), 1, file);
   fwrite( &j, sizeof(int), 1, file);
   fwrite( &vsSize, sizeof(int), 1, file);
   fwrite( &fsSize, sizeof(int), 1, file);
   fwrite( &data[0], sizeof(float), i * j, file);
   fwrite( &norms[0], sizeof(float), i * j, file);
   fwrite( &vshader[0], sizeof(char), vsSize, file);
   fwrite( &fshader[0], sizeof(char), fsSize, file);
   fclose(file);

}

SerializedObject importObject(const char *infile) {

   SerializedObject obj;
   FILE * file = fopen(infile, "rb");
   int dataSize;
   int vsSize;
   int fsSize;

   // Read the header
   fread(&obj.i, sizeof(int), 1, file);
   fread(&obj.j, sizeof(int), 1, file);
   fread(&vsSize, sizeof(int), 1, file);
   fread(&fsSize, sizeof(int), 1, file);

   dataSize = obj.i * obj.j * sizeof(float);

   // Allocate memory to hold the arrays for the data and normals   
   obj.data = (float*)malloc(dataSize);
   obj.norms = (float*)malloc(dataSize);

   fread(&obj.data[0], sizeof(float) , obj.j * obj.i, file);
   fread(&obj.norms[0], sizeof(float) , obj.j * obj.i, file);
   
   // Allocate memory for filenames
   obj.vShader = (char *)malloc(vsSize * sizeof(char));
   obj.fShader = (char *)malloc(fsSize * sizeof(char));

   // Read filenames
   fread(&obj.vShader[0], sizeof(char), vsSize, file);
   fread(&obj.fShader[0], sizeof(char), fsSize, file);

   fclose(file);

   return obj;

}

void freeObject(SerializedObject obj) {
   free(obj.data);
   free(obj.norms);
   free(obj.vShader);
   free(obj.fShader);
}
