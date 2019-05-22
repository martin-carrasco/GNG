#include <iostream>
#include "GNG/gng.h"


void gng_still(){
    PictureGNGContainer<GNGAlgorithm, GNGTrait> container;
    container.init();
    container.start();
}
void gng_moving(){
    MovingPictureGNGContainer<GNGAlgorithm, GNGTrait> container;
    container.init();
    container.start();
}
void gng_video(){
    VideoGNGContainer<GNGAlgorithm, GNGTrait> container;
    container.init();
    container.start();
}

void ugng_still(){
    PictureGNGContainer<UGNGAlgorithm, UGNGTrait> container;
    container.init();
    container.start();
}
void ugng_moving(){
    MovingPictureGNGContainer<UGNGAlgorithm, UGNGTrait> container;
    container.init();
    container.start();
}
void ugng_video(){
    VideoGNGContainer<UGNGAlgorithm, UGNGTrait> container;
    container.init();
    container.start();
}


int main(){
    //Growing Neural Gas test
   
    //gng_still();

    //gng_moving();

    //gng_video();

    //Utility based Growing Neural Gas test
    //ugng_still();

    //ugng_moving();

    ugng_video();

    
    return 0;
}
