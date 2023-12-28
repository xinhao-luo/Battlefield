#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <iostream>
#include "field.h"

// Load map
Field* loadMap(std::istream& is);

// Add units
bool addFootman(Field& field, bool side, int row, int col);

// Add units
bool addKnight(Field& field, bool side, int row, int col);

//Add Mountain
bool addMountain(Field& field, int row, int col);

//Add Water
bool addWater(Field& field, int row, int col);

//Add Forest
bool addForest(Field& field, int row, int col);

// Main loop for playing the game
void play(Field& field, std::istream& is, std::ostream& os);

// Display the battle field
void displayField(std::ostream& os, const Field& field,
                  const Grid<bool>& grd = Grid<bool>());

#endif // ENGINE_H_INCLUDED
