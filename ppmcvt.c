#include "pbm.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>

//THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
//A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Robert Motroc

PBMImage* opt_b(PPMImage* ppm);
void opt_r(PPMImage* ppm, int color);
PPMImage* opt_n(PPMImage* ppm, int f);
PPMImage* opt_t(PPMImage* ppm, int f);
void opt_m(PPMImage* ppm);
void opt_s(PPMImage* ppm);
void opt_i(PPMImage* ppm, int c1, int c2);
PGMImage* opt_g(PPMImage* ppm, int f);


int main( int argc, char *argv[]){

    int c;
    int b=0, g=0, i=0, r=0, s=0, m=0, t=0, n=0, o=0;
    char* inputarg;
    char* output = NULL;
    char* input = NULL;

    // Use getopt() to select/collect the options and appropriate arguments.
    // Print messages to standard error channel and exit with 1 for error cases.
    while((c = getopt(argc, argv, ":bg:i:r:smt:n:o:")) != -1){
        if (b+g+i+r+s+m+t+n > 1){
            fprintf(stderr, "Error: Multiple transformations specified\n");
            exit(1);
        }
        switch(c) {
        case 'b':
            b++;
            break;
        case 'g':
            g++;
            inputarg = argv[optind-1];
            if (atoi(inputarg)>65535){
                fprintf(stderr, "Invalid max grayscale pixel value: %d; must be less than 65,536\n", atoi(inputarg));
                exit(1);
            }
            break;
        case 'i':
            i++;
            inputarg = argv[optind-1];
            if ( strcmp(inputarg, "red") != 0 && strcmp(inputarg, "green") != 0 && strcmp(inputarg, "blue") != 0){
                fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'\n", inputarg);
                exit(1);
            }
            break;
        case 'r':
            r++;
            inputarg = argv[optind-1];
            if ( strcmp(inputarg, "red") != 0 && strcmp(inputarg, "green") != 0 && strcmp(inputarg, "blue") != 0){
                fprintf(stderr, "Error: Invalid channel specification: (%s); should be 'red', 'green', or 'blue'\n", inputarg);
                exit(1);
            }
            break;
        case 's':
            s++;
            break; 
        case 'm':
            m++;
            break;
        case 't':
            t++;
            inputarg = argv[optind-1];
            if ( (atoi(inputarg) < 1) || (atoi(inputarg) > 8)){
                fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", atoi(inputarg));
                exit(1);
            }
            break;
        case 'n':
            n++;
            inputarg = argv[optind-1];
            if ( (atoi(inputarg) < 1) || (atoi(inputarg) > 8)){
                fprintf(stderr, "Error: Invalid scale factor: %d; must be 1-8\n", atoi(inputarg));
                exit(1);
            }
            break;
        case 'o':
            o++;
            output = argv[optind-1];
            break;
        case '?':
            fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
            exit(1);
        case ':':
            if (optopt == 'o'){
                fprintf(stderr, "Error: No output file specified\n");
                exit(1);
            }
            fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
            exit(1);
        default:
            break;
        }
    }

    if (b+g+i+r+s+m+t+n > 1){
        fprintf(stderr, "Error: Multiple transformations specified\n");
        exit(1);
    }
    if (optind != argc){
        input = argv[optind];
    } else {
        fprintf(stderr, "Error: No input file specified\n"); 
        exit(1);
    }
     if (o == 0){
        fprintf(stderr, "Error: No output file specified\n");
        exit(1);
    }
 

    PPMImage *ppm = read_ppmfile(input);

    
    if (g == 1){ // Grayscale
        PGMImage* out = opt_g(ppm, atoi(inputarg));
        write_pgmfile(out, output);
        del_ppmimage(ppm);
        del_pgmimage(out);

    } else if (i == 1){ // Isolate
        if (strcmp(inputarg, "red") == 0){
            opt_i(ppm, 1, 2);
        } else if (strcmp(inputarg, "green") == 0){
            opt_i(ppm, 0, 2);
        } else { 
            opt_i(ppm, 0, 1);
        }
        write_ppmfile(ppm, output);
        del_ppmimage(ppm);

    } else if (r == 1){ // Remove
        if (strcmp(inputarg, "red") == 0){
            opt_r(ppm, 0);
            write_ppmfile(ppm, output);
            del_ppmimage(ppm);
        } else if (strcmp(inputarg, "green") == 0){
            opt_r(ppm, 1);
            write_ppmfile(ppm, output);
            del_ppmimage(ppm);
        } else { 
            opt_r(ppm, 2);
            write_ppmfile(ppm, output);
            del_ppmimage(ppm);
        }

    } else if (s == 1){ // Sepia
        opt_s(ppm);
        write_ppmfile(ppm, output);
        del_ppmimage(ppm);

    } else if (m == 1){ // Mirror 
        opt_m(ppm);
        write_ppmfile(ppm, output);
        del_ppmimage(ppm);

    } else if (t == 1){ // Thumbnail
        PPMImage* out = opt_t(ppm, atoi(inputarg));
        write_ppmfile(out, output);
        del_ppmimage(out);
        del_ppmimage(ppm);

    } else if (n == 1){ // Tile Thumbnail
        PPMImage* out = opt_n(ppm,atoi(inputarg));
        write_ppmfile(out, output);
        del_ppmimage(out);
        del_ppmimage(ppm);
    } else { // Black and White
        PBMImage* out = opt_b(ppm);
        write_pbmfile(out, output);
        del_pbmimage(out);
        del_ppmimage(ppm);

    }


    return 0;
}


