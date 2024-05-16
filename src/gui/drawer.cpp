#include "gui.h"

// Element container
static std::vector<std::unique_ptr<element>> elements;

// Global unique id
static element_id_t unique_id = 0;

// Total number of elements addedx
static uint16_t num_elements = 0;


// Add point to drawing elements
static void add_point(const Point& p) {
    elements.push_back(std::make_unique<Point>(p));
    num_elements++;
}

// Add straight line to drawing elements
static void add_straight_line(const straight_line& s) {
    elements.push_back(std::make_unique<straight_line>(s));
    num_elements++;
}

// Add orthogonal line to drawing elements
static void add_orthogonal_line(const orthogonal_line& o) {
    elements.push_back(std::make_unique<orthogonal_line>(o));
    num_elements++;
}

// Add rectangle to drawing elements
static void add_rectangle(const rectangle& r) {
    elements.push_back(std::make_unique<rectangle>(r));
    num_elements++;
}

static void delete_element_by_id(const element_id_t _id) {
    // Check if id matches Lambda
    std::function<bool(const std::unique_ptr<element>&)> id_match = [_id](const std::unique_ptr<element>& elem) {
        return elem->unique_id == _id;
    };

    // Find the iterator pointing to the element to delete
    std::vector<std::unique_ptr<element>>::iterator it = std::find_if(elements.begin(), elements.end(), id_match);

    if (it != elements.end()) {
        // Element with matching ID found, erase it from the vector
        num_elements--;
        elements.erase(it);
    }
}


static void move_element_point_by_id(const element_id_t _id, const float _x, const float _y) {
    for (std::unique_ptr<element>& elem : elements) {
        if (elem->unique_id == _id) {
            // Check if the element is movable (has a move function)
            elem->move_element_point(_x, _y);
        }
    }
}


static void move_element_by_id(const element_id_t _id, const float _x, const float _y) {
    for (std::unique_ptr<element>& elem : elements) {
        if (elem->unique_id == _id) {
            // Check if the element is movable (has a move function)
            elem->move_element(_x, _y);
        }
    }
}


static void lock_hit_point_by_id(const element_id_t _id) {
    for (std::unique_ptr<element>& elem : elements) {
        if (elem->unique_id == _id) {
            // Check if the element is movable (has a move function)
            elem->lock_element_hit_point();
        }
    }
}


static void release_hit_point_by_id(const element_id_t _id) {
    for (std::unique_ptr<element>& elem : elements) {
        if (elem->unique_id == _id) {
            // Check if the element is movable (has a move function)
            elem->release_element_hit_point();
        }
    }
}


/* Draw a rectengular canvas between top-left and bottom right points */
void draw_canvas(ImDrawList*& draw_list, const ImVec2 p1, const ImVec2 p2)
{
    draw_list->AddRectFilled(p1, p2, IM_COL32(50, 50, 50, 255));        // Fill canvas bg
    draw_list->AddRect(p1, p2, IM_COL32(255, 255, 255, 255));           // Outline canvas
}


// Element hit detection for all elements included
element_id_t element_hit_detection (const ImVec2 mouse_relative_coordinate) {
    uint16_t num_elements = elements.size();
    for(int idx = 0; idx < num_elements;  idx++) {
        if (elements[idx]->is_hit(mouse_relative_coordinate.x, mouse_relative_coordinate.y)) {
            return elements[idx]->unique_id;
        }
    }
    return -1;
}


