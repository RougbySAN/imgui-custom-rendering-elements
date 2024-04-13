#include "orthogonal_line.h"

// Constructor
orthogonal_line::orthogonal_line(const Point& point1, const Point& point2, const element_id_t _unique_id) : p1(point1), p2(point2), element(ORTHOGONAL_LINE, _unique_id) {
    calculateMidpoint(); // Calculate the midpoint upon construction

    calculateLineExits(p1.getX() - p2.getX(), p1.getY() - p2.getY());   // Calculate the cable exit direction from point
}

orthogonal_line::orthogonal_line(const orthogonal_line& _o) :   p1(_o.getPoint1()), 
                                                                p2(_o.getPoint2()), 
                                                                mid_point(_o.getMidpoint()), 
                                                                element(ORTHOGONAL_LINE, _o.unique_id), 
                                                                p1_exit(_o.p1_exit), 
                                                                p2_exit(_o.p2_exit),
                                                                recorded_hit_point(_o.recorded_hit_point) {}

orthogonal_line::orthogonal_line(const float x1, const float y1, const float x2, const float y2, const element_id_t _unique_id) : element(ORTHOGONAL_LINE, _unique_id) {
    p1 = Point(x1, y1);
    p2 = Point(x2, y2);

    calculateMidpoint(); // Calculate the midpoint upon construction

    calculateLineExits(p1.getX() - p2.getX(), p1.getY() - p2.getY());   // Calculate the cable exit direction from point
}

// Getter functions for endpoints
const Point& orthogonal_line::getPoint1() const { return p1; }
const Point& orthogonal_line::getPoint2() const { return p2; }

// Getter and setter function for midpoint
const Point& orthogonal_line::getMidpoint() const { return mid_point; }
void orthogonal_line::setMidpoint(const Point& point) { mid_point = point; }

// Setter functions for endpoints
void orthogonal_line::setPoint1(const Point& point) { p1 = point; calculateMidpoint(); }
void orthogonal_line::setPoint2(const Point& point) { p2 = point; calculateMidpoint(); }

// Function to calculate the length of the line
double orthogonal_line::length() const {
    float dx = p1.getX() - p2.getX();
    float dy = p1.getY() - p2.getY();
    return (dx + dy);
}


std::vector<straight_line> orthogonal_line::GetLineList() {
    std::vector<straight_line> line_list;
    straight_line temp_line;

    switch (p1_exit)
    {
    case DIR_LEFT: {
        temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(mid_point.getX(), p1.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p1.getY()), Point(mid_point.getX(), p2.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p2.getY()), Point(p2.getX(), p2.getY()));
        line_list.push_back(temp_line);
        break;
    }
    case DIR_RIGHT: {
        temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(mid_point.getX(), p1.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p1.getY()), Point(mid_point.getX(), p2.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p2.getY()), Point(p2.getX(), p2.getY()));
        line_list.push_back(temp_line);
        break;
    }
    case DIR_UP: {
        // temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(p1.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);

        // temp_line = straight_line(Point(p1.getX(), mid_point.getY()), Point(p2.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);

        // temp_line = straight_line(Point(p2.getX(), p2.getY()), Point(p2.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);
        break;
    }
    case DIR_DOWN: {
        // temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(p1.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);

        // temp_line = straight_line(Point(p1.getX(), mid_point.getY()), Point(p2.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);

        // temp_line = straight_line(Point(p2.getX(), p2.getY()), Point(p2.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);
        break;
    }
    
    default:
        break;
    }
    
    return line_list;
}

/* --------------------------------PUBLIC OVERRIDEN FUNCTIONS --------------------------------- */

// Element hit detection
bool orthogonal_line::is_hit(const float _x, const float _y) {
    if (mid_point_is_hit(_x, _y)) {
        recorded_hit_point = OL_MID_POINT;
        return true;
    }
    return exit_point_is_hit(_x, _y);
}

// Drag/Move Whole Element as a shape
void orthogonal_line::drag_element(const float _dx, const float _dy) {
    p1.drag_element(_dx, _dy);
    p2.drag_element(_dx, _dy);
    mid_point.drag_element(_dx, _dy);
}
void orthogonal_line::move_element(const float _x, const float _y) {
    p1.move_element(_x, _y);
    p2.move_element(_x, _y);
    mid_point.move_element(_x, _y);
}

