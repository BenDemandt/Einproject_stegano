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
PPM *getPPM(FILE *fd) {
    PPM *image = (PPM *)malloc(sizeof(PPM));
    image->format = (char *)malloc(MAX_LEN);

    fgets(fd, MAX_LEN, image->format);
    sscanf(fd, "%s", image->format);

    if (strcmp(image->format, "P3") != 0) {
        printf("Invalid Image Type");
        exit(0);
    }
    char c = getc(fd);
    image->comments = (NODE*)malloc(sizeof(NODE));
    NODE *temp = image->comments;
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
    fscanf(fd, "%d", &image->width);
    fscanf(fd, "%d", &image->height);
    fscanf(fd, "%d", &image->max);

    image->arr = (Pixel*)malloc(image->height * image->width * sizeof(Pixel));

    int t = 0;
    int j = 0;

    while (j < image->height * image->width) {
        t = fscanf(fd, "%d", &image->arr[j].red);
        t = fscanf(fd, "%d", &image->arr[j].green);
        t = fscanf(fd, "%d", &image->arr[j].blue);
        j = j + 1;
    }

    return image;
}

PPM *loadPPMFromFile(char *filename) {
    FILE *file;

    /* TODO: step 1, open the file */
    file = fopen("C:\\Users\\Olivia\\source\\repos\\f28hs-2020-21-cwk1-c\\PPMfiles", "r");

    /* step 2: Check that file has been loaded correctly; show error otherwise*/
    if (file != NULL) {
        PPM *ppm;
        /* ODO: step 3, get the PPM data from the file with the getPPM function */
        getPPM(file);

        if (ppm == NULL) {
            /* TODO: step 4, display error if file cannot been parsed into a PPM struct*/
            printf("File cannot be parsed.\n");
        }

        /* TODO: step 5, close the file */
        fclose(file);
        /* step 6: return the PPM */
        return ppm;
    } else {
        fclose(file);
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
    switch (argc) {
    case 2:
        /* TODO: not enough arguments, print an error message */
        printf("Not enough arguments. \n");
        break;
    case 3:
        if (strcmp(argv[1], "e") == 0) { //Argument "e" - encode PPM
            PPM *ppm = loadPPMFromFile(argv[2]);
            createPPM(ppm);

            /*Check that PPM is valid; continue if it is, exit otherwise */
            if (ppm != NULL) {
                PPM *encodedPPM;
                /* TODO: encode the ppm with the text and assign to encodedPPM */
                encodedPPM = encode(argv[2], encodedPPM);
                /*Check that no error occured*/
                if (encodedPPM == NULL) {
                    return EXIT_FAILURE;
                } else {
                    /* TODO: print a confirmation that encoding was successful */
                    printf("Encoding successful. \n");
                    /* TODO: print the PPM to standard output with showPPM */
                    showPPM(encodedPPM);
                    return EXIT_SUCCESS;
                }
            } else {
                return EXIT_FAILURE;
            }
        } else {
            printf("Unrecognised or incomplete command line.\n");
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

                    printf("Decoded message:\n");

                    /* TODO: print the decoded message */
                    printf("%p",decodedMsg);
                    return EXIT_SUCCESS;
                } else {
                    return EXIT_FAILURE;
                }
            } else {
                return EXIT_FAILURE;
            }
        } else {
            fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
            return EXIT_FAILURE;
        }
        break;
    default:
        fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
        return EXIT_FAILURE;
    }
}