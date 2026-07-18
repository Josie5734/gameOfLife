#pragma once
#include <vector>

class Grid {
  public:
    Grid(int width, int height);

    bool get(int x, int y) const;
    void set(int x, int y, bool alive);
    void toggle(int x, int y);
    void clear();
    void step();

    int width() const { return w; }
    int height() const { return h; }

  private:
    int getNumNeighbours(int x, int y) const;

    int w, h;
    std::vector<std::vector<bool>> cells;
    std::vector<std::vector<bool>> buffer;
};
