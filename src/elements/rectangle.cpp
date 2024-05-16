#include "rectangle.h"


// Constructors
rectangle::rectangle(const Point& _top_left, const float _width, const float _height, const element_id_t unique_id) :  top_left(_top_left),
                                                                                                                        width(_width),
                                                                                                                        height(_height),
                                                                                                                        element(RECTANGLE, unique_id) {}


rectangle::rectangle(const rectangle& rect) :   top_left(rect.top_left),
                                                width(rect.width),
                                                height(rect.height),
                                                element(RECTANGLE, rect.unique_id) {}


rectangle::rectangle(const float _x, const float _y, const float _width, const float _height, const element_id_t unique_id) :   top_left(Point(_x, _y)),
                                                                                                                                    width(_width),
                                                                                                                                    height(_height),
                                                                                                                                    element(RECTANGLE, unique_id) {}


rectangle::rectangle() :    top_left(Point(0, 0)),
                            width(0),
                            height(0),
                            element(RECTANGLE, 0) {}

// Getter functions
const Point& rectangle::getTopLeft() const { return top_left; }
float rectangle::getWidth() const { return width; }
float rectangle::getHeight() const { return height; }



// Setter functions
void rectangle::setTopLeft(const Point& _point) { top_left = _point; }
void rectangle::setTopLeft(const float _x, const float _y) { top_left = Point(_x, _y); }
void rectangle::setWidth(const float _width) { width = _width; }
void rectangle::setHeight(const float _height) { height = _height; }


// Public overriden functions from element base class
bool rectangle::is_hit(const float _x, const float _y) {
    float right = top_left.getX() + width;
    float bottom = top_left.getY() + height;

    return _x >= top_left.getX() && _x <= right && _y >= top_left.getY() && _y <= bottom;

}


void rectangle::drag_element(const float _dx, const float _dy) {
    top_left.drag_element(_dx, _dy);
}


void rectangle::move_element(const float _x, const float _y) {
    top_left.move_element(_x, _y);
}


void rectangle::drag_element_point(const float _dx, const float _dy) {
    top_left.drag_element(_dx, _dy);    
}


void rectangle::move_element_point(const float _x, const float _y) {
    top_left.move_element(_x, _y);
}


void rectangle::lock_element_hit_point() {
    /* Nothing */
}


void rectangle::release_element_hit_point() {
    /* Nothing */
}



// Function to get lines representing the rectangle
std::vector<straight_line> rectangle::GetLineList() const {
    std::vector<straight_line> line_list;

    // Calculate coordinates for the rectangle edges
    float right = top_left.getX() + width;
    float bottom = top_left.getY() + height;

    // Create straight lines for each side of the rectangle
    straight_line temp_line;

    // Top edge
    temp_line = straight_line(top_left, Point(right, top_left.getY()));
    line_list.push_back(temp_line);

    // Right edge
    temp_line = straight_line(Point(right, top_left.getY()), Point(right, bottom));
    line_list.push_back(temp_line);

    // Bottom edge
    temp_line = straight_line(Point(right, bottom), Point(top_left.getX(), bottom));
    line_list.push_back(temp_line);

    // Left edge
    temp_line = straight_line(Point(top_left.getX(), bottom), top_left);
    line_list.push_back(temp_line);

    return line_list;
}
