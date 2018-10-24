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

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 980

using namespace std;


int main(int argc, char *argv[]) {
    GNGContainer<GNGTrait> container;
    container.init();
    container.start();

	return 0;
}
