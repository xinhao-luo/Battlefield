#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <ostream>
#include <Grid.h>

// Forward declaration of the class of units
class Unit;

// Forward declaration of the class of terrains
class Terrain;

/* Battle field */
class Field {
public:
    // Constructor
    Field(int h, int w);

    // Get the height and width of the field
    int getHeight() const;
    int getWidth() const;

    // Check if the row and column is in bounds
    bool inBounds(int row, int col) const;

    // Display the field on the out stream os
    void display(std::ostream& os, const Grid<bool>& grd = Grid<bool>()) const;

    // Get the unit at row and col
    Unit* getUnit(int row, int col) const;

    //Get the terrain at row and col
    Terrain* getTerrain(int row, int col) const;

    // Set a new unit at row and col
    // Return false if the coordinate is already occupied
    bool setUnit(int row, int col, Unit* u);

    //set a unit to be NULL
    void beattacked(int row, int col);

    // Set a new terrain at row and col
    // Return false if the coordinate is already occupied
    bool setTerrain(int row, int col, Terrain* t);

    // Move a unit to a different coordinate
    // Return false if the target coordinate is already occupied
    bool moveUnit(int srow, int scol, int trow, int tcol);

    //check if it has a unit
    bool Ahasunit() const;
    bool Bhasunit() const;

    // Destructor
    ~Field();

private:
    // Store the units
    Grid<Unit*> units;
    //store the terrains
    Grid<Terrain*> terrains;
};

#endif // FIELD_H_INCLUDED
