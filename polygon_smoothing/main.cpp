/*
================================================================================
Filename: main.cpp
Description: Main program file with entrance point. Create Polygon and get smoothing Points
================================================================================
*/

#include "smooth_polygon.h"
#include <vector>
#include <fstream>
#include "misc_functions.h"
using namespace std;

#define NUM_AREA 2
#define NUM_UGV 2

void selectArea(vector<SmoothPolygon> sp, Point* ugv, int areaUGV[]) {
    vector<Point> ep;
    float ugvsCost[2][2];

    // 영역 선택 알고리즘
    for (int i = 0; i < NUM_AREA; i++) {
        for (int j = 0; j < NUM_UGV; j++) {
            ep = sp[i].getEntryPath(ugv[j]); // 각 ugv-area의 거리 계산
            ugvsCost[i][j] = dist(ep[0].a, ep[0].b, ep[1].a, ep[1].b);
        }
    }

    bool selected[NUM_AREA] = { 0, };
    float minCost = ugvsCost[0][0];

    for (int i = 0; i < NUM_AREA; i++) {
        for (int j = 0; j < NUM_UGV; j++) {

            areaUGV[i] = j;
            if (minCost >= ugvsCost[i][j] && !selected[j]) {
                areaUGV[i] = j;
                selected[j] = true;
                break;
            }
        }
    }
}


int main(int argc, char** argv)
{
    // input data
    vector<SmoothPolygon> sp;
    vector < vector<Point>> area;
    vector<Point> area1;
    vector<Point> area2;
    Point ugv[2];
    int areaUGV[2];
    
    vector<vector<Point>> smoothArea;
    vector<Point> entryPath1;
    vector<Point> entryPath2;

    area1.push_back(Point(165.0, 219.0));
    area1.push_back(Point(169.0, 223.0));
    area1.push_back(Point(187.0, 246.0));
    area1.push_back(Point(237.0, 310.0));
    area1.push_back(Point(240.0, 314.0));
    area1.push_back(Point(242.0, 317.0));
    area1.push_back(Point(242.0, 320.0));
    area1.push_back(Point(240.0, 334.0));
    area1.push_back(Point(231.0, 393.0));
    area1.push_back(Point(225.0, 432.0));
    area1.push_back(Point(224.0, 437.0));
    area1.push_back(Point(218.0, 435.0));
    area1.push_back(Point(184.0, 423.0));
    area1.push_back(Point(40.0001, 372.0));
    area1.push_back(Point(150.0, 237.0));
    area1.push_back(Point(159.0, 226.0));

    area2.push_back(Point(40.0, 371.0));
    area2.push_back(Point(56.0, 118.0));
    area2.push_back(Point(153.0, 151.0));
    area2.push_back(Point(164.5, 216.0));

    ugv[0] = Point(27.0f, 0.0f); // ugv's current Position
    ugv[1] = Point(241.0f, 12.0f); // ugv's current Position
    area.push_back(area2);
    area.push_back(area1);

    float smooth_quality = 0.1; // 0.05 <= smooth_quilty <= 0.45
    int smooth_points = 10;
    float Minimum_turning_radius = 12;
    


    // =============================================================
    // processing
    for (int i = 0; i < NUM_AREA; i++)
        sp.push_back(SmoothPolygon(smooth_quality, smooth_points));
    for (int i = 0; i < NUM_AREA; i++)
        smoothArea.push_back(sp[i].getSmoothPolygon(area[i], 1 / Minimum_turning_radius));
    
    selectArea(sp, ugv, areaUGV); // 영역 선택

    entryPath1 = sp[0].getEntryPath(ugv[areaUGV[0]]);
    entryPath2 = sp[1].getEntryPath(ugv[areaUGV[1]]);



    // ===============================================================
    //DEBUG
    cout << "최소회전반경 : " << 1 / Minimum_turning_radius << endl;
    cout << "Polygon Major Points" << endl;
    string filePath = "C:\\Users\\daniel\\Desktop\\major.txt";

    ofstream writeFile(filePath.data());

    for (int i = 0; i < area[0].size(); i++) {
        if (writeFile.is_open()) {
            writeFile << area[0][i] << endl;
            
        }
    }

    string filePath7 = "C:\\Users\\daniel\\Desktop\\major2.txt";

    ofstream writeFile7(filePath7.data());
    for (int i = 0; i < area[1].size(); i++) {
        if (writeFile7.is_open()) {
            writeFile7 << area[1][i] << endl;

        }
    }

    writeFile.close();
    writeFile7.close();
        
    
    cout << "======================" << endl;
    cout << endl;

    string filePath2 = "C:\\Users\\daniel\\Desktop\\minor.txt";
    ofstream writeFile2(filePath2.data());
    cout << "Polygon Minor Points" << endl;
    for (int i = 0; i < smoothArea[0].size(); ++i) {
        if (writeFile2.is_open()) {
            writeFile2 << smoothArea[0][i] << endl;
        }
    }

    string filePath5 = "C:\\Users\\daniel\\Desktop\\minor2.txt";
    ofstream writeFile5(filePath5.data());
    for (int i = 0; i < smoothArea[1].size(); ++i) {
        if (writeFile5.is_open()) {
            writeFile5 << smoothArea[1][i] << endl;
        }
    }
    writeFile2.close();

    cout << "==========================================" << endl << endl;


    string filePath3 = "C:\\Users\\daniel\\Desktop\\entry.txt";
    ofstream writeFile3(filePath3.data());
    cout << "Entry path Minor Points" << endl;
    for (int i = 0; i < entryPath1.size(); ++i) {
        if (writeFile3.is_open()) {
            cout << entryPath1[i] << endl;
            writeFile3 << entryPath1[i] << endl;
        }
    }
    string filePath4 = "C:\\Users\\daniel\\Desktop\\entry2.txt";
    ofstream writeFile4(filePath4.data());

    for (int i = 0; i < entryPath2.size(); ++i) {
        if (writeFile4.is_open()) {
            cout << entryPath2[i] << endl;
            writeFile4 << entryPath2[i] << endl;
        }
    }
    writeFile4.close();
    

    return 0;
}
