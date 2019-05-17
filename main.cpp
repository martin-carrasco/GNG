#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <assert.h>
#include "GNG/gng.h"

using namespace std;


int main(int argc, char *argv[]) {
    	int mode = atoi(argv[1]);
        if(mode == 0){
            GNGContainer<GNGTrait> container;
            container.init();
            container.start();
        }else if(mode == 1){
            VideoGNGContainer<UGNGTrait> container;
            container.init();
            container.start();
        }
        else{
            PictureGNGContainer<UGNGTrait> container;
            container.init();
            container.start();
        }

	return 0;
}
