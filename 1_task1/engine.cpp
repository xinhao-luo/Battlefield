#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include "engine.h"
#include "actions.h"
#include "units.h"
#include "algorithms.h"
using namespace std;

// Load map
Field* loadMap(istream& is)
{
    int M, N, NT, NU;
    string line;
    getline(is, line);
    stringstream ss(line);
    ss >> M >> N >> NT >> NU;
    Field* f = new Field(M, N);
    int R, C;
    char T;
    string line1;
    string line2;
    for(int i = 0;i < NT;i++){
        getline(is, line1);
        stringstream ter(line1);
        ter >> R >> C >> T;
        Terrain* loadterrain;
        switch(T) {
        case 'W':
            loadterrain = new Terrain(Water, R, C);
            break;
        case 'M':
            loadterrain = new Terrain(Mountain, R, C);
            break;
        case 'F':
            loadterrain = new Terrain(Forest, R, C);
            break;
        default:
            assert(false);
        }
        f -> setTerrain(R, C, loadterrain);
    }
    int RU, CU;
    char S;
    string U;
    for(int i = 0;i < NU;i++){
        getline(is, line2);
        stringstream uni(line2);
        uni >> RU >> CU >> S >> U;
        Unit* loadunit;
        bool sd;
        if(S == 'A')
            sd = true;
        else
            sd = false;
        if(U == "KN")
            loadunit = new Unit(KNIGHT, sd, RU, CU);
        else
            loadunit = new Unit(FOOTMAN, sd, RU, CU);
        f -> setUnit(RU, CU, loadunit);
    }
    return f;
}

// Add units
bool addFootman(Field& field, bool sd, int row, int col)
{
    if (field.getUnit(row, col) != NULL)
        return false;

    Unit* ft = new Unit(FOOTMAN, sd, row, col);
    return field.setUnit(row, col, ft);
}

// Add units
bool addKnight(Field& field, bool sd, int row, int col)
{
    if (field.getUnit(row, col) != NULL)
        return false;

    Unit* ft = new Unit(KNIGHT, sd, row, col);
    return field.setUnit(row, col, ft);
}

//Add terrains
bool addMountain(Field& field, int row, int col)
{
    if (field.getTerrain(row, col) != NULL)
        return false;

    Terrain* te = new Terrain(Mountain, row, col);
    return field.setTerrain(row, col, te);
}

//Add Water
bool addWater(Field& field, int row, int col)
{
    if (field.getTerrain(row, col) != NULL)
        return false;

    Terrain* te = new Terrain(Water, row, col);
    return field.setTerrain(row, col, te);
}

//Add Forest
bool addForest(Field& field, int row, int col)
{
    if (field.getTerrain(row, col) != NULL)
        return false;

    Terrain* te = new Terrain(Forest, row, col);
    return field.setTerrain(row, col, te);
}

bool checkBounds(ostream& os, const Field& field, int row, int col);
bool readRowCol(const string& str, int& row, int& col);
bool queryRowCol(istream& is, ostream& os, int& row, int& col);
bool performActions(ostream& os, istream&is, Field& field, Unit* u);
bool checkWinning(ostream& os, Field& field);

// Main loop for playing the game
void play(Field& field, istream& is, ostream& os)
{
    bool side = true;
    int numTurns = 1;
    while (is)
    {
        os << endl;
        // Print the new map
        displayField(os, field);

        string player;
        if (side)
            player = "Player A";
        else
            player = "Player B";


        os << "Turn " << numTurns << " (" << player << ")" << endl;

        // Choose the source coordination
        os << "Please select a unit: ";
        int srow, scol;
        if (!queryRowCol(is, os, srow, scol))
            continue;
        if (!checkBounds(os, field, srow, scol))
            continue;

        // Select a unit
        Unit* u;
        SelectUResult sres = selectUnit(field, side, srow, scol, u);
        if (sres == SELECTU_NO_UNIT) {
            os << "No unit at (" << srow << ", " << scol << ")!" << endl;
            continue;
        } else if (sres == SELECTU_WRONG_SIDE) {
            os << "Unit at (" << srow << ", " << scol << ") is an enemy!" << endl;
            continue;
        }

        // Perform actions
        if (!performActions(os, is, field, u))
            continue;

        side = !side;
        numTurns++;
    }
}

// Check if (row, col) is in bounds
bool checkBounds(ostream& os, const Field& field, int row, int col)
{
    if (!field.inBounds(row, col)) {
        os << "(" <<row << ", " << col << ") is out of bound" << endl;
        return false;
    }
    return true;
}

// Read row and column from the string
bool readRowCol(const string& str, int& row, int& col)
{
    stringstream ss(str);
    ss >> row >> col;
    if (ss)
        return true;
    else
        return false;
}

