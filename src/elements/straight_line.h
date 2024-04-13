#pragma once

#include "point.h"
#include "private.h"

class straight_line : public element{
private:
    Point p1;
    Point p2;

    straight_line_points_t recorded_hit_point;
    straight_line_points_t locked_hit_point; 

public:
    // Constructors
    straight_line(const Point& point1, const Point& point2, const element_id_t unique_id = 0);
    straight_line(const straight_line& _s);
    straight_line();

    // Getter functions for endpoints
    const Point& getPoint1() const;
    const Point& getPoint2() const;

    // Setter functions for endpoints
    void setPoint1(const Point& point);
    void setPoint2(const Point& point);


    /* Public overriden functions from element base class*/

    // Function to check if a point defined by x and y coordinates is on the line with a given margin
    bool is_hit(const float x, const float y) override;

    // Drag/Move Whole Element as a shape
    void drag_element(const float _dx, const float _dy) override;
    void move_element(const float _x, const float _y) override;

    // Drag/Move hit point of element and recalculate shape
    void drag_element_point(const float _dx, const float _dy) override;
    void move_element_point(const float _x, const float _y) override;

    // lock/release hit point on the element
    void lock_element_hit_point() override;
    void release_element_hit_point() override;

private:

    /* PRIVATE HELPER FUNCTIONS */

    // Function to calculate the length of the line
    double length() const;

};