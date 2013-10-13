#include <iostream>
#include <stdio.h>
#include "../utils/ObjectIO.h"

using namespace std;

int main(void) {
   cout << "Converting 1d arrays to equivalent 2d arrays!\n\n";
   float** data = (float**)malloc(10*4*sizeof(float*));
   for (int i=0;i<10;i++) {
      data[i] = (float*)calloc(2, sizeof(float));
      for (int j=0;j<4;j++) {
         data[i][j] = j;
      }
   }
   float* oneDimData = toOneDim(10,4,data);
   cout << "Sample one dimensional array of points\n";
   for (int i=0;i<10*4;++i) {
      cout << oneDimData[i] << " ";
   }
   cout << "\n\nSample two dimensional array of same points\n";
   float** twoDimData = toTwoDim(10,4,oneDimData);
   for (int i=0;i<10;++i) {
      for (int j=0;j<4;++j) {
         cout << twoDimData[i][j] << " ";
      }
      cout << "\n";
   }
   free(oneDimData);
   for (int j=0;j<10;++j) {
      free(data[j]);
      free(twoDimData[j]);
   }
   free(twoDimData);
   free(data);
   return 0;
}