// Drag/Move hit point of element and recalculate shape
void orthogonal_line::drag_element_point(const float _dx, const float _dy) {
    switch (locked_hit_point)
        {
        case OL_POINT_1: {
            p1.drag_element(_dx, _dy);
            calculateMidpoint();
            break;
        }

        case OL_POINT_2: {
            p2.drag_element(_dx, _dy);
            calculateMidpoint();
            break;
        }

        case OL_MID_POINT: {
            dragMidpoint(_dx, _dy);
            break;
        }

        case OL_NONE: {

            break;
        }
        default:
            break;
        }
}
void orthogonal_line::move_element_point(const float _x, const float _y) {
    switch (locked_hit_point)
        {
        case OL_POINT_1: {
            p1.move_element(_x, _y);
            calculateMidpoint();
            break;
        }

        case OL_POINT_2: {
            p2.move_element(_x, _y);
            calculateMidpoint();
            break;
        }

        case OL_MID_POINT: {
            moveMidpoint(_x, _y);
            break;
        }

        case OL_NONE: {

            break;
        }
        default:
            break;
        }
}

// lock/release hit point on the element
void orthogonal_line::lock_element_hit_point() {
    locked_hit_point = recorded_hit_point;
}
void orthogonal_line::release_element_hit_point() {
    locked_hit_point = OL_NONE;
}


// PRIVATE

void orthogonal_line::dragMidpoint(const float dx, const float dy) {
    mid_point.setX(mid_point.getX() + dx);
    mid_point.setY(mid_point.getY() + dy);

    // Ensure X coordinate is within limits
    float upper_limit = std::max(p1.getX(), p2.getX());
    float lower_limit = std::min(p1.getX(), p2.getX());

    mid_point.setX(clamp_value(mid_point.getX(), upper_limit, lower_limit));

    // Ensure X coordinate is within limits
    upper_limit = std::max(p1.getY(), p2.getY());
    lower_limit = std::min(p1.getY(), p2.getY());

    mid_point.setY(clamp_value(mid_point.getY(), upper_limit, lower_limit));
}


void orthogonal_line::moveMidpoint(const float _x, const float _y) {
    // Update the midpoint coordinates
    mid_point.setX(_x);
    mid_point.setY(_y);

    // Ensure X coordinate is within limits
    float upper_limit_x = std::max(p1.getX(), p2.getX());
    float lower_limit_x = std::min(p1.getX(), p2.getX());
    mid_point.setX(clamp_value(mid_point.getX(), upper_limit_x, lower_limit_x));

    // Ensure Y coordinate is within limits
    float upper_limit_y = std::max(p1.getY(), p2.getY());
    float lower_limit_y = std::min(p1.getY(), p2.getY());
    mid_point.setY(clamp_value(mid_point.getY(), upper_limit_y, lower_limit_y));
}


void orthogonal_line::drag_point1(const float dx, const float dy) {
    p1.setX(p1.getX() + dx);
    p1.setY(p1.getY() + dy);
}

void orthogonal_line::drag_point2 (const float dx, const float dy) {
    p2.setX(p2.getX() + dx);
    p2.setY(p2.getY() + dy);
}

bool orthogonal_line::exit_point_is_hit(const float _x, const float _y) {
    
    if (p1.is_hit(_x, _y)) {
        recorded_hit_point = OL_POINT_1;
        return true;
    }
    else if (p2.is_hit(_x, _y)) {
        recorded_hit_point = OL_POINT_2;
        return true;
    }
    else {
        recorded_hit_point = OL_NONE;
        return false;
    }
}


bool orthogonal_line::mid_point_is_hit(const float _x, const float _y) {
    std::vector<straight_line> line_list = this->GetLineList();
    return line_list[1].is_hit(_x, _y);
}


void orthogonal_line::update_midpoint() {
    calculateMidpoint();
}

// Private helper function to calculate midpoint
void orthogonal_line::calculateMidpoint() {
    float midX = (p1.getX() + p2.getX()) / 2;
    float midY = (p1.getY() + p2.getY()) / 2;
    mid_point = Point(midX, midY);
}

void orthogonal_line::calculateLineExits(const float dx, const float dy) {
    if (dx > 0 && dy > 0) {
        p1_exit = DIR_LEFT;
        p2_exit = DIR_RIGHT; 
    } else if (dx > 0 && dy < 0) {
        p1_exit = DIR_LEFT;
        p2_exit = DIR_RIGHT;
    } else if (dx < 0 && dy > 0) {
        p1_exit = DIR_RIGHT;
        p2_exit = DIR_LEFT;
    } else {
        p1_exit = DIR_RIGHT;
        p2_exit = DIR_LEFT;
    }
}

int orthogonal_line::clamp_value(const float val, const float limit_upper, const float limit_lower) {
    
    return std::max(limit_lower + MID_POINT_DRAG_MARGIN, std::min(val, limit_upper - MID_POINT_DRAG_MARGIN));
    
}