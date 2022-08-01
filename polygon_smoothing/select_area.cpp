#include "select_area.h"

bool compare(const areaByUGV& p1, const areaByUGV& p2) {
    return p1.dist < p2.dist;
}

void selectArea(SmoothPolygon* sp, Point* ugv, int areaUGV[], int num_areas, int num_ugvs) {
    vector<Point> ep;
    areaByUGV* ugvsCost = new areaByUGV[num_areas * num_ugvs];

    for (int i = 0; i < num_areas; i++) {
        for (int j = 0; j < num_ugvs; j++) {
            ep = sp[i].getEntryPath(ugv[j]); // 각 ugv-area의 거리 계산
            ugvsCost[i * num_areas + j].areaIdx = i;
            ugvsCost[i * num_areas + j].ugvIdx = j;
            for (int k = 0; k < ep.size() - 1; k++) {
                ugvsCost[i * num_areas + j].dist += dist(ep[k].a, ep[k].b, ep[k + 1].a, ep[k + 1].b);
            }
        }
    }
    sort(ugvsCost, ugvsCost + num_areas * num_ugvs, compare);

    bool* selected = new bool[num_areas];
    for (int i = 0; i < num_areas; i++) selected[i] = false;

    for (int i = 0; i < num_areas * num_ugvs; i++) {
        if (!selected[ugvsCost[i].areaIdx]) {
            areaUGV[ugvsCost[i].areaIdx] = ugvsCost[i].ugvIdx;
            selected[ugvsCost[i].areaIdx] = true;
        }
    }
}