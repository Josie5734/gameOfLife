#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "consts.h"
#include "game.h"
#include "grid.h"
#include "gui.h"
#include "raygui.h"

// create grid object
Grid grid(GRID_WIDTH, GRID_HEIGHT);

// variables
Vector2 mousePos{0.0f, 0.0f};      // mouse position, updated on click
float deltaTime{0.0f};             // delta time for time between each frame
float timer{0.0f};                 // timer for updating
const float UPDATE_INTERVAL{1.0f}; // interval to update at (in seconds)
bool running{false};               // is the sim running

// toggle the running state
void toggleRunning() {
    running = !running;
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
            if (running) {   // if sim is running
                grid.step(); // update grid
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
        drawButtons(grid, running);
        // draw buttons

        EndDrawing();
        //
        // draw
        //
    }

    CloseWindow();
    return 0;
}
