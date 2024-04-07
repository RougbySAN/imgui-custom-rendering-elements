#pragma once
#include "private.h"
#include "straight_line.h"

#include <vector>

class orthogonal_line {
private:
    Point p1; // First endpoint of the line
    Point p2; // Second endpoint of the line
    Point mid_point; // Midpoint of shortest path

    enum exit_direction p1_exit;
    enum exit_direction p2_exit;

public:
    // Constructor
    orthogonal_line(const Point& point1, const Point& point2);

    orthogonal_line(const float x1, const float y1, const float x2, const float y2);

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

    bool mid_point_is_hit(const float _x, const float _y);

    void dragMidpoint(float dx, float dy);


    std::vector<straight_line> GetLineList();

    void update_middpoint();


private:
    // Private helper function to calculate midpoint
    void calculateMidpoint();

    void calculateLineExits(const float dx, const float dy);

    int clamp_value(const float val, const float limit_upper, const float limit_lower);
};