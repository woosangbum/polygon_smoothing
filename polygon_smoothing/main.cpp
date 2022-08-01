/*
================================================================================
Filename: main.cpp
Description: Main program file with entrance point. Create Polygon and get smoothing Points
================================================================================
*/

#include <vector>
#include <fstream>
#include "smooth_polygon.h"
#include "select_area.h"
using namespace std;

int main(int argc, char** argv)
{
    // input data =============================================
    int num_areas = 2;
    int num_ugvs = 2;
    SmoothPolygon *sp= new SmoothPolygon[num_areas];
    vector < vector<Point>> area; // Areas
    vector<Point> area1;  // Area1
    vector<Point> area2;  // Area2

    Point *ugv = new Point[num_ugvs];  // ugv's initial position
    float smooth_quality = 0.1; // 0.05 <= smooth_quilty <= 0.45
    int smooth_points = 10;
    float Minimum_turning_radius = 12;

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


    // out put Data ========================================
    int *areaUGV = new int[num_areas];  // ugv's number for each area
    vector<vector<Point>> smoothArea;
    vector<Point> *entryPath = new vector<Point>[num_ugvs];

    // =============================================================
    // processing
    for (int i = 0; i < num_areas; i++){
        sp[i].num_smooth_points = smooth_points;
        sp[i].round_qualities = smooth_quality;
        smoothArea.push_back(sp[i].getSmoothPolygon(area[i], 1 / Minimum_turning_radius)); // smooting 된 영역 경로 저장
    }

    selectArea(sp, ugv, areaUGV, num_areas, num_ugvs); // 영역 선택

    for (int i = 0; i < num_areas; i++) {
        entryPath[i] = sp[i].getEntryPath(ugv[areaUGV[i]]);  // 각 영역별 진입경로 생성
    }


    // ===============================================================
    ////DEBUG
    //cout << "최소회전반경 : " << 1 / Minimum_turning_radius << endl;
    //cout << "Polygon Major Points" << endl;
    //string filePath = "C:\\Users\\daniel\\Desktop\\major.txt";

    //ofstream writeFile(filePath.data());

    //for (int i = 0; i < area[0].size(); i++) {
    //    if (writeFile.is_open()) {
    //        cout << area[0][i] << endl;
    //        writeFile << area[0][i] << endl;
    //        
    //    }
    //}

    //string filePath7 = "C:\\Users\\daniel\\Desktop\\major2.txt";

    //ofstream writeFile7(filePath7.data());
    //for (int i = 0; i < area[1].size(); i++) {
    //    if (writeFile7.is_open()) {
    //        cout << area[1][i] << endl;
    //        writeFile7 << area[1][i] << endl;

    //    }
    //}

    //writeFile.close();
    //writeFile7.close();
    //    
    //
    //cout << "======================" << endl;
    //cout << endl;

    //string filePath2 = "C:\\Users\\daniel\\Desktop\\minor.txt";
    //ofstream writeFile2(filePath2.data());
    //cout << "Polygon Minor Points" << endl;
    //for (int i = 0; i < smoothArea[0].size(); ++i) {
    //    if (writeFile2.is_open()) {
    //        cout << smoothArea[0][i] << endl;
    //        writeFile2 << smoothArea[0][i] << endl;
    //    }
    //}

    //string filePath5 = "C:\\Users\\daniel\\Desktop\\minor2.txt";
    //ofstream writeFile5(filePath5.data());
    //for (int i = 0; i < smoothArea[1].size(); ++i) {
    //    if (writeFile5.is_open()) {
    //        cout << smoothArea[1][i] << endl;
    //        writeFile5 << smoothArea[1][i] << endl;
    //    }
    //}
    //writeFile2.close();

    //cout << "==========================================" << endl << endl;


    //string filePath3 = "C:\\Users\\daniel\\Desktop\\entry.txt";
    //ofstream writeFile3(filePath3.data());
    //cout << "Entry path Minor Points" << endl;
    //for (int i = 0; i < entryPath[0].size(); ++i) {
    //    if (writeFile3.is_open()) {
    //        cout << entryPath[0][i] << endl;
    //        writeFile3 << entryPath[0][i] << endl;
    //    }
    //}
    //string filePath4 = "C:\\Users\\daniel\\Desktop\\entry2.txt";
    //ofstream writeFile4(filePath4.data());

    //for (int i = 0; i < entryPath[1].size(); ++i) {
    //    if (writeFile4.is_open()) {
    //        cout << entryPath[1][i] << endl;
    //        writeFile4 << entryPath[1][i] << endl;
    //    }
    //}
    //writeFile4.close();
    

    return 0;
}
