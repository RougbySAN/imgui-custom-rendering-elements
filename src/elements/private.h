#pragma once

#include "stdint.h"

enum exit_direction {
    UP = 1,
    DOWN,
    LEFT, 
    RIGHT,
};

#define MID_POINT_DRAG_MARGIN 4.0f

#define POINT_HIT_DETECTION_MARGIN 4.0f
#define LINE_HIT_DETECTION_MARGIN 4.0f