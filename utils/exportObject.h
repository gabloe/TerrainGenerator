#ifndef ____exportObject__
#define ____exportObject__

#include <iostream>

#endif /* defined(____exportObject__) */

typedef struct {
   int i;
   int j;
   float **data;
   float **norms;
   char *vShader;
   char *fShader;
} SerializedObject;

void exportObject(const char*, const int, const int, float**, float**, const char*, const char*);

SerializedObject importObject(const char*);

void freeObject(SerializedObject);

int main(void);
