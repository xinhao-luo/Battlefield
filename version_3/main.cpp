#include <iostream>
#include <fstream>
#include "field.h"
#include "units.h"
#include "engine.h"
using namespace std;

int main()
{
    /*string filename = "field.txt";
    ofstream ofs;
    ofs.open(filename.c_str());
    if (!ofs) {
        cout << "Cannot open the file: " << filename << endl;
        return -1;
    }*/


    // Set units
    /*addFootman(f, true, 2, 0);
    addFootman(f, true, 2, 1);
    addFootman(f, true, 2, 2);
    addFootman(f, true, 2, 7);

    addFootman(f, false, 7, 4);
    addFootman(f, false, 8, 3);
    addFootman(f, false, 7, 3);
    addFootman(f, false, 9, 5);
    addKnight(f, true, 4, 8);
    addKnight(f, true, 5, 8);
    addKnight(f, true, 6, 8);
    addKnight(f, true, 7, 8);
    addKnight(f, false, 2, 8);
    addKnight(f, false, 3, 8);
    addKnight(f, false, 9, 8);
    addKnight(f, false, 10, 8);
    addMountain(f, 10, 10);
    addForest(f, 3, 5);
    addWater(f, 4, 8);*/
    //displayField(cout, f);
    Field* fp = loadMap(cin);
    Field f = *fp;
    play(f, cin, cout);

    // ofs.close();
    return 0;
    /*string filename = "map1.txt";
    ifstream ifs;
    ifs.open(filename.c_str());
    if (!ifs) {
    cout << "Cannot open the file: " << filename << endl;
    return -1;
    }
    Field* f = loadMap(ifs);
    if (f == NULL) {
    cout << "Failed to load map!" << endl;
    return -1;
    }
    play(*f, cin, cout);
    delete f;
    ifs.close();
    return 0;*/
}
