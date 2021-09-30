#include "map.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const string templateFileName = "template.txt";

void initialize(tMap& map, int size){

    map.size = size;

    map.terrain = new int*[size];
    for(int i = 0; i < size; i++){
        map.terrain[i] = new int[size];
        for (int j = 0; j < size; j++)
            map.terrain[i][j] = iNothing;
    }
}

void show(const tMap map) {
    for (int i = 0; i < map.size; i++){
        for(int j = 0; j < map.size; j++){
            if(j == map.start.x && i == map.start.y)
                cout << Start;
            else if(j == map.end.x && i == map.end.y)
                cout << End;
            else if(map.terrain[j][i] == iRock)
                cout << Rock;
            else if(map.terrain[j][i] > iNothing)
                cout << Ant;
            else
                cout << Nothing; 
        }
        cout << endl;
    }
}

void genTemplate(tMap& map){
    ofstream file;
    file.open(templateFileName);

    if(file.is_open()){
        for(int i = 0; i < map.size; i++){
            for(int j = 0; j < map.size; j++)
                file << "# ";
            file << endl;
        }

        file.close();
    }
    initialize(map, map.size);
}

void readTemplate(tMap& map){
    ifstream file;
    char aux;

    file.open(templateFileName);

    if(file.is_open()){

        for(int i = 0; i < map.size; i++){
            for(int j = 0; j < map.size; j++){
                file >> aux;

               if(aux == Start){
                   map.terrain[j][i] = iStart;
                   map.start.x = j;
                   map.start.y = i;
               }
               else if(aux == End){
                   map.terrain[j][i] = iEnd;
                   map.end.x = j;
                   map.end.y = i;
               }
               else if(aux == Rock)
                   map.terrain[j][i] = iRock;
                else
                    map.terrain[j][i] = iNothing;
            }

        }

        file.close();
    }
}

void genMap(tMap& map){

    //cout << "Dimension of the map: ";
    //cin >> map.size;
    map.size = 50;
    initialize(map, map.size);
    //genTemplate(map);

    //cout << "Get into the file template.txt \nthis file is a template of the map which is gonna be used \n\nplace the following parts of the terrain wherever you want in the template \n";
    //cout << "rocks(" << Rock << ") \nstart(" << Start << ") \nEnd(" << End << ")\n\n";
    //system("read -p waiting... k");

    readTemplate(map);
}
