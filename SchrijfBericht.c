#include <stdio.h>
#include "stego.h"

void header(FILE *, int, FILE *);
int get_message_length(char[]);
int message_fits(int, int, int);
int count(FILE *);
void encode_length(FILE *, FILE *, int);
void encode_message(FILE *, FILE *, int, char *, int, int);

int main(int argc, char **argv) {
  if(argc < 3) {
    printf("er zijn 2 parameters nodig.");
    return 1;
  }

  FILE *fp;
  if((fp = fopen(argv[2], "r+")) == NULL) {
    printf("kon bestand %s niet openen", argv[2]);
    return 1;
  } 
  
  if(read_ppm_type(fp)) {
    
    char *myMessage = (char *)argv[1];
    int message_length = get_message_length(myMessage);
    int w = get_width(fp);
    int h = get_height(fp);

    if(message_fits(message_length, w, h)) {
      if(read_color_depth(fp)) {
      	FILE *fp_t = fopen("test.bmp","w");
      	int i = count(fp);  
      	header(fp, i, fp_t);
        encode_length(fp, fp_t, (message_length - 8) / 8);
      	encode_message(fp, fp_t, (message_length - 8), myMessage, w, h);
      	printf("coderen gelukt");
      	
      	fclose(fp);
      	fclose(fp_t);
      } else {
	printf("kleurdiepte moet 255 zijn");
	return 1;
      }
    } else {
      printf("bestand is te klein");
      return 1;
    }
  } else {
    printf("verkeerd bestandsformaat");
    return 1;
  }

  return 0;
}

void header(FILE *fp1, int numLines, FILE *fp2) {
  int i;
  char temp;

  rewind(fp1); 

  for(i = 0; i < numLines; i++) {
    while((temp = fgetc(fp1)) != EOF && temp != '\n') {
      fputc(temp, fp2);
    }
    fputc('\n', fp2);
  }

  return;
}
int get_message_length(char my_msg[]) {
  int i = 0;
  while(my_msg[i] != '\0') {
    i++;
  }
  return i * 8 + 8;
}
int message_fits(int length, int w, int h) {
  return length < w * h * 3;
}
int count(FILE *fp) {
  char temp; int count = 0;

  rewind(fp);

  while((temp = fgetc(fp)) != EOF) 
    if(temp == '\n') 
      count++;

  return count; 
}

void encode_length(FILE *in, FILE *out, int length) {
  char temp; 
  int i, l;
  for(i = 0; i < 8; i++) {
    temp = fgetc(in);
      l = length; 
      l >>= 7 - i;
      if((l & 1) == 1) {
      	if((temp & 1) == 0) {
      	  temp++;
	       }
      } else {
      	if((temp & 1) == 1) {
      	  temp--;
      	}
      }
    fputc(temp, out);
  }

  return;
}

void encode_message(FILE *in, FILE *out, int num_to_encode, char* my_message, int w, int h) {
  int encoded = 0;
  unsigned char temp;
  int idx = 0, num_coppied = 0; 
  char current; 

  int fileSize = (w * h * 3) - 8;  
  int i;

  for(i = 0; i < fileSize; i++) {
    temp = fgetc(in);
    current = my_message[idx];
    
    current >>= 7 - num_coppied;
    num_coppied++;

    if(encoded <= num_to_encode) {
      encoded++;
      if((current & 1) == 1) {
      	if((temp & 1) == 0) {
      	  temp++;
      	}
      } else {
      	if((temp & 1) == 1) {
      	  temp--;
      	}
      }
      if(num_coppied == 8) {
      	idx++;
      	num_coppied = 0;
      }
    }
    
    fputc(temp, out);
  }
 
  return;
}