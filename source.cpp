#include <iostream>
#include "Ants.h"
using namespace std;

//inheritance still needs to be implemented
//fix the visualization of a poblation
//implement some functions to be able to look the specific path of just one ant to show the best of all the simulation for example

//Probando nueva rama
//Estoy en Main

int main(){
    srand(time(NULL));
    tPoblationsList pobs;
    tMap map;
    int generaciones = 100;
    pobs.list = new tPoblation[pobs.max];

    genMap(map);
    newPoblation(pobs, map, "Random");

    for(int i = 0; i < generaciones; i++){
        simPoblation(pobs.list[i], map, (i == generaciones - 1) ? true : false);
        newPoblation(pobs, map);
    }

    return 0;
}