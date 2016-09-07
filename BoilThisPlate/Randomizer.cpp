#include "Randomizer.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;


void reseedRandomizer ()
{
    // reseed random generator
    time_t now;
    time(&now);
    srand(now);
    rand();
}

int getRandomInt()
{
    return int(rand());
}
