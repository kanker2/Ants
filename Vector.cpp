#include "Vector.h"
#include <cmath>

double distance(tVec v1, tVec v2){
    int dx = abs(v1.x - v2.x), dy = abs(v2.y - v2.y); 
    return (dx + dy);
}