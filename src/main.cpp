#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
// #include <functional>
// #include <vector>
#include "consts.h"
#include "game.h"
#include "gui.h"

// non const values
bool grid[GRID_WIDTH][GRID_HEIGHT]{{false}};   // the cell grid
bool buffer[GRID_WIDTH][GRID_HEIGHT]{{false}}; // a buffer of the cell grid used for updating
Vector2 mousePos{0.0f, 0.0f};                  // mouse position, updated on click
float deltaTime{0.0f};                         // delta time for time between each frame
float timer{0.0f};                             // timer for updating
const float UPDATE_INTERVAL{1.0f};             // interval to update at (in seconds)
bool running{false};                           // is the sim running

// make cell alive/unalive (used only for mouse clicks, not during cycle calculation)
void swapCell(int x, int y) { grid[x][y] = !grid[x][y]; }

// get number of live neighbours of cell at x,y
int getNumNeighbours(int x, int y) {
    int n{0};                                                // number of neighbours
    for (int xx = x - 1; xx <= x + 1; xx++) {                // for cells x-1,x,x+1
        for (int yy = y - 1; yy <= y + 1; yy++) {            // for cells y-1,y,y+1
            int wrappedX = (xx + GRID_WIDTH) % GRID_WIDTH;   // deal with wrapping around the grid by doing (xx=3: (3+50)%50=3)(xx=-1: (-1+50)%50=49)
            int wrappedY = (yy + GRID_HEIGHT) % GRID_HEIGHT; // same for yy
            if (!(xx == x && yy == y)) {                     // for cells that are not x,y (the cell being checked)
                if (grid[wrappedX][wrappedY]) {              // if cell is alive
                    n += 1;                                  // iterate neighbour count
                }
            }
        }
    }
    return n; // return neighbour count
}

// swap the buffer grid into the game grid and reset the buffer
void swapBuffer() {
    for (int y = 0; y < GRID_HEIGHT; y++) { // for each cell
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[x][y] = buffer[x][y]; // put value of buffer into cell
            buffer[x][y] = false;      // reset buffer value
        }
    }
}

// clear the grid and the buffer
void clearGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[x][y] = false;
            buffer[x][y] = false;
        }
    }
}

// toggle the running state
void toggleRunning() {
    running = !running;
}

// cycle the game, update the grid
void cycle() {
    for (int y = 0; y < GRID_HEIGHT; y++) { // for each cell
        for (int x = 0; x < GRID_WIDTH; x++) {
            int n = getNumNeighbours(x, y);           // get number of alive neightbours
            if ((grid[x][y]) && (n == 2 || n == 3)) { // if alive and exactly 2 or 3 neighbours: alive
                buffer[x][y] = true;
            } else if ((!grid[x][y]) && (n == 3)) { // else if not alive but has 3 neightbours: alive
                buffer[x][y] = true;
            } else { // else just dies/stays dead
                buffer[x][y] = false;
            }
        }
    }
    swapBuffer(); // replace grid with buffer grid
}

int main() {
    // screen size
    const int screenWidth{static_cast<int>((PADDING * 2) + GRID_RECTANGLE.width)};                     // padding either side + grid width
    const int screenHeight{static_cast<int>((PADDING * 2) + GRID_RECTANGLE.height + (PADDING + 100))}; // same as width + height of buttons + pad between grid and button

    InitWindow(screenWidth, screenHeight, "Game of Life - raylib test");
    SetTargetFPS(60);

    GuiEnableTooltip(); // enable button tooltips

    // create buttons

    while (!WindowShouldClose()) {
        //
        // update
        //
        deltaTime = GetFrameTime(); // get time since last frame
        timer += deltaTime;         // add to timer

        // when timer reaches update interval, reset and if running, do cycle
        if (timer >= UPDATE_INTERVAL) {
            if (running) { // if sim is running
                cycle();   // update grid
            }
            timer -= UPDATE_INTERVAL; // subtract interval to keep extra time
                                      // e.g timer = 1.02, so subtract interval(1.0), still keeps the 0.02
                                      // seconds for next update cycle for consistency
        }

        // inputs
        mousePos = GetMousePosition();                               // get mousepos
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT); // get pressed state

        // check buttons
        // toggle running on space keypress
        if (IsKeyPressed(KEY_SPACE)) {
            toggleRunning();
        }

        // mouse clicks to toggle squares
        if (mousePressed and CheckCollisionPointRec(mousePos, GRID_RECTANGLE)) {
            int x = (mousePos.x - PADDING) / CELL_SIZE; // get mousePos in terms of cells
            int y = (mousePos.y - PADDING) / CELL_SIZE;
            swapCell(x, y);
        }
        //
        // update
        //

        //
        // draw
        //
        BeginDrawing();
        ClearBackground(WHITE);

        // draw grid
        drawGameGrid(grid);
        // draw grid

        // draw buttons
        drawButtons(running);
        // draw buttons

        EndDrawing();
        //
        // draw
        //
    }

    CloseWindow();
    return 0;
}