// Query row and column from input stream
bool queryRowCol(istream& is, ostream& os, int& row, int& col)
{
    string line;
    if (!getline(is, line)) {
        os << "Read line failed!" << endl;
        return false;
    }

    if (!readRowCol(line, row, col)) {
        os << "Incorrect input! "<< endl;
        return false;
    }

    return true;
}

// Display actions
void displayActions(ostream& os, const Vector<Action>& acts)
{
    for (int i = 0; i < acts.size(); i++) {
        os << i+1 << ".";
        switch (acts[i]) {
        case MOVE:
            os << "Move ";
            break;

        case SKIP:
            os << "Skip ";
            break;

        default:
            assert(false);
        }
    }
    os << endl;
}

// Display a menu and query for the actions
bool queryAction(ostream& os, istream& is, Vector<Action>& acts, Action& res)
{
    int i;
    string line;
    getline(is, line);
    stringstream ss(line);
    ss >> i;
    if (!ss) return false;
    i--;
    if (i < 0 || i >= acts.size())
        return false;
    else {
        res = acts[i];
        return true;
    }
}

// Collection actions that can be performed by a unit
Vector<Action> collectActions(Unit* u)
{
    Vector<Action> acts = getActions(u);

    return acts;
}

// Check if there is only skip action
bool isAllSkip(const Vector<Action>& acts)
{
    for (int i = 0; i < acts.size(); i++)
        if (acts[i] != SKIP)
            return false;
    return true;
}

// Convert field to costs
Grid<int> getFieldCosts(const Field& field)
{
    int h = field.getHeight();
    int w = field.getWidth();
    Grid<int> costs(h, w);
    Unit* u;
    Terrain* t;
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            u = field.getUnit(i, j);
            t = field.getTerrain(i, j);
            if((u == NULL) && (t == NULL))
                costs[i][j] = 1;
            else{
                if(u != NULL)
                    costs[i][j] = 999;
                else{
                    if(t->getterrainSymbol() == "$ ")
                        costs[i][j] = 2;
                    else
                        costs[i][j] = 999;
                }
            }
        }
    }

    return costs;
}

// Perform the move action
bool performMove(ostream& os, istream& is, Field& field, Unit* u)
{
    // Display the reachable points
    Grid<bool> grd =
        searchReachable(getFieldCosts(field), u->getRow(), u->getCol(), u->getMovPoints());
    os << endl;
    displayField(os, field, grd);

    // Choose the target coordinate
    os << "Please enter your destination: ";
    int trow, tcol;
    if (!queryRowCol(is, os, trow, tcol))
        return false;

    // Move the target
    if(trow >= field.getHeight() || tcol >= field.getWidth() || trow < 0 || tcol < 0){
        os << "(" <<trow << ", " << tcol << ") is out of bound" << endl;
        return false;
    }else{
        if(!grd[trow][tcol]){
            os<< "Cannot reach"<<" (" <<trow << ", " << tcol << ")!"<<endl;
            return false;
        }else
            moveUnit(field, u, trow, tcol);
    }

    return true;
}

// Perform actions
bool performActions(ostream& os, istream&is, Field& field, Unit* u)
{
    Vector<Action> acts = collectActions(u);
    if (isAllSkip(acts))
        return true;

    displayActions(os, acts);
    os << "Select your action: ";
    Action act;
    if (!queryAction(os, is, acts, act)) {
        os << "Invalid action!" << endl;
        return false;
    }

    switch(act) {
    case MOVE:
        return performMove(os, is, field, u);

    case SKIP:
        return true;

    default:
        os << "(Action not implemented yet)" << endl;
        return false;
    }

    //assert (false);
}

// Print the horizontal line
void printHLine(ostream& os, int n)
{
    os << "  ";
    for (int i = 0; i < n; i++)
        os << "+--";
    os << "+" << endl;
}

// Display the field on the out stream os
void displayField(ostream& os, const Field& field, const Grid<bool>& grd)
{
    int height = field.getHeight();
    int width = field.getWidth();

    // Print the x coordinates
    os << "  ";
    for (int i = 0; i < width; i++)
        os << ' ' << setw(2) << i;
    os << endl;

    printHLine(os, width);
    for (int i = 0; i < height; i++) {
        os << setw(2) << i;
        for (int j = 0; j < width; j++) {
            os << '|';
            Unit* u = field.getUnit(i,j);
            Terrain* t = field.getTerrain(i, j);
            if (u != NULL)
                os << u->getSymbol();
            else if (grd.inBounds(i,j) && grd[i][j])
                os << ". ";
            else {
                if(t != NULL)
                    os << t->getterrainSymbol();
                else
                    os << "  ";
            }
        }
        os << '|' << endl;
        printHLine(os, width);
    }
    os << endl;
}

