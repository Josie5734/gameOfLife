#include "raylib.h"
#include <functional>
#include <string>

// grid and cell size
const int GRID_WIDTH{50};
const int GRID_HEIGHT{50};
const int CELL_SIZE{10};

// non const values
bool grid[GRID_WIDTH][GRID_HEIGHT]{{false}};   // the cell grid
bool buffer[GRID_WIDTH][GRID_HEIGHT]{{false}}; // a buffer of the cell grid used for updating
Vector2 mousePos{0.0f, 0.0f};                  // mouse position, updated on click
float deltaTime{0.0f};                         // delta time for time between each frame
float timer{0.0f};                             // timer for updating
const float UPDATE_INTERVAL{1.0f};             // interval to update at (in seconds)
bool running{false};                           // is the sim running

// draw the grid
void drawGame() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[x][y]) { // draw a green rectangle if cell is alive
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
            }
            // always draw an outline
            DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
        }
    }
}

// make cell alive/unalive (used only for mouse clicks, not during cycle
// calculation)
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

// buttons
class Button {
  public:
    // construct
    Button(float x, float y, float w, float h, std::string text, std::function<void()> onClick) : x(x), y(y), w(w), h(h), text(text), onClick(onClick) {};

    // values
    bool isHovered{false}; // if button is being hovered over
    bool isClicked{false}; // if button is clicked
    float x{0};            // position and sizing
    float y{0};
    float w{0};
    float h{0};
    Rectangle bounds{x, y, w, h};  // put position/sizing into a Rectangle to describe the "hitbox" of the button
    std::string text{"default"};   // the text on the button
    std::function<void()> onClick; // onClick function for when clicked

    // check if button is hovered
    void checkHovered(Vector2 mousePosition) {
        isHovered = CheckCollisionPointRec(mousePosition, bounds);
    }

    // check if button is clicked
    void checkClicked() {
        isClicked = isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        if (isClicked && onClick) {
            onClick();
        }
    }

    // update button
    void update(Vector2 mPos) {
        checkHovered(mPos);
        checkClicked();
    }

    // draw button
    void draw() {
        Color color = isHovered ? LIGHTGRAY : GRAY;
        DrawRectangleRec(bounds, color);
        DrawText(text.c_str(), x + 10, y + 10, 20, BLACK);
    }
};

int main() {
    // create buttons
    Button start(CELL_SIZE * 1, (GRID_HEIGHT * CELL_SIZE) + (CELL_SIZE * 1), 150, 40, "Start/Stop", []() { running = !running; });

    // screen size
    const int screenWidth{GRID_WIDTH * CELL_SIZE}; // full grid size + buttons and some padding
    const int screenHeight{static_cast<int>((GRID_HEIGHT * CELL_SIZE) + (start.h + CELL_SIZE))};

    InitWindow(screenWidth, screenHeight, "Game of Life - raylib test");
    SetTargetFPS(60);

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
        mousePos = GetMousePosition(); // get mousepos

        // check buttons
        start.update(mousePos);

        // mouse clicks to toggle squares
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int x = mousePos.x / CELL_SIZE; // get mousePos in terms of cells
            int y = mousePos.y / CELL_SIZE;
            if (y <= GRID_HEIGHT * CELL_SIZE) { // if within the grid, toggle the cell its over
                swapCell(x, y);
            }
        }

        // toggle running on space keypress
        if (IsKeyPressed(KEY_SPACE)) {
            running = !running;
        }
        //
        // update
        //

        //
        // draw
        //
        BeginDrawing();
        ClearBackground(WHITE);
        drawGame();
        start.draw();
        EndDrawing();
        //
        // draw
        //
    }

    CloseWindow();
    return 0;
}
