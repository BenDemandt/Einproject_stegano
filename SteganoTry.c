#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 54
int t = 0;

int j = 0;
typedef struct NODE {
    char *val;
    struct NODE *next;
} NODE;

/*
   Stores the RGB values of a pixel
 */
typedef struct Pixel {
    int red;
    int green;
    int blue;
} Pixel;

/*
   Stores the contents of a PPM file
 */
typedef struct PPM {
    char *format;
    NODE *comments;
    int width, height;
    int max;
    Pixel *arr;
} PPM;

NODE *copy(NODE *first)
{
    NODE *second = NULL, *previous = NULL;

    while (first != NULL) {
        NODE *temp = (NODE *)malloc(sizeof(NODE));
        temp->val = first->val;
        temp->next = NULL;

        if (second == NULL) {
            second = temp;
            previous = temp;
        } else {
            previous->next = temp;
            previous = temp;
        }
        first = first->next;
    }
    return second;
}

//Copy the PPM File Returning Pointer
struct PPM* createPPM(PPM *old)
{
    PPM* new = (PPM *)malloc(sizeof(PPM));

    strcpy(new->format, old->format);
    new->comments = copy(old->comments);

    new->height = old->height;
    new->width = old->width;
    new->max = old->max;

    new->arr = (Pixel *)malloc(old->height * old->width * sizeof(Pixel));
    memcpy(new->arr, old->arr, old->height * old->width * sizeof(Pixel));

    return new;
}

/*
   Returns the PPM struct from the file fd
 */
    const char *karakter;
    PPM *getPPM(FILE *fd) {
    PPM *afbeelding = malloc(sizeof(*afbeelding));
    afbeelding->format = malloc(MAX_LEN);

    if(fgets(afbeelding->format, MAX_LEN, fd) != NULL)
    {
		scanf(karakter, "%s", afbeelding->format);
	}
    if (strcmp(afbeelding->format, "P3") != 0) {
        printf("Ongeldig formaat");
        exit(0);
    }
    char c = getc(fd);
    afbeelding->comments = (NODE*)malloc(sizeof(NODE));
    NODE *temp = afbeelding->comments;
    while ((c = getc(fd)) == '#') {
        fseek(fd, -1, SEEK_CUR);
        char str[50];
        fgets(str, 50, fd);
        temp->val = (char*)malloc(strnlen(str, 50));
        strcpy(temp->val, str);
        temp->val[strlen(temp->val) - 1] = 0;
        temp->next = (NODE*)malloc(sizeof(NODE));
        temp = temp->next;
        temp->next = NULL;
    }

    fseek(fd, -1, SEEK_CUR);
    fscanf(fd, "%d", &afbeelding->width);
    fscanf(fd, "%d", &afbeelding->height);
    fscanf(fd, "%d", &afbeelding->max);

    afbeelding->arr = (Pixel*)malloc(afbeelding->height * afbeelding->width * sizeof(Pixel));

   

    while (j < afbeelding->height * afbeelding->width) {
        t = fscanf(fd, "%d", &afbeelding->arr[j].red);
        t = fscanf(fd, "%d", &afbeelding->arr[j].green);
        t = fscanf(fd, "%d", &afbeelding->arr[j].blue);
        j = j + 1;
    }

    return afbeelding;
}

PPM *loadPPMFromFile(char *filenaam) {
    FILE *fptr;

    /* TODO: step 1, open the file */
    fptr = fopen("C:\\Users\\12000189\\Documents\\Cprog2\\EindprojectC\\test.ppm", "r");

    /* step 2: Check that file has been loaded correctly; show error otherwise*/
    if (fptr != NULL) {
        PPM *ppm;
        /* ODO: step 3, get the PPM data from the file with the getPPM function */
        getPPM(fptr);

        if (ppm == NULL) {
            /* TODO: step 4, display error if file cannot been parsed into a PPM struct*/
            printf("bestand kan niet worden verwerkt\n");
        }

        /* TODO: step 5, close the file */
        fclose(fptr);
        /* step 6: return the PPM */
        return ppm;
    } else {
        fclose(fptr);
        return NULL;
    } 
}

/*
   Prints a PPM struct in the format of a PPM file
 */
void showPPM(PPM *ppm) {

    printf("%s\n", ppm->format); //print format

    //print comments
    NODE *n = ppm->comments;
    while (n->next != NULL) {
        printf("%s\n", n->val);
        n = n->next;
    }

    //print width, height and max
    printf("%d %d\n%d\n", ppm->width, ppm->height, ppm->max);

    //print the array containing the pixels
    int j;
    for (j = 0; j < ppm->height * ppm->width; ++j) {
        printf("%d %d %d\n", ppm->arr[j].red, ppm->arr[j].green, ppm->arr[j].blue);
    }

    return;

}

