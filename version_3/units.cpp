#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

Unit::Unit(UnitType u, bool sd, int row, int col)
    : type(u), side(sd), urow(row), ucol(col)
    {}

bool Unit::getSide() const
{
    return side;
}

int Unit::getMovPoints() const
{
    switch (type) {
    case FOOTMAN:
        return 3;
    case KNIGHT:
        return 5;
    case ARCHER:
        return 3;
    }
    //assert (false);
}

// Get the coordinate of the current unit
int Unit::getRow() const
{
    return urow;
}

int Unit::getCol() const
{
    return ucol;
}

// Set the coordinates
void Unit::setCoord(int row, int col)
{
    urow = row;
    ucol = col;
}

// Function for symbols
string getSymbolFootman(bool side)
{
    if (side)
        return "FT";
    else
        return "ft";
}

// Function for symbols
string getSymbolKnight(bool side)
{
    if (side)
        return "KN";
    else
        return "kn";
}

// Function for symbols
string getSymbolArcher(bool side)
{
    if (side)
        return "AR";
    else
        return "ar";
}

// Get the symbol of the unit
string Unit::getSymbol() const
{
    switch (type) {
    case FOOTMAN:
        return getSymbolFootman(side);
    case KNIGHT:
        return getSymbolKnight(side);
    case ARCHER:
        return getSymbolArcher(side);
    }
    //assert (false);
}

/* Terrain */

Terrain::Terrain(TerrainType t,int col, int row)
    : type(t), trow(row), tcol(col)
    {}

// Get the coordinate of the current terrain
int Terrain::getterrainRow() const
{
    return trow;
}

int Terrain::getterrainCol() const
{
    return tcol;
}

// Set the coordinates
void Terrain::setterrainCoord(int row, int col)
{
    trow = row;
    tcol = col;
}

// Get the symbol of the terrain
string Terrain::getterrainSymbol() const
{
    switch (type) {
    case Mountain:
        return "/\\";
    case Water:
        return "~~";
    case Forest:
        return "$ ";
    }
    //assert (false);
}


