#pragma once

#include "private.h"
#include "point.h"
#include "straight_line.h"

#include <vector>

class rectangle : public element {
private:
    Point top_left; // Top-left corner point of the rectangle
    float width;   // Width of the rectangle
    float height;  // Height of the rectangle

public:
    // Constructors
    rectangle(const Point& _top_left, const float _width, const float _height, const element_id_t unique_id);
    rectangle(const rectangle& rect);
    rectangle(const float _x, const float _y, const float _width, const float _height, const element_id_t unique_id);
    rectangle();

    // Getter functions
    const Point& getTopLeft() const;
    float getWidth() const;
    float getHeight() const;

    // Setter functions
    void setTopLeft(const Point& _point);
    void setTopLeft(const float _x, const float _y);
    void setWidth(const float _width);
    void setHeight(const float _height);

    /* Public overriden functions from element base class*/

    bool is_hit(const float _x, const float _y) override;
    void drag_element(const float _dx, const float _dy) override;
    void move_element(const float _x, const float _y) override;
    void drag_element_point(const float _dx, const float _dy) override;
    void move_element_point(const float _x, const float _y) override;
    void lock_element_hit_point() override;
    void release_element_hit_point() override;

    // Function to get lines representing the rectangle
    std::vector<straight_line> GetLineList() const;

private:
    /* PRIVATE HELPER FUNCTIONS */
    
};