/*
   Encodes text into red field of PPM
   Returns the encoded PPM
 */
PPM *encode(char *text, PPM *i) {
    PPM *str = createPPM(i);
    int random;
    srand((unsigned)time(NULL));
    int randMax = (i->height * i->width) / (strlen(text) + 1);

    random = rand() % randMax;

    if (random < 1) {
    if (random < 1) {
        random = 1;
    }

    int k = 0;
    int j = random;

    //Red fields swapped with ASCII int
    while (k < strlen(text)) {
        if (str->arr[j].red == text[k]) {
            j = j + 1; // if the values are the same we encode in the next  pixel.
        } else {
            str->arr[j].red = text[k];
            k = k + 1;
            j = j + random;
        }
    }

    return str;
}


/*
   Compares 2 PPM structs and decodes the message hidden within
   Returns the decoded message if the images have the same dimensions
   Returns NULL otherwise
 */
char *decode(PPM *i1, PPM *i2) {
    int i = 0;
    int j = 0;

    char *str = (char*)malloc(sizeof(char));

    while (i < i1->height * i1->width) {
        if (i1->arr[i].red != i2->arr[i].red) {
            str[j] = i2->arr[i].red;
            j = j + 1;
        }
        i = i + 1;
    }

    str = realloc(str, i);

    return str;
    printf("%s", str);
}

/* TODO: Question 3 */
int main(int argc, char *argv[]) {

    /* check arguments */
    switch (argc) 
	{
    case 2:
        /* TODO: not enough arguments, print an error message */
        printf("niet genoeg argumenten \n");
        break;
    case 3:
        if (strcmp(argv[1], "e") == 0) 
		{ //Argument "e" - encode PPM
            PPM *ppm = loadPPMFromFile(argv[2]);
            createPPM(ppm);

            /*Check that PPM is valid; continue if it is, exit otherwise */
            if (ppm != NULL) 
			{
                PPM *encodedPPM;
                /* TODO: encode the ppm with the text and assign to encodedPPM */
                encodedPPM = encode(argv[2], encodedPPM);
                /*Check that no error occured*/
                if (encodedPPM == NULL) 
				{
                    return EXIT_FAILURE;
                } 
				else 
				{
                    /* TODO: print a confirmation that encoding was successful */
                    printf("coderen is succesvol \n");
                    /* TODO: print the PPM to standard output with showPPM */
                    showPPM(encodedPPM);
                    return EXIT_SUCCESS;
                }
            } 
			else 
			{
                return EXIT_FAILURE;
            }
        } else {
            printf("onbekende of incomplete command line.\n");
            return EXIT_FAILURE;
        }
        break;
    case 4:
        if (strcmp(argv[1], "d") == 0) { //Argument "d" - decode PPM
            PPM *comparisonPPM;
            PPM *encodedPPM;

            /* TODO: get comparison file filename from argv, load it with
               loadPPMFromFile then assign to comparisonPPM */

            comparisonPPM = loadPPMFromFile(argv[2]);
            /* TODO: get encoded file filename from argv, load it with
               loadPPMFromFile then assign to encodedPPM */

            encodedPPM = loadPPMFromFile(argv[2]);
            /*Check both PPMs are valid; continue if so, exit otherwise */
            if (comparisonPPM != NULL && encodedPPM != NULL) {
                char *decodedMsg;

                /* TODO: decode the encodedPPM with the comparisonPPM and assign to decodedMsg */
                decodedMsg = decode(encodedPPM, comparisonPPM);
                /*Check that the message has been decoded*/
                if (decodedMsg != NULL) {
                    /* TODO: print a confirmation message that the message was decoded */

                    printf("gedecodeerd bericht:\n");

                    /* TODO: print the decoded message */
                    printf("%p",decodedMsg);
                    return EXIT_SUCCESS;
                } else {
                    return EXIT_MISLUKT;
                }
            } else {
                return EXIT_FAILURE;
            }
        } else {
            fprintf(stderr, "obekende of incomplete command line.\n\n");
            return EXIT_FAILURE;
        }
        break;
    default:
        fprintf(stderr, "onbekende of incomplete line.\n\n");
        return EXIT_FAILURE;
    }
	
  }
}