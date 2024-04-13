#pragma once

#include <math.h>
#include "private.h"


class Point : public element {
private:
    float x; // X-coordinate of the point
    float y; // Y-coordinate of the point

public:
    // Constructor
    Point(const float xCoord, const float yCoord, const element_id_t _unique_id = 0);
    Point(const Point& _p);
    Point();

    // Getters and setters for x and y coordinates
    float getX() const;
    void setX(const float newX);
    
    float getY() const;
    void setY(const float newY);


    // Function to calculate distance between this point and another point
    double distanceTo(const Point& other) const;
    double distanceTo(const float _x, const float _y) const;


    /* Public overriden functions from element base class*/

    // Hit detection with margin
    bool is_hit(const float _x, const float _y) override;

    // Drag/Move Whole Element as a shape
    void drag_element(const float _dx, const float _dy) override;
    void move_element(const float _x, const float _y) override;

    // Drag/Move hit point of element and recalculate shape
    void drag_element_point(const float _dx, const float _dy) override;
    void move_element_point(const float _x, const float _y) override;

private:
    /* PRIVATE HELPER FUNCTIONS */
     // Change and Update point
    void drag_point(const float _dx, const float _dy);
    void update_point(const float _x, const float _y);

};