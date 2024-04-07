#include "gui.h"


// Add a |_| looking shape
static void PathConcaveShape(ImDrawList* draw_list, float x, float y, float sz)
{
    const ImVec2 pos_norms[] = { { 0.0f, 0.0f }, { 0.3f, 0.0f }, { 0.3f, 0.7f }, { 0.7f, 0.7f }, { 0.7f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    for (const ImVec2& p : pos_norms)
        draw_list->PathLineTo(ImVec2(x + 0.5f + (int)(sz * p.x), y + 0.5f + (int)(sz * p.y)));
}


/* Draw a rectengular canvas between top-left and bottom right points */

void draw_canvas(ImDrawList*& draw_list, const ImVec2 p1, const ImVec2 p2)
{
    draw_list->AddRectFilled(p1, p2, IM_COL32(50, 50, 50, 255));        // Fill canvas bg
    draw_list->AddRect(p1, p2, IM_COL32(255, 255, 255, 255));           // Outline canvas

}


// Hit detection for points
int16_t point_hit_detection(const std::vector<Point> exits, const ImVec2 mouse_coord) {
    for(int i = 0; i < exits.size(); i++)
    {

        if(exits[i].isHit(mouse_coord.x, mouse_coord.y)) {
            return i;
        }
    }
    return -1;
}


// Hit detection for orthogonal line
int16_t mid_point_hit_detection(std::vector<orthogonal_line> ortho_lines, const ImVec2 mouse_coord) {
    for(int i = 0; i < ortho_lines.size(); i++)
    {
        if(ortho_lines[i].mid_point_is_hit(mouse_coord.x, mouse_coord.y)) {
            return i;
        }
    }
    return -1;
}


void Draw(bool* p_open)
{
    if (ImGui::Begin("Orthogonal Lines !", p_open))
    {

        // ------------------------------------------------------------------------------------
        // ------------------------ Declerations ----------------------------------------------
        // ------------------------------------------------------------------------------------

        // Points
        static std::vector<Point> exits;
        
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

        ImGui::Checkbox("Enable grid", &opt_enable_grid);
        ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
        ImGui::Text("Mouse Left: Click to add points,\nMouse Right: drag to scroll, click for context menu.");


        // UI Elements lists
        std::vector<orthogonal_line> orthogonal_line_list;


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
        if (is_canvas_hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && delta_drag_left_mouse.x == 0.0f && delta_drag_left_mouse.y == 0.0f)
        {
            exits.push_back(Point(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y));

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

       
        // Mouse locked element ids
        static int16_t lock_point_id = -1;
        static int16_t lock_mid_point_id = -1;

        // Element hit detection
        int16_t hit_point = point_hit_detection(exits, mouse_pos_in_canvas);
        int16_t hit_mid_point = mid_point_hit_detection(orthogonal_line_list, mouse_pos_in_canvas);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (hit_point != -1) {
                lock_point_id = hit_point;
            } else if (hit_mid_point != -1) {
                lock_mid_point_id = hit_mid_point;
            }
        }

        if (is_canvas_held) {
            if (lock_point_id != -1) {
                exits[lock_point_id].update_point(mouse_pos_in_canvas.x, mouse_pos_in_canvas.y);
            } else if (lock_mid_point_id != -1) {
                orthogonal_line_list[lock_mid_point_id].dragMidpoint(delta_drag_left_mouse.x, delta_drag_left_mouse.y);
            }
        }

        // Release lock elements when released
        if (!is_canvas_held) {
            lock_point_id = -1;
            lock_mid_point_id = -1;
        }


        std::cout << "The hit point is " << lock_point_id << " the hit ortho is " << lock_mid_point_id << "mid_point " << hit_mid_point<< std::endl;

        for (int i = 0; i + 1 < exits.size(); i += 2)
        {
            // Add orthogonal lines to be drawn
            orthogonal_line_list.push_back(orthogonal_line(exits[i], exits[i + 1]));
        }

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

        /* Put white circles on every left click point */

        for(int i = 0; i < exits.size(); i++) {
            draw_list->AddCircleFilled(ImVec2(origin.x + exits[i].getX(), origin.y + exits[i].getY()), 5, IM_COL32(255, 255, 255, 255));
        }

        /* Draw Orthogonal Lines */
        for(int i = 0; i < orthogonal_line_list.size(); i++)
        {
            std::vector<straight_line> line_list = orthogonal_line_list[i].GetLineList();

            for (int n = 0; n < line_list.size(); n++)
            {
                draw_list->AddLine(ImVec2(origin.x + line_list[n].getPoint1().getX(), origin.y + line_list[n].getPoint1().getY()), ImVec2(origin.x + line_list[n].getPoint2().getX(), origin.y + line_list[n].getPoint2().getY()), IM_COL32(255, 255, 0, 255), 2.0f);
            }
        }

        draw_list->PopClipRect();  

        ImGui::End();
    }

}
