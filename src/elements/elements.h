#pragma once

#include <math.h>
#include <algorithm> // For std::min and std::max
#include <vector>
#include <cmath> // For sqrt and pow functions

enum exit_direction {
    UP = 1,
    DOWN,
    LEFT, 
    RIGHT,
};

#define MID_POINT_DRAG_MARGIN 10

#define POINT_HIT_DETECTION_MARGIN 100
#define LINE_HIT_DETECTION_MARGIN 4

class Point {
private:
    float x; // X-coordinate of the point
    float y; // Y-coordinate of the point

public:
    // Constructor
    Point(const float xCoord, const float yCoord) : x(xCoord), y(yCoord) {}
    Point() : x(0), y(0) {}

    // Getters and setters for x and y coordinates
    float getX() const { return x; }
    void setX(const float newX) { x = newX; }
    
    float getY() const { return y; }
    void setY(const float newY) { y = newY; }

    void drag_point(const float _x, const float _y) {
        x += _x;
        y += _y;
    }

    void update_point(const float _x, const float _y) {
        x = _x;
        y = _y;
    }

    // Function to calculate distance between this point and another point
    double distanceTo(const Point& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }

    // Function to calculate distance between this point and another point
    double distanceTo(const float _x, const float _y) const {
        float dx = x - _x;
        float dy = y - _y;
        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }

    // Hit detection with margin
    bool isHit(const Point& other) const {
        return distanceTo(other) <= POINT_HIT_DETECTION_MARGIN;
    }

    // Hit detection with margin for x and y coordinates
    bool isHit(const float _x, const float _y) const {
        return distanceTo(_x, _y) <= POINT_HIT_DETECTION_MARGIN;
    }
};


class straight_line {
private:
    Point p1;
    Point p2;

public:
    // Constructors
    straight_line(const Point& point1, const Point& point2) : p1(point1), p2(point2) {}
    straight_line() : p1(Point()), p2(Point()) {}

    // Getter functions for endpoints
    const Point& getPoint1() const { return p1; }
    const Point& getPoint2() const { return p2; }

    // Setter functions for endpoints
    void setPoint1(const Point& point) { p1 = point; }
    void setPoint2(const Point& point) { p2 = point; }

    // Function to calculate the length of the line
    double length() const {
        float dx = p1.getX() - p2.getX();
        float dy = p1.getY() - p2.getY();
        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }

    // Function to check if a point is on the line with a given margin
    bool is_point_on_line(const Point& point, double margin = 0.0) const {
        // Calculate the distance from the line to the point
        double distance = std::abs((p2.getY() - p1.getY()) * point.getX() - 
                                   (p2.getX() - p1.getX()) * point.getY() + 
                                   p2.getX() * p1.getY() - p2.getY() * p1.getX()) /
                          std::sqrt(std::pow(p2.getY() - p1.getY(), 2) + 
                                    std::pow(p2.getX() - p1.getX(), 2));

        // Check if the distance is within the margin
        return distance <= margin;
    }

    // Function to check if a point defined by x and y coordinates is on the line with a given margin
    bool is_point_on_line(const float x, const float y, double margin = 0.0) const {
        // Calculate the distance from the line to the point
        double distance = std::abs((p2.getY() - p1.getY()) * x - 
                                (p2.getX() - p1.getX()) * y + 
                                p2.getX() * p1.getY() - p2.getY() * p1.getX()) /
                        std::sqrt(std::pow(p2.getY() - p1.getY(), 2) + 
                                    std::pow(p2.getX() - p1.getX(), 2));

        // Check if the distance is within the margin
        return distance <= margin;
    }
};

class orthogonal_line {
private:
    Point p1; // First endpoint of the line
    Point p2; // Second endpoint of the line
    Point mid_point; // Midpoint of shortest path

    enum exit_direction p1_exit;
    enum exit_direction p2_exit;

public:
    // Constructor
    orthogonal_line(const Point& point1, const Point& point2) : p1(point1), p2(point2) {
        calculateMidpoint(); // Calculate the midpoint upon construction

        calculateLineExits(p1.getX() - p2.getX(), p1.getY() - p2.getY());   // Calculate the cable exit direction from point
    }

    orthogonal_line(const float x1, const float y1, const float x2, const float y2) {
        p1 = Point(x1, y1);
        p2 = Point(x2, y2);

        calculateMidpoint(); // Calculate the midpoint upon construction

        calculateLineExits(p1.getX() - p2.getX(), p1.getY() - p2.getY());   // Calculate the cable exit direction from point
    }

    // Getter functions for endpoints
    const Point& getPoint1() const { return p1; }
    const Point& getPoint2() const { return p2; }

    // Getter and setter function for midpoint
    const Point& getMidpoint() const { return mid_point; }
    void setMidpoint(const Point& point) { mid_point = point; }

    // Setter functions for endpoints
    void setPoint1(const Point& point) { p1 = point; calculateMidpoint(); }
    void setPoint2(const Point& point) { p2 = point; calculateMidpoint(); }

    // Function to calculate the length of the line
    double length() const {
        float dx = p1.getX() - p2.getX();
        float dy = p1.getY() - p2.getY();
        return (dx + dy);
    }

    bool mid_point_is_hit(const float _x, const float _y) {
        std::vector<straight_line> line_list = this->GetLineList();
        return line_list[1].is_point_on_line(_x, _y, LINE_HIT_DETECTION_MARGIN);
    }

    void dragMidpoint(float dx, float dy) {
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


    std::vector<straight_line> GetLineList() {
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

    void update_middpoint() {
        calculateMidpoint();
    }


private:
    // Private helper function to calculate midpoint
    void calculateMidpoint() {
        float midX = (p1.getX() + p2.getX()) / 2;
        float midY = (p1.getY() + p2.getY()) / 2;
        mid_point = Point(midX, midY);
    }

    void calculateLineExits(const float dx, const float dy) {
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

    int clamp_value(const float val, const float limit_upper, const float limit_lower) {
        
        return std::max(limit_lower + MID_POINT_DRAG_MARGIN, std::min(val, limit_upper - MID_POINT_DRAG_MARGIN));
        
    }
};
