#pragma once
#include "raylib.h"

// grid, cell size and ui values
const int GRID_WIDTH{50}; // number of cells
const int GRID_HEIGHT{50};
const int CELL_SIZE{15};              // size of cells in pixels
const int BUTTON_SIZE{CELL_SIZE * 3}; // size of buttons

const int PADDING{CELL_SIZE}; // padding space from edges or between buttons
// the rectangle for the grid, used for sizing and click hitboxing
const Rectangle GRID_RECTANGLE{PADDING, PADDING, GRID_WIDTH *CELL_SIZE, GRID_HEIGHT *CELL_SIZE};
const int UI_START_X{PADDING}; // the start of the ui in x,y
const int UI_START_Y{static_cast<int>(PADDING * 2 + GRID_RECTANGLE.height)};
