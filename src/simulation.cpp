#include "simulation.h"
#include <algorithm>

Simulation::Simulation() {};

void Simulation::toggleRunning() {
    running = !running;
}

bool Simulation::shouldStep() {
    if (running && timer >= updateInterval) {
        timer -= updateInterval; // only remove interval to maintain overflow time
        return true;
    }
    return false;
}

void Simulation::updateTimer(float delta) {
    if (running) { // only update if running to avoid timer buildup
        timer += delta;
    }
}

float Simulation::getUpdateSpeed() {
    return 1.0f / updateInterval;
}

void Simulation::increaseUpdateSpeed() {
    // clamp to minimum speed
    updateInterval = std::max(updateInterval - stepAmount, minInterval);
}

void Simulation::decreaseUpdateSpeed() {
    // clamp to maximum speed
    updateInterval = std::min(updateInterval + stepAmount, maxInterval);
}
