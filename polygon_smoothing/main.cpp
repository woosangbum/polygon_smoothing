﻿/*
================================================================================
Filename: main.cpp
Description: Main program file with entrance point. Create Polygon and get smoothing Points
================================================================================
*/

#include <vector>
#include <fstream>
#include "smooth_polygon.h"
#include "misc_functions.h"
#include "select_area.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    // input data =============================================
    int num_areas = 2;
    int num_ugvs = 2;
    SmoothPolygon *sp= new SmoothPolygon[num_areas];
    vector < vector<Pos>> area; // Areas
    vector<Pos> area1;  // Area1
    vector<Pos> area2;  // Area2

    Pos *ugv = new Pos[num_ugvs];  // ugv's initial position
    float smooth_quality = 0.1; // 0.05 <= smooth_quilty <= 0.45
    int smooth_points = 5;
    float Minimum_turning_radius = 12;

    string line;
    ifstream file("C:\\Users\\daniel\\Desktop\\input1.txt"); // example.txt 파일을 연다. 없으면 생성. 
    if (file.is_open()) {
        while (getline(file, line)) {
            string a, b;
            a = line.substr(line.find("xpoint:") + 8, 3);
            b = line.substr(line.find("ypoint:") + 8, 3);
            area1.push_back(Pos(stof(a), stof(b)));
        }
        file.close(); // 열었떤 파일을 닫는다. 
    }
    else {
        cout << "Unable to open file";
        return 1;
    }

    string line2;
    ifstream file2("C:\\Users\\daniel\\Desktop\\input2.txt"); // example.txt 파일을 연다. 없으면 생성. 
    if (file2.is_open()) {
        while (getline(file2, line2)) {
            string a, b;
            a = line2.substr(line.find("xpoint:") + 8, 3);
            b = line2.substr(line.find("ypoint:") + 8, 3);
            area2.push_back(Pos(stof(a), stof(b)));
        }
        file2.close(); // 열었던 파일을 닫는다. 
    }
    else {
        cout << "Unable to open file";
        return 1;
    }


    ugv[0] = Pos(27.0f, 0.0f); // ugv's current Position
    ugv[1] = Pos(241.0f, 12.0f); // ugv's current Position
    area.push_back(area2);
    area.push_back(area1);


    // out put Data ========================================
    int *areaUGV = new int[num_areas];  // ugv's number for each area
    vector<vector<Pos>> smoothArea;
    vector<Pos> *entryPath = new vector<Pos>[num_ugvs];

    // =============================================================
    // processing

    // 영역 스무딩 및 선형 보간
    for (int i = 0; i < num_areas; i++){
        sp[i].num_smooth_points = smooth_points;
        sp[i].round_qualities = smooth_quality;
        smoothArea.push_back(sp[i].getSmoothPolygon(area[i], 1 / Minimum_turning_radius));
        //smoothArea[i] = getLinearInterpolation(smoothArea[i], true);
    }

    selectArea(sp, ugv, areaUGV, num_ugvs, num_ugvs); // 영역 선택

    // 영역 별 진입 경로 생성 및 선형 보간
    for (int i = 0; i < num_areas; i++) {
        entryPath[i] = sp[i].getEntryPath(ugv[areaUGV[i]]);
        //entryPath[i] = getLinearInterpolation(entryPath[i], false);
    }

    printf("asdasdasdawdawfawfafs");
    vector <Pos> temp = sp[0].getTotalPath(ugv[areaUGV[0]]);
    for (int i = 0; i < temp.size(); i++) {
            cout << "(" << temp[i].x << ", " << temp[i].y << ")" << endl;
    }

    // ===============================================================
    //DEBUG
    cout << "Polygon Major Points" << endl;
    string filePath = "C:\\Users\\daniel\\Desktop\\major.txt";
    
    ofstream writeFile(filePath.data());
    
    for (int i = 0; i < temp.size(); i++) {
        if (writeFile.is_open()) {
            cout << "(" << temp[i].x << ", " << temp[i].y << ")" << endl;
            writeFile << "(" << temp[i].x << ", " << temp[i].y << ")" << endl;
        }
    }
    
    string filePath7 = "C:\\Users\\daniel\\Desktop\\major2.txt";
    
    ofstream writeFile7(filePath7.data());
    for (int i = 0; i < area[1].size(); i++) {
        if (writeFile7.is_open()) {
            cout << "(" << area[1][i].x << ", " << area[1][i].y << ")" << endl;
            writeFile7 << "(" << area[1][i].x << ", " << area[1][i].y << ")" << endl;
    
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
            cout << "(" << smoothArea[0][i].x << ", " << smoothArea[0][i].y << ")" << endl;
            writeFile2 << "(" << smoothArea[0][i].x << ", " << smoothArea[0][i].y << ")" << endl;
        }
    }
    
    string filePath5 = "C:\\Users\\daniel\\Desktop\\minor2.txt";
    ofstream writeFile5(filePath5.data());
    for (int i = 0; i < smoothArea[1].size(); ++i) {
        if (writeFile5.is_open()) {
            cout << "(" << smoothArea[1][i].x << ", " << smoothArea[1][i].y << ")" << endl;
            writeFile5 << "(" << smoothArea[1][i].x << ", " << smoothArea[1][i].y << ")" << endl;
        }
    }
    writeFile2.close();
    
    cout << "==========================================" << endl << endl;
    
    
    string filePath3 = "C:\\Users\\daniel\\Desktop\\entry.txt";
    ofstream writeFile3(filePath3.data());
    cout << "Entry path Minor Points" << endl;
    for (int i = 0; i < entryPath[0].size(); ++i) {
        if (writeFile3.is_open()) {
            cout << "(" << entryPath[0][i].x << ", " << entryPath[0][i].y << ")" << endl;
            writeFile3 << "(" << entryPath[0][i].x << ", " << entryPath[0][i].y << ")" << endl;
        }
    }

    cout << "Path2 " << endl;
    cout << endl;
    string filePath4 = "C:\\Users\\daniel\\Desktop\\entry2.txt";
    ofstream writeFile4(filePath4.data());
    
    for (int i = 0; i < entryPath[1].size(); ++i) {
        if (writeFile4.is_open()) {
            cout << "(" << entryPath[1][i].x << ", " << entryPath[1][i].y << ")" << endl;
            writeFile4 << "(" << entryPath[1][i].x << ", " << entryPath[1][i].y << ")" << endl;
        }
    }
    writeFile4.close();
    

    return 0;
}
