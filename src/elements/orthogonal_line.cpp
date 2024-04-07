#include "orthogonal_line.h"

// Constructor
orthogonal_line::orthogonal_line(const Point& point1, const Point& point2) : p1(point1), p2(point2) {
    calculateMidpoint(); // sCalculate the midpoint upon construction

    calculateLineExits(p1.getX() - p2.getX(), p1.getY() - p2.getY());   // Calculate the cable exit direction from point
}

orthogonal_line::orthogonal_line(const float x1, const float y1, const float x2, const float y2) {
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

bool orthogonal_line::mid_point_is_hit(const float _x, const float _y) {
    std::vector<straight_line> line_list = this->GetLineList();
    return line_list[1].is_point_on_line(_x, _y, LINE_HIT_DETECTION_MARGIN);
}

void orthogonal_line::dragMidpoint(float dx, float dy) {
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


std::vector<straight_line> orthogonal_line::GetLineList() {
    std::vector<straight_line> line_list;
    straight_line temp_line;

    switch (p1_exit)
    {
    case LEFT: {
        temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(mid_point.getX(), p1.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p1.getY()), Point(mid_point.getX(), p2.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p2.getY()), Point(p2.getX(), p2.getY()));
        line_list.push_back(temp_line);
        break;
    }
    case RIGHT: {
        temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(mid_point.getX(), p1.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p1.getY()), Point(mid_point.getX(), p2.getY()));
        line_list.push_back(temp_line);

        temp_line = straight_line(Point(mid_point.getX(), p2.getY()), Point(p2.getX(), p2.getY()));
        line_list.push_back(temp_line);
        break;
    }
    case UP: {
        // temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(p1.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);

        // temp_line = straight_line(Point(p1.getX(), mid_point.getY()), Point(p2.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);

        // temp_line = straight_line(Point(p2.getX(), p2.getY()), Point(p2.getX(), mid_point.getY()));
        // line_list.push_back(temp_line);
        break;
    }
    case DOWN: {
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


// PRIVATE
void orthogonal_line::update_middpoint() {
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
        p1_exit = LEFT;
        p2_exit = RIGHT; 
    } else if (dx > 0 && dy < 0) {
        p1_exit = LEFT;
        p2_exit = RIGHT;
    } else if (dx < 0 && dy > 0) {
        p1_exit = RIGHT;
        p2_exit = LEFT;
    } else {
        p1_exit = RIGHT;
        p2_exit = LEFT;
    }
}

int orthogonal_line::clamp_value(const float val, const float limit_upper, const float limit_lower) {
    
    return std::max(limit_lower + MID_POINT_DRAG_MARGIN, std::min(val, limit_upper - MID_POINT_DRAG_MARGIN));
    
}