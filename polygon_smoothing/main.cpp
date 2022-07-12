/*
================================================================================
Filename: main.cpp
Description: Main program file with entrance point. Create Polygon and get smoothing Points
================================================================================
*/

#include "smooth_polygon.h"
#include <vector>

using namespace std;


int main(int argc, char** argv)
{
    // input data
    vector<Point> polygon;
    Point currentPostion;

    vector<Point> smoothPolygon;
    vector<Point> entryPath;

    //polygon.push_back(Point(165.0, 219.0));
    //polygon.push_back(Point(169.0, 223.0));
    //polygon.push_back(Point(187.0, 246.0));
    //polygon.push_back(Point(237.0, 310.0));
    //polygon.push_back(Point(240.0, 314.0));
    //polygon.push_back(Point(242.0, 317.0));
    //polygon.push_back(Point(242.0, 320.0));
    //polygon.push_back(Point(240.0, 334.0));
    //polygon.push_back(Point(231.0, 393.0));
    //polygon.push_back(Point(225.0, 432.0));
    //polygon.push_back(Point(224.0, 437.0));
    //polygon.push_back(Point(218.0, 435.0));
    //polygon.push_back(Point(184.0, 423.0));
    //polygon.push_back(Point(40.0001, 372.0));
    //polygon.push_back(Point(150.0, 237.0));
    //polygon.push_back(Point(159.0, 226.0));
    //currentPostion = Point(27.0f, 0.0f); // ugv's current Position



    polygon.push_back(Point(0.0, 0.0));
    polygon.push_back(Point(200.0, 50.0));
    polygon.push_back(Point(220.0, 100.0));
    currentPostion = Point(27.0f, 0.0f); // ugv's current Position

    float smooth_quality = 0.25; // 0.05 <= smooth_quilty <= 0.45
    int smooth_points = 5;
    float Minimum_turning_radius = 6;

    //processing
    SmoothPolygon smooth_polygon = SmoothPolygon(smooth_quality, smooth_points);
    smoothPolygon = smooth_polygon.getSmoothPolygon(polygon, 1/ Minimum_turning_radius); // output : smooth polygon edge points(minor points)
    entryPath = smooth_polygon.getEntryPath(currentPostion, polygon); // output : smooth Entry Path Edge points(minor points)

    // ===============================================================
    cout << "최소회전반경 : " << 1 / Minimum_turning_radius << endl;
    cout << "Polygon Major Points" << endl;
    for (int i = 0; i < polygon.size(); ++i)
        cout << polygon[i] << endl;
    
    cout << "======================" << endl;
    cout << endl;

    
    cout << "Polygon Minor Points" << endl;
    for (int i = 0; i < smoothPolygon.size(); ++i)
        cout << smoothPolygon[i] << endl;

    cout << "==========================================" << endl << endl;


    
    cout << "Entry path Minor Points" << endl;
    for (int i = 0; i < entryPath.size(); ++i)
        cout << entryPath[i] << endl;
    
    

    
}
