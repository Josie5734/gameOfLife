#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "consts.h"
#include "grid.h"
#include "gui.h"
#include "raygui.h"
#include "simulation.h"
#include <iostream>

// create grid object
Grid grid{GRID_WIDTH, GRID_HEIGHT};

Simulation simulation{};

// variables
Vector2 mousePos{0.0f, 0.0f}; // mouse position, updated on click

int main() {
    // screen size
    const int screenWidth{static_cast<int>((PADDING * 2) + GRID_RECTANGLE.width)};                             // padding either side + grid width
    const int screenHeight{static_cast<int>((PADDING * 2) + GRID_RECTANGLE.height + (BUTTON_SIZE + PADDING))}; // same as width + height of buttons + pad between grid and button

    InitWindow(screenWidth, screenHeight, "Game of Life - raylib test");
    SetTargetFPS(60);

    // set the raygui styling
    setStyles();

    while (!WindowShouldClose()) {
        //
        // update
        //
        float deltaTime = GetFrameTime();  // get time since last frame
        simulation.updateTimer(deltaTime); // update simulation timer
        if (simulation.shouldStep()) {     // step if should step
            grid.step();
        };

        // inputs
        mousePos = GetMousePosition(); // get mousepos
        // which mouse buttons are held down
        bool mouseLeft = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        bool mouseRight = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

        // check buttons
        // toggle running on space keypress
        if (IsKeyPressed(KEY_SPACE)) {
            simulation.toggleRunning();
        }

        // if mouse is in the grid and (mouseLeft XOR mouseRight) are down (not both)
        if ((mouseLeft ^ mouseRight) && CheckCollisionPointRec(mousePos, GRID_RECTANGLE)) {
            int x = (mousePos.x - PADDING) / CELL_SIZE; // get mousePos in terms of cells
            int y = (mousePos.y - PADDING) / CELL_SIZE;
            if (mouseLeft) {           // if left click
                grid.set(x, y, true);  // make cells alive
            } else if (mouseRight) {   // if right click
                grid.set(x, y, false); // make cells dead
            }
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
        drawButtons(grid, simulation);
        // draw buttons

        EndDrawing();
        //
        // draw
        //
    }

    CloseWindow();
    return 0;
}
