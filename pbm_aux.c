//THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
//A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Robert Motroc

#include "pbm.h"
#include<stdlib.h>

// constructor for PPMImage
PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m ){
    PPMImage *ptr;
    ptr = (PPMImage*) malloc(sizeof(PPMImage));
    for (int i = 0; i < 3; i++){
        ptr->pixmap[i] = (unsigned int**) malloc(h * sizeof(unsigned int*));
        for (int j = 0; j < h; j++)
            ptr->pixmap[i][j] = (unsigned int*) malloc(w * sizeof(unsigned int)); 
    }   
    ptr->width = w;
    ptr->height = h;
    ptr->max = m;

    return ptr;
}

// constructor for PBMImage
PBMImage * new_pbmimage( unsigned int w, unsigned int h ){
    PBMImage *ptr;
    ptr = (PBMImage*) malloc(sizeof(PBMImage));

    ptr->pixmap = (unsigned int**) malloc(h * sizeof(unsigned int*));
    for (int i = 0; i < h; i++)
        ptr->pixmap[i] = (unsigned int*) malloc(w * sizeof(unsigned int));
    

    ptr->width = w;
    ptr->height = h;

    return ptr;
}

// constructor for PGMImage
PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m ){
    PGMImage *ptr;
    ptr = (PGMImage*) malloc(sizeof(PGMImage));

    ptr->pixmap = (unsigned int**) malloc(h * sizeof(unsigned int*));
    
    for (int i = 0; i < h; i++)
        ptr->pixmap[i] = (unsigned int*) malloc(w * sizeof(unsigned int));

    ptr->width = w;
    ptr->height = h;
    ptr->max = m;

    return ptr;
}

// deconstructor for PPMImage
void del_ppmimage( PPMImage * p ){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < p -> height; j++)
            free(p->pixmap[i][j]);
        free(p->pixmap[i]);
    }
    free(p);
}

// deconstructor for PGMImage
void del_pgmimage( PGMImage * p ){
    for (int i = 0; i < p -> height; i++)
        free(p->pixmap[i]);
    free(p->pixmap);
    free(p);
}

// deconstructor for PBMImage
void del_pbmimage( PBMImage * p ){
    for (int i = 0; i < p -> height; i++)
        free(p->pixmap[i]);

    free(p -> pixmap);
    free(p);
}

