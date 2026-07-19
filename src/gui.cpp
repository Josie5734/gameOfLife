#include "gui.h"
#include "consts.h"
#include "raygui.h"
#include "raylib.h"
#include "simulation.h"
#include <format>
#include <string>

// styling
void setStyles() {
    GuiEnableTooltip();
}

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
void drawButtons(Grid &grid, Simulation &sim) {
    float x{UI_START_X};                     // track the x value for the button (iterated after each button)
    float y{static_cast<float>(UI_START_Y)}; // y value
    float spacing{BUTTON_SIZE + PADDING};

    // play/pause
    GuiSetTooltip("Play/Pause");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(sim.running ? ICON_PLAYER_PAUSE : ICON_PLAYER_PLAY, nullptr))) {
        sim.toggleRunning();
    }
    x += spacing;

    // step once
    GuiSetTooltip("Step 1 Cycle");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_STEP_OVER, nullptr))) {
        if (sim.running) { // disable running
            sim.toggleRunning();
        }
        grid.step(); // do one cycle
    }
    x += spacing;

    // clear
    GuiSetTooltip("Clear Grid");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_EMPTYBOX_SMALL, nullptr))) {
        if (sim.running) {
            sim.toggleRunning();
        }
        grid.clear();
    }
    x += spacing;

    // big gap
    x += spacing * 5;

    // decrease simulation speed
    GuiSetTooltip("Decrease simulation speed");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_ARROW_LEFT_FILL, nullptr))) {
        sim.decreaseUpdateSpeed();
    }
    x += spacing;

    // output simulation speed
    DrawRectangleRec((Rectangle){x, y, BUTTON_SIZE * 2, BUTTON_SIZE}, GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL)));
    DrawRectangleLinesEx((Rectangle){x, y, BUTTON_SIZE * 2, BUTTON_SIZE}, 2, GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL)));
    std::string label = std::format("{:.2f} c/s", sim.getUpdateSpeed());
    int textWidth = MeasureText(label.c_str(), 20); // measure textwidth
    DrawText(label.c_str(), x + static_cast<float>((BUTTON_SIZE * 2 - textWidth)) / 2, y + static_cast<float>((BUTTON_SIZE - 20)) / 2, 20, BLACK);
    x += spacing + BUTTON_SIZE;

    // increase simulation speed
    GuiSetTooltip("Increase Simulation Speed");
    if (GuiButton((Rectangle){x, y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_ARROW_RIGHT_FILL, nullptr))) {
        sim.increaseUpdateSpeed();
    }
    x += spacing;
}