void Draw(bool* p_open)
{
    if (ImGui::Begin("Custom Canvas !", p_open))
    {

        // ------------------------------------------------------------------------------------
        // ------------------------ Declerations ----------------------------------------------
        // ------------------------------------------------------------------------------------

        // Points
        static std::vector<Point> new_points;
        
        // Drag Amount
        static ImVec2 scrolling_left_mouse(0.0f, 0.0f);
        static ImVec2 scrolling_right_mouse(0.0f, 0.0f);

        // Enable grid
        static bool opt_enable_grid = false;
        
        // Enable Grid
        static bool opt_enable_context_menu = true;

        // Mouse Drag amount
        ImVec2 delta_drag_left_mouse;
        ImVec2 delta_drag_right_mouse;

        // Canvas top-left and bottom-right points
        ImVec2 canvas_p0;
        ImVec2 canvas_p1;

        // Canvas held | hovered
        bool is_canvas_hovered = false;
        bool is_canvas_held = false;

        // Is adding element
        static bool is_adding_element = false;
        static element_type_t is_adding_element_type = NONE;

        // Mouse cursor type
        static ImGuiMouseCursor current_cursor = ImGuiMouseCursor_Arrow;

        ImGui::Checkbox("Enable grid", &opt_enable_grid);
        ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
        ImGui::Text("Mouse Left: Click to add points,\nMouse Right: drag to scroll, click for context menu.");


        // UI Elements lists
        static std::vector<orthogonal_line> orthogonal_line_list;


        // Element hit detection
        static element_id_t lock_element_id = -1;
        element_id_t hit_element_id;


        // ------------------------------------------------------------------------------------
        // ------------------------ Calculations ----------------------------------------------
        // ------------------------------------------------------------------------------------
        
        // Calculate canvas size
        canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
        
        // Set minimum available space to 50 by 50 minimum
        if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);    // Calculate bottom right corner coordinate


        // Mouse Input/Output and ImDrawList
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();


        // Get Mouse draging inputs
        delta_drag_left_mouse = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
        delta_drag_right_mouse = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);


        // This will catch our interactions
        ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        is_canvas_hovered = ImGui::IsItemHovered(); // Hovered
        is_canvas_held = ImGui::IsItemActive();   // Held


        // Calculate Origin point and relative mouse position
        const ImVec2 origin(canvas_p0.x, canvas_p0.y); // Lock scrolled origin
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);   // Mouse position relative to top-left of canvas


        // Add points on left click without draging
        if (is_adding_element && is_canvas_hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && delta_drag_left_mouse.x == 0.0f && delta_drag_left_mouse.y == 0.0f)
        {
            switch (is_adding_element_type)
            {
            case POINT: {
                Point temp(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y, unique_id++);
                add_point(temp);
                is_adding_element = false;
                is_adding_element_type = NONE;
                current_cursor = ImGuiMouseCursor_Arrow;
                break;
            }
            case STRAIGHT_LINE: {
                
                Point temp(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y, unique_id++);
                new_points.push_back(temp);

                if (new_points.size() >= 2) {       // If there are two points
                    add_straight_line(straight_line(new_points[0], new_points[1], unique_id++));
                    new_points.clear();
                    is_adding_element = false;
                    is_adding_element_type = NONE;
                    current_cursor = ImGuiMouseCursor_Arrow;
                }
                break;
            }
            case ORTHOGONAL_LINE: {
                Point temp(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y, unique_id++);
                new_points.push_back(temp);

                if (new_points.size() >= 2) {       // If there are two points
                    add_orthogonal_line(orthogonal_line(new_points[0], new_points[1], unique_id++));
                    new_points.clear();
                    is_adding_element = false;
                    is_adding_element_type = NONE;
                    current_cursor = ImGuiMouseCursor_Arrow;
                }
                break;
            }
            case RECTANGLE: {
                rectangle temp(Point(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y), \
                                180, \
                                180, \
                                unique_id++);
                add_rectangle(temp);
                is_adding_element = false;
                is_adding_element_type = NONE;
                current_cursor = ImGuiMouseCursor_Arrow;
                break;
            }
            default:
                break;
            }
        }

        // Save right mouse drag amounts
        if (is_canvas_held && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            scrolling_right_mouse.x += io.MouseDelta.x;
            scrolling_right_mouse.y += io.MouseDelta.y;
        }


        // Save left mouse drag amounts
        if (is_canvas_held && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            scrolling_left_mouse.x += io.MouseDelta.x;
            scrolling_left_mouse.y += io.MouseDelta.y;
        }

    
        // Element hit detection
        hit_element_id = element_hit_detection(mouse_pos_in_canvas);


        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (hit_element_id != -1) {
                lock_element_id = hit_element_id;
                lock_hit_point_by_id(lock_element_id);
            }
        }

        if (is_canvas_held) {
            if (lock_element_id != -1) {
                move_element_point_by_id(lock_element_id, mouse_pos_in_canvas.x, mouse_pos_in_canvas.y);
            }
        }

        // Release lock elements when released
        if (!is_canvas_held) {
            lock_element_id = -1;
            release_hit_point_by_id(lock_element_id);
            // TODO: Update last locked element clear
        }


        // Context menu (under default mouse threshold)
        if (delta_drag_right_mouse.x == 0.0f && delta_drag_right_mouse.y == 0.0f)
            ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        if (ImGui::BeginPopup("context"))
        {
            
            if (ImGui::MenuItem("Add Point", NULL, false, !is_adding_element)) {
                is_adding_element = true;
                is_adding_element_type = POINT;
                current_cursor = ImGuiMouseCursor_Hand;
            }
            if (ImGui::MenuItem("Add Straight Line", NULL, false, !is_adding_element)) {
                is_adding_element = true;
                is_adding_element_type = STRAIGHT_LINE;
                current_cursor = ImGuiMouseCursor_Hand;
            }
            if (ImGui::MenuItem("Add Orthogonal Line", NULL, false, !is_adding_element)) {
                is_adding_element = true;
                is_adding_element_type = ORTHOGONAL_LINE;
                current_cursor = ImGuiMouseCursor_Hand;
            }
            if (ImGui::MenuItem("Add Rectangle", NULL, false, !is_adding_element)) {
                is_adding_element = true;
                is_adding_element_type = RECTANGLE;
                current_cursor = ImGuiMouseCursor_Hand;
            }
            if (ImGui::MenuItem("Cancel", NULL, false, is_adding_element)) {
                is_adding_element = false;
                is_adding_element_type = NONE;
                current_cursor = ImGuiMouseCursor_Arrow;
            }

            ImGui::EndPopup();
        }

        ImGui::SetMouseCursor(current_cursor);


        // ------------------------------------------------------------------------------------
        // ------------------------ Drawing ---------------------------------------------------
        // ------------------------------------------------------------------------------------

        // Draw the canvas
        draw_canvas(draw_list, canvas_p0, canvas_p1);

        // Draw grid + all lines in the canvas
        draw_list->PushClipRect(canvas_p0, canvas_p1, true);    // Clip the next drawing elements in the canvas only

        if (opt_enable_grid)    // Draw gridd layout
        {
            const float GRID_STEP = 64.0f;
            for (float x = fmodf(scrolling_right_mouse.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
            for (float y = fmodf(scrolling_right_mouse.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
        }


        for(uint16_t idx = 0; idx < num_elements; idx++) {
            switch (elements[idx]->element_type)
            {
                case POINT: {
                    Point* pointPtr = dynamic_cast<Point*>(elements[idx].get());
                    draw_list->AddCircleFilled(ImVec2(origin.x + pointPtr->getX(), origin.y + pointPtr->getY()), 5, IM_COL32(255, 255, 255, 255));
                    break;
                }
                case STRAIGHT_LINE: {
                    straight_line* straight_line_ptr = dynamic_cast<straight_line*>(elements[idx].get());
                    draw_list->AddLine(ImVec2(origin.x + straight_line_ptr->getPoint1().getX(), origin.y + straight_line_ptr->getPoint1().getY()), ImVec2(origin.x + straight_line_ptr->getPoint2().getX(), origin.y + straight_line_ptr->getPoint2().getY()), IM_COL32(255, 255, 0, 255), 2.0f);
                    break;
                }
                case ORTHOGONAL_LINE: {
                    orthogonal_line* orthogonal_line_ptr = dynamic_cast<orthogonal_line*>(elements[idx].get());
                    std::vector<straight_line> line_list = orthogonal_line_ptr->GetLineList();
                    for(int j = 0; j < line_list.size(); j++) {
                        draw_list->AddLine(ImVec2(origin.x + line_list[j].getPoint1().getX(), origin.y + line_list[j].getPoint1().getY()), ImVec2(origin.x + line_list[j].getPoint2().getX(), origin.y + line_list[j].getPoint2().getY()), IM_COL32(255, 255, 0, 255), 2.0f);
                    }
                    break;
                }
                case RECTANGLE: {
                    rectangle* rectangle_ptr = dynamic_cast<rectangle*>(elements[idx].get());
                    std::vector<straight_line> line_list = rectangle_ptr->GetLineList();
                    for(int j = 0; j < line_list.size(); j++) {
                        draw_list->AddLine(ImVec2(origin.x + line_list[j].getPoint1().getX(), origin.y + line_list[j].getPoint1().getY()), ImVec2(origin.x + line_list[j].getPoint2().getX(), origin.y + line_list[j].getPoint2().getY()), IM_COL32(255, 255, 0, 255), 2.0f);
                    }
                    break;
                }

                default:
                    break;
            }
        }

        draw_list->PopClipRect();  

        ImGui::End();
    }

}
