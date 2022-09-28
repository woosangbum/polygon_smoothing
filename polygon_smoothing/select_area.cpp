/*
================================================================================
Filename: select_area.cpp
Description: Definition of mission area assignment function
================================================================================
*/

#include "select_area.h"

bool compare(const areaByUGV& p1, const areaByUGV& p2) {
    return p1.dist < p2.dist;
}

void selectArea(SmoothPolygon* sp, Pos* ugv, int areaUGV[], int num_area, int num_ugvs) {
    vector<Pos> ep;
    areaByUGV* ugvsCost = new areaByUGV[num_ugvs * num_area];

    // Create an entry path for all (area-ugv) cases, then store the total path length in the table (ugvsCost)
    for (int i = 0; i < num_area; i++) {
        for (int j = 0; j < num_ugvs; j++) {
            ep = sp[i].getEntryPath(ugv[j]);
            ugvsCost[i * num_area + j].areaIdx = i;
            ugvsCost[i * num_area + j].ugvIdx = j;
            for (int k = 0; k < ep.size() - 1; k++) {
                ugvsCost[i * num_area + j].dist += dist(ep[k].x, ep[k].y, ep[k + 1].x, ep[k + 1].y);
            }
        }
    }

    // Sort structures by Dist
    sort(ugvsCost, ugvsCost + num_ugvs * num_ugvs, compare);

    // TODO : Modify the path as close as possible
    bool* selectedArea = new bool[num_ugvs];
    for (int i = 0; i < num_ugvs; i++) selectedArea[i] = false;

    bool* selectedUgv = new bool[num_ugvs];
    for (int i = 0; i < num_ugvs; i++) selectedUgv[i] = false;

    for (int i = 0; i < num_ugvs * num_ugvs; i++) {
        if (!selectedArea[ugvsCost[i].areaIdx] && !selectedUgv[ugvsCost[i].ugvIdx]) {
            areaUGV[ugvsCost[i].areaIdx] = ugvsCost[i].ugvIdx;
            selectedArea[ugvsCost[i].areaIdx] = true;
            selectedUgv[ugvsCost[i].ugvIdx] = true;
        }
    }
}