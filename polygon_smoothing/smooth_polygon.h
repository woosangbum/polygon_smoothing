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
#include "Pos.h"

using namespace std;

//class Point
//{
//public:
//    float a=0.0, b=0.0;
//    Point() {};
//    float x() {
//        return a;
//    }
//    float y() {
//        return b;
//    }
//    Point(float a, float b) {
//        this->a = a;
//        this->b = b;
//    }
//    void setX(float a) {
//        this->a = a;
//    }
//    void setY(float b) {
//        this->b = b;
//    }
//    friend std::ostream& operator << (std::ostream& out, const Point& point)
//    {
//        out << "( " << point.a << ", " << point.b << " )";
//
//        return out;
//    }
//};

class SmoothPolygon
{
private:
    Pos entryPoint;
    Pos secondPoint;
    vector<Pos> entryPath;
    

    // polygon
    int num_major_points = 0;  // Number of major points that form an outer polygon
    vector<Pos> major_points;  // Vector of the major polygon points
    int num_minor_points = 0;  // Number of minor points that form a smoothed polygon
    vector<Pos> minor_points;  // Vector of the minor polygon points

    // entry path
    int num_major_points_entry = 0;  // Number of major points that form an outer polygon
    vector<Pos> major_points_entry;  // Vector of the major polygon points
    int num_minor_points_entry = 0;  // Number of minor points that form a smoothed polygon
    vector<Pos> minor_points_entry;  // Vector of the minor polygon points
    // Method for calculation of the coordinates of major polygon points
    void setMajorPoints(vector<Pos> mp);

    // Method for calculation of the coordinates of minoe polygon points
    void calcMinorPoints();
    vector<Pos> calcMinorPoints_Each(vector<Pos> mp, int mpn, int mi, float rt, int sp);

    void setMajorPoints_entry(vector<Pos> mp);
    void calcMinorPoints_entry();
    void checkCurvature(float mc);

public:
    float round_qualities = 0;  // // Rounding quality of the polygon corners 
    int num_smooth_points = 0;  // Number of the single corner smooth points
    SmoothPolygon(); // default constructor
    SmoothPolygon(float rq, int sp); // constructor
    ~SmoothPolygon() { };
    vector<Pos> getSmoothPolygon(vector<Pos> mp, float mc);
    vector<Pos> getEntryPath(Pos startPoint);
    vector<Pos> getTotalPath(Pos startPoint);
    int entryPointIdx=0;

};

#endif // SMOOTH_POLYGON_H_
