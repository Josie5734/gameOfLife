#include "gui.h"
#include "consts.h"
#include "game.h"
#include "raygui.h"
#include "raylib.h"

// draw the grid
void drawGameGrid(Grid &grid) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid.get(x, y)) { // draw a green rectangle if cell is alive
                DrawRectangle(PADDING + x * CELL_SIZE, PADDING + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
            }
            // always draw an outline
            DrawRectangleLines(PADDING + x * CELL_SIZE, PADDING + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
        }
    }
    DrawRectangleLinesEx(GRID_RECTANGLE, 2, BLACK); // draw outline of whole grid so edges stand out
}

// draw the buttons
void drawButtons(Grid &grid, bool running) {
    float x{UI_START_X};                     // track the x value for the button (iterated after each button)
    float y{static_cast<float>(UI_START_Y)}; // y value

    // play/pause
    GuiSetTooltip("Play/Pause");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(running ? ICON_PLAYER_PAUSE : ICON_PLAYER_PLAY, nullptr))) {
        toggleRunning();
    }
    x += BUTTON_SIZE + PADDING;

    // step once
    GuiSetTooltip("Step 1 Cycle");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_STEP_OVER, nullptr))) {
        if (running) { // disable running
            toggleRunning();
        }
        grid.step(); // do one cycle
    }
    x += BUTTON_SIZE + PADDING;

    // clear
    GuiSetTooltip("Clear Grid");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_EMPTYBOX_SMALL, nullptr))) {
        if (running) {
            toggleRunning();
        }
        grid.clear();
    }
    x += BUTTON_SIZE + PADDING;
}
