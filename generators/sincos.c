#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846264338327 
#define SCALE 255

short f(short y, short x, short w, short h);
void write(const char* fname, short *data, short w, short h);

int main(int argc, char** argv)
   {
   short w=100;
   short h=100;
   sscanf( argv[1],"%hd",&w );
   sscanf( argv[2],"%hd",&h );
   srand(time(NULL));
   short* data = (short*)malloc(w*h*sizeof(short));
   short i;
   short j;
   for (i=0;i<h;++i) {
      for (j=0;j<w;++j) {
         data[(i*w)+j] = f(i,j,w,h);
      }
   }
   /*for (i=0;i<h;++i) {
      for (j=0;j<w;++j) {
         printf("%d\t",data[(i*w)+j]);
      } 
      printf("\n");
   }*/
   write("out.pgm",data,w,h);
   free(data);
   return 0;
   }

short f(short y, short x, short w, short h)
   {
   double res = abs((sin(x*PI/180.0) + cos(y*PI/180.0))/2 * SCALE);
   return (short)(res);
   }

void write(const char* fname, short *data, short w, short h)
   {
   FILE* fp_img = NULL;
   if (!(fp_img = fopen(fname, "wt"))) {
      printf("Error with mesh file.\n");
      return;
   }
   long long int img_written = fprintf(fp_img, "P2 %d %d 255", w, h);
   int i;
   int j;
   for (i = 0; i < h; i++) {
      img_written += fprintf(fp_img, "\n");
      for (j = 0; j < w; j++) {
         img_written += fprintf(fp_img, "%3d ", data[(i*w)+j]);
      }
   }
   fclose(fp_img);
   }
