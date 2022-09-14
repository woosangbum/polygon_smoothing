#include "select_area.h"

bool compare(const areaByUGV& p1, const areaByUGV& p2) {
    return p1.dist < p2.dist;
}

void selectArea(SmoothPolygon* sp, Pos* ugv, int areaUGV[], int num_area, int num_ugvs) {
    vector<Pos> ep;
    areaByUGV* ugvsCost = new areaByUGV[num_ugvs * num_ugvs];

    for (int i = 0; i < num_ugvs; i++) {
        for (int j = 0; j < num_ugvs; j++) {
            ep = sp[i].getEntryPath(ugv[j]); // 각 ugv-area의 거리 계산
            ugvsCost[i * num_ugvs + j].areaIdx = i;
            ugvsCost[i * num_ugvs + j].ugvIdx = j;
            for (int k = 0; k < ep.size() - 1; k++) {
                ugvsCost[i * num_ugvs + j].dist += dist(ep[k].x, ep[k].y, ep[k + 1].x, ep[k + 1].y);
            }
        }
    }
    for (int i = 0; i < num_ugvs * num_ugvs; i++)
        cout << ugvsCost[i].areaIdx << " " << ugvsCost[i].ugvIdx << " " << ugvsCost[i].dist << endl;

    sort(ugvsCost, ugvsCost + num_ugvs * num_ugvs, compare);

    cout << "sorted" << endl;
    for (int i = 0; i < num_ugvs * num_ugvs; i++)
        cout << ugvsCost[i].areaIdx << " " << ugvsCost[i].ugvIdx << " " << ugvsCost[i].dist << endl;

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