/*
================================================================================
Filename: main.cpp
Description: Main program file with entrance point. Create Polygon and get smoothing Points
================================================================================
*/

#include "smooth_polygon.h"
#include <vector>
#include <iostream>


using namespace std;
int main(int argc, char** argv)
{
    // Create the smooth polygon object
    vector<Point> polygon;
    polygon.push_back(Point(0.0, 0.0));
    polygon.push_back(Point(15.0, 2.0));
    polygon.push_back(Point(27.0, 17.2));
    polygon.push_back(Point(17.4, 22.7));
    polygon.push_back(Point(-10.0, 15.24));

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
