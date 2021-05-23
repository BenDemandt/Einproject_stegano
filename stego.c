#include <stdio.h> 
#include <stdlib.h>
#include "stego.h"

int read_ppm_type(FILE *fp) {
  char temp = fgetc(fp); 
  if(temp == 'P' && fgetc(fp) == '6') {
    fgetc(fp);
    return 1;
  }

  return 0;
}

int get_width(FILE *fp) {
  int w;
  fscanf(fp, "%d", &w);
  return w;
}


int get_height(FILE *fp) {
  int h;
  fscanf(fp, "%d", &h);
  return h;
}

int read_color_depth(FILE *fp) {
  int c;
  fscanf(fp, "%d", &c);
  if(c == 255) {
    fgetc(fp);    
    return 1;
  }
  return 0;
}