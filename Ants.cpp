#include "Ants.h"
#include <stdio.h>
#include <assert.h>

const int Precision = 1000;
const double MutationRate = 0.01;
const int IncreaseDecrease = 10;

void operator +=(tVec& vec, tMove dir){

    switch(dir){
        case Up:
            vec.x += 0;
            vec.y += -1;
        break;
        case Right:
            vec.x += 1;
            vec.y += 0;
        break;
        case Down:
            vec.x += 0;
            vec.y += 1;
        break;
            vec.x += -1;
            vec.y += 0;
        case Left:
            vec.x += -1;
            vec.y += 0;
        break;
    };
}

bool operator <(const tAnt& ant1, const tAnt& ant2){
    return (ant1.score < ant2.score);
}



void decrease(tPath& path){
    tMove* aux = new tMove[path.max - IncreaseDecrease];

     for(int i = 0; i < path.num; i++)
        aux[i] = path.moves[i];

    delete[] path.moves;

    path.max -= IncreaseDecrease;
    path.moves = aux;
}

void increase(tPath& path){
    tMove* aux = new tMove[path.max + IncreaseDecrease];

    for(int i = 0; i < path.num; i++)
        aux[i] = path.moves[i];

    delete[] path.moves;

    path.max += IncreaseDecrease;
    path.moves = aux;
}

void increase(tPoblationsList& pobs){
    tPoblation* aux = new tPoblation[pobs.max + Increase];

    for(int i = 0; i < pobs.num; i++)
        aux[i] = pobs.list[i];
    
    delete[] pobs.list;
    pobs.list = aux;
    pobs.max += Increase;
}

void randomize(tAnt& ant, int mapDim){

    ant.path.num = rand() % (2 * mapDim);
    ant.path.max = ant.path.num;
    ant.path.moves = new tMove[ant.path.num];

    for(int i = 0; i < ant.path.num; i++)
        ant.path.moves[i] = tMove(rand() % 4);  
}

void randomize(tPoblation& pob, int mapDim){
    pob.num = 5 * mapDim;
    pob.max = pob.num;
    pob.list = new tAnt[pob.num];

    for(int i = 0; i < pob.num; i++)
        randomize(pob.list[i], mapDim);
}

void assignPosToPoblation(tVec pos, tPoblation pob){
    for(int i = 0; i < pob.num; i++)
        pob.list[i].pos = pos;
}

tAnt* selectAnt(const tPoblation& pob){
    long long int totalProb = 0;

    for(int i = 0; i < pob.num; i++)
        totalProb += pob.list[i].score * Precision;

    long long int umbral = rand() % totalProb, sum = 0;
    int antChoosen = 0;

    while(umbral > sum && antChoosen < pob.num){
        sum += pob.list[antChoosen].score * Precision;
        antChoosen++;
    }

    return &pob.list[antChoosen];
}
//TOFIX/
//Posiblemente un fallo en la creacion de las tAnts
//
tAnt combineAnts(const tAnt& ant1, const tAnt& ant2){
    tAnt newAnt;
    newAnt.path.num = (ant1.path.num > ant2.path.num) ? ant1.path.num : ant2.path.num;
    
    newAnt.path.moves = new tMove[newAnt.path.num];
    
    for(int i = 0; i < newAnt.path.num; i++){
        if (i < ant1.path.num && i < ant2.path.num)
            newAnt.path.moves[i] = ((i % 2 == 0) ? ant1.path.moves[i] : ant2.path.moves[i]);
        else
            newAnt.path.moves[i] = (ant1.path.num > ant2.path.num) ? ant1.path.moves[i] : ant2.path.moves[i];
    }

    return newAnt;
}

void insertMove(tPath& path, int position){
    if(position > path.num)
        position = path.num;

    if(path.num == path.max)
        increase(path);

    for(int i = path.num; position < i; i--)
        path.moves[i] = path.moves[i - 1];
    path.moves[position] = tMove(rand() % 4);

    path.num++;
}

void deleteMove(tPath& path, int position){
    if(position > path.num)
        position = path.num - 1;

    for(int i = position; i < path.num - 1; i++)
        path.moves[i] = path.moves[i + 1];
    path.num--;
}

void modify (tPath& path, int position){
    if(position > path.num)
        position = path.num -1;

    path.moves[position] = tMove(rand() % 4);
}

