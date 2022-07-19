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
private:
    Point entryPoint;
    Point secondPoint;
    vector<Point> entryPath;
    float round_qualities;  // // Rounding quality of the polygon corners 
    int num_smooth_points;  // Number of the single corner smooth points

    // polygon
    int num_major_points;  // Number of major points that form an outer polygon
    vector<Point> major_points;  // Vector of the major polygon points
    int num_minor_points;  // Number of minor points that form a smoothed polygon
    vector<Point> minor_points;  // Vector of the minor polygon points

    // entry path
    int num_major_points_entry;  // Number of major points that form an outer polygon
    vector<Point> major_points_entry;  // Vector of the major polygon points
    int num_minor_points_entry;  // Number of minor points that form a smoothed polygon
    vector<Point> minor_points_entry;  // Vector of the minor polygon points

    // Method for calculation of the coordinates of major polygon points
    void setMajorPoints(vector<Point> mp);

    // Method for calculation of the coordinates of minoe polygon points
    void calcMinorPoints();
    vector<Point> calcMinorPoints_Each(vector<Point> mp, int mpn, int mi, float rt, int sp);

    void setMajorPoints_entry(vector<Point> mp);
    void calcMinorPoints_entry();
    void checkCurvature(float mc);

public:
    SmoothPolygon(float rq, int sp); // constructor
    vector<Point> getSmoothPolygon(vector<Point> mp, float mc);
    vector<Point> getEntryPath(Point startPoint);

};




#endif // SMOOTH_POLYGON_H_
