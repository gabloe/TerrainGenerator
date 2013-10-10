#include "exportObject.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

struct glObj {
    const float* data;
    const float* norms;
    const char* vshader;
    const char* fshader;
};

void serializeChunk(const char* outfile, const float *data, const float *norms, const char* vshader, const char* fshader) {
    
    int offset = 0;
    
    // Create output stream
    ofstream os(outfile, ios::out | ios::binary);
    
    // Create buffer big enough to fit the data
    char *buf = (char *)malloc(sizeof(data) + sizeof(norms) + sizeof(vshader) + sizeof(fshader));
    
    // Copy data into buffer.  Also calculate offsets for the header
    memcpy(buf + offset, data, sizeof(data));
    offset += sizeof(data);
    const char *dataHeader = intToCstr(offset);
    
    memcpy(buf+offset,norms,sizeof(norms));
    offset += sizeof(norms);
    const char *normsHeader = intToCstr(offset);

    memcpy(buf+offset,vshader,sizeof(vshader));
    offset += sizeof(vshader);
    const char *vshaderHeader = intToCstr(offset);

    memcpy(buf+offset,fshader,sizeof(fshader));
    offset += sizeof(fshader);
    const char *fshaderHeader = intToCstr(offset);
    
    char *header = new char[sizeof(int)*4];
    
    // Concat the offsets for each piece of data
    strcat(header,dataHeader);
    strcat(header,normsHeader);
    strcat(header,vshaderHeader);
    strcat(header,fshaderHeader);
    
    // Write the header to the buffer
    memcpy(buf+offset,header,sizeof(header));

    // Write buffer to output stream
    os.write(buf, sizeof(buf));
    os.flush();
    os.close();
    
}


// Convert an integer to a char array
const char* intToCstr(int number) {
    std::ostringstream ostr;
    ostr << number;
    std::string theNumberString = ostr.str();
    return (const char*)theNumberString.c_str();
}

// Test
int main(void) {
   
    float *data = new float[3];
    float *norms = new float[3];
    const char *out = "./test.dat";
    const char *vshader = "./vshader.shd";
    const char *fshader = "./fshader.shd";
    data[0] = 5.8;
    data[1] = 2.23454;
    data[2] = 341.7;
    norms[0] = 14.0;
    norms[1] = -13.0;
    norms[2] = 11.0;
    serializeChunk(out,data,norms,vshader,fshader);
    return 0;
    
}