void mutation(tAnt& ant){
    int aux = rand() % 3;
    double mutate = rand() % Precision;
    mutate /= Precision;

    if(mutate > (1 - MutationRate)){
        if(aux == 0)
            insertMove(ant.path, rand() % ant.path.num);
        else if (aux == 1)
            deleteMove(ant.path, rand() % ant.path.num);
        else
            modify(ant.path, rand() % ant.path.num);
    }
}

void nextGen(tPoblation& oldPob, tPoblation& newPob){
    tAnt aux1, aux2;
    newPob.num = oldPob.num;
    newPob.max = oldPob.max;

    newPob.list = new tAnt[newPob.num];

    for(int i = 0; i < newPob.num; i++){
        newPob.list[i] = combineAnts(*selectAnt(oldPob), *selectAnt(oldPob));
    }
    
    for(int i = 0; i < newPob.num; i++)
        mutation(newPob.list[i]);
}

void newPoblation(tPoblationsList& pobs, const tMap& map, string typeOfPob){
    if(pobs.num == pobs.max)
        increase(pobs);

    if(typeOfPob == "Evolved")
        nextGen(pobs.list[pobs.num - 1], pobs.list[pobs.num]);
    else
        randomize(pobs.list[pobs.num], map.size);

    assignPosToPoblation(map.start, pobs.list[pobs.num]);

    pobs.num++;
}

bool inRange(tVec vec, int size){
    return (0 <= vec.x && 0 <= vec.y && 
            vec.x < size && vec.y < size);
}

void move(tVec& pos, tMove dir, tMap& map){
    tVec aux = pos;

    aux += dir;
    if( inRange(aux, map.size) && (
        map.terrain[aux.x][aux.y] >= iNothing ||
        map.terrain[aux.x][aux.y] == iStart )){
        
        if(map.terrain[pos.x][pos.y] != Start)
            map.terrain[pos.x][pos.y]--;

        if(map.terrain[aux.x][aux.y] != Start) 
            map.terrain[aux.x][aux.y]++;

        pos = aux;
    }
}

void merge(tAnt *arrMerged, tAnt* arr1, int num1, tAnt* arr2, int num2){
    int c1 = 0, c2 = 0; 
    for(int i = 0; i < num1 + num2; i++){
        if(c1 < num1 && c2 < num2){
            if(arr1[c1] < arr2[c2]){
                arrMerged[i] = arr1[c1];
                c1++;
            }
            else{
                arrMerged[i] = arr2[c2];
                c2++;
            }
        }
        else if(c1 < num1){
            arrMerged[i] = arr1[c1];
            c1++;
        }
        else{
            arrMerged[i] = arr2[c2];
            c2++;
        }
    }
}

void mergeSort(tAnt* array, int length){
    if(length > 1){
        tAnt* arr1 = array, * arr2 = array + length/2 + 1;
        mergeSort(arr1, length/2);
        mergeSort(arr2,length - length/2);

        merge(array, arr1, length/2, arr2, length - length/2);
    }
}


void sort(tPoblation& pob){
    bool change = true;
    int i = 0;
    tAnt aux;

    while(change && i < pob.num){

        change = false;
        for(int j = i; j < pob.num - 1; j++){
            if(pob.list[j].score > pob.list[j + 1].score){
                aux = pob.list[j];
                pob.list[j] = pob.list[j + 1];
                pob.list[j + 1] = aux;

                change = true;
            }
        }
        i++;
    }
}

double performance(int numOfMoves, tVec finalPos, const tMap& map){
    double distancePerformance = distance(finalPos, map.end) / distance(map.start, map.end);
    //double timePerformance = numOfMoves / still needs to be improved
    return ((distancePerformance > 1)? 0 : 1 - distancePerformance);
}

void checkPerformance(tPoblation& pob, const tMap& map){
    for(int i = 0; i < pob.num; i++)
        pob.list[i].score = performance(pob.list[i].path.num, pob.list[i].pos, map);
}

void simPoblation(tPoblation& pob, tMap& map, bool exhibit){
    bool moved = true;
    int movesDone = 0;

    while(moved){
        moved = false;

        system("clear");
        system("read -p ...");
        if(exhibit){
            show(map);
            cout << "Moves: " << movesDone << endl;
            system("sleep 0.1");
        }

        for(int i = 0; i < pob.num; i++){
            if(movesDone < pob.list[i].path.num){
                move(pob.list[i].pos, pob.list[i].path.moves[movesDone], map);
                moved = true;
            }
        }

        movesDone++;
    }

    checkPerformance(pob, map);
    sort(pob);
}
