#pragma once

#include "stdint.h"

typedef enum element_type {
    POINT = 0,
    STRAIGHT_LINE,
    ORTHOGONAL_LINE,
    RECTANGLE,

    TOTAL_NUM_ELEMENTS,

    NONE,
}element_type_t;


typedef enum exit_direction {
    DIR_UP = 1,
    DIR_DOWN,
    DIR_LEFT, 
    DIR_RIGHT,
}exit_direction_t;


typedef enum straight_line_points {
    SL_POINT_1 = 1,
    SL_POINT_2,
    SL_BODY,
    SL_NONE,
}straight_line_points_t;


typedef enum orthogonal_line_points {
    OL_POINT_1 = 1,
    OL_POINT_2,
    OL_MID_POINT, 
    OL_NONE,
}orthogonal_line_points_t;

#define MID_POINT_DRAG_MARGIN 4.0f

#define POINT_HIT_DETECTION_MARGIN 4.0f
#define LINE_HIT_DETECTION_MARGIN 5.0f


typedef int16_t element_id_t;

// Base class for all elements
class element {
public:
    element_id_t unique_id;
    enum element_type element_type;

public:
    element(const enum element_type _type, const element_id_t _unique_id = 0) : element_type(_type), unique_id(_unique_id) {}

    virtual ~element() {}

    virtual bool is_hit(const float _x, const float _y) { return false;}

    virtual void drag_element(const float _dx, const float _dy) {}

    virtual void drag_element_point(const float _dx, const float _dy) {}

    virtual void move_element(const float _x, const float _y) {}

    virtual void move_element_point(const float _x, const float _y) {}

    virtual void lock_element_hit_point() {}

    virtual void release_element_hit_point() {}

};