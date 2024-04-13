#pragma once
#include "private.h"
#include "straight_line.h"

#include <vector>

class orthogonal_line : public element {
private:
    Point p1; // First endpoint of the line
    Point p2; // Second endpoint of the line
    Point mid_point; // Midpoint of shortest path

    exit_direction_t p1_exit;
    exit_direction_t p2_exit;

    orthogonal_line_points_t recorded_hit_point;
    orthogonal_line_points_t locked_hit_point;

public:
    // Constructor
    orthogonal_line(const Point& point1, const Point& point2, const element_id_t _unique_id = 0);
    orthogonal_line(const orthogonal_line& _o);
    orthogonal_line(const float x1, const float y1, const float x2, const float y2, const element_id_t _unique_id = 0);

    // Getter functions for endpoints
    const Point& getPoint1() const;
    const Point& getPoint2() const;

    // Getter and setter function for midpoint
    const Point& getMidpoint() const;
    void setMidpoint(const Point& point);

    // Setter functions for endpoints
    void setPoint1(const Point& point);
    void setPoint2(const Point& point);

    // Function to calculate the length of the line
    double length() const;

    std::vector<straight_line> GetLineList();

    void update_midpoint();

    /* PUBLIC OVERRIDEN FUNCTIONS FROM ELEMENT BASE CLASS*/

    // Hit Detection for orthogonal line
    bool is_hit(const float _x, const float _y) override;


    // Drag/Move Whole Element as a shape
    void drag_element(const float _dx, const float _dy) override;
    void move_element(const float _x, const float _y) override;

    // Drag/Move hit point of element and recalculate shape
    void drag_element_point(const float _dx, const float _dy) override;
    void move_element_point(const float _x, const float _y) override;

    // lock/release hit point on the element
    void lock_element_hit_point() override;
    void release_element_hit_point() override;

    void drag_point (const float _x, const float _y, enum orthogonal_line_points point);

private:
    // Private helper functions

    void dragMidpoint(const float dx, const float dy);

    void moveMidpoint(const float _x, const float _y);

    void drag_point1(const float dx, const float dy);

    void drag_point2 (const float dx, const float dy);

    bool exit_point_is_hit(const float _x, const float _y);

    bool mid_point_is_hit(const float _x, const float _y);

    void calculateMidpoint();

    void calculateLineExits(const float dx, const float dy);

    int clamp_value(const float val, const float limit_upper, const float limit_lower);
};