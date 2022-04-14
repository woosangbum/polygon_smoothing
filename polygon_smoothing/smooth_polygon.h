#pragma once
/*
================================================================================
Filename: smooth_polygon.h
Description: Declaration of the SmoothPolygon class members, Point class members.
================================================================================
*/

#ifndef SMOOTH_POLYGON_H_
#define SMOOTH_POLYGON_H_

#include <vector>
#include <iostream>
using namespace std;

class Point
{
public:
    float a=0.0, b=0.0;
    Point() {};
    float x() {
        return a;
    }
    float y() {
        return b;
    }
    Point(float a, float b) {
        this->a = a;
        this->b = b;
    }
    void setX(float a) {
        this->a = a;
    }
    void setY(float b) {
        this->b = b;
    }
    friend std::ostream& operator << (std::ostream& out, const Point& point)
    {
        out << "( " << point.a << ", " << point.b << " )";

        return out;
    }
};

class SmoothPolygon
{
public:
    int num_major_points;  // Number of major points that form an outer polygon
    vector<Point> major_points;  // Vector of the major polygon points

    float round_qualities;  // // Rounding quality of the polygon corners 
    int num_smooth_points;  // Number of the single corner smooth points
    int num_minor_points;  // Number of minor points that form a smoothed polygon
    vector<Point> minor_points;  // Vector of the minor polygon points

    SmoothPolygon(vector<Point> mp, float rq, int sp);
private:

    // Method for calculation of the coordinates of major polygon points
    void setMajorPoints(vector<Point> mp);

    // Method for calculation of the coordinates of minoe polygon points
    void calcMinorPoints();
};




#endif // SMOOTH_POLYGON_H_
