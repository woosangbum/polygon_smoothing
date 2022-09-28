/*
================================================================================
Filename: select_area.h
Description: Definition of mission area assignment function
================================================================================
*/

#pragma once
#include "smooth_polygon.h"
#include <algorithm>
#include "misc_functions.h"
#include "Pos.h"

struct areaByUGV {
    int areaIdx;
    int ugvIdx;
    float dist;
    vector<Pos> path;
};

bool compare(const areaByUGV& p1, const areaByUGV& p2);

void selectArea(SmoothPolygon* sp, Pos* ugv, int areaUGV[], int num_area, int num_ugvs);