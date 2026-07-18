#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "consts.h"
#include "grid.h"
#include "gui.h"
#include "raygui.h"
#include "simulation.h"

// create grid object
Grid grid{GRID_WIDTH, GRID_HEIGHT};

Simulation simulation{};

// variables
Vector2 mousePos{0.0f, 0.0f}; // mouse position, updated on click

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
        float deltaTime = GetFrameTime();  // get time since last frame
        simulation.updateTimer(deltaTime); // update simulation timer
        if (simulation.shouldStep()) {     // step if should step
            grid.step();
        };

        // inputs
        mousePos = GetMousePosition();                               // get mousepos
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT); // get pressed state

        // check buttons
        // toggle running on space keypress
        if (IsKeyPressed(KEY_SPACE)) {
            simulation.toggleRunning();
        }

        // mouse clicks to toggle squares
        if (mousePressed and CheckCollisionPointRec(mousePos, GRID_RECTANGLE)) {
            int x = (mousePos.x - PADDING) / CELL_SIZE; // get mousePos in terms of cells
            int y = (mousePos.y - PADDING) / CELL_SIZE;
            grid.toggle(x, y);
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
