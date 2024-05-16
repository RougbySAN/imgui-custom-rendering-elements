#include "point.h"

// Constructor
Point::Point(const float xCoord, const float yCoord, const element_id_t _unique_id) : x(xCoord), y(yCoord), element(POINT, Color(), _unique_id) {}
Point::Point(const Point& _p) : x(_p.getX()),  y(_p.getY()), element(POINT, Color(), _p.unique_id) {}
Point::Point() : x(0), y(0), element(POINT, Color(), 0) {}


// Getters and setters for x and y coordinates
float Point::getX() const { return x; }
void Point::setX(const float newX) { x = newX; }

float Point::getY() const { return y; }
void Point::setY(const float newY) { y = newY; }


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


/* --------------------------------PUBLIC OVERRIDEN FUNCTIONS --------------------------------- */

// Hit detection with margin for x and y coordinates
bool Point::is_hit(const float _x, const float _y) {
    return distanceTo(_x, _y) <= POINT_HIT_DETECTION_MARGIN;
}

// Drag/Move Whole Element as a shape
void Point::drag_element(const float _dx, const float _dy) {
    drag_point(_dx, _dy);
}
void Point::move_element(const float _x, const float _y) {
    update_point(_x, _y);
}

// Drag/Move hit point of element and recalculate shape
void Point::drag_element_point(const float _dx, const float _dy) {
    drag_point(_dx, _dy);
}
void Point::move_element_point(const float _x, const float _y) {
    update_point(_x, _y);
}


/* -------------------------------- PRIVATE HELPER FUNCTIONS----------------------------------- */

void Point::drag_point(const float _dx, const float _dy) {
    x += _dx;
    y += _dy;
}

void Point::update_point(const float _x, const float _y) {
    x = _x;
    y = _y;
}
