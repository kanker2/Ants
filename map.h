#pragma once
#include "Vector.h"

const char Ant = '#', Nothing = ' ', Start = 'S', End = 'E', Rock = 'R';
const int iNothing = 0, iStart = -1, iEnd = -2, iRock = -3;

const int Dim = 40;

typedef int ** tTerrain;

struct tMap{
    tTerrain terrain = nullptr;
    int size = 1;
    tVec start, end;
};

void initialize(tMap& map, int size = Dim);

void show(const tMap map);

void genMap(tMap& map);