#include "point.h"


// Constructor
Point::Point(const float xCoord, const float yCoord) : x(xCoord), y(yCoord) {}
Point::Point() : x(0), y(0) {}

// Getters and setters for x and y coordinates
float Point::getX() const { return x; }
void Point::setX(const float newX) { x = newX; }

float Point::getY() const { return y; }
void Point::setY(const float newY) { y = newY; }

void Point::drag_point(const float _dx, const float _dy) {
    x += _dx;
    y += _dy;
}

void Point::update_point(const float _x, const float _y) {
    x = _x;
    y = _y;
}

// Function to calculate distance between this point and another point
double Point::distanceTo(const Point& other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

// Function to calculate distance between this point and another point
double Point::distanceTo(const float _x, const float _y) const {
    float dx = x - _x;
    float dy = y - _y;
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

// Hit detection with margin
bool Point::isHit(const Point& other) const {
    return distanceTo(other) <= POINT_HIT_DETECTION_MARGIN;
}

// Hit detection with margin for x and y coordinates
bool Point::isHit(const float _x, const float _y) const {
    return distanceTo(_x, _y) <= POINT_HIT_DETECTION_MARGIN;
}
