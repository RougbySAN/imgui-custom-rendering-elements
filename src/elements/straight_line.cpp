#include "straight_line.h"


// Constructors
straight_line::straight_line(const Point& point1, const Point& point2, const element_id_t unique_id) :  p1(point1),
                                                                                                        p2(point2),
                                                                                                        element(STRAIGHT_LINE, unique_id),
                                                                                                        recorded_hit_point(SL_NONE) {}


straight_line::straight_line(const straight_line& _s) : p1(_s.getPoint1()),
                                                        p2(_s.getPoint2()),
                                                        element(STRAIGHT_LINE, _s.unique_id),
                                                        recorded_hit_point(_s.recorded_hit_point) {}

                                                        
straight_line::straight_line() :    p1(Point()),
                                    p2(Point()),
                                    element(STRAIGHT_LINE, 0),
                                    recorded_hit_point(SL_NONE) {}

// Getter functions for endpoints
const Point& straight_line::getPoint1() const { return p1; }
const Point& straight_line::getPoint2() const { return p2; }

// Setter functions for endpoints
void straight_line::setPoint1(const Point& point) { p1 = point; }
void straight_line::setPoint2(const Point& point) { p2 = point; }



/* --------------------------------PUBLIC OVERRIDEN FUNCTIONS --------------------------------- */

// Drag/Move Whole Element as a shape
void straight_line::drag_element(const float _dx, const float _dy) {
    p1.drag_element(_dx, _dy);
    p2.drag_element(_dx, _dy);
}
void straight_line::move_element(const float _x, const float _y) {
    p1.move_element(_x, _y);
    p2.move_element(_x, _y);
}

// Drag/Move hit point of element and recalculate shape
void straight_line::drag_element_point(const float _dx, const float _dy) {
    switch (locked_hit_point)
    {
    case SL_POINT_1: {
        p1.drag_element(_dx, _dy);
        break;
    }
    case SL_POINT_2: {
        p2.drag_element(_dx, _dy);
        break;
    }
    case SL_BODY: {
        // Nothing
        break;
    }
    case SL_NONE: {
        // Nothing
        break;
    }
    default:
        break;
    }
}
void straight_line::move_element_point(const float _x, const float _y) {
    switch (locked_hit_point)
    {
    case SL_POINT_1: {
        p1.move_element(_x, _y);
        break;
    }
    case SL_POINT_2: {
        p2.move_element(_x, _y);
        break;
    }
    case SL_BODY: {
        // Nothing
        break;
    }
    case SL_NONE: {
        // Nothing
        break;
    }
    default:
        break;
    }
}


// Function to check if a point defined by x and y coordinates is on the line with a given margin
bool straight_line::is_hit(const float x, const float y) {
     // Calculate the distances from the point to line endpoints
    double distance_to_p1 = p1.distanceTo(x, y);
    double distance_to_p2 = p2.distanceTo(x, y);

    // Calculate the distance from the line to the point
    double distance_to_line = std::abs((p2.getY() - p1.getY()) * x -
        (p2.getX() - p1.getX()) * y +
        p2.getX() * p1.getY() - p2.getY() * p1.getX()) /
        std::sqrt(std::pow(p2.getY() - p1.getY(), 2) +
            std::pow(p2.getX() - p1.getX(), 2));

    // Check if the distances are within the margin
    if (distance_to_p1 <= LINE_HIT_DETECTION_MARGIN) {
        recorded_hit_point = SL_POINT_1; 
        return true;
    } else if (distance_to_p2 <= LINE_HIT_DETECTION_MARGIN) {
        recorded_hit_point = SL_POINT_2; 
        return true;
    } else if (distance_to_line <= LINE_HIT_DETECTION_MARGIN) {
        recorded_hit_point = SL_BODY; 
        return true;
    } else {
        recorded_hit_point = SL_NONE; 
        return false;
    }
}

// lock/release hit point on the element
void straight_line::lock_element_hit_point() {
    locked_hit_point = recorded_hit_point;
}
void straight_line::release_element_hit_point() {
    locked_hit_point = SL_NONE;
}

/* -------------------------------- PRIVATE HELPER FUNCTIONS----------------------------------- */

// Function to calculate the length of the line
double straight_line::length() const {
    float dx = p1.getX() - p2.getX();
    float dy = p1.getY() - p2.getY();
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}