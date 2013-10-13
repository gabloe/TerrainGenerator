#include <iostream>
#include <stdio.h>
#include "../utils/ObjectIO.h"

using namespace std;

int main(void) {
   int numVertices = 10;
   int numFields = 4;
   cout << "Converting 1d arrays to equivalent 2d arrays!\n\n";
   float** data = (float**)malloc(numVertices * numFields * sizeof(float*));
   for (int i=0;i<numVertices;++i) {
      data[i] = (float*)calloc(numFields, sizeof(float));
      for (int j=0;j<numFields;++j) {
         data[i][j] = j;
      }
   }
   float* oneDimData = toOneDim(numVertices,numFields,data);
   cout << "Sample one dimensional array of points\n";
   for (int i=0;i<numVertices * numFields;++i) {
      cout << oneDimData[i] << " ";
   }
   cout << "\n\nSample two dimensional array of same points\n";
   float** twoDimData = toTwoDim(numVertices, numFields, oneDimData);
   for (int i=0;i<numVertices;++i) {
      for (int j=0;j<numFields;++j) {
         cout << twoDimData[i][j] << " ";
      }
      cout << "\n";
   }
   free(oneDimData);
   for (int j=0;j<numVertices;++j) {
      free(data[j]);
      free(twoDimData[j]);
   }
   free(twoDimData);
   free(data);
   return 0;
}
