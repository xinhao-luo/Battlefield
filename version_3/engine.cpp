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
        else if(U == "FT")
            loadunit = new Unit(FOOTMAN, sd, RU, CU);
        else
            loadunit = new Unit(ARCHER, sd, RU, CU);
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

// Add units
bool addArcher(Field& field, bool sd, int row, int col)
{
    if (field.getUnit(row, col) != NULL)
        return false;

    Unit* ft = new Unit(ARCHER, sd, row, col);
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
bool aftermove(ostream& os, istream& is, Field& field, Unit* u);
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
        if(!field.Bhasunit()){
            os<<"Congratulations! Player A has won!"<<endl;
            break;
        }
        if(!field.Ahasunit()){
            os<<"Congratulations! Player B has won!"<<endl;
            break;
        }
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

        if(!field.Ahasunit()){
            os << endl;
            displayField(os, field);
            os<<"Congratulations! Player B has won!"<<endl;
            break;
        }
        if(!field.Bhasunit()){
            os << endl;
            displayField(os, field);
            os<<"Congratulations! Player A has won!"<<endl;
            break;
        }
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

//read dir from the string
bool readdir(const string& str, int& dirs)
{
    stringstream ss(str);
    ss >> dirs;
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

//Query direction from input stream
bool querydir(istream& is, ostream& os, int& dirs)
{
    string line;
    if (!getline(is, line)) {
        os << "Read line failed!" << endl;
        return false;
    }

    if(!readdir(line, dirs))
        return false;

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

        case ATTACK:
            os << "Attack ";
            break;

        case SHOOT:
            os << "Shoot ";
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
    os<<endl;
    displayField(os, field);
    return aftermove(os, is, field, u);
}


// Perform the attack action
bool performAttack(ostream& os, istream& is, Field& field, Unit* u)
{
    enum Directon{UP, DOWN, LEFT, RIGHT};
    Vector<Directon> dir = {UP, DOWN, LEFT, RIGHT};
    for(int i = 0; i < 4;i++){
        os<< i + 1 << ".";
        switch(dir[i]) {
        case UP:
            os << "UP ";
            break;

        case DOWN:
            os << "DOWN ";
            break;

        case LEFT:
            os << "LEFT ";
            break;

        case RIGHT:
            os << "RIGHT ";
            break;

        default:
            assert(false);
        }
    }
    os<<endl;
    os<< "Please enter the direction: ";
    int dirs;
    if(!querydir(is, os, dirs))
        return false;
    if(dirs < 0 || dirs > 4){
        os<< "Incorrect input! "<< endl;
        return false;
    }
    Unit* ut;
    int trow, tcol;
    switch(dir[dirs - 1]) {
    case UP:
        trow = u ->getRow() - 1;
        tcol = u ->getCol();
        break;

    case DOWN:
        trow = u ->getRow() + 1;
        tcol = u ->getCol();
        break;

    case LEFT:
        trow = u ->getRow();
        tcol = u ->getCol() - 1;
        break;

    case RIGHT:
        trow = u ->getRow();
        tcol = u ->getCol() + 1;
        break;

    default:
        assert(false);
    }
    if(trow < 0 || trow >= field.getHeight() || tcol < 0 || tcol >= field.getWidth()){
        os<<"("<<trow<<", "<<tcol<<")"<<" is out of bound"<<endl;
        return false;
    }else{
        ut = field.getUnit(trow, tcol);
        if(ut == NULL){
            os<< "No unit to attack at "<<"("<<trow<<", "<<tcol<<")"<<endl;
            return false;
        }else{
            field.beattacked(trow, tcol);
            return true;
        }
    }
}

// Perform the attack action
bool performShoot(ostream& os, istream& is, Field& field, Unit* u)
{
    enum Directon{UP, DOWN, LEFT, RIGHT};
    Vector<Directon> dir = {UP, DOWN, LEFT, RIGHT};
    for(int i = 0; i < 4;i++){
        os<< i + 1 << ".";
        switch(dir[i]) {
        case UP:
            os << "UP ";
            break;

        case DOWN:
            os << "DOWN ";
            break;

        case LEFT:
            os << "LEFT ";
            break;

        case RIGHT:
            os << "RIGHT ";
            break;

        default:
            assert(false);
        }
    }
    os<<endl;
    os<< "Please enter the direction: ";
    int dirs;
    if(!querydir(is, os, dirs))
        return false;
    if(dirs < 0 || dirs > 4){
        os<< "Incorrect input! "<< endl;
        return false;
    }
    Unit* ut;
    int trow, tcol;
    switch(dir[dirs - 1]) {
    case UP:
        trow = u ->getRow() - 2;
        tcol = u ->getCol();
        break;

    case DOWN:
        trow = u ->getRow() + 2;
        tcol = u ->getCol();
        break;

    case LEFT:
        trow = u ->getRow();
        tcol = u ->getCol() - 2;
        break;

    case RIGHT:
        trow = u ->getRow();
        tcol = u ->getCol() + 2;
        break;

    default:
        assert(false);
    }
    if(trow < 0 || trow >= field.getHeight() || tcol < 0 || tcol >= field.getWidth()){
        os<<"("<<trow<<", "<<tcol<<")"<<" is out of bound"<<endl;
        return false;
    }else{
        ut = field.getUnit(trow, tcol);
        if(ut == NULL){
            os<< "No unit to attack at "<<"("<<trow<<", "<<tcol<<")"<<endl;
            return false;
        }else{
            field.beattacked(trow, tcol);
            return true;
        }
    }
}

//perform after move attack
bool aftermove(ostream& os, istream& is, Field& field, Unit* u)
{
    Vector<Action> acts;
    if(u -> getSymbol() == "AR" || u -> getSymbol() == "ar"){
        acts = {SHOOT, SKIP};
        for(int i = 0;i < 2;i++){
            os<< i + 1 << ".";
            switch (acts[i]) {
            case SHOOT:
                os << "Shoot ";
                break;

            case SKIP:
                os << "Skip ";
                break;

            default:
                assert(false);
            }
        }
    }else{
        acts = {ATTACK, SKIP};
        for(int i = 0;i < 2;i++){
            os<< i + 1 << ".";
            switch (acts[i]) {
            case ATTACK:
                os << "Attack ";
                break;

            case SKIP:
                os << "Skip ";
                break;

            default:
                assert(false);
            }
        }
    }
    os<<endl;
    os<<"Select your action: ";
    Action act;
    if (!queryAction(os, is, acts, act)) {
        if(!is){
            os << "Invalid action!"<<endl;
            return false;
        }else{
            os << "Invalid action!" << endl;
            return aftermove(os, is, field, u);
        }
    }
    switch (act) {
    case ATTACK:
        if(!performAttack(os, is, field, u))
            return aftermove(os, is, field, u);
        else
            return true;

    case SHOOT:
        if(!performShoot(os, is, field, u))
            return aftermove(os, is, field, u);
        else
            return true;

    case SKIP:
        return true;

    default:
        os << "(Action not implemented yet)" << endl;
        return false;
    }
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

    case ATTACK:
        return performAttack(os, is, field, u);

    case SHOOT:
        return performShoot(os, is, field, u);

    case SKIP:
        return true;

    default:
        os << "(Action not implemented yet)" << endl;
        return false;
    }

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


