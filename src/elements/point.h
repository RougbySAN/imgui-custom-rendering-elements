#pragma once

#include <math.h>
#include "private.h"


class Point {
private:
    float x; // X-coordinate of the point
    float y; // Y-coordinate of the point

public:
    uint16_t unique_id;

public:
    // Constructor
    Point(const float xCoord, const float yCoord);
    Point();

    // Getters and setters for x and y coordinates
    float getX() const;
    void setX(const float newX);
    
    float getY() const;
    void setY(const float newY);

    // Change and Update point
    void drag_point(const float _dx, const float _dy);
    void update_point(const float _x, const float _y);

    // Function to calculate distance between this point and another point
    double distanceTo(const Point& other) const;
    double distanceTo(const float _x, const float _y) const;

    // Hit detection with margin
    bool isHit(const Point& other) const;
    bool isHit(const float _x, const float _y) const;
};