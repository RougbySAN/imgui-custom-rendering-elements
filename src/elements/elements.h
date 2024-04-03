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

class Point {
private:
    int x; // X-coordinate of the point
    int y; // Y-coordinate of the point

public:
    // Constructor
    Point(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}
    Point() : x(0), y(0) {}

    // Getters and setters for x and y coordinates
    int getX() const { return x; }
    void setX(int newX) { x = newX; }
    
    int getY() const { return y; }
    void setY(int newY) { y = newY; }

    // Function to calculate distance between this point and another point
    double distanceTo(const Point& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }

    // Function to calculate distance between this point and another point
    double distanceTo(const int _x, const int _y) const {
        int dx = x - _x;
        int dy = y - _y;
        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
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
        int dx = p1.getX() - p2.getX();
        int dy = p1.getY() - p2.getY();
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
        int dx = p1.getX() - p2.getX();
        int dy = p1.getY() - p2.getY();
        return (dx + dy);
    }

    void dragMidpoint(int dx, int dy) {
        // Calculate the direction vector of the line from p1 to p2
        int line_dx = p2.getX() - p1.getX();
        int line_dy = p2.getY() - p1.getY();

        // Calculate the length of the line segment between p1 and p2
        double line_length = std::sqrt(line_dx * line_dx + line_dy * line_dy);

        // Calculate the unit vector along the line
        double line_unit_x = line_dx / line_length;
        double line_unit_y = line_dy / line_length;

        // Calculate the dot product of the drag vector and the unit vector along the line
        int dot_product = dx * line_unit_x + dy * line_unit_y;

        // Project the dragged amount onto the line
        int dx_projected = static_cast<int>(dot_product * line_unit_x);
        int dy_projected = static_cast<int>(dot_product * line_unit_y);

        // Update the midpoint coordinates
        int new_mid_x = mid_point.getX() + dx_projected;
        int new_mid_y = mid_point.getY() + dy_projected;

        // Update the midpoint
        mid_point = Point(new_mid_x, new_mid_y);
    }

    // // Function to drag the midpoint within certain bounds
    // void dragMidpoint(int dx, int dy) {

    //     /* Calculate midpoint drag */
    //     switch (p1_exit)
    //     {
    //     case LEFT: {
    //         dy = 0;
    //         dx = clamp_value(dx, p1.getX(), p2.getX());
    //         break;
    //     }
    //     case RIGHT: {
    //         dy = 0;
    //         dx = clamp_value(dx, p2.getX(), p1.getX());
    //         break;
    //     }
    //     case UP: {
    //         dx = 0;
    //         dy = clamp_value(dy, p2.getY(), p1.getY());
    //         break;
    //     }
    //     case DOWN: {
    //         dx = 0;
    //         dy = clamp_value(dy, p1.getY(), p2.getY());
    //         break;
    //     }
        
    //     default:
    //         break;
    //     }

    //     /* Update mid point */
    //     int newX = mid_point.getX() + dx;
    //     int newY = mid_point.getY() + dy;
    //     mid_point = Point(newX, newY);
    // }


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

            temp_line = straight_line(Point(mid_point.getX(), p2.getY()), Point(mid_point.getX(), p2.getY()));
            line_list.push_back(temp_line);

            temp_line = straight_line(Point(mid_point.getX(), p2.getY()), Point(p2.getX(), p2.getY()));
            line_list.push_back(temp_line);
            break;
        }
        case UP: {
            temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(p1.getX(), mid_point.getY()));
            line_list.push_back(temp_line);

            temp_line = straight_line(Point(p1.getX(), mid_point.getY()), Point(p2.getX(), mid_point.getY()));
            line_list.push_back(temp_line);

            temp_line = straight_line(Point(p2.getX(), p2.getY()), Point(p2.getX(), mid_point.getY()));
            line_list.push_back(temp_line);
            break;
        }
        case DOWN: {
            temp_line = straight_line(Point(p1.getX(), p1.getY()), Point(p1.getX(), mid_point.getY()));
            line_list.push_back(temp_line);

            temp_line = straight_line(Point(p1.getX(), mid_point.getY()), Point(p2.getX(), mid_point.getY()));
            line_list.push_back(temp_line);

            temp_line = straight_line(Point(p2.getX(), p2.getY()), Point(p2.getX(), mid_point.getY()));
            line_list.push_back(temp_line);
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
        int midX = (p1.getX() + p2.getX()) / 2;
        int midY = (p1.getY() + p2.getY()) / 2;
        mid_point = Point(midX, midY);
    }

    void calculateLineExits(const int dx, const int dy) {
        if (dx > 0 && dy > 0) {
            p1_exit = RIGHT;
            p2_exit = LEFT; 
        } else if (dx > 0 && dy < 0) {
            p1_exit = RIGHT;
            p2_exit = LEFT;
        } else if (dx < 0 && dy > 0) {
            p1_exit = LEFT;
            p2_exit = RIGHT;
        } else {
            p1_exit = LEFT;
            p2_exit = RIGHT;
        }
    }

    int clamp_value(const int x, const int limit_upper, const int limit_lower) {
        
        if(x > limit_upper) {
            return limit_upper;
        }

        if(x < limit_lower) {
            return limit_lower;
        }

        return x;
        
    }
};
