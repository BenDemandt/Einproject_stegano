#include <stdio.h>
#include "stego.h"

int get_msg_length(FILE *);
void decode_message(int, FILE *);

int main(int argc, char **argv) {
  FILE *fp;

  if((fp = fopen(argv[1], "rb")) == NULL) {
    printf("zorg dat er een bestand is om te openenen");
    return 1;
  }

  if(read_ppm_type(fp)) {
    get_width(fp);
    get_height(fp);
    if(read_color_depth(fp)) { 
      int length = get_msg_length(fp);
     
      printf("gedecodeerd bericht: \n");
      decode_message(length, fp);

      fclose(fp);
     
    } else {
      printf("foute kleurdiepte");
      return 1;
    }
  } else {
    printf("verkeerd bestandsformaat");
    return 1;
  }

  return 0;
}

int get_msg_length(FILE *fp) {
  char temp = 0;
  int length = 0;
  int i;

  for(i=0; i < 8; i++) {
    temp = fgetc(fp);
 
    if(i > 0) length <<= 1;
    length |= (temp & 1);
  }

    return length;
}

void decode_message(int length, FILE *fp) {
  int readTo = length * 8, numRead = 0, i;
  unsigned char charBuffer = 0;
  char temp;
  char secret_message[length + 1];
  int idx = 0;
  
  while(numRead <= readTo) {
    temp = fgetc(fp); 
    if(numRead % 8 == 0) {
      secret_message[idx] = charBuffer;
      idx++;
      charBuffer = 0;
    } else {
      charBuffer <<= 1;
    }
    charBuffer |= (temp & 1);
    numRead++;
  }
  

  for(i = 1; i < idx; i++) {
    printf("%c", secret_message[i]);
  }

  printf("\n\n");

  return;
}