#include <iostream>
#include <stdlib.h>
#include "../utils/ObjectIO.h"

using namespace std;

float* genData(int, int);
void printData(SerializedObject);

int main(void) {
   const char* outFile = "test.obj";
   int numItems = 10;
   int numFields = 4;
   float* data = genData(numItems,numFields);
   float* norms = genData(numItems,numFields);
   const char* vShader = "vshader";
   const char* fShader = "fshader";
   ExportObject(outFile,numItems,numFields,data,norms,vShader,fShader);
   SerializedObject obj = ImportObject(outFile);
   printData(obj);
   FreeObject(obj);
   return 0;
}

float* genData(int items, int fields) {
   float* data = (float *)malloc(items * fields * sizeof(float));
   for (int i=0;i<items * fields;i+=fields) {
      for (int j=0;j<fields;++j) {
         data[i+j] = j;
      }
   }
   return data;
}

void printData(SerializedObject obj) {
   cout << "The number of vertices are: " << obj.i << "\n";
   cout << "Each vertex has " << obj.j << " fields" << "\n";
   cout << "Data:\n";
   for (int i=0;i<obj.i*obj.j;i+=obj.j) {
      for (int j=0;j<obj.j;j++) {
         cout << obj.data[i+j] << " ";
      }
      cout << "\n";
   }
   cout << "Normals:\n";
   for (int i=0;i<obj.i*obj.j;i+=obj.j) {
      for (int j=0;j<obj.j;j++) {
         cout << obj.norms[i+j] << " ";
      }
      cout << "\n";
   }
   cout << "Vertex shader: " << obj.vShader << "\n";
   cout << "Fragment shader: " << obj.fShader << "\n";

}
