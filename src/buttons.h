#pragma once

#include "raylib.h"
#include <functional>
#include <string>

// index of each button for direct access
enum buttonIndex { PLAY_PAUSE = 0 };

// buttons
class Button {
  public:
    // construct
    Button(float x, float y, float w, float h, std::function<std::string()> label, std::function<void()> onClick) : x(x), y(y), w(w), h(h), label(label), onClick(onClick) {};

    // values
    bool isHovered{false}; // if button is being hovered over
    bool isClicked{false}; // if button is clicked
    float x{0};            // position and sizing
    float y{0};
    float w{0};
    float h{0};
    Rectangle bounds{x, y, w, h};       // put position/sizing into a Rectangle to describe the "hitbox" of the button
    std::function<std::string()> label; // the button label, stored as function to allow changing via onclick events
    std::function<void()> onClick;      // onClick function for when clicked

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
        // color based on ifHovered or not
        Color color = isHovered ? LIGHTGRAY : GRAY;
        DrawRectangleRec(bounds, color);
        std::string text = label(); // calculate current text
        DrawText(text.c_str(), x + 10, y + 10, 20, BLACK);
    }
};
