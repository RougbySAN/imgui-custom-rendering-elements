#include "straight_line.h"


// Constructors
straight_line::straight_line(const Point& point1, const Point& point2) : p1(point1), p2(point2) {}
straight_line::straight_line() : p1(Point()), p2(Point()) {}

// Getter functions for endpoints
const Point& straight_line::getPoint1() const { return p1; }
const Point& straight_line::getPoint2() const { return p2; }

// Setter functions for endpoints
void straight_line::setPoint1(const Point& point) { p1 = point; }
void straight_line::setPoint2(const Point& point) { p2 = point; }

// Function to calculate the length of the line
double straight_line::length() const {
    float dx = p1.getX() - p2.getX();
    float dy = p1.getY() - p2.getY();
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

// Function to check if a point is on the line with a given margin
bool straight_line::is_point_on_line(const Point& point, double margin = 0.0) const {
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
bool straight_line::is_point_on_line(const float x, const float y, double margin = 0.0) const {
    // Calculate the distance from the line to the point
    double distance = std::abs((p2.getY() - p1.getY()) * x - 
                            (p2.getX() - p1.getX()) * y + 
                            p2.getX() * p1.getY() - p2.getY() * p1.getX()) /
                    std::sqrt(std::pow(p2.getY() - p1.getY(), 2) + 
                                std::pow(p2.getX() - p1.getX(), 2));

    // Check if the distance is within the margin
    return distance <= margin;
}