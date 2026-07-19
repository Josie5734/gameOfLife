#pragma once
#include "grid.h"
#include "simulation.h"

// set styling for raygui ui elements
void setStyles();

// draw the grid of cells
void drawGameGrid(Grid &grid);

// draw the ui buttons along the bottom
void drawButtons(Grid &grid, Simulation &sim);
