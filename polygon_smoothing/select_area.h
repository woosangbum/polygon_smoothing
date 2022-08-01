#pragma once
#include "smooth_polygon.h"
#include<algorithm>
#include "misc_functions.h"



struct areaByUGV {
    int areaIdx;
    int ugvIdx;
    float dist;
};

bool compare(const areaByUGV& p1, const areaByUGV& p2);

void selectArea(SmoothPolygon* sp, Point* ugv, int areaUGV[], int num_areas, int num_ugvs);