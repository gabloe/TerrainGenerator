/*
Import and export object data to/from binary files.

Written by: Gabriel Loewen
*/

typedef struct {
    int i;
    int j;
    float *data;
    float *norms;
    char *vShader;
    char *fShader;
} SerializedObject;

void ExportObject(const char*, const int, const int, float*, float*, const char*, const char*);
SerializedObject ImportObject(const char*);
void FreeObject(SerializedObject);
