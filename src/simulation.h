#pragma once

class Simulation {
  public:
    Simulation();
    bool running{false}; // is simulation running

    // toggle if the simulation is running
    void toggleRunning();

    // if the simulation should step
    bool shouldStep();

    // update timer
    void updateTimer(float delta);

    // get update speed in  cycles/second
    float getUpdateSpeed();

    // increase update interval speed
    void increaseUpdateSpeed();

    // decrease update interval speed
    void decreaseUpdateSpeed();

  private:
    float timer{0.0f};          // timer tracking for updates
    float updateInterval{1.0f}; // how often to update (in seconds)
    float minInterval{0.1f};    // minium speed (fastest) is every 0.1 seconds
    float maxInterval{5.0f};    // max speed (slowest) is every 5 seconds
    float stepAmount{0.1f};     // how much to increase/decrease interval by
};
