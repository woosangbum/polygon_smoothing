/*
================================================================================
Filename: main.cpp
Description: Main program file with entrance point. Create Polygon and get smoothing Points
================================================================================
*/

#include "smooth_polygon.h"
#include <vector>
#include <math.h>

using namespace std;


int main(int argc, char** argv)
{
    // Create the smooth polygon object
    vector<Point> polygon;
    polygon.push_back(Point(192.0, 186.0));
    polygon.push_back(Point(228.0, 186.0));
    polygon.push_back(Point(229.0, 187.0));
    polygon.push_back(Point(231.0, 191.0));
    polygon.push_back(Point(237.0, 205.0));
    polygon.push_back(Point(260.0, 259.0));
    polygon.push_back(Point(280.0, 306.0)); //
    polygon.push_back(Point(279.0, 308.0));
    polygon.push_back(Point(277.0, 311.0));
    polygon.push_back(Point(264.0, 329.0));
    polygon.push_back(Point(211.0, 402.0));
    polygon.push_back(Point(209.0, 404.0));
    polygon.push_back(Point(208.0, 404.0)); // /
    polygon.push_back(Point(206.0, 403.0));
    polygon.push_back(Point(203.0, 401.0));
    polygon.push_back(Point(152.0, 364.0));
    polygon.push_back(Point(141.0, 356.0));
    polygon.push_back(Point(138.0, 353.0));
    polygon.push_back(Point(138.0, 352.0)); //
    polygon.push_back(Point(143.0, 336.0));
    polygon.push_back(Point(168.0, 259.0));

    


    float smooth_quality = 0.45; // 0.05 <= smooth_quilty <= 0.45
    int smooth_points = 10;

    SmoothPolygon* smooth_polygon = new SmoothPolygon(polygon, smooth_quality, smooth_points);
    
    cout << "Major Points" << endl;
    for (int i = 0; i < smooth_polygon->num_major_points; ++i)
        cout << smooth_polygon->major_points[i] << endl;
    
    cout << "======================" << endl << endl;

    // output : smooth polygon edge points(minor points)
    cout << "Minor Points" << endl;
    for (int i = 0; i < smooth_polygon->num_minor_points; ++i) 
        cout << smooth_polygon->minor_points[i] << endl;
    
}
