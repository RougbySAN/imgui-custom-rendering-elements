#pragma once

#include "point.h"
#include "private.h"

class straight_line {
private:
    Point p1;
    Point p2;

public:
    uint16_t unique_id;


public:
    // Constructors
    straight_line(const Point& point1, const Point& point2, const uint16_t unique_id = 0);
    straight_line();

    // Getter functions for endpoints
    const Point& getPoint1() const;
    const Point& getPoint2() const;

    // Setter functions for endpoints
    void setPoint1(const Point& point);
    void setPoint2(const Point& point);

    // Function to calculate the length of the line
    double length() const;

    // Function to check if a point is on the line with a given margin
    bool is_point_on_line(const Point& point, double margin) const;

    // Function to check if a point defined by x and y coordinates is on the line with a given margin
    bool is_point_on_line(const float x, const float y, double margin) const;
};