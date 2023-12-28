#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <string>
#include "field.h"

/* Type of Units */
enum UnitType {FOOTMAN, KNIGHT};

/* Class for units */
class Unit {
public:
    Unit(UnitType u, bool sd, int row, int col);

    // Check which side the unit belongs to
    bool getSide() const;

    // Get the coordinate of the current unit
    int getRow() const;
    int getCol() const;

    // Set the coordinates
    void setCoord(int row, int col);

    // Get your movement point
    int getMovPoints() const;

    // Return a displayable character
    // Depending on the player you may get different results
    std::string getSymbol() const;

private:
    UnitType type;
    bool side;
    int urow, ucol;
};

/* Type of Units */
enum TerrainType {Mountain, Water, Forest};

/* Class for terrains */
class Terrain {
public:
    Terrain(TerrainType t,int col, int row);

    // Get the coordinate of the current terrain
    int getterrainRow() const;
    int getterrainCol() const;

    // Set the coordinates
    void setterrainCoord(int row, int col);

    // Return a terrain symbol
    std::string getterrainSymbol() const;

private:
    TerrainType type;
    int trow, tcol;
};

#endif // UNITS_H_INCLUDED
