#include <string>
#include <cassert>
#include <iomanip>
#include "field.h"
#include "units.h"
using namespace std;


/* Battle field */

// Constructor
Field::Field(int h, int w)
    : units(h, w), terrains(h, w)
{
    assert (h > 0 && h <=20 && w > 0 && w <= 20);
}

// Get the height of the field
int Field::getHeight() const
{
    return units.numRows();
}

// Get the width of the field
int Field::getWidth() const
{
    return units.numCols();
}

// In bounds check
bool Field::inBounds(int row, int col) const
{
    return units.inBounds(row, col)&&terrains.inBounds(row, col);
}

// Get the unit at row and col
Unit* Field::getUnit(int row, int col) const
{
    assert (units.inBounds(row, col));

    return units[row][col];
}

// Get the terrain at row and col
Terrain* Field::getTerrain(int row, int col) const
{
    assert (terrains.inBounds(row, col));

    return terrains[row][col];
}

// Set a new unit at row and col
// Return false if the coordinate is already occupied
// or unreachable
bool Field::setUnit(int row, int col, Unit* u)
{
    assert (units.inBounds(row, col));

    if (units[row][col] == NULL) {
        u->setCoord(row, col);
        units[row][col] = u;
        return true;
    }
    else
        return false;
}

//set a unit to be NULL
void Field::beattacked(int row, int col){
    assert (units.inBounds(row, col));
    units[row][col] = NULL;
}

// Set a new terrain at row and col
// Return false if the coordinate is already occupied
// or unreachable
bool Field::setTerrain(int row, int col, Terrain* t)
{
    assert (terrains.inBounds(row, col));

    if (terrains[row][col] == NULL) {
        t->setterrainCoord(row, col);
        terrains[row][col] = t;
        return true;
    }
    else
        return false;
}

// Move a unit to a different coordinate
// Return false if the source does not exist or
// the target coordinate is already occupied or
// unreachable
bool Field::moveUnit(int srow, int scol, int trow, int tcol)
{
    assert (units.inBounds(srow, scol));
    assert (units.inBounds(trow, tcol));
    if(srow != trow || scol != tcol){
        Unit* u = units[srow][scol];
        u->setCoord(trow, tcol);
        units[trow][tcol] = u;
        units[srow][scol] = NULL;
        return true;
    }else
        return true;

}

//check if it has a unit
bool Field::Ahasunit() const
{
    for(Unit* unit : units)
        if(unit != NULL && unit -> getSide())
            return true;
    return false;
}

bool Field::Bhasunit() const
{
    for(Unit* unit : units)
        if(unit != NULL && !unit -> getSide())
            return true;
    return false;
}

// Reclaim all the units
Field::~Field()
{
    for (int i = 0; i < units.numRows(); i++){
        for (int j = 0; j < units.numCols(); j++){
            if (units[i][j] != NULL)
                delete units[i][j];
            if (terrains[i][j] != NULL)
                delete terrains[i][j];
        }
    }
}