// takes input of PPMImage structs and performs black and white conversion on it
PBMImage* opt_b(PPMImage* ppm){
    PBMImage* output = new_pbmimage(ppm->width, ppm->height);
    for (int i = 0; i < output->height; i++){
        for (int j = 0; j < output->width; j++){
            float avg = 0;
            for (int k = 0; k < 3; k++)
                avg += ppm->pixmap[k][i][j];
            avg /= 3.0;
            avg *= 2.0;
            if (avg < ppm->max){
                output->pixmap[i][j] = 1;
            } else {
                output->pixmap[i][j] = 0;
            }
        }
    }
    return output;
}

// takes input of PPMImage struct and isolates a specific RGB channel
void opt_i(PPMImage* ppm, int c1, int c2){
    for (int i = 0; i < ppm->height; i++){
        for (int j = 0; j< ppm->width; j++){
            ppm->pixmap[c1][i][j] = 0;
        }
    }
    for (int i = 0; i < ppm->height; i++){
        for (int j = 0; j< ppm->width; j++){
            ppm->pixmap[c2][i][j] = 0;
        }
    }
}


// takes input of PPMImage struct and removes a specific color
void opt_r(PPMImage* ppm, int color){
    for (int i = 0; i < ppm->height; i++){
        for (int j = 0; j< ppm->width; j++){
            ppm->pixmap[color][i][j] = 0;
        }
    }
}



// takes PPMImage struct and applies a grayscale conversion
PGMImage* opt_g(PPMImage* ppm, int f){
    PGMImage* pgm = new_pgmimage(ppm->width, ppm->height, f);
    for (int i = 0; i < pgm->height; i++){
        for (int j = 0; j < pgm->width; j++){
            float avg = 0;
            for (int k = 0; k < 3; k++)
                avg += ppm->pixmap[k][i][j];
            avg *= (float)pgm->max; 
            avg /= (float)ppm->max * 3.0;
            pgm->pixmap[i][j] = (unsigned int) avg;
        }
    }
    return pgm;
}


// takes PPMImage struct and applies a sepia color conversion
void opt_s(PPMImage* ppm){
    float red = 0;
    float green = 0;
    float blue = 0;
    for (int i = 0; i < ppm->height; i++){
        for (int j = 0; j < ppm->width; j++){
            red = ppm->pixmap[0][i][j];
            green = ppm->pixmap[1][i][j];
            blue = ppm->pixmap[2][i][j];
            if ((unsigned int)(0.393*red + 0.769*green + 0.189*blue) >= ppm->max) {
                ppm->pixmap[0][i][j] = ppm->max;
            } else {
                ppm->pixmap[0][i][j] = (unsigned int)(0.393*red + 0.769*green + 0.189*blue);
            }
            if ((unsigned int)(0.349*red + 0.686*green + 0.168*blue) >= ppm->max) {
                ppm->pixmap[1][i][j] = ppm->max;
            } else {
                ppm->pixmap[1][i][j] = (unsigned int)(0.349*red + 0.686*green + 0.168*blue);
            }
            if ((unsigned int)(0.272*red + 0.534*green + 0.131*blue) >= ppm->max) {
                ppm->pixmap[2][i][j] = ppm->max;
            } else {
                ppm->pixmap[2][i][j] = (unsigned int)(0.272*red + 0.534*green + 0.131*blue);
            }
        }
    }
}


// takes input PPMimage struct and applies the mirror transformation by copying the first half
// of the data an applying it to the second half
void opt_m(PPMImage* ppm){
    for (int i = 0; i < ppm->height; i++){
        for (int j = 0; j < ppm->width/2; j++){
            for(int k = 0; k < 3; k++)
                ppm->pixmap[k][i][ppm->width-j-1] = ppm->pixmap[k][i][j];
        }
    }
}


// takes input PPMImage struct and applies the thumbnail conversion
// it turns the given image into a thumbnail by reducing it using a 
// scaling factory between 1 and 8.
PPMImage* opt_t(PPMImage* ppm, int f){
    unsigned int tn_width, tn_height;
    if (f >= ppm->width){
        tn_width = 1;
    } else if (ppm->width % f != 0){
        tn_width = ppm->width/f + 1;
    } else {
        tn_width = ppm->width/f; 
    }

    if (f >= ppm->height){
        tn_height = 1;
    } else if (ppm->height % f != 0){
        tn_height = ppm->height/f + 1;
    } else {
        tn_height = ppm->height/f;
    }

    int tn_i_counter = 0;
    PPMImage* tn = new_ppmimage(tn_width, tn_height, ppm->max);
    for (int i = 0; i < ppm->height; i += f){
        int tn_j_counter = 0;
        for (int j = 0; j < ppm->width; j += f){
            for (int k = 0; k < 3; k++)
                tn->pixmap[k][tn_i_counter][tn_j_counter] = ppm->pixmap[k][i][j];
            tn_j_counter++;
        }
        tn_i_counter++;
    }
    return tn;

}

// takes input PPMImage struct and applies a tile conversion it 
// creates tile thumbnails baed on a scaling factor between 1 and 8.
PPMImage* opt_n(PPMImage* ppm, int f){
    unsigned int w = ppm->width / f; 
    unsigned int h = ppm->height / f;

    if (ppm->width % f != 0) 
        w++;

    if (ppm->height % f != 0)
        h++;
    
    PPMImage * new = new_ppmimage(ppm->width, ppm->height, ppm->max);
    for(int i = 0; i < ppm->height; i++){
        for(int j = 0; j < ppm->width; j++){
            for (int k = 0; k<3; k++){
                new->pixmap[k][i][j]=ppm->pixmap[k][i%h*f][j%w*f];
            }
        }
    }
    return new;
}