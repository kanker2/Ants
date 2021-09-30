#pragma once
#include "map.h"
#include "Vector.h"
#include <iostream>
#include <string>
using namespace std; 

//Symbols to know the dir an ant is taking
typedef enum tMove { Up, Right, Down, Left };

void operator +=(tVec& vec, tMove dir);

const int Increase = 5;

struct tPath {
    tMove* moves = nullptr;
    int num = 0, max = Increase;
};

struct tAnt{
    tPath path;
    double score = 0;
    tVec pos;
};

struct tPoblation{
    tAnt* list;
    int num = 0, max = Increase;
};

struct tPoblationsList{
    tPoblation* list = nullptr;
    int num = 0, max = Increase;
};

void newPoblation(tPoblationsList& pobs, const tMap& map, string typeOfPob = "Evolved");

void move(tVec& pos, tMove dir, tMap& map);

void simPoblation(tPoblation& pob, tMap& map, bool exhibit = true);