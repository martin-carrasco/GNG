#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <assert.h>
#include "graph.h"
#include "GNG/gng.h"
#include "SFML/Graphics.hpp"

using namespace std;


int main(int argc, char *argv[]) {
    	int mode = atoi(argv[1]);
        if(mode == 0){
            GNGContainer<GNGTrait> container;
            container.init();
            container.start();
        }else{
            PictureGNGContainer<GNGTrait> container;
            container.init();
            container.start();
        }

	return 0;
}
