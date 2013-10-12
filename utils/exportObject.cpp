#include "exportObject.h"
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
void exportObject(const char *outfile, const int i, const int j, float **data, float **norms, const char *vshader, const char *fshader) {
   
   FILE * file = fopen(outfile, "wb");
   int vsSize = strlen(vshader);
   int fsSize = strlen(fshader);
   fwrite(&i, sizeof(int), 1, file);
   fwrite(&j, sizeof(int), 1, file);
   fwrite(&vsSize, sizeof(int), 1, file);
   fwrite(&fsSize, sizeof(int), 1, file);
   fwrite(&data[0][0], sizeof(float), i * j, file);
   fwrite(&norms[0][0], sizeof(float), i * j, file);
   fwrite(&vshader[0], sizeof(char), vsSize, file);
   fwrite(&fshader[0], sizeof(char), fsSize, file);
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
   fread(&fsSize, sizeof(int), 1, file);
   fread(&vsSize, sizeof(int), 1, file);

   dataSize = obj.i * obj.j * sizeof(float);

   // Allocate memory to hold the arrays for the data and normals   
   obj.data = (float**)malloc(obj.i * obj.j * sizeof(float *));
   obj.norms = (float**)malloc(obj.i * obj.j * sizeof(float *));

   // Allocate memory for each vertex and read in the data from the file
   for (int i=0;i<obj.i;++i) {
      obj.data[i] = (float *)calloc(obj.j, sizeof(float));
      fread(&obj.data[i][0], sizeof(float), obj.j, file);
   }

   // Allocate memory for each normal and read in the data from the file
   for (int i=0;i<obj.i;++i) {
      obj.norms[i] = (float *)calloc(obj.j, sizeof(float));
      fread(&obj.norms[i][0], sizeof(float), obj.j, file);
   }

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
   for (int i=0;i<obj.i;++i) {
      free(obj.data[i]);
      free(obj.norms[i]);
   }
   free(obj.data);
   free(obj.norms);
   free(obj.vShader);
   free(obj.fShader);
}

// Test
int main(void) {
   
    float **data = new float*[1000];
    float **norms = new float*[1000];
    const char *out = "./test.dat";
    const char *vshader = "./vshader.shd";
    const char *fshader = "./fshader.shd";
    for (int i=0; i< 1000; i++) {
       data[i] = new float[3];
       data[i][0] = 1;
       data[i][1] = 2;
       data[i][2] = 3;
    }
    for (int j=0; j< 1000; j++) {
       norms[j] = new float[3];
       norms[j][0] = 1;
       norms[j][1] = 2;
       norms[j][2] = 3;
    }
    exportObject(out,1000,3,data,norms,vshader,fshader);
    for (int i=0;i<1000;i++) {
       delete data[i];
       delete norms[i];
    }
    delete data;
    delete norms;
    SerializedObject obj = importObject("./test.dat");
    cout << "Number of vertices: " << obj.i << "\nNumber of dimensions per vertex: " << obj.j << "Data:\n\n";
    for (int i=0;i<obj.i;++i) {
       for (int j=0;j<obj.j;++j) {
           cout << obj.data[i][j] << " ";
       }
       cout << "\n";
    }
    freeObject(obj);
    return 0;
    
}
