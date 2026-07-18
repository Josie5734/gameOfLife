#include "grid.h"

Grid::Grid(int width, int height)
    : w(width),
      h(height),
      cells(width, std::vector<bool>(height, false)),
      buffer(width, std::vector<bool>(height, false)) {};

bool Grid::get(int x, int y) const {
    return cells[x][y];
}

void Grid::set(int x, int y, bool alive) {
    cells[x][y] = alive;
}

void Grid::toggle(int x, int y) {
    cells[x][y] = !cells[x][y];
}

void Grid::clear() {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            cells[x][y] = false;
            buffer[x][y] = false;
        }
    }
}

void Grid::step() {
    for (int y = 0; y < h; y++) { // for each cell
        for (int x = 0; x < w; x++) {
            int n = getNumNeighbours(x, y);            // get number of alive neightbours
            if ((cells[x][y]) && (n == 2 || n == 3)) { // if alive and exactly 2 or 3 neighbours: alive
                buffer[x][y] = true;
            } else if ((!cells[x][y]) && (n == 3)) { // else if not alive but has 3 neightbours: alive
                buffer[x][y] = true;
            } else { // else just dies/stays dead
                buffer[x][y] = false;
            }
        }
    }
    cells.swap(buffer); // swap contents of buffer into cells
    // leaves content of cells in buffer, which should get overwritten next step()
}

int Grid::getNumNeighbours(int x, int y) const {
    int n{0};                                     // number of neighbours
    for (int xx = x - 1; xx <= x + 1; xx++) {     // for cells x-1,x,x+1
        for (int yy = y - 1; yy <= y + 1; yy++) { // for cells y-1,y,y+1
            int wrappedX = (xx + w) % w;          // deal with wrapping around the grid by doing (xx=3: (3+50)%50=3)(xx=-1: (-1+50)%50=49)
            int wrappedY = (yy + h) % h;          // same for yy
            if (!(xx == x && yy == y)) {          // for cells that are not x,y (the cell being checked)
                if (cells[wrappedX][wrappedY]) {  // if cell is alive
                    n += 1;                       // iterate neighbour count
                }
            }
        }
    }
    return n; // return neighbour count
}